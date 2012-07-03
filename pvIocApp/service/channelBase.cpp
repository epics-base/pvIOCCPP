/* channelBase.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#include <string>
#include <stdexcept>
#include <memory>

#include "dbFldTypes.h"
#include "dbDefs.h"

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/noDefaultMethods.h>

#include <pv/standardField.h>
#include <pv/channelBase.h>

namespace epics { namespace pvAccess { 

using namespace epics::pvData;
using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;

typedef LinkedListNode<ChannelProcess::shared_pointer> ChannelProcessListNode;
typedef LinkedListNode<ChannelGet::shared_pointer> ChannelGetListNode;
typedef LinkedListNode<ChannelPut::shared_pointer> ChannelPutListNode;
typedef LinkedListNode<ChannelPutGet::shared_pointer> ChannelPutGetListNode;
typedef LinkedListNode<epics::pvData::Monitor::shared_pointer> ChannelMonitorListNode;
typedef LinkedListNode<ChannelRPC::shared_pointer> ChannelRPCListNode;
typedef LinkedListNode<ChannelArray::shared_pointer> ChannelArrayListNode;

ChannelBase::ChannelBase(
    ChannelProvider::shared_pointer const & provider,
    ChannelRequester::shared_pointer const & requester,
    String name)
:   provider(provider),
    requester(requester),
    channelName(name),
    channelProcessList(),
    channelGetList(),
    channelPutList(),
    channelPutGetList(),
    channelMonitorList(),
    channelRPCList(),
    channelArrayList(),
    beingDestroyed(false)
{
//printf("ChannelBase::ChannelBase\n");
}

ChannelBase::~ChannelBase()
{
//printf("ChannelBase::~ChannelBase\n");
}

void ChannelBase::destroy()
{
//printf("ChannelBase::destroy\n");
    {
        Lock xx(mutex);
        beingDestroyed = true;
    }
    while(true) {
        ChannelProcessListNode *node = channelProcessList.getHead();
        if(node==0) break;
        ChannelProcess::shared_pointer &channelProcess = node->getObject();
        channelProcess->destroy();
    }
    while(true) {
        ChannelGetListNode *node = channelGetList.getHead();
        if(node==0) break;
        ChannelGet::shared_pointer &channelGet = node->getObject();
        channelGet->destroy();
    }
    while(true) {
        ChannelPutListNode *node = channelPutList.getHead();
        if(node==0) break;
        ChannelPut::shared_pointer &channelPut = node->getObject();
        channelPut->destroy();
    }
    while(true) {
        ChannelPutGetListNode *node = channelPutGetList.getHead();
        if(node==0) break;
        ChannelPutGet::shared_pointer &channelPutGet = node->getObject();
        channelPutGet->destroy();
    }
    while(true) {
        ChannelMonitorListNode *node = channelMonitorList.getHead();
        if(node==0) break;
        Monitor::shared_pointer &channelMonitor = node->getObject();
        channelMonitor->destroy();
    }
    while(true) {
        ChannelRPCListNode *node = channelRPCList.getHead();
        if(node==0) break;
        ChannelRPC::shared_pointer &channelRPC = node->getObject();
        channelRPC->destroy();
    }
    while(true) {
        ChannelArrayListNode *node = channelArrayList.getHead();
        if(node==0) break;
        ChannelArray::shared_pointer &channelArray = node->getObject();
        channelArray->destroy();
    }
    std::tr1::static_pointer_cast<ChannelBaseProvider>(provider)->removeChannel(ChannelBase::shared_pointer(this));
}


void ChannelBase::addChannelProcess(ChannelProcess::shared_pointer const & channelProcess)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelProcess::shared_pointer xxx = channelProcess;
    ChannelProcessListNode *node = new ChannelProcessListNode(xxx);
    channelProcessList.addTail(*node);
}

void ChannelBase::addChannelGet(ChannelGet::shared_pointer const &channelGet)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelGet::shared_pointer xxx = channelGet;
    ChannelGetListNode *node = new ChannelGetListNode(xxx);
    channelGetList.addTail(*node);
}

void ChannelBase::addChannelPut(ChannelPut::shared_pointer const &channelPut)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelPut::shared_pointer xxx = channelPut;
    ChannelPutListNode *node = new ChannelPutListNode(xxx);
    channelPutList.addTail(*node);
}

void ChannelBase::addChannelPutGet(ChannelPutGet::shared_pointer const &channelPutGet)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelPutGet::shared_pointer xxx = channelPutGet;
    ChannelPutGetListNode *node = new ChannelPutGetListNode(xxx);
    channelPutGetList.addTail(*node);
}

void ChannelBase::addChannelMonitor(Monitor::shared_pointer const &monitor)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    Monitor::shared_pointer xxx = monitor;
    ChannelMonitorListNode *node = new ChannelMonitorListNode(xxx);
    channelMonitorList.addTail(*node);
}

void ChannelBase::addChannelRPC(ChannelRPC::shared_pointer const &channelRPC)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelRPC::shared_pointer xxx = channelRPC;
    ChannelRPCListNode *node = new ChannelRPCListNode(xxx);
    channelRPCList.addTail(*node);
}

void ChannelBase::addChannelArray(ChannelArray::shared_pointer const &channelArray)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelArray::shared_pointer xxx = channelArray;
    ChannelArrayListNode *node = new ChannelArrayListNode(xxx);
    channelArrayList.addTail(*node);
}

void ChannelBase::removeChannelProcess(ChannelProcess::shared_pointer const &ref)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelProcessListNode *node = channelProcessList.getHead();
    while(node!=0) {
        ChannelProcess::shared_pointer * xxx = &node->getObject();
        if(xxx==&ref) {
            channelProcessList.remove(*node);
            delete node;
            return;
        }
        node = channelProcessList.getNext(*node);
    }
}

void ChannelBase::removeChannelGet(ChannelGet::shared_pointer const &ref)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelGetListNode *node = channelGetList.getHead();
    while(node!=0) {
        ChannelGet::shared_pointer * xxx = &node->getObject();
        if(xxx==&ref) {
            channelGetList.remove(*node);
            delete node;
            return;
        }
        node = channelGetList.getNext(*node);
    }
}

void ChannelBase::removeChannelPut(ChannelPut::shared_pointer const &ref)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelPutListNode *node = channelPutList.getHead();
    while(node!=0) {
        ChannelPut::shared_pointer * xxx = &node->getObject();
        if(xxx==&ref) {
            channelPutList.remove(*node);
            delete node;
            return;
        }
        node = channelPutList.getNext(*node);
    }
}

void ChannelBase::removeChannelPutGet(ChannelPutGet::shared_pointer const &ref)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelPutGetListNode *node = channelPutGetList.getHead();
    while(node!=0) {
        ChannelPutGet::shared_pointer * xxx = &node->getObject();
        if(xxx==&ref) {
            channelPutGetList.remove(*node);
            delete node;
            return;
        }
        node = channelPutGetList.getNext(*node);
    }
}

void ChannelBase::removeChannelMonitor(Monitor::shared_pointer const &ref)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelMonitorListNode *node = channelMonitorList.getHead();
    while(node!=0) {
        Monitor::shared_pointer * xxx = &node->getObject();
        if(xxx==&ref) {
            channelMonitorList.remove(*node);
            delete node;
            return;
        }
        node = channelMonitorList.getNext(*node);
    }
}

void ChannelBase::removeChannelRPC(ChannelRPC::shared_pointer const &ref)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelRPCListNode *node = channelRPCList.getHead();
    while(node!=0) {
        ChannelRPC::shared_pointer * xxx = &node->getObject();
        if(xxx==&ref) {
            channelRPCList.remove(*node);
            delete node;
            return;
        }
        node = channelRPCList.getNext(*node);
    }
}

void ChannelBase::removeChannelArray(ChannelArray::shared_pointer const &ref)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelArrayListNode *node = channelArrayList.getHead();
    while(node!=0) {
        ChannelArray::shared_pointer * xxx = &node->getObject();
        if(xxx==&ref) {
            channelArrayList.remove(*node);
            delete node;
            return;
        }
        node = channelArrayList.getNext(*node);
    }
}

String ChannelBase::getRequesterName()
{
    return requester->getRequesterName();
}

void ChannelBase::message(
        String message,
        MessageType messageType)
{
    requester->message(message,messageType);
}

ChannelProvider::shared_pointer const & ChannelBase::getProvider()
{
    return provider;
}

String ChannelBase::getRemoteAddress()
{
    return String("local");
}

Channel::ConnectionState ChannelBase::getConnectionState()
{
    return Channel::CONNECTED;
}

String ChannelBase::getChannelName()
{
    return channelName;
}

ChannelRequester::shared_pointer const & ChannelBase::getChannelRequester()
{
    return requester;
}

bool ChannelBase::isConnected()
{
    return true;
}

void ChannelBase::getField(GetFieldRequester::shared_pointer const &requester,
        String subField)
{
    Status status(Status::STATUSTYPE_ERROR,
        String("client asked for illegal field"));
    requester->getDone(status,FieldConstPtr());
}

AccessRights ChannelBase::getAccessRights(
        PVField::shared_pointer const &pvField)
{
    throw std::logic_error(String("Not Implemented"));
}

ChannelProcess::shared_pointer ChannelBase::createChannelProcess(
        ChannelProcessRequester::shared_pointer const & channelProcessRequester,
        PVStructure::shared_pointer const & pvRequest)
{
    Status status(Status::STATUSTYPE_ERROR,
        String("ChannelProcess not supported"));
    channelProcessRequester->channelProcessConnect(
        status,
        ChannelProcess::shared_pointer());
    return ChannelProcess::shared_pointer();
}

ChannelGet::shared_pointer ChannelBase::createChannelGet(
        ChannelGetRequester::shared_pointer const &channelGetRequester,
        PVStructure::shared_pointer const &pvRequest)
{
    Status status(Status::STATUSTYPE_ERROR,
        String("ChannelGet not supported"));
    channelGetRequester->channelGetConnect(
        status,
        ChannelGet::shared_pointer(),
        PVStructure::shared_pointer(),
        BitSet::shared_pointer());
    return ChannelGet::shared_pointer();
}

ChannelPut::shared_pointer ChannelBase::createChannelPut(
        ChannelPutRequester::shared_pointer const &channelPutRequester,
        PVStructure::shared_pointer const &pvRequest)
{
    Status status(Status::STATUSTYPE_ERROR,
        String("ChannelPut not supported"));
    channelPutRequester->channelPutConnect(
        status,
        ChannelPut::shared_pointer(),
        PVStructure::shared_pointer(),
        BitSet::shared_pointer());
    return ChannelPut::shared_pointer();
}

ChannelPutGet::shared_pointer ChannelBase::createChannelPutGet(
        ChannelPutGetRequester::shared_pointer const &channelPutGetRequester,
        PVStructure::shared_pointer const &pvRequest)
{
    Status status(Status::STATUSTYPE_ERROR,
        String("ChannelPutGet not supported"));
    channelPutGetRequester->channelPutGetConnect(
        status,
        ChannelPutGet::shared_pointer(),
        PVStructure::shared_pointer(),
        PVStructure::shared_pointer());
    return ChannelPutGet::shared_pointer();
}

ChannelRPC::shared_pointer ChannelBase::createChannelRPC(
        ChannelRPCRequester::shared_pointer const & channelRPCRequester,
        PVStructure::shared_pointer const & pvRequest)
{
    Status status(Status::STATUSTYPE_ERROR,
        String("ChannelRPC not supported"));
    channelRPCRequester->channelRPCConnect(status,ChannelRPC::shared_pointer());
    return ChannelRPC::shared_pointer();
}

Monitor::shared_pointer ChannelBase::createMonitor(
        MonitorRequester::shared_pointer const &monitorRequester,
        PVStructure::shared_pointer const &pvRequest)
{
    Status status(Status::STATUSTYPE_ERROR,
        String("ChannelMonitor not supported"));
    Monitor::shared_pointer thisPointer = dynamic_pointer_cast<Monitor>(getPtrSelf());
    monitorRequester->monitorConnect(
        status,
        thisPointer,
        StructureConstPtr());
    return Monitor::shared_pointer();
}

ChannelArray::shared_pointer ChannelBase::createChannelArray(
        ChannelArrayRequester::shared_pointer const &channelArrayRequester,
        PVStructure::shared_pointer const &pvRequest)
{
    Status status(Status::STATUSTYPE_ERROR,
        String("ChannelArray not supported"));
    channelArrayRequester->channelArrayConnect(
        status,
        ChannelArray::shared_pointer(),
        PVArray::shared_pointer());
    return ChannelArray::shared_pointer();
}

void ChannelBase::printInfo()
{
    printf("ChannelBase provides access to service\n");
}

void ChannelBase::printInfo(StringBuilder out)
{
    *out += "ChannelBase provides access to service";
}

}}
