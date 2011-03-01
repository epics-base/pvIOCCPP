/* install.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef INSTALL_H
#define INSTALL_H
#include <string>
#include <stdexcept>
#include <memory>

#include <pvIntrospect.h>
#include <pvData.h>
#include <noDefaultMethods.h>
#include <pvEnumerated.h>
#include <thread.h>

#include "pvDatabase.h"
#include "support.h"

namespace epics { namespace pvData { 

class AfterStart;
class AfterStartNode;
class AfterStartRequester;
class NewAfterStartRequester;

class Install;
class SupportCreation;

// from support.h
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


class AfterStartNode : private NoDefaultMethods {
public:
    AfterStartNode(){}
    virtual ~AfterStartNode() {}
};

class AfterStartRequester {
public:
    virtual void callback(AfterStartNode &node) = 0;
};

class NewAfterStartRequester {
public:
    virtual void callback(AfterStart &afterStart) = 0;
};

class AfterStart : private NoDefaultMethods {
public:
    AfterStart();
    ~AfterStart();
    static std::auto_ptr<AfterStartNode> createNode(
        AfterStartRequester &requester);
    static void newAfterStartRegister(NewAfterStartRequester &requester);
    static void newAfterStartUnregister(NewAfterStartRequester &requester);
    void requestCallback(
        AfterStartNode &node,
        bool afterMerge,
        ThreadPriority priority);
    void callRequesters(bool afterMerge);
    void done(AfterStartNode &node);
    void doneAndRequest(
        AfterStartNode &node,
        bool afterMerge,
        ThreadPriority priority);
private:
    //TBD
};

class SupportCreation : private NoDefaultMethods {
public:
    SupportCreation(PVDatabase &pvDatabase,Requester &requester);
    ~SupportCreation();
    bool createSupport();
    bool initializeSupport();
    bool  startSupport(AfterStart &afterStart);
private:
    //TBD
};


class Install {
public:
    static Install &getInstall();
    bool installStructures(String xmlFile,Requester &requester);
    bool installStructures(PVDatabase &pvDatabase,Requester &requester);
    bool installStructure(PVStructure &pvStructure,Requester &requester);
    bool installRecords(String xmlFile,Requester &requester);
    bool installRecords(PVDatabase &pvDatabase,Requester &requester);
    bool installRecord(PVRecord &pvRecord,Requester &requester);
private:
    Install();
    ~Install();
    //TBD
};

extern Install &getInstall();

}}

#endif  /* INSTALL_H */
