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

namespace epics { namespace pvData { 

SupportState SupportStateFunc::getSupportState(int value)
{
    throw std::logic_error(String("Not Implemented"));
}

RecordProcess &RecordProcess::create(PVRecord &pvRecord)
{
    throw std::logic_error(String("Not Implemented"));
}

void RecordProcess::destroy()
{
    throw std::logic_error(String("Not Implemented"));
}

RecordProcess::RecordProcess(PVRecord &pvRecord)
{
    throw std::logic_error(String("Not Implemented"));
}

RecordProcess::~RecordProcess()
{
    throw std::logic_error(String("Not Implemented"));
}

Support::Support(String name,PVRecordField &pvRecordField)
{
    throw std::logic_error(String("Not Implemented"));
}

Support::~Support()
{
    throw std::logic_error(String("Not Implemented"));
}


}}
