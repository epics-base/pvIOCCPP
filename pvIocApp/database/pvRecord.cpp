/* pvRecord.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/* Marty Kraimer 2011.03 */
#include <string>
#include <stdexcept>
#include <memory>

#include "pvIntrospect.h"
#include "pvData.h"
#include "noDefaultMethods.h"
#include "support.h"
#include "pvDatabase.h"

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;

PVRecord::PVRecord(String recordName,std::auto_ptr<PVStructure> pvStructure)
{
    throw std::logic_error(String("Not Implemented"));
}

PVRecord::~PVRecord()
{
    throw std::logic_error(String("Not Implemented"));
}

RecordProcess &PVRecord::getRecordProcess()
{
    throw std::logic_error(String("Not Implemented"));
}

void PVRecord::setRecordProcess(RecordProcess &recordProcess)
{
    throw std::logic_error(String("Not Implemented"));
}

PVRecordStructure &getPVRecordStructure()
{
    throw std::logic_error(String("Not Implemented"));
}

PVRecordField *PVRecord::findPVRecordField(PVField &pvField)
{
    throw std::logic_error(String("Not Implemented"));
}

String PVRecord::getRecordName()
{
    throw std::logic_error(String("Not Implemented"));
}

void PVRecord::message(
        String message,
        MessageType messageType)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVRecord::removeRequester(Requester &requester)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVRecord::lock()
{
    throw std::logic_error(String("Not Implemented"));
}

void PVRecord::unlock()
{
    throw std::logic_error(String("Not Implemented"));
}

void PVRecord::lockOtherRecord(PVRecord &otherRecord)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVRecord::beginGroupPut()
{
    throw std::logic_error(String("Not Implemented"));
}

void PVRecord::endGroupPut()
{
    throw std::logic_error(String("Not Implemented"));
}

void PVRecord::registerListener(PVListener &pvListener)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVRecord::unregisterListener(PVListener &pvListener)
{
    throw std::logic_error(String("Not Implemented"));
}

bool PVRecord::isRegisteredListener(PVListener &pvListener)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVRecord::removeEveryListener()
{
    throw std::logic_error(String("Not Implemented"));
}

void PVRecord::registerClient(PVRecordClient &pvRecordClient)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVRecord::unregisterClient(PVRecordClient &pvRecordClient)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVRecord::detachClients()
{
    throw std::logic_error(String("Not Implemented"));
}

int PVRecord::getNumberClients()
{
    throw std::logic_error(String("Not Implemented"));
}


PVRecordField::PVRecordField(PVField &pvField,
    PVRecordStructure &parent,
    PVRecord &pvRecord)
{
    throw std::logic_error(String("Not Implemented"));
}

PVRecordField::~PVRecordField()
{
    throw std::logic_error(String("Not Implemented"));
}

Support *PVRecordField::getSupport()
{
    throw std::logic_error(String("Not Implemented"));
}

void PVRecordField::setSupport(Support &support)
{
    throw std::logic_error(String("Not Implemented"));
}

PVRecordStructure *PVRecordField::getParent()
{
    throw std::logic_error(String("Not Implemented"));
}

PVField &PVRecordField::getPVField()
{
    throw std::logic_error(String("Not Implemented"));
}

void PVRecordField::replacePVField(
    std::auto_ptr<PVField> pvField)
{
    throw std::logic_error(String("Not Implemented"));
}

String PVRecordField::getFullFieldName()
{
    throw std::logic_error(String("Not Implemented"));
}

String PVRecordField::getFullName()
{
    throw std::logic_error(String("Not Implemented"));
}

PVRecord &PVRecordField::getPVRecord()
{
    throw std::logic_error(String("Not Implemented"));
}

void PVRecordField::renameField(String newName)
{
    throw std::logic_error(String("Not Implemented"));
}

bool PVRecordField::addListener(PVListener &pvListener)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVRecordField::removeListener(PVListener &vListener)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVRecordField::postPut()
{
    throw std::logic_error(String("Not Implemented"));
}


PVRecordStructure::PVRecordStructure(PVStructure &pvStructure,
    PVRecordStructure &parent,
    PVRecord &pvRecord)
: PVRecordField(pvStructure,parent,pvRecord)
{
    throw std::logic_error(String("Not Implemented"));
}

PVRecordStructure::~PVRecordStructure()
{
    throw std::logic_error(String("Not Implemented"));
}

PVRecordField * const *PVRecordStructure::getPVRecordFields()
{
    throw std::logic_error(String("Not Implemented"));
}

PVStructure &PVRecordStructure::getPVStructure()
{
    throw std::logic_error(String("Not Implemented"));
}


}}
