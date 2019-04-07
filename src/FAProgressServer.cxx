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
FAProgressServer::FAProgressServer(Long64_t events, Int_t port)
    : TObject()
{
    // Constructor.

    // init members
    fServer = 0;
    fEvents = events;
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
void FAProgressServer::Start()
{
    // Start the server.

    // check server status
    if (!fServer)
    {
        Error("Start", "Cannot listen to socket - server is not running!");
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

    // start timer
    fTimer->Reset();
    fTimer->Start();

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
            Long64_t ev_done;
            switch (cmd)
            {
                case kPrint:
                    Print();
                    break;
                case kAddProcEvents:
                    mess->ReadLong64(ev_done);
                    fEventsDone += ev_done;
                    break;
                case kAddProcEventsPrint:
                    mess->ReadLong64(ev_done);
                    fEventsDone += ev_done;
                    Print();
                    break;
                default:
                    Error("Start", "Unknown command code '%d'", cmd);
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
void FAProgressServer::Stop()
{
    // Stop the server.

    // user info
    fTimer->Stop();
    Info("Stop", "Finished in %s", FAUtils::FormatTimeSec(fTimer->RealTime()).Data());

    // check server status
    if (!fServer)
    {
        Error("StopListening", "Server is not running!");
        return;
    }

    // set running flag
    fIsRunning = kFALSE;
}

//______________________________________________________________________________
Int_t FAProgressServer::GetPort()
{
    // Return the server port.

    if (fServer)
        return fServer->GetLocalPort();
    else
        return 0;
}

//______________________________________________________________________________
void FAProgressServer::CreateServer(Long64_t events, Int_t port)
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
        fgServer->Stop();
        delete fgServer;
    }

    // create server
    fgServer = new FAProgressServer(events, port);

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
        fgServer->Start();

    return 0;
}

//______________________________________________________________________________
Int_t FAProgressServer::GetServerPort()
{
    // Return the port of the singleton server.

    // start the server
    if (fgServer)
        return fgServer->GetPort();
    else
        return 0;
}

//______________________________________________________________________________
void FAProgressServer::StopServer()
{
    // Stop the singleton server.

    // stop the server
    if (fgServer && fgServer->IsRunning())
    {
        fgServer->Stop();
        fgServerThread->Join();
        delete fgServerThread;
        delete fgServer;
        fgServer = 0;
        fgServerThread = 0;
    }
}

//______________________________________________________________________________
void FAProgressServer::Print(Option_t* option) const
{
    // Print the content of this class.

    // calculate rate
    fTimer->Stop();
    Double_t rate = fEventsDone / fTimer->RealTime();
    fTimer->Continue();

    // user info
    Info("Print", "%.1f%% processed - %s remaining",
                  100.*(Double_t)fEventsDone/(Double_t)fEvents,
                  FAUtils::FormatTimeSec((fEvents-fEventsDone)/rate).Data());
}

