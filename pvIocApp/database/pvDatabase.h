/* pvDatabase.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef PVDATABASE_H
#define PVDATABASE_H
#include <string>
#include <stdexcept>
#include <memory>

#include "pvIntrospect.h"
#include "pvData.h"
#include "noDefaultMethods.h"
#include "support.h"

namespace epics { namespace pvData { 

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

class PVRecord : private NoDefaultMethods {
public:
    RecordProcess &getRecordProcess();
    void setRecordProcess(RecordProcess &recordProcess);
    PVRecordField *findPVRecordField(PVField &pvField);
    PVRecordStructure &getPVRecordStructure();
    String getRecordName();
    void message(String message, MessageType messageType);
    void addRequester(Requester &requester);
    void removeRequester(Requester &requester);
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
    PVRecord(String recordName,std::auto_ptr<PVStructure> pvStructure);
    ~PVRecord();
    friend class PVRecordCreate;
    // TBD
};

class PVRecordField : public Requester , private NoDefaultMethods {
public:
    Support *getSupport();
    void setSupport(Support &support);
    PVRecordStructure *getParent();
    PVField &getPVField();
    void replacePVField(std::auto_ptr<PVField> pvField);
    String getFullFieldName();
    String getFullName();
    PVRecord &getPVRecord();
    void renameField(String newName);
    bool addListener(PVListener &pvListener);
    void removeListener(PVListener &vListener);
    void postPut();
private:
    PVRecordField(PVField &pvField,
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
    PVRecordField **getPVRecordFields();
    PVStructure &getPVStructure();
private:
    PVRecordStructure(PVStructure &pvStructure,
        PVRecordStructure &parent,
        PVRecord &pvRecord);
    ~PVRecordStructure();
    friend class PVRecord;
    friend class PVRecordField;
    // TBD
};
    
class PVDatabase : public Requester , private NoDefaultMethods {
public:
    String getName();
    void mergeIntoMaster();
    PVRecord *findRecord(String recordName);
    bool addRecord(std::auto_ptr<PVRecord> record);
    bool removeRecord(PVRecord &record);
    void getRecordNames(PVStringArray &result);
    // pointer to array of PVRecord
    PVRecord ** getRecords();
    PVStructure *findStructure(String structureName);
    bool addStructure(std::auto_ptr<PVStructure> structure);
    bool removeStructure(PVStructure &structure);
    void getStructureNames(PVStringArray &result);
    // pointer to array of PVStructure
    PVStructure ** getStructures();
    void message(String message, MessageType messageType);
    void addRequester(Requester &requester);
    void removeRequester(Requester &requester);
    void recordList(PVStringArray &result,String regularExpression);
    void structureList(PVStringArray &result,String regularExpression);
    void recordToString(StringBuilder buf, String regularExpression);
    void structureToString(StringBuilder buf, String regularExpression);
private:
    PVDatabase(String name);
    ~PVDatabase();
    friend class PVDatabaseFactory;
    // TBD
};

class PVRecordCreate : private NoDefaultMethods {
public:
    static PVRecordCreate & getPVRecordCreate();
    std::auto_ptr<PVRecord> createPVRecord(
        String recordName,std::auto_ptr<PVStructure> pvStructure);
    std::auto_ptr<PVStructure> createPVStructure(
        PVStructure *parent, String fieldName,
        PVDatabase *pvDatabase, String structureName);
private:
    PVRecordCreate();
    ~PVRecordCreate();
};

class PVDatabaseFactory : private NoDefaultMethods {
public:
    static PVDatabaseFactory &getPVDatabaseFactory();
    PVDatabase &create(String name);
    PVDatabase &getMaster();
    PVDatabase *getBeingInstalled();
private:
    PVDatabaseFactory();
    ~PVDatabaseFactory();
};

class PVReplace : private NoDefaultMethods {
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
