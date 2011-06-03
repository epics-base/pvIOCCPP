/* pvServerBaseProvider.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#include <string>
#include <stdexcept>
#include <memory>

#include <pvIntrospect.h>
#include <pvData.h>
#include <noDefaultMethods.h>
#include <lock.h>
#include <epicsExit.h>
#include <dbAccess.h>

#include "support.h"
#include "pvDatabase.h"
#include "pvServiceBase.h"

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;

typedef LinkedListNode<PVServiceBase> ChannelListNode;
typedef LinkedList<PVServiceBase> ChannelList;


PVServiceBaseProvider::PVServiceBaseProvider(
    String providerName
)
: providerName(providerName),
  channelProviderPtr(this),
  beingDestroyed(false)
{
printf("PVServiceBaseProvider::PVServiceBaseProvider %s\n",providerName.c_str());
    registerChannelProvider(channelProviderPtr);
}

PVServiceBaseProvider::~PVServiceBaseProvider()
{
printf("PVServiceBaseProvider::~PVServiceBaseProvider\n");
}

void PVServiceBaseProvider::destroy()
{
printf("PVServiceBaseProvider::destroy\n");
    Lock xx(mutex);
    beingDestroyed = true;
    unregisterChannelProvider(channelProviderPtr);
    while(true) {
        ChannelListNode *node = channelList.removeHead();
        if(node==0) break;
        PVServiceBase &pvServerBase = node->getObject();
        pvServerBase.destroy();
        delete node;
    }
}

String PVServiceBaseProvider::getProviderName()
{
    return providerName;
}

void PVServiceBaseProvider::channelFound(
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

Channel::shared_pointer PVServiceBaseProvider::createChannel(
    String channelName,
    ChannelRequester::shared_pointer  const &channelRequester,
    short priority)
{
    return createChannel(channelName,channelRequester,priority,"");
}

void PVServiceBaseProvider::channelNotCreated(
    ChannelRequester::shared_pointer const &channelRequester)
{
    Status notFoundStatus(Status::STATUSTYPE_ERROR,String("pv not found"));
    channelRequester->channelCreated(
        notFoundStatus,
        Channel::shared_pointer());
}

void PVServiceBaseProvider::channelCreated(PVServiceBase::shared_pointer channel)
{
    ChannelListNode *channelListNode = new ChannelListNode(*channel.get());
    channelList.addTail(*channelListNode);
    channel->getChannelRequester()->channelCreated(Status::OK,channel);
}

void PVServiceBaseProvider::removeChannel(PVServiceBase &channel)
{
    if(beingDestroyed) return;
    ChannelListNode *channelListNode = channelList.getHead();
    while(channelListNode!=0) {
        PVServiceBase *chan = &channelListNode->getObject();
        if(chan==&channel) {
          channelList.remove(*channelListNode);
          delete channelListNode; 
          return;
        }
        channelListNode = channelList.getNext(*channelListNode);
    }
    String message("PVServiceBaseProvider::removeChannel ");
    message += channel.getChannelName();
    message += " but channel not in channelList";
    channel.message(message,errorMessage);
}

}}
