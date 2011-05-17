/* pvDatabase.cpp */
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
#include "install.h"

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;


SupportCreation::SupportCreation(
    PVDatabase &pvDatabase,Requester &requester)
{
    throw std::logic_error(String("Not Implemented"));
}

SupportCreation::~SupportCreation()
{
    throw std::logic_error(String("Not Implemented"));
}

bool SupportCreation::createSupport()
{
    throw std::logic_error(String("Not Implemented"));
}

bool SupportCreation::initializeSupport()
{
    throw std::logic_error(String("Not Implemented"));
}

bool SupportCreation::startSupport(AfterStart &afterStart)
{
    throw std::logic_error(String("Not Implemented"));
}


}}
