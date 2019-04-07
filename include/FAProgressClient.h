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


#ifndef FooAna_FAProgressClient
#define FooAna_FAProgressClient

#include "Rtypes.h"

class TSocket;

class FAProgressClient : public TObject
{

protected:
    TSocket* fSocket;       // client socket

public:
    FAProgressClient() : TObject(),
                         fSocket(0) { }
    virtual ~FAProgressClient();

    Bool_t Connect(const Char_t* server, Int_t port);
    Bool_t Connect();
    Bool_t RequestPrint();
    Bool_t AddProcessedEvents(Long64_t n);

    ClassDef(FAProgressClient, 1)  // progress monitoring in processing (client)
};

#endif

