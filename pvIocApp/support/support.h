/* support.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef SUPPORT_H
#define SUPPORT_H
#include <string>
#include <stdexcept>
#include <memory>

#include <pvIntrospect.h>
#include <pvData.h>
#include <noDefaultMethods.h>
#include <pvEnumerated.h>
#include <timeStamp.h>

#include "pvDatabase.h"
#include "install.h"
#include "util.h"

namespace epics { namespace pvData { 

enum SupportState{
    readyForInitialize,
    readyForStart,
    ready,
    zoombie
};

class SupportStateFunc;
class ProcessCallbackRequester;
class ProcessContinueRequester;
class ProcessToken;
class RecordProcess;
class RecordProcessRequester;
class Support;
class SupportProcessRequester;

// from pvDatabase.h
class PVDatabase;
class PVListener;
class PVRecordClient;
class PVRecord;
class PVRecordField;
class PVRecordStructure;
class PVReplace;
class PVRecordCreate;
class PVDatabaseFactory;


class SupportStateFunc {
public:
    static SupportState getSupportState(int value);
    static std::auto_ptr<PVEnumerated>  getSupportState(PVField &pvField);
};

class ProcessCallbackRequester : public Requester {
public:
    virtual void processCallback() = 0;
};

class ProcessContinueRequester {
public:
    virtual void processContinue() = 0;
};

class RecordProcessRequester : public Requester {
public:
    virtual void becomeProcessor() = 0;
    virtual void canNotProcess(String reason) = 0;
    virtual void lostRightToProcess() = 0;
    virtual void recordProcessResult(RequestResult requestResult) = 0;
    virtual void recordProcessComplete() = 0;
}; 

class SupportProcessRequester {
    virtual void supportProcessDone(RequestResult requestResult) = 0;
};

class ProcessToken : private NoDefaultMethods {
public:
    ProcessToken(){}
    virtual ~ProcessToken() {}
};

class RecordProcess : private NoDefaultMethods {
public:
    static RecordProcess &create(PVRecord &pvRecord);
    void destroy();
    bool isEnabled();
    bool setEnabled(bool value);
    bool isActive();
    PVRecord &getRecord();
    bool isTrace();
    bool setTrace(bool value);
    SupportState getSupportState();
    void initialize();
    void start(AfterStart &afterStart);
    void stop();
    void uninitialize();
    ProcessToken *requestProcessToken(
        RecordProcessRequester &recordProcessRequester);
    void releaseProcessToken(ProcessToken &processToken);
    void forceInactive();
    String getRecordProcessRequesterName();
    void queueProcessRequest(ProcessToken &processToken);
    void process(ProcessToken &processToken,bool leaveActive);
    void process(ProcessToken &processToken,
        bool leaveActive,TimeStamp *timeStamp);
    void setInactive(ProcessToken &processToken);
    void processContinue(ProcessContinueRequester &processContinueRequester);
    void requestProcessCallback(ProcessCallbackRequester &processCallbackRequester);
    void setTimeStamp(TimeStamp timeStamp);
    void getTimeStamp(TimeStamp timeStamp);
private:
    RecordProcess(PVRecord &pvRecord);
    ~RecordProcess();
};

class Support : public Requester , private NoDefaultMethods {
public:
    Support(String name,PVRecordField &pvRecordField);
    ~Support();
    String getSupportName();
    SupportState getSupportState();
    PVRecordField &getPVRecordField();
    virtual void initialize();
    virtual void start(AfterStart afterStart);
    virtual void stop();
    virtual void uninitialize();
    virtual void process(SupportProcessRequester &supportProcessRequester);
protected:
    void setSupportState(SupportState state);
    bool checkSupportState(SupportState expectedState,String message);
private:
    // TBD
};

}}

#endif  /* SUPPORT_H */
