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

    static const Int_t fgNetTimeout;

public:
    FAProgressClient() : TObject(),
                         fSocket(0) { }
    FAProgressClient(const Char_t* server, Int_t port);
    virtual ~FAProgressClient();

    Bool_t Connect(const Char_t* server, Int_t port);
    void Deconnect();

    Bool_t RequestInit(Long64_t n);
    Bool_t ReportEvents(Long64_t n);
    Bool_t PrintProgress();
    Bool_t RequestFinish();
    Bool_t RequestStop();

    ClassDef(FAProgressClient, 0)  // progress monitoring in processing (client)
};

#endif

