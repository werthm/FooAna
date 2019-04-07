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
    if (!fSocket->IsValid())
    {
        Error("Connect", "Could not open connection to '%s' on port %d", server, port);
        return kFALSE;
    }

    return kTRUE;
}

//______________________________________________________________________________
Bool_t FAProgressClient::Connect()
{
    // Connect to the singleton server.
    // Return kTRUE on success, otherwise kFALSE.

    return Connect("localhost", FAProgressServer::GetServerPort());
}

//______________________________________________________________________________
Bool_t FAProgressClient::RequestPrint()
{
    // Send a print request to the server.
    // Return kTRUE on success, otherwise kFALSE.

    // check connection
    if (fSocket && fSocket->IsValid())
    {
        TMessage mes(FAProgressServer::kPrint);
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
        TMessage mes(FAProgressServer::kAddProcEvents);
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

