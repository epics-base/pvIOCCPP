/* afterStart.cpp */
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

AfterStartNode::AfterStartNode()
{
    throw std::logic_error(String("Not Implemented"));
}

AfterStartNode::~AfterStartNode()
{
    throw std::logic_error(String("Not Implemented"));
}

AfterStart::AfterStart()
{
    throw std::logic_error(String("Not Implemented"));
}

AfterStart::~AfterStart()
{
    throw std::logic_error(String("Not Implemented"));
}

std::auto_ptr<AfterStartNode> AfterStart::createNode(
    AfterStartRequester &requester)
{
    throw std::logic_error(String("Not Implemented"));
}

void AfterStart::newAfterStartRegister(NewAfterStartRequester &requester)
{
    throw std::logic_error(String("Not Implemented"));
}

void AfterStart::newAfterStartUnregister(NewAfterStartRequester &requester)
{
    throw std::logic_error(String("Not Implemented"));
}

void AfterStart::requestCallback(
    AfterStartNode &node,
    bool afterMerge,
    ThreadPriority priority)
{
    throw std::logic_error(String("Not Implemented"));
}

void AfterStart::callRequesters(bool afterMerge)
{
    throw std::logic_error(String("Not Implemented"));
}

void AfterStart::done(AfterStartNode &node)
{
    throw std::logic_error(String("Not Implemented"));
}

void AfterStart::doneAndRequest(
    AfterStartNode &node,
    bool afterMerge,
    ThreadPriority priority)
{
    throw std::logic_error(String("Not Implemented"));
}


}}
