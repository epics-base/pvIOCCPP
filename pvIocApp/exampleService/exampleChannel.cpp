/* exampleChannel.cpp */
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

#include "exampleChannel.h"

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;

ExampleChannel::ExampleChannel(
    ExampleChannelProvider *exampleChannelProvider,
    PVServiceBaseProvider::shared_pointer const & channelProvider,
    ChannelRequester::shared_pointer const & requester,
    String name
)
:  PVServiceBase(channelProvider,requester,name),
   channelProvider(exampleChannelProvider)
{
printf("ExampleChannel::ExampleChannel\n");
    PVServiceBase::shared_pointer channel(this);
    channelProvider->channelCreated(channel);
}

ExampleChannel::~ExampleChannel()
{
printf("ExampleChannel::~ExampleChannel\n");
}

void ExampleChannel::getField(GetFieldRequester::shared_pointer const &requester,
        String subField)
{
    requester->getDone(Status::OK,channelProvider->getField());
}


ChannelGet::shared_pointer ExampleChannel::createChannelGet(
        ChannelGetRequester::shared_pointer const &channelGetRequester,
        PVStructure::shared_pointer const &pvRequest)
{
    ExampleChannelGet * exampleChannelGet = new ExampleChannelGet(
        channelProvider, getPtrSelf(),channelGetRequester);
    ChannelGet::shared_pointer channelGet(exampleChannelGet);
    return channelGet;
}

ChannelPut::shared_pointer ExampleChannel::createChannelPut(
        ChannelPutRequester::shared_pointer const &channelPutRequester,
        PVStructure::shared_pointer const &pvRequest)
{
    ExampleChannelPut * exampleChannelPut = new ExampleChannelPut(
        channelProvider, getPtrSelf(),channelPutRequester);
    ChannelPut::shared_pointer channelPut(exampleChannelPut);
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
