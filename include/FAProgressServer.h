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

#include "FAUtils.h"

class TServerSocket;
class TSocket;
class TThread;

class FAProgressMessage : public TObject
{

public:
    Long64_t fEvents;
    Long64_t fEventsDone;
    Double_t fTime;

    FAProgressMessage() : fEvents(0), fEventsDone(0), fTime(0) { }
    FAProgressMessage(Long64_t e, Long64_t ed, Double_t t)
        : fEvents(e), fEventsDone(ed), fTime(t) { }

    virtual void Print(Option_t* option = "") const
    {
        Double_t rate = fEventsDone / fTime;
        Info("Print", "%.1f%% processed - %s remaining (%.2e events/s) ",
             100.*(Double_t)fEventsDone/(Double_t)fEvents,
             FAUtils::FormatTimeSec((fEvents-fEventsDone)/rate).Data(), rate);
    }

    ClassDef(FAProgressMessage, 1)  // progress monitoring in processing (server)
};

class FAProgressServer : public TObject
{

public:
    enum ENetMsg {
        kInit,
        kReportEvents,
        kRequestProgress,
        kFinish,
        kStop
    };

protected:
    TServerSocket* fServer;     // server socket
    Bool_t fIsRunning;          // running flag
    Long64_t fEvents;           // total number of events
    Long64_t fEventsDone;       // number of processed events
    Double_t fStartTime;        // start epoch
    Double_t fStopTime;         // stop epoch

    void Init(Long64_t events);
    void SendProgress(TSocket* s);
    void Finish();

    static const Int_t fgNetTimeout;
    static FAProgressServer* fgServer;
    static TThread* fgServerThread;

public:
    FAProgressServer() : TObject(),
                         fServer(0),
                         fIsRunning(kFALSE),
                         fEvents(0), fEventsDone(0),
                         fStartTime(0), fStopTime(0) { }
    FAProgressServer(Int_t port);
    virtual ~FAProgressServer();

    Bool_t IsRunning() const { return fIsRunning; }

    void Listen();
    void StopListening();
    Int_t GetPort();

    static void CreateServer(Int_t port);
    static void* RunServer(void* arg);
    static void StopServer();

    ClassDef(FAProgressServer, 0)  // progress monitoring in processing (server)
};

#endif

