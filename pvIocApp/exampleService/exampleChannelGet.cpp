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
    ExampleChannelProvider *channelProvider,
    PVServiceBase::shared_pointer const & exampleChannel,
    ChannelGetRequester::shared_pointer const &channelGetRequester)
: channelProvider(channelProvider),
  exampleChannel(exampleChannel),
  channelGetRequester(channelGetRequester),
  pvTop(channelProvider->createTop()),
  bitSet()
{
    int numFields = pvTop->getNumberFields();
printf("numFields %d\n",numFields);
printf("ExampleChannelGet::ExampleChannelGet\n");
    bitSet.reset(new BitSet(numFields));
printf("pvTop->getNumberFields %d\n",pvTop->getNumberFields());
String builder;
pvTop->toString(&builder);
printf("pvTop\n %s\n",builder.c_str());
builder.clear();
bitSet->set(1);
bitSet->toString(&builder);
printf("bitSet\n %s\n",builder.c_str());
printf("calling channelGetConnect %p\n",channelGetRequester.get());
    channelGetRequester->channelGetConnect(
       Status::OK,
       getPtrSelf(),
       pvTop,
       bitSet);
printf("calling addChannelGet\n");
    exampleChannel->addChannelGet(*this);
printf("ExampleChannelGet::ExampleChannelGet done\n");
}

ExampleChannelGet::~ExampleChannelGet()
{
printf("ExampleChannelGet destruct\n");
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
    channelProvider->getData(pvTop.get(),bitSet.get());
    channelGetRequester->getDone(Status::OK);
    if(lastRequest) destroy();
}

}}

