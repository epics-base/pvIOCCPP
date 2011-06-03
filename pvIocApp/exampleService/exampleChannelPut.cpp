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
    ExampleChannelProvider *channelProvider,
    PVServiceBase::shared_pointer const & exampleChannel,
    ChannelPutRequester::shared_pointer const &channelPutRequester)
: channelProvider(channelProvider),
  exampleChannel(exampleChannel),
  channelPutRequester(channelPutRequester),
  pvTop(channelProvider->createTop()),
  bitSet(new BitSet(pvTop->getNumberFields()))
{
printf("ExampleChannelPut::ExampleChannelPut()\n");
    bitSet.reset(new BitSet(pvTop->getNumberFields()));
    channelPutRequester->channelPutConnect(
       Status::OK,
       getPtrSelf(),
       pvTop,
       bitSet);
    exampleChannel->addChannelPut(*this);

}

ExampleChannelPut::~ExampleChannelPut()
{
printf("ExampleChannelPut::~ExampleChannelPut()\n");
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
    channelProvider->putData(pvTop.get());
    channelPutRequester->putDone(Status::OK);
    if(lastRequest) destroy();
}

void ExampleChannelPut::get()
{
    channelProvider->getData(pvTop.get(),bitSet.get());
    channelPutRequester->getDone(Status::OK);
}

}}

