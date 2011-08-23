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


#include <pv/pvData.h>
#include <pv/pvAccess.h>

#include <pv/exampleChannel.h>


namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;

ExampleChannelGet::ExampleChannelGet(
    ExamplePVTop::shared_pointer const & examplePVTop,
    PVServiceBase::shared_pointer const & exampleChannel,
    ChannelGetRequester::shared_pointer const &channelGetRequester)
: firstTime(true),
  examplePVTop(examplePVTop),
  exampleChannel(exampleChannel),
  channelGetRequester(channelGetRequester),
  pvTop(),
  bitSet()
{
}

ExampleChannelGet::~ExampleChannelGet()
{
}

bool ExampleChannelGet::init(PVStructure::shared_pointer const & pvRequest)
{
    pvTop.reset(examplePVTop->createTop());
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
    exampleChannel->removeChannelGet(*this);
}

void ExampleChannelGet::get(bool lastRequest)
{
    examplePVTop->getData(pvTop.get(),bitSet.get());
    if(firstTime) {
        firstTime = false;
        bitSet->clear();
        bitSet->set(0);
    }
    channelGetRequester->getDone(Status::OK);
    if(lastRequest) destroy();
}

}}

