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

#include "pvIntrospect.h"
#include "pvData.h"
#include "pvAccess.h"
#include "noDefaultMethods.h"
#include "support.h"

namespace epics { namespace pvIOC { 

class PVDatabase;
class PVListener;
class PVRecordClient;
class PVRecord;
class PVRecordField;
class PVRecordStructure;
class PVReplace;
class PVRecordCreate;
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

class PVRecord : private epics::pvData::NoDefaultMethods {
public:
    RecordProcess &getRecordProcess();
    void setRecordProcess(RecordProcess &recordProcess);
    PVRecordField *findPVRecordField(epics::pvData::PVField &pvField);
    PVRecordStructure &getPVRecordStructure();
    epics::pvData::String getRecordName();
    void message(
        epics::pvData::String message,
        epics::pvData::MessageType messageType);
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
    PVRecord(
        epics::pvData::String recordName,
        std::auto_ptr<epics::pvData::PVStructure> pvStructure);
    ~PVRecord();
    friend class PVRecordCreate;
    // TBD
};

class PVRecordField : public epics::pvData::Requester , private epics::pvData::NoDefaultMethods {
public:
    Support *getSupport();
    void setSupport(Support &support);
    PVRecordStructure *getParent();
    epics::pvData::PVField &getPVField();
    void replacePVField(std::auto_ptr<epics::pvData::PVField> pvField);
    epics::pvData::String getFullFieldName();
    epics::pvData::String getFullName();
    PVRecord &getPVRecord();
    void renameField(epics::pvData::String newName);
    bool addListener(PVListener &pvListener);
    void removeListener(PVListener &vListener);
    void postPut();
private:
    PVRecordField(epics::pvData::PVField &pvField,
        PVRecordStructure &parent,
        PVRecord &pvRecord);
    ~PVRecordField();
    friend class PVRecord;
    friend class PVRecordStructure;
    // TBD
};

class PVRecordStructure : public PVRecordField {
public:
    // array of pointers to PVRecordField`
    PVRecordField * const *getPVRecordFields();
    epics::pvData::PVStructure &getPVStructure();
private:
    PVRecordStructure(epics::pvData::PVStructure &pvStructure,
        PVRecordStructure &parent,
        PVRecord &pvRecord);
    ~PVRecordStructure();
    friend class PVRecord;
    friend class PVRecordField;
    // TBD
};
    
class PVDatabase : public epics::pvData::Requester , private epics::pvData::NoDefaultMethods {
public:
    epics::pvData::String getName();
    void mergeIntoMaster();
    PVRecord *findRecord(epics::pvData::String recordName);
    bool addRecord(std::auto_ptr<PVRecord> record);
    bool removeRecord(PVRecord &record);
    void getRecordNames(epics::pvData::PVStringArray &result);
    // pointer to array of PVRecord
    PVRecord * const * getRecords();
    epics::pvData::PVStructure *findStructure(
        epics::pvData::String structureName);
    bool addStructure(std::auto_ptr<epics::pvData::PVStructure> structure);
    bool removeStructure(epics::pvData::PVStructure &structure);
    void getStructureNames(epics::pvData::PVStringArray &result);
    // pointer to array of PVStructure
    epics::pvData::PVStructure * const * getStructures();
    void message(
        epics::pvData::String message,
        epics::pvData::MessageType messageType);
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
    PVDatabase(epics::pvData::String name);
    ~PVDatabase();
    friend class PVDatabaseFactory;
    // TBD
};

class PVRecordCreate : private epics::pvData::NoDefaultMethods {
public:
    static PVRecordCreate & getPVRecordCreate();
    std::auto_ptr<PVRecord> createPVRecord(
        epics::pvData::String recordName,
        std::auto_ptr<epics::pvData::PVStructure> pvStructure);
    std::auto_ptr<epics::pvData::PVStructure> createPVStructure(
        epics::pvData::PVStructure *parent,
        epics::pvData::String fieldName,
        PVDatabase * const pvDatabase,
        epics::pvData::String structureName);
private:
    PVRecordCreate();
    ~PVRecordCreate();
};

class PVDatabaseFactory : private epics::pvData::NoDefaultMethods {
public:
    static PVDatabaseFactory &getPVDatabaseFactory();
    std::auto_ptr<PVDatabase> create(epics::pvData::String name);
    PVDatabase &getMaster();
    PVDatabase *getBeingInstalled();
private:
    PVDatabaseFactory();
    ~PVDatabaseFactory();
};

class PVReplace : private epics::pvData::NoDefaultMethods {
public:
    static PVReplace & getPVReplace();
    void replace(PVDatabase &pvDatabase);
    void replace(PVDatabase &pvDatabase,
        PVRecord &pvRecord,
        PVRecordStructure &pvRecordStructure);
    void replace(PVDatabase &pvDatabase,
        PVRecord &pvRecord,
        PVRecordField &pvRecordField);
private:
    PVReplace();
    ~PVReplace();
};


}}

#endif  /* PVDATABASE_H */
