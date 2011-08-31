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

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>

#include <pv/exampleService.h>

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;


ExampleChannelPut::ExampleChannelPut(
    ExamplePVTop::shared_pointer const & examplePVTop,
    PVServiceBase::shared_pointer const & exampleChannel,
    ChannelPutRequester::shared_pointer const &channelPutRequester)
: examplePVTop(examplePVTop),
  exampleChannel(exampleChannel),
  channelPutRequester(channelPutRequester),
  pvTop(),
  bitSet()
{
}

ExampleChannelPut::~ExampleChannelPut()
{
}

bool ExampleChannelPut::init(PVStructure::shared_pointer const & pvRequest)
{
    pvTop.reset(examplePVTop->createTop());
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
    examplePVTop->putData(pvTop.get());
    channelPutRequester->putDone(Status::OK);
    if(lastRequest) destroy();
}

void ExampleChannelPut::get()
{
    examplePVTop->getData(pvTop.get(),bitSet.get());
    channelPutRequester->getDone(Status::OK);
}

}}

