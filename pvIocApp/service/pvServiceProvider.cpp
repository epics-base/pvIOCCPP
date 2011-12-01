/* pvServiceProvider.cpp */
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
#include <pv/lock.h>
#include <epicsExit.h>
#include <dbAccess.h>

#include <pv/support.h>
#include <pv/pvDatabase.h>
#include <pv/pvServiceProvider.h>

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;

static ChannelBaseProvider::shared_pointer pvServiceProvider;

class ServicePVTopBase
{
public:
    ServicePVTopBase(ServicePVTop::shared_pointer servicePVTop)
    : servicePVTop(servicePVTop){}
    ServicePVTop::shared_pointer servicePVTop;
    epics::pvData::LinkedList<epics::pvAccess::Channel::shared_pointer> channelList;
};

typedef LinkedListNode<ServicePVTopBase> TopListNode;
typedef LinkedList<ServicePVTopBase> TopList;

ChannelBaseProvider::shared_pointer PVServiceProvider::getPVServiceProvider()
{
    static Mutex mutex;
    Lock xx(mutex);

    if(pvServiceProvider.get()==0) {
        pvServiceProvider = ChannelBaseProvider::shared_pointer(
            new PVServiceProvider());
        pvServiceProvider->init();
    }
    return pvServiceProvider;
}

PVServiceProvider::PVServiceProvider()
: ChannelBaseProvider("pvService"),
  topList()
{
//printf("PVServiceProvider::PVServiceProvider\n");
}

PVServiceProvider::~PVServiceProvider()
{
printf("PVServiceProvider::~PVServiceProvider\n");
destroy();
pvServiceProvider.reset();
}

void PVServiceProvider::destroy()
{
printf("PVServiceProvider::destroy\n");
    Lock xx(mutex);
    while(true) {
        TopListNode *node = topList.removeHead();
        if(node==0) break;
        ServicePVTopBase &pvTop = node->getObject();
        pvTop.servicePVTop->destroy();
        delete node;
        delete &pvTop;
    }
}

ChannelFind::shared_pointer PVServiceProvider::channelFind(
    String name,
    ChannelFindRequester::shared_pointer const &channelFindRequester)
{
//printf("PVServiceProvider::channelFind\n");
    TopListNode *node = topList.getHead();
    bool result = false;
    while(node!=0) {
        ServicePVTopBase &pvTop = node->getObject();
        if((pvTop.servicePVTop->getName().compare(name)==0)) {
            result = true;
            break;
        }
        node = topList.getNext(*node);
    }
    channelFound(result,channelFindRequester);
    return ChannelFind::shared_pointer();
}

Channel::shared_pointer PVServiceProvider::createChannel(
    String channelName,ChannelRequester::shared_pointer  const &channelRequester,
    short priority,
    String address)
{
//printf("PVServiceProvider::createChannel\n");
    TopListNode *node = topList.getHead();
    while(node!=0) {
        ServicePVTopBase &pvTopBase = node->getObject();
        ServicePVTop::shared_pointer pvTop = pvTopBase.servicePVTop;
//printf("channelName %s getName %s\n",channelName.c_str(),pvTop->getName().c_str());
        if((pvTop->getName().compare(channelName)==0)) {
//printf("calling pvTop.createChannel\n");
            ChannelBase::shared_pointer channel =
                 pvTop->createChannel(channelRequester,
                      static_cast<ChannelProvider::shared_pointer>(
                          getPtrSelf()));
//printf("calling channelCreated(channel) %p\n",channel.get());
            channelCreated(channel);
            return channel;
        }
        node = topList.getNext(*node);
    }
    ChannelBaseProvider::channelNotCreated(channelRequester);
    return Channel::shared_pointer();
}

void PVServiceProvider::addRecord(
    ServicePVTop::shared_pointer servicePVTop)
{
//printf("PVServiceProvider::addRecord\n");
    Lock xx(mutex);
    ServicePVTopBase *servicePVTopBase = new ServicePVTopBase(servicePVTop);
    TopListNode *node = new TopListNode(*servicePVTopBase);
    topList.addTail(*node);
}

void PVServiceProvider::removeRecord(
    ServicePVTop::shared_pointer servicePVTop)
{
//printf("PVServiceProvider::removeRecord\n");
    Lock xx(mutex);
    TopListNode *node = topList.getHead();
    while(node!=0) {
        ServicePVTopBase &pvTop = node->getObject();
        if(&pvTop.servicePVTop==&servicePVTop) {
            topList.remove(*node);
            delete node;
            return;
        }
        node = topList.getNext(*node);
    }
}

}}
