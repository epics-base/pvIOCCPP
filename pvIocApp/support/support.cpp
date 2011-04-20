/* support.cpp */
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

SupportState SupportStateFunc::getSupportState(int value)
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

RecordProcess &RecordProcess::create(PVRecord &pvRecord)
{
    throw std::logic_error(String("Not Implemented"));
}

void RecordProcess::destroy()
{
    throw std::logic_error(String("Not Implemented"));
}

bool RecordProcess::isEnabled()
{
    throw std::logic_error(String("Not Implemented"));
}

bool RecordProcess::setEnabled(bool value)
{
    throw std::logic_error(String("Not Implemented"));
}

bool RecordProcess::isActive()
{
    throw std::logic_error(String("Not Implemented"));
}

PVRecord &RecordProcess::getRecord()
{
    throw std::logic_error(String("Not Implemented"));
}

bool RecordProcess::isTrace()
{
    throw std::logic_error(String("Not Implemented"));
}

bool RecordProcess::setTrace(bool value)
{
    throw std::logic_error(String("Not Implemented"));
}

SupportState RecordProcess::getSupportState()
{
    throw std::logic_error(String("Not Implemented"));
}

void RecordProcess::initialize()
{
    throw std::logic_error(String("Not Implemented"));
}

void RecordProcess::start(AfterStart &afterStart)
{
    throw std::logic_error(String("Not Implemented"));
}

void RecordProcess::stop()
{
    throw std::logic_error(String("Not Implemented"));
}

void RecordProcess::uninitialize()
{
    throw std::logic_error(String("Not Implemented"));
}

ProcessToken *RecordProcess::requestProcessToken(
        RecordProcessRequester &recordProcessRequester)
{
    throw std::logic_error(String("Not Implemented"));
}

void RecordProcess::releaseProcessToken(ProcessToken &processToken)
{
    throw std::logic_error(String("Not Implemented"));
}

void RecordProcess::forceInactive()
{
    throw std::logic_error(String("Not Implemented"));
}

String RecordProcess::getRecordProcessRequesterName()
{
    throw std::logic_error(String("Not Implemented"));
}

void RecordProcess::queueProcessRequest(ProcessToken &processToken)
{
    throw std::logic_error(String("Not Implemented"));
}

void RecordProcess::process(ProcessToken &processToken,bool leaveActive)
{
    throw std::logic_error(String("Not Implemented"));
}

void RecordProcess::process(ProcessToken &processToken,
        bool leaveActive,TimeStamp *timeStamp)
{
    throw std::logic_error(String("Not Implemented"));
}

void RecordProcess::setInactive(ProcessToken &processToken)
{
    throw std::logic_error(String("Not Implemented"));
}

void RecordProcess::processContinue(
    ProcessContinueRequester &processContinueRequester)
{
    throw std::logic_error(String("Not Implemented"));
}

void RecordProcess::setTimeStamp(TimeStamp timeStamp)
{
    throw std::logic_error(String("Not Implemented"));
}

void RecordProcess::getTimeStamp(TimeStamp timeStamp)
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

String Support::getSupportName()
{
    throw std::logic_error(String("Not Implemented"));
}

SupportState Support::getSupportState()
{
    throw std::logic_error(String("Not Implemented"));
}

PVRecordField &Support::getPVRecordField()
{
    throw std::logic_error(String("Not Implemented"));
}

void Support::initialize()
{
    throw std::logic_error(String("Not Implemented"));
}

void Support::start(AfterStart afterStart)
{
    throw std::logic_error(String("Not Implemented"));
}

void Support::stop()
{
    throw std::logic_error(String("Not Implemented"));
}

void Support::uninitialize()
{
    throw std::logic_error(String("Not Implemented"));
}

void Support::process(SupportProcessRequester &supportProcessRequester)
{
    throw std::logic_error(String("Not Implemented"));
}

void Support::setSupportState(SupportState state)
{
    throw std::logic_error(String("Not Implemented"));
}

bool Support::checkSupportState(
        SupportState expectedState,
        String message)
{
    throw std::logic_error(String("Not Implemented"));
}


}}
