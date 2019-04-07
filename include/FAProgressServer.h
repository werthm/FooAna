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
        kPrint,
        kAddProcEvents,
        kAddProcEventsPrint
    };

protected:
    TServerSocket* fServer;     // server socket
    Bool_t fIsRunning;          // running flag
    Long64_t fEvents;           // total number of events
    Long64_t fEventsDone;       // number of processed events
    TStopwatch* fTimer;         // stop watch

    static const Int_t fgNetTimeout;
    static FAProgressServer* fgServer;
    static TThread* fgServerThread;

public:
    FAProgressServer() : TObject(),
                         fServer(0),
                         fIsRunning(kFALSE),
                         fEvents(0), fEventsDone(0),
                         fTimer(0) { }
    FAProgressServer(Long64_t events, Int_t port = 0);
    virtual ~FAProgressServer();

    Bool_t IsRunning() const { return fIsRunning; }
    Long64_t GetEvents() const { return fEvents; }
    Long64_t GetEventsProcessed() const { return fEventsDone; }

    void Start();
    void Stop();
    Int_t GetPort();

    virtual void Print(Option_t* option = "") const;

    static void CreateServer(Long64_t events, Int_t port = 0);
    static Int_t GetServerPort();
    static void* RunServer(void* arg);
    static void StopServer();

    ClassDef(FAProgressServer, 1)  // progress monitoring in processing (server)
};

#endif

