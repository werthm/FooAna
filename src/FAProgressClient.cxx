/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAProgressClient                                                     //
//                                                                      //
// Client for monitoring the progress of multi-process processing.      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TSocket.h"
#include "TMessage.h"
#include "TError.h"

#include "FAProgressClient.h"
#include "FAProgressServer.h"

ClassImp(FAProgressClient)

// init static class members
const Int_t FAProgressClient::fgNetTimeout = 50;

//______________________________________________________________________________
FAProgressClient::FAProgressClient(const Char_t* server, Int_t port)
{
    // Constructor.

    // init members
    fSocket = 0;

    // connect to server
    Connect(server, port);
}

//______________________________________________________________________________
FAProgressClient::~FAProgressClient()
{
    // Destructor.

    if (fSocket)
    {
        fSocket->Close();
        delete fSocket;
    }
}

//______________________________________________________________________________
Bool_t FAProgressClient::Connect(const Char_t* server, Int_t port)
{
    // Connect to the server 'server' on port 'port'.
    // Return kTRUE on success, otherwise kFALSE.

    // delete old socket
    if (fSocket)
    {
        fSocket->Close();
        delete fSocket;
    }

    // try to create client socket
    fSocket = new TSocket(server, port);
    if (fSocket->IsValid())
    {
        Info("Connect", "Connected to server '%s' on port %d", server, port);
    }
    else
    {
        Error("Connect", "Could not open connection to '%s' on port %d", server, port);
        return kFALSE;
    }

    return kTRUE;
}

//______________________________________________________________________________
void FAProgressClient::Deconnect()
{
    // Deconnect from the server.
    // Return kTRUE on success, otherwise kFALSE.

    if (fSocket)
    {
        fSocket->Close();
        delete fSocket;
        fSocket = 0;
    }
}

//______________________________________________________________________________
Bool_t FAProgressClient::RequestInit(Long64_t n)
{
    // Send an init request to the server.
    // Return kTRUE on success, otherwise kFALSE.

    // check connection
    if (fSocket && fSocket->IsValid())
    {
        // send command and total number of events
        TMessage mes;
        mes.WriteInt(FAProgressServer::kInit);
        mes.WriteLong64(n);
        fSocket->Send(mes);
        return kTRUE;
    }
    else
    {
        Error("RequestInit()", "No connection to server!");
        return kFALSE;
    }
}

//______________________________________________________________________________
Bool_t FAProgressClient::ReportEvents(Long64_t n)
{
    // Report 'n' processed events to the server.
    // Return kTRUE on success, otherwise kFALSE.

    // check connection
    if (fSocket && fSocket->IsValid())
    {
        // send command and number of processed events
        TMessage mes;
        mes.WriteInt(FAProgressServer::kReportEvents);
        mes.WriteLong64(n);
        fSocket->Send(mes);
        return kTRUE;
    }
    else
    {
        Error("ReportEvents()", "No connection to server!");
        return kFALSE;
    }
}

//______________________________________________________________________________
Bool_t FAProgressClient::PrintProgress()
{
    // Print the current progress.
    // Return kTRUE on success, otherwise kFALSE.

    // check connection
    if (fSocket && fSocket->IsValid())
    {
        // send command
        TMessage mes;
        mes.WriteInt(FAProgressServer::kRequestProgress);
        fSocket->Send(mes);

        // receive progress
        if (fSocket->Select(TSocket::kRead, fgNetTimeout) == 1)
        {
            // receive object
            TMessage* mes_in;
            fSocket->Recv(mes_in);

            // read progress object
            FAProgressMessage* prog = (FAProgressMessage*)mes_in->ReadObjectAny(FAProgressMessage::Class());
            if (prog)
            {
                prog->Print();
                delete prog;
            }
            delete mes_in;
        }

        return kTRUE;
    }
    else
    {
        Error("PrintProgress()", "No connection to server!");
        return kFALSE;
    }
}

//______________________________________________________________________________
Bool_t FAProgressClient::RequestFinish()
{
    // Send a finish request to the server.
    // Return kTRUE on success, otherwise kFALSE.

    // check connection
    if (fSocket && fSocket->IsValid())
    {
        // send command
        TMessage mes;
        mes.WriteInt(FAProgressServer::kRequestProgress);
        fSocket->Send(mes);

        // receive progress
        if (fSocket->Select(TSocket::kRead, fgNetTimeout) == 1)
        {
            // receive object
            TMessage* mes_in;
            fSocket->Recv(mes_in);

            // read progress object
            FAProgressMessage* prog = (FAProgressMessage*)mes_in->ReadObjectAny(FAProgressMessage::Class());
            if (prog)
            {
                Info("RequestFinish", "Finished processing in %s (average %.2e events/s)",
                     FAUtils::FormatTimeSec(prog->fTime).Data(), prog->fEvents/prog->fTime);
                delete prog;
            }
            delete mes_in;
        }

        // send command
        mes.Reset();
        mes.WriteInt(FAProgressServer::kFinish);
        fSocket->Send(mes);

        return kTRUE;
    }
    else
    {
        Error("RequestFinish()", "No connection to server!");
        return kFALSE;
    }
}

//______________________________________________________________________________
Bool_t FAProgressClient::RequestStop()
{
    // Send a stop request to the server.
    // Return kTRUE on success, otherwise kFALSE.

    // check connection
    if (fSocket && fSocket->IsValid())
    {
        // send command
        TMessage mes;
        mes.WriteInt(FAProgressServer::kStop);
        fSocket->Send(mes);
        return kTRUE;
    }
    else
    {
        Error("RequestStop()", "No connection to server!");
        return kFALSE;
    }
}

