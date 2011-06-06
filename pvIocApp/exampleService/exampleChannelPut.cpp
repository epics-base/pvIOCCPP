/* exampleChannelPut.cpp */
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

#include <dbAccess.h>
#include <dbEvent.h>
#include <dbNotify.h>
#include <dbCommon.h>

#include <pvIntrospect.h>
#include <pvData.h>
#include <pvAccess.h>

#include "exampleChannel.h"

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;


ExampleChannelPut::ExampleChannelPut(
    ExampleChannelProvider *exampleChannelProvider,
    PVServiceBase::shared_pointer const & exampleChannel,
    ChannelPutRequester::shared_pointer const &channelPutRequester)
: exampleChannelProvider(exampleChannelProvider),
  exampleChannel(exampleChannel),
  channelPutRequester(channelPutRequester),
  pvTop(),
  bitSet()
{
printf("ExampleChannelPut::ExampleChannelPut()\n");
}

ExampleChannelPut::~ExampleChannelPut()
{
printf("ExampleChannelPut::~ExampleChannelPut()\n");
}

bool ExampleChannelPut::init(PVStructure::shared_pointer const & pvRequest)
{
    pvTop.reset(exampleChannelProvider->createTop());
    bitSet.reset(new BitSet(pvTop->getNumberFields()));
    channelPutRequester->channelPutConnect(
       Status::OK,
       getPtrSelf(),
       pvTop,
       bitSet);
    return true;
}

String ExampleChannelPut::getRequesterName() {
    return channelPutRequester->getRequesterName();
}

void ExampleChannelPut::message(String message,MessageType messageType)
{
    channelPutRequester->message(message,messageType);
}

void ExampleChannelPut::destroy() {
    exampleChannel->removeChannelPut(*this);
}

void ExampleChannelPut::put(bool lastRequest)
{
    exampleChannelProvider->putData(pvTop.get());
    channelPutRequester->putDone(Status::OK);
    if(lastRequest) destroy();
}

void ExampleChannelPut::get()
{
    exampleChannelProvider->getData(pvTop.get(),bitSet.get());
    channelPutRequester->getDone(Status::OK);
}

}}

