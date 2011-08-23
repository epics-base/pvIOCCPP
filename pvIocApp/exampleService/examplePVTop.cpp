/* examplePVTop.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#include <string>
#include <stdexcept>
#include <memory>


#include <pv/exampleChannel.h>

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;

ExamplePVTop::ExamplePVTop(String channelName)
: channelName(channelName),
  pvTop()
{
}

ExamplePVTop::~ExamplePVTop()
{
}

void ExamplePVTop::destroy()
{
}

void ExamplePVTop::init()
{
    PVDataCreate *pvDataCreate = getPVDataCreate();
    PVScalar *pvScalar = pvDataCreate->createPVScalar(0,"value",pvDouble);
    PVFieldPtrArray pvFields = new PVFieldPtr[1];
    pvFields[0] = pvScalar;
    pvTop.reset(pvDataCreate->createPVStructure(0,String(),1,pvFields));
}

PVServiceBase::shared_pointer ExamplePVTop::createChannel(
    ChannelRequester::shared_pointer  const &channelRequester,
    PVServiceProvider::shared_pointer const &provider)
{
    ExampleChannel *exampleChannel = new ExampleChannel(
            provider,
            channelRequester,channelName,getPtrSelf());
    PVServiceBase::shared_pointer channel(exampleChannel);
    exampleChannel->init();
    return channel;
}

String ExamplePVTop::getName()
{
    return channelName;
}

FieldConstPtr ExamplePVTop::getField()
{
    return pvTop->getField();
}


PVStructure * ExamplePVTop::createTop()
{
   return getPVDataCreate()->createPVStructure(0,String(),pvTop.get());
}

void ExamplePVTop::getData(PVStructure *pvStructure,BitSet *bitSet)
{
    bitSet->clear();
    PVDouble *pv0 = pvTop->getDoubleField("value");
    PVDouble *pv1 = pvStructure->getDoubleField("value");
    double val0 = pv0->get();
    double val1 = pv1->get();
    if(val0!=val1) {
        bitSet->set(0);
        pv1->put(val0);
    }
}

void ExamplePVTop::putData(PVStructure *pvStructure)
{
    PVDouble *pv0 = pvTop->getDoubleField("value");
    PVDouble *pv1 = pvStructure->getDoubleField("value");
    pv0->put(pv1->get());
}

}}
