/* pvReplace.cpp */
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


PVReplace::PVReplace()
{
    throw std::logic_error(String("Not Implemented"));
}

PVReplace::~PVReplace()
{
    throw std::logic_error(String("Not Implemented"));
}

PVReplace &PVReplace::getPVReplace()
{
    throw std::logic_error(String("Not Implemented"));
}

void PVReplace::replace(PVDatabase &pvDatabase)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVReplace::replace(PVDatabase &pvDatabase,
    PVRecord &pvRecord,
    PVRecordStructure &pvRecordStructure)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVReplace::replace(PVDatabase &pvDatabase,
    PVRecord &pvRecord,
    PVRecordField &pvRecordField)
{
    throw std::logic_error(String("Not Implemented"));
}


}}
