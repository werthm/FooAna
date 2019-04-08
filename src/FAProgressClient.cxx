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
Bool_t FAProgressClient::RequestPrint()
{
    // Send a print request to the server.
    // Return kTRUE on success, otherwise kFALSE.

    // check connection
    if (fSocket && fSocket->IsValid())
    {
        TMessage mes;
        mes.WriteInt(FAProgressServer::kPrint);
        fSocket->Send(mes);
        return kTRUE;
    }
    else
    {
        Error("RequestPrint()", "No connection to server!");
        return kFALSE;
    }
}

//______________________________________________________________________________
Bool_t FAProgressClient::AddProcessedEvents(Long64_t n)
{
    // Add 'n' events to the list on the server.
    // Return kTRUE on success, otherwise kFALSE.

    // check connection
    if (fSocket && fSocket->IsValid())
    {
        TMessage mes;
        mes.WriteInt(FAProgressServer::kAddProcEvents);
        mes.WriteLong64(n);
        fSocket->Send(mes);
        return kTRUE;
    }
    else
    {
        Error("AddProcessedEvents()", "No connection to server!");
        return kFALSE;
    }
}

//______________________________________________________________________________
Bool_t FAProgressClient::AddProcessedEventsAndPrint(Long64_t n)
{
    // Add 'n' events to the list on the server and send a print request.
    // Return kTRUE on success, otherwise kFALSE.

    // check connection
    if (fSocket && fSocket->IsValid())
    {
        TMessage mes;
        mes.WriteInt(FAProgressServer::kAddProcEventsPrint);
        mes.WriteLong64(n);
        fSocket->Send(mes);
        return kTRUE;
    }
    else
    {
        Error("AddProcessedEventsAndPrint()", "No connection to server!");
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
        TMessage mes;
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

