/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAProgressServer                                                     //
//                                                                      //
// Server for monitoring the progress of multi-process processing.      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TServerSocket.h"
#include "TMonitor.h"
#include "TMessage.h"
#include "TError.h"
#include "TThread.h"
#include "TStopwatch.h"

#include "FAProgressServer.h"
#include "FAUtils.h"

ClassImp(FAProgressServer)

// init static class members
const Int_t FAProgressServer::fgNetTimeout = 50;
FAProgressServer* FAProgressServer::fgServer = 0;
TThread* FAProgressServer::fgServerThread = 0;

//______________________________________________________________________________
FAProgressServer::FAProgressServer(Int_t port)
    : TObject()
{
    // Constructor.

    // init members
    fServer = 0;
    fEvents = 0;
    fEventsDone = 0;
    fTimer = new TStopwatch();

    // try to create server socket
    fServer = new TServerSocket(port);
    if (fServer->IsValid())
    {
        Info("FAProgressServer", "Server started on port %d", fServer->GetLocalPort());
    }
    else
    {
        Error("FAProgressServer", "Could not open server socket on port %d", port);
        fServer = 0;
    }
}

//______________________________________________________________________________
FAProgressServer::~FAProgressServer()
{
    // Destructor.

    if (fServer)
    {
        fServer->Close();
        delete fServer;
    }
}

//______________________________________________________________________________
void FAProgressServer::Listen()
{
    // Start the server listening loop.

    // check server status
    if (!fServer)
    {
        Error("Listen", "Cannot listen to socket - server is not running!");
        return;
    }

    // set running flag
    fIsRunning = kTRUE;

    // create monitor to watch network sockets
    TMonitor* mon = new TMonitor();
    mon->Add(fServer);

    // create list for network sockets
    TList* sockets = new TList();
    sockets->SetOwner(kTRUE);

    // listen to port
    while (1)
    {
        // check running status
        if (!fIsRunning) break;

        // remove inactive sockets from monitor, close and destroy them
        for (Int_t i = 0; i < sockets->GetSize(); i++)
        {
            TSocket* t = (TSocket*)sockets->At(i);
            if (!t->IsValid())
            {
                mon->Remove(t);
                t->Close();
                sockets->Remove(t);
                delete t;
            }
        }

        // wait for connections via socket
        TSocket* s = mon->Select(fgNetTimeout);

        // check socket connection
        if (s == (TSocket*)-1) continue;

        // accept new connections
        if (s->IsA() == TServerSocket::Class())
        {
            // get socket
            TSocket* sn = ((TServerSocket*) s)->Accept();

            // add socket to monitor
            mon->Add(sn);

            // add socket to list
            sockets->Add(sn);

            continue;
        }

        // accept message
        TMessage* mess;
        s->Recv(mess);

        // skip empty message
        if (!mess) continue;

        // interpret message
        if (mess->What() == kMESS_ANY)
        {
            // read command code
            Int_t cmd;
            mess->ReadInt(cmd);

            // parse command
            Long64_t n;
            switch (cmd)
            {
                case kInit:
                    mess->ReadLong64(n);
                    Init(n);
                    break;
                case kPrint:
                    PrintProgress();
                    break;
                case kAddProcEvents:
                    mess->ReadLong64(n);
                    fEventsDone += n;
                    break;
                case kAddProcEventsPrint:
                    mess->ReadLong64(n);
                    fEventsDone += n;
                    PrintProgress();
                    break;
                case kFinish:
                    Finish();
                    break;
                case kStop:
                    StopListening();
                    break;
                default:
                    Error("Listen", "Unknown command code '%d'", cmd);
            }
        }

        // clean-up
        delete mess;
    }

    // remove sockets from monitor and close them
    for (Int_t i = 0; i < sockets->GetSize(); i++)
    {
        TSocket* t = (TSocket*)sockets->At(i);
        mon->Remove(t);
        t->Close();
    }

    // clean-up of monitor
    mon->Remove(fServer);
    delete mon;

    // clean-up of sockets and socket list
    delete sockets;
}

//______________________________________________________________________________
void FAProgressServer::StopListening()
{
    // Stop the server.

    // check server status
    if (fServer)
    {
        // set running flag
        fIsRunning = kFALSE;

        // user info
        Info("StopListening", "Stopping server on port %d", fServer->GetLocalPort());
    }
    else
    {
        Error("StopListening", "Server is not running!");
    }
}

//______________________________________________________________________________
Int_t FAProgressServer::GetPort()
{
    // Return the port of the server.

    if (fServer)
        return fServer->GetLocalPort();
    else
        return 0;
}

//______________________________________________________________________________
void FAProgressServer::Init(Long64_t events)
{
    // Init the server with 'events' events to process.

    // reset event counters
    fEvents = events;
    fEventsDone = 0;

    // reset timer
    fTimer->Reset();
    fTimer->Start();

    // user info
    Info("Init", "Started processing %lld events", fEvents);
}

//______________________________________________________________________________
void FAProgressServer::PrintProgress()
{
    // Print the progress.

    // calculate rate
    fTimer->Stop();
    Double_t rate = fEventsDone / fTimer->RealTime();
    fTimer->Continue();

    // user info
    Info("Print", "%.1f%% processed - %s remaining",
                  100.*(Double_t)fEventsDone/(Double_t)fEvents,
                  FAUtils::FormatTimeSec((fEvents-fEventsDone)/rate).Data());
}

//______________________________________________________________________________
void FAProgressServer::Finish()
{
    // Finish the progress measurement and print summary.

    // stop timer
    fTimer->Stop();

    // user info
    Info("Stop", "Finished processing in %s", FAUtils::FormatTimeSec(fTimer->RealTime()).Data());
}

//______________________________________________________________________________
void FAProgressServer::CreateServer(Int_t port)
{
    // Create and start the singleton server.

    // delete old server and thread
    if (fgServerThread)
    {
        fgServerThread->Kill();
        delete fgServerThread;
    }
    if (fgServer)
    {
        fgServer->StopListening();
        delete fgServer;
    }

    // create server
    fgServer = new FAProgressServer(port);

    // start the server
    fgServerThread = new TThread(RunServer);
    fgServerThread->Run();
}

//______________________________________________________________________________
void* FAProgressServer::RunServer(void* arg)
{
    // Run the singleton server.

    // start the server
    if (fgServer)
        fgServer->Listen();

    return 0;
}

//______________________________________________________________________________
void FAProgressServer::StopServer()
{
    // Stop the singleton server.

    // stop the server
    if (fgServer && fgServer->IsRunning())
    {
        fgServer->StopListening();
        fgServerThread->Join();
        delete fgServerThread;
        delete fgServer;
        fgServer = 0;
        fgServerThread = 0;
    }
}

