/* pvDatabase.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
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

PVDatabase::PVDatabase(String name)
{
    throw std::logic_error(String("Not Implemented"));
}

PVDatabase::~PVDatabase()
{
    throw std::logic_error(String("Not Implemented"));
}

PVRecordCreate &PVRecordCreate::getPVRecordCreate()
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

PVDatabaseFactory &PVDatabaseFactory::getPVDatabaseFactory()
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

PVReplace &PVReplace::getPVReplace()
{
    throw std::logic_error(String("Not Implemented"));
}

PVReplace::PVReplace()
{
    throw std::logic_error(String("Not Implemented"));
}

PVReplace::~PVReplace()
{
    throw std::logic_error(String("Not Implemented"));
}

}}
