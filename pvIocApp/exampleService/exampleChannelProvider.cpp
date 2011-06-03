/* exampleChannelProvider.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#include <string>
#include <stdexcept>
#include <memory>


#include "exampleChannel.h"

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;

ExampleChannelProvider::ExampleChannelProvider(String channelName)
: PVServiceBaseProvider("exampleChannel"),
  channelName(channelName),
  pvTop()
{
printf("ExampleChannelProvider::ExampleChannelProvider\n");
    PVDataCreate *pvDataCreate = getPVDataCreate();
    PVScalar *pvScalar = pvDataCreate->createPVScalar(0,"value",pvDouble);
    PVFieldPtrArray pvFields = new PVFieldPtr[1];
    pvFields[0] = pvScalar;
    pvTop = PVStructure::shared_pointer(
        pvDataCreate->createPVStructure(0,String(),1,pvFields));
}

ExampleChannelProvider::~ExampleChannelProvider()
{
printf("ExampleChannelProvider::~ExampleChannelProvider\n");
}

ChannelFind::shared_pointer ExampleChannelProvider::channelFind(
    String name,
    ChannelFindRequester::shared_pointer const &channelFindRequester)
{
    channelFound(
        ((channelName.compare(name)==0) ? true : false),
        channelFindRequester);
    return ChannelFind::shared_pointer();
}

Channel::shared_pointer ExampleChannelProvider::createChannel(
    String name,
    ChannelRequester::shared_pointer  const &channelRequester,
    short priority,
    String address)
{
    if(channelName.compare(name)!=0) {
        channelNotCreated(channelRequester);
        return Channel::shared_pointer();
    }
    ExampleChannel *exampleChannel = new ExampleChannel(
            this,
            getChannelProvider(),
            channelRequester,channelName);
    PVServiceBase::shared_pointer channel(exampleChannel);
    channelCreated(channel);
    return channel;
}

FieldConstPtr ExampleChannelProvider::getField()
{
    return pvTop->getField();
}

PVStructure * ExampleChannelProvider::createTop()
{
   return getPVDataCreate()->createPVStructure(0,String(),pvTop.get());
}

void ExampleChannelProvider::getData(PVStructure *pvStructure,BitSet *bitSet)
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

void ExampleChannelProvider::putData(PVStructure *pvStructure)
{
    PVDouble *pv0 = pvTop->getDoubleField("value");
    PVDouble *pv1 = pvStructure->getDoubleField("value");
    pv0->put(pv1->get());
}

}}
