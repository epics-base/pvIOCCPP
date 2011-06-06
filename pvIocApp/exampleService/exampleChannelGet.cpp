/* exampleChannelGet.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/* Marty Kraimer 2011.03 */
/* This connects to a V3 record and presents the data as a PVStructure
 * It provides access to  value, alarm, display, and control.
 */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <stdexcept>
#include <memory>


#include <pvData.h>
#include <pvAccess.h>

#include "exampleChannel.h"


namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;

ExampleChannelGet::ExampleChannelGet(
    ExampleChannelProvider *exampleChannelProvider,
    PVServiceBase::shared_pointer const & exampleChannel,
    ChannelGetRequester::shared_pointer const &channelGetRequester)
: exampleChannelProvider(exampleChannelProvider),
  exampleChannel(exampleChannel),
  channelGetRequester(channelGetRequester),
  pvTop(),
  bitSet()
{ }

ExampleChannelGet::~ExampleChannelGet()
{
printf("ExampleChannelGet destruct\n");
}

bool ExampleChannelGet::init(PVStructure::shared_pointer const & pvRequest)
{
    pvTop.reset(exampleChannelProvider->createTop());
    int numFields = pvTop->getNumberFields();
    bitSet.reset(new BitSet(numFields));
    channelGetRequester->channelGetConnect(
       Status::OK,
       getPtrSelf(),
       pvTop,
       bitSet);
    return true;
}

String ExampleChannelGet::getRequesterName() {
    return channelGetRequester->getRequesterName();
}

void ExampleChannelGet::message(String message,MessageType messageType)
{
    channelGetRequester->message(message,messageType);
}

void ExampleChannelGet::destroy() {
printf("ExampleChannelGet::destroy\n");
    exampleChannel->removeChannelGet(*this);
}

void ExampleChannelGet::get(bool lastRequest)
{
    exampleChannelProvider->getData(pvTop.get(),bitSet.get());
    channelGetRequester->getDone(Status::OK);
    if(lastRequest) destroy();
}

}}

