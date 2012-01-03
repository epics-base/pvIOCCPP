/* channelBaseProvider.cpp */
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
#include <pv/channelBase.h>

namespace epics { namespace pvAccess { 

using namespace epics::pvData;
using namespace epics::pvAccess;

typedef LinkedListNode<ChannelBase> ChannelListNode;
typedef LinkedList<ChannelBase> ChannelList;


ChannelBaseProvider::ChannelBaseProvider(
    String providerName
)
: providerName(providerName),
  beingDestroyed(false)
{
printf("ChannelBaseProvider::ChannelBaseProvider %s\n",providerName.c_str());
}

void ChannelBaseProvider::init()
{
    registerChannelProvider(getPtrSelf());
}

ChannelBaseProvider::~ChannelBaseProvider()
{
printf("ChannelBaseProvider::~ChannelBaseProvider\n");
}

void ChannelBaseProvider::destroy()
{
printf("ChannelBaseProvider::destroy\n");
    Lock xx(mutex);
    beingDestroyed = true;
    unregisterChannelProvider(getPtrSelf());
    while(true) {
        ChannelListNode *node = channelList.removeHead();
        if(node==0) break;
        ChannelBase &pvServerBase = node->getObject();
        pvServerBase.destroy();
        delete node;
    }
}

String ChannelBaseProvider::getProviderName()
{
    return providerName;
}

void ChannelBaseProvider::channelFound(
    bool found,
    ChannelFindRequester::shared_pointer const & channelFindRequester)
{
    if(found) {
        channelFindRequester->channelFindResult(
            Status::OK,
            ChannelFind::shared_pointer(),
            true);
    } else {
        Status notFoundStatus(Status::STATUSTYPE_ERROR,String("pv not found"));
        channelFindRequester.get()->channelFindResult(
            notFoundStatus,
            ChannelFind::shared_pointer(),
            false);
    }
}

Channel::shared_pointer ChannelBaseProvider::createChannel(
    String channelName,
    ChannelRequester::shared_pointer  const &channelRequester,
    short priority)
{
    return createChannel(channelName,channelRequester,priority,"");
}

void ChannelBaseProvider::channelNotCreated(
    ChannelRequester::shared_pointer const &channelRequester)
{
    Status notFoundStatus(Status::STATUSTYPE_ERROR,String("pv not found"));
    channelRequester->channelCreated(
        notFoundStatus,
        Channel::shared_pointer());
}

void ChannelBaseProvider::channelCreated(Channel::shared_pointer channel)
{
printf("ChannelBaseProvider::channelCreated\n");
    Lock xx(mutex);
    ChannelBase *xxx = static_cast<ChannelBase *>(channel.get());
    ChannelListNode *channelListNode = new ChannelListNode(*xxx);
    channelList.addTail(*channelListNode);
    channel->getChannelRequester()->channelCreated(Status::OK,channel);
}

void ChannelBaseProvider::removeChannel(ChannelBase &channel)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelListNode *channelListNode = channelList.getHead();
    while(channelListNode!=0) {
        ChannelBase *chan = &channelListNode->getObject();
        if(chan==&channel) {
          channelList.remove(*channelListNode);
          delete channelListNode; 
          return;
        }
        channelListNode = channelList.getNext(*channelListNode);
    }
    String message("ChannelBaseProvider::removeChannel ");
    message += channel.getChannelName();
    message += " but channel not in channelList";
    channel.message(message,errorMessage);
}

}}