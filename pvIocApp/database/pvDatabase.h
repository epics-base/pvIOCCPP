/* pvDatabase.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/* Marty Kraimer 2011.03 */
#ifndef PVDATABASE_H
#define PVDATABASE_H
#include <string>
#include <stdexcept>
#include <memory>
#include <map>
#include <list>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>
#include <pv/convert.h>
#include <pv/linkedList.h>
#include <pv/lock.h>
#include <pv/noDefaultMethods.h>
#include <pv/executor.h>
#include <pv/messageQueue.h>

namespace epics { namespace pvIOC { 

class PVDatabase;
class PVListener;
class PVRecordClient;
class PVRecord;
class PVRecordField;
class PVRecordStructure;
class PVDatabaseFactory;


//from support
class SupportStateFunc;
class ProcessCallbackRequester;
class ProcessContinueRequester;
class ProcessToken;
class RecordProcess;
class RecordProcessRequester;
class Support;
class SupportProcessRequester;

typedef Support * const SupportPtr;
typedef RecordProcess * const RecordProcessPtr;
typedef PVDatabase * const PVDatabasePtr;
typedef PVRecord *const PVRecordPtr;
typedef PVRecordPtr * const PVRecordPtrArray;
typedef PVRecordStructure * const PVRecordStructurePtr;
typedef PVRecordStructurePtr * const PVRecordStructurePtrArray;
typedef PVRecordField * const PVRecordFieldPtr;
typedef PVRecordFieldPtr * const PVRecordFieldPtrArray;

// following are for private fields
typedef epics::pvData::LinkedListNode<epics::pvData::Requester> RequesterListNode;
typedef epics::pvData::LinkedList<epics::pvData::Requester> RequesterList;
typedef epics::pvData::LinkedListNode<PVListener> PVListenerListNode;
typedef epics::pvData::LinkedList<PVListener> PVListenerList;
typedef epics::pvData::LinkedListNode<PVRecordClient> PVRecordClientListNode;
typedef epics::pvData::LinkedList<PVRecordClient> PVRecordClientList;
typedef std::map<epics::pvData::String,PVRecordPtr> RecordMap;
typedef std::map<epics::pvData::String,epics::pvData::PVStructurePtr> StructureMap;
// end definitions for private fields


class PVListener {
public:
    virtual void dataPut(PVRecordField &pvRecordField) = 0;
    virtual void dataPut(
        PVRecordStructure &requested,
        PVRecordField &pvRecordField) = 0;
    virtual void beginGroupPut(PVRecord &pvRecord) = 0;
    virtual void endGroupPut(PVRecord &pvRecord) = 0;
    virtual void unlisten(PVRecord &pvRecord) = 0;
};

class PVRecordClient {
public:
    virtual void detach(PVRecord &pvRecord) = 0;
};

class PVRecord :
    virtual public epics::pvData::Requester,
    private epics::pvData::NoDefaultMethods
{
public:
    PVRecord(
        epics::pvData::String recordName,
        std::auto_ptr<epics::pvData::PVStructure> pvStructure);
    void destroy();
    virtual epics::pvData::String getRequesterName();
    virtual void message(epics::pvData::String message, epics::pvData::MessageType);
    void toString(epics::pvData::StringBuilder buf) ;
    void toString(epics::pvData::StringBuilder buf,int indentLevel) ;
    RecordProcessPtr getRecordProcess();
    void setRecordProcess(RecordProcess &recordProcess);
    PVRecordField &findPVRecordField(epics::pvData::PVField &pvField);
    PVRecordStructure &getPVRecordStructure();
    epics::pvData::String getRecordName();
    void addRequester(epics::pvData::Requester &requester);
    void removeRequester(epics::pvData::Requester &requester);
    void lock();
    void unlock();
    void lockOtherRecord(PVRecord &otherRecord);
    void beginGroupPut();
    void endGroupPut();
    void registerListener(PVListener &pvListener);
    void unregisterListener(PVListener &pvListener);
    bool isRegisteredListener(PVListener &pvListener);
    void removeEveryListener();
    void registerClient(PVRecordClient &pvRecordClient);
    void unregisterClient(PVRecordClient &pvRecordClient);
    void detachClients();
    int getNumberClients();
private:
    ~PVRecord();
    static epics::pvData::int32 numberRecords;
    epics::pvData::String recordName;
    std::auto_ptr<epics::pvData::PVStructure> pvStructure;
    std::auto_ptr<PVRecordStructure> pvRecordStructure;
    epics::pvData::int32 id;
    epics::pvData::int32 depthGroupPut;
    RecordProcess* recordProcess;
    epics::pvData::Convert &convert;
    RequesterList requesterList;
    PVListenerList listenerList;
    PVRecordClientList clientList;
    epics::pvData::Mutex mutex;
    epics::pvData::Lock recordLock;
    int lockRecursiveCount;
};

class PVRecordField :
    virtual public epics::pvData::Requester ,
    virtual public epics::pvData::PostHandler ,
    private epics::pvData::NoDefaultMethods
{
public:
    virtual ~PVRecordField();
    virtual epics::pvData::String getRequesterName();
    virtual void message(
        epics::pvData::String, epics::pvData::MessageType);
    SupportPtr getSupport();
    void setSupport(Support &support);
    PVRecordStructurePtr getParent();
    epics::pvData::PVField &getPVField();
    epics::pvData::String getFullFieldName();
    epics::pvData::String getFullName();
    PVRecord &getPVRecord();
    void renameField(epics::pvData::String newName);
    bool addListener(PVListener &pvListener);
    void removeListener(PVListener &vListener);
    virtual void postPut();
private:
    PVRecordField(epics::pvData::PVField &pvField,
        PVRecordStructurePtr parent,
        PVRecord &pvRecord);
    void postParent(PVRecordField &subField);
    void postSubField();
    void callListener();
    void createNames();
    friend class PVRecord;
    friend class PVRecordStructure;
    PVListenerList listenerList;
    Support *support;
    epics::pvData::PVField &pvField;
    PVRecord &pvRecord;
    PVRecordStructure* parent;
    bool isStructure;
    epics::pvData::String fullName;
    epics::pvData::String fullFieldName;
};

class PVRecordStructure : public PVRecordField {
public:
    virtual ~PVRecordStructure();
    // array of pointers to PVRecordField`
    PVRecordFieldPtrArray getPVRecordFields();
    epics::pvData::PVStructure &getPVStructure();
private:
    PVRecordStructure(epics::pvData::PVStructure &pvStructure,
        PVRecordStructurePtr parent,
        PVRecord &pvRecord);
    friend class PVRecord;
    friend class PVRecordField;
    epics::pvData::PVStructure &pvStructure;
    PVRecordField ** pvRecordFieldPtrArray;
};
    
class PVDatabase :
     public virtual epics::pvData::Requester,
     public virtual epics::pvData::Command,
     private epics::pvData::NoDefaultMethods
{
public:
    virtual ~PVDatabase();
    virtual epics::pvData::String getRequesterName();
    virtual void message(
        epics::pvData::String, epics::pvData::MessageType);
    epics::pvData::String getName();
    virtual void command();
    void mergeIntoMaster();
    PVRecordPtr findRecord(epics::pvData::String recordName);
    bool addRecord(PVRecordPtr record);
    bool removeRecord(PVRecord &record);
    void getRecordNames(epics::pvData::PVStringArray &result);
    epics::pvData::PVStructurePtr findStructure(
        epics::pvData::String structureName);
    bool addStructure(epics::pvData::PVStructurePtr structure);
    bool removeStructure(epics::pvData::PVStructure &structure);
    void getStructureNames(epics::pvData::PVStringArray &result);
    void addRequester(epics::pvData::Requester &requester);
    void removeRequester(epics::pvData::Requester &requester);
    void recordList(
         epics::pvData::PVStringArray &result,
         epics::pvData::String regularExpression);
    void structureList(
        epics::pvData::PVStringArray &result,
        epics::pvData::String regularExpression);
    void recordToString(
        epics::pvData::StringBuilder buf,
        epics::pvData::String regularExpression);
    void structureToString(
        epics::pvData::StringBuilder buf,
        epics::pvData::String regularExpression);
private:
    void merge(RecordMap,StructureMap);
    PVDatabase(epics::pvData::String name);
    friend class PVDatabaseFactory;
    static int messageQueueSize;
    epics::pvData::String name;
    RecordMap recordMap;
    StructureMap structureMap;
    bool isMaster;
    epics::pvData::Mutex databaseMutex;
    epics::pvData::Mutex requesterMutex;
    RequesterList requesterList;
    epics::pvData::MessageQueue messageQueue;
    epics::pvData::Executor *executor;
    epics::pvData::ExecutorNode *executorNode;
};

class PVDatabaseFactory : private epics::pvData::NoDefaultMethods {
public:
    ~PVDatabaseFactory();
    static PVDatabaseFactory &getPVDatabaseFactory();
    std::auto_ptr<PVDatabase> create(epics::pvData::String name);
    PVDatabase &getMaster();
    PVDatabase &getBeingInstalled();
private:
    PVDatabaseFactory();
    std::auto_ptr<PVDatabase> master;
    std::auto_ptr<PVDatabase> beingInstalled;
};


}}

#endif  /* PVDATABASE_H */
