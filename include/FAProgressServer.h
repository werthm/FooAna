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


#ifndef FooAna_FAProgressServer
#define FooAna_FAProgressServer

#include "Rtypes.h"

class TServerSocket;
class TSocket;
class TThread;
class TStopwatch;

class FAProgressServer : public TObject
{

public:
    enum ENetMsg {
        kInit,
        kPrint,
        kAddProcEvents,
        kAddProcEventsPrint,
        kFinish,
        kStop
    };

protected:
    TServerSocket* fServer;     // server socket
    Bool_t fIsRunning;          // running flag
    Long64_t fEvents;           // total number of events
    Long64_t fEventsDone;       // number of processed events
    TStopwatch* fTimer;         // stop watch

    void Init(Long64_t events);
    void PrintProgress();
    void Finish();

    static const Int_t fgNetTimeout;
    static FAProgressServer* fgServer;
    static TThread* fgServerThread;

public:
    FAProgressServer() : TObject(),
                         fServer(0),
                         fIsRunning(kFALSE),
                         fEvents(0), fEventsDone(0),
                         fTimer(0) { }
    FAProgressServer(Int_t port);
    virtual ~FAProgressServer();

    Bool_t IsRunning() const { return fIsRunning; }

    void Listen();
    void StopListening();
    Int_t GetPort();

    static void CreateServer(Int_t port);
    static void* RunServer(void* arg);
    static void StopServer();

    ClassDef(FAProgressServer, 1)  // progress monitoring in processing (server)
};

#endif

