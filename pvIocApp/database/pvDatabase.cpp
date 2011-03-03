/* pvDatabase.cpp */
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

PVDatabase::PVDatabase(String name)
{
    throw std::logic_error(String("Not Implemented"));
}

PVDatabase::~PVDatabase()
{
    throw std::logic_error(String("Not Implemented"));
}

String PVDatabase::getName()
{
    throw std::logic_error(String("Not Implemented"));
}

void PVDatabase::mergeIntoMaster()
{
    throw std::logic_error(String("Not Implemented"));
}

PVRecord *PVDatabase::findRecord(String recordName)
{
    throw std::logic_error(String("Not Implemented"));
}

bool PVDatabase::addRecord(std::auto_ptr<PVRecord> record)
{
    throw std::logic_error(String("Not Implemented"));
}

bool PVDatabase::removeRecord(PVRecord &record)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVDatabase::getRecordNames(PVStringArray &result)
{
    throw std::logic_error(String("Not Implemented"));
}

PVRecord * const * PVDatabase::getRecords()
{
    throw std::logic_error(String("Not Implemented"));
}

PVStructure *PVDatabase::findStructure(String structureName)
{
    throw std::logic_error(String("Not Implemented"));
}

bool PVDatabase::addStructure(std::auto_ptr<PVStructure> structure)
{
    throw std::logic_error(String("Not Implemented"));
}

bool PVDatabase::removeStructure(PVStructure &structure)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVDatabase::getStructureNames(PVStringArray &result)
{
    throw std::logic_error(String("Not Implemented"));
}

PVStructure * const *PVDatabase::getStructures()
{
    throw std::logic_error(String("Not Implemented"));
}

void PVDatabase::message(
        String message,
        MessageType messageType)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVDatabase::addRequester(Requester &requester)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVDatabase::removeRequester(Requester &requester)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVDatabase::recordList(
         PVStringArray &result,
         String regularExpression)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVDatabase::structureList(
        PVStringArray &result,
        String regularExpression)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVDatabase::recordToString(
        StringBuilder buf,
        String regularExpression)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVDatabase::structureToString(
        StringBuilder buf,
        String regularExpression)
{
    throw std::logic_error(String("Not Implemented"));
}


PVRecordCreate::PVRecordCreate()
{
    throw std::logic_error(String("Not Implemented"));
}

PVRecordCreate::~PVRecordCreate()
{
    throw std::logic_error(String("Not Implemented"));
}

PVRecordCreate &PVRecordCreate::getPVRecordCreate()
{
    throw std::logic_error(String("Not Implemented"));
}

std::auto_ptr<PVRecord>PVRecordCreate::createPVRecord(
    String recordName,
    std::auto_ptr<PVStructure> pvStructure)
{
    throw std::logic_error(String("Not Implemented"));
}

std::auto_ptr<PVStructure> PVRecordCreate::createPVStructure(
    PVStructure *parent,
    String fieldName,
    PVDatabase * const pvDatabase,
    String structureName)
{
    throw std::logic_error(String("Not Implemented"));
}

PVDatabaseFactory::PVDatabaseFactory()
{
    throw std::logic_error(String("Not Implemented"));
}

PVDatabaseFactory::~PVDatabaseFactory()
{
    throw std::logic_error(String("Not Implemented"));
}

PVDatabaseFactory &PVDatabaseFactory::getPVDatabaseFactory()
{
    throw std::logic_error(String("Not Implemented"));
}

std::auto_ptr<PVDatabase> PVDatabaseFactory::create(String name)
{
    throw std::logic_error(String("Not Implemented"));
}

PVDatabase &PVDatabaseFactory::getMaster()
{
    throw std::logic_error(String("Not Implemented"));
}

PVDatabase *PVDatabaseFactory::getBeingInstalled()
{
    throw std::logic_error(String("Not Implemented"));
}


}}
