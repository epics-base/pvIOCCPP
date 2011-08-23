/* exampleChannel.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#include <string>
#include <stdexcept>
#include <memory>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/noDefaultMethods.h>

#include <pv/exampleChannel.h>

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;

ExampleChannel::ExampleChannel(
    PVServiceBaseProvider::shared_pointer const & channelProvider,
    ChannelRequester::shared_pointer const & requester,
    String name,
    ExamplePVTop::shared_pointer const &examplePVTop
)
:  PVServiceBase(channelProvider,requester,name),
   examplePVTop(examplePVTop)
{
}

void ExampleChannel::init()
{
}

ExampleChannel::~ExampleChannel()
{
}

void ExampleChannel::getField(GetFieldRequester::shared_pointer const &requester,
        String subField)
{
    requester->getDone(Status::OK,examplePVTop->getField());
}


ChannelGet::shared_pointer ExampleChannel::createChannelGet(
        ChannelGetRequester::shared_pointer const &channelGetRequester,
        PVStructure::shared_pointer const &pvRequest)
{
    ExampleChannelGet * exampleChannelGet = new ExampleChannelGet(
        examplePVTop, getPtrSelf(),channelGetRequester);
    ChannelGet::shared_pointer channelGet(exampleChannelGet);
    if(exampleChannelGet->init(pvRequest)) addChannelGet(*exampleChannelGet);
    return channelGet;
}

ChannelPut::shared_pointer ExampleChannel::createChannelPut(
        ChannelPutRequester::shared_pointer const &channelPutRequester,
        PVStructure::shared_pointer const &pvRequest)
{
    ExampleChannelPut * exampleChannelPut = new ExampleChannelPut(
        examplePVTop, getPtrSelf(),channelPutRequester);
    ChannelPut::shared_pointer channelPut(exampleChannelPut);
    if(exampleChannelPut->init(pvRequest)) addChannelPut(*exampleChannelPut);
    return channelPut;
}



void ExampleChannel::printInfo()
{
    printf("ExampleChannel provides access to Example records\n");
}

void ExampleChannel::printInfo(StringBuilder out)
{
    *out += "ExampleChannel provides access to Example records";
}

}}
