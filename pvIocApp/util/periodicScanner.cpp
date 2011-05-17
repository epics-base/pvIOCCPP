/* periodicScanner.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#include <string>
#include <stdexcept>
#include <memory>

#include <pvIntrospect.h>
#include <pvData.h>
#include <noDefaultMethods.h>

#include "support.h"
#include "pvDatabase.h"
#include "util.h"

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;

PeriodicScanner::PeriodicScanner()
{
    throw std::logic_error(String("Not Implemented"));
}

PeriodicScanner::~PeriodicScanner()
{
    throw std::logic_error(String("Not Implemented"));
}

PeriodicScanner& PeriodicScanner::getPeriodicScanner()
{
    throw std::logic_error(String("Not Implemented"));
}

bool PeriodicScanner::addRecord(PVRecord &pvRecord)
{
    throw std::logic_error(String("Not Implemented"));
}

bool PeriodicScanner::removeRecord(
        PVRecord &pvRecord, double rate, ThreadPriority threadPriority)
{
    throw std::logic_error(String("Not Implemented"));
}

void PeriodicScanner::toString(StringBuilder buf)
{
    throw std::logic_error(String("Not Implemented"));
}

void PeriodicScanner::show( StringBuilder buf, ThreadPriority threadPriority)
{
    throw std::logic_error(String("Not Implemented"));
}

void PeriodicScanner::show( StringBuilder buf, double rate)
{
    throw std::logic_error(String("Not Implemented"));
}

void PeriodicScanner::show(
     StringBuilder buf, double rate, ThreadPriority threadPriority)
{
    throw std::logic_error(String("Not Implemented"));
}

}}
