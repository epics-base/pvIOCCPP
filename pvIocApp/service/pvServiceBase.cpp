/* pvServiceBase.cpp */
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

#include <pv/support.h>
#include <pv/pvDatabase.h>
#include <pv/standardField.h>
#include <pv/pvServiceBase.h>

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;

typedef LinkedListNode<epics::pvAccess::ChannelProcess> ChannelProcessListNode;
typedef LinkedList<epics::pvAccess::ChannelProcess> ChannelProcessList;
typedef LinkedListNode<epics::pvAccess::ChannelGet> ChannelGetListNode;
typedef LinkedList<epics::pvAccess::ChannelGet> ChannelGetList;
typedef LinkedListNode<epics::pvAccess::ChannelPut> ChannelPutListNode;
typedef LinkedList<epics::pvAccess::ChannelPut> ChannelPutList;
typedef LinkedListNode<epics::pvAccess::ChannelPutGet> ChannelPutGetListNode;
typedef LinkedList<epics::pvAccess::ChannelPutGet> ChannelPutGetList;
typedef LinkedListNode<epics::pvData::Monitor> ChannelMonitorListNode;
typedef LinkedList<epics::pvData::Monitor> ChannelMonitorList;
typedef LinkedListNode<epics::pvAccess::ChannelRPC> ChannelRPCListNode;
typedef LinkedList<epics::pvAccess::ChannelRPC> ChannelRPCList;
typedef LinkedListNode<epics::pvAccess::ChannelArray> ChannelArrayListNode;
typedef LinkedList<epics::pvAccess::ChannelArray> ChannelArrayList;


PVServiceBase::PVServiceBase(
    PVServiceBaseProvider::shared_pointer const & provider,
        ChannelRequester::shared_pointer const & requester,
        String name
    )
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
//printf("PVServiceBase::PVServiceBase\n");
}

PVServiceBase::~PVServiceBase()
{
//printf("PVServiceBase::~PVServiceBase\n");
}

void PVServiceBase::destroy()
{
//printf("PVServiceBase::destroy\n");
    {
        Lock xx(mutex);
        beingDestroyed = true;
    }
    while(true) {
        ChannelProcessListNode *node = channelProcessList.getHead();
        if(node==0) break;
        ChannelProcess &channelProcess = node->getObject();
        channelProcess.destroy();
    }
    while(true) {
        ChannelGetListNode *node = channelGetList.getHead();
        if(node==0) break;
        ChannelGet &channelGet = node->getObject();
        channelGet.destroy();
    }
    while(true) {
        ChannelPutListNode *node = channelPutList.getHead();
        if(node==0) break;
        ChannelPut &channelPut = node->getObject();
        channelPut.destroy();
    }
    while(true) {
        ChannelPutGetListNode *node = channelPutGetList.getHead();
        if(node==0) break;
        ChannelPutGet &channelPutGet = node->getObject();
        channelPutGet.destroy();
    }
    while(true) {
        ChannelMonitorListNode *node = channelMonitorList.getHead();
        if(node==0) break;
        Monitor &channelMonitor = node->getObject();
        channelMonitor.destroy();
    }
    while(true) {
        ChannelRPCListNode *node = channelRPCList.getHead();
        if(node==0) break;
        ChannelRPC &channelRPC = node->getObject();
        channelRPC.destroy();
    }
    while(true) {
        ChannelArrayListNode *node = channelArrayList.getHead();
        if(node==0) break;
        ChannelArray &channelArray = node->getObject();
        channelArray.destroy();
    }
    std::tr1::static_pointer_cast<PVServiceBaseProvider>(provider)->removeChannel(*this);
}


void PVServiceBase::addChannelProcess(ChannelProcess & channelProcess)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelProcessListNode *node = new ChannelProcessListNode(channelProcess);
    channelProcessList.addTail(*node);
}

void PVServiceBase::addChannelGet(ChannelGet &channelGet)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelGetListNode *node = new ChannelGetListNode(channelGet);
    channelGetList.addTail(*node);
}

void PVServiceBase::addChannelPut(ChannelPut &channelPut)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelPutListNode *node = new ChannelPutListNode(channelPut);
    channelPutList.addTail(*node);
}

void PVServiceBase::addChannelPutGet(ChannelPutGet &channelPutGet)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelPutGetListNode *node = new ChannelPutGetListNode(channelPutGet);
    channelPutGetList.addTail(*node);
}

void PVServiceBase::addChannelMonitor(Monitor &monitor)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelMonitorListNode *node = new ChannelMonitorListNode(monitor);
    channelMonitorList.addTail(*node);
}

void PVServiceBase::addChannelRPC(ChannelRPC &channelRPC)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelRPCListNode *node = new ChannelRPCListNode(channelRPC);
    channelRPCList.addTail(*node);
}

void PVServiceBase::addChannelArray(ChannelArray &channelArray)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelArrayListNode *node = new ChannelArrayListNode(channelArray);
    channelArrayList.addTail(*node);
}

void PVServiceBase::removeChannelProcess(ChannelProcess &ref)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelProcessListNode *node = channelProcessList.getHead();
    while(node!=0) {
        ChannelProcess * xxx = &node->getObject();
        if(xxx==&ref) {
            channelProcessList.remove(*node);
            delete node;
            return;
        }
        node = channelProcessList.getNext(*node);
    }
}

void PVServiceBase::removeChannelGet(ChannelGet &ref)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelGetListNode *node = channelGetList.getHead();
    while(node!=0) {
        ChannelGet * xxx = &node->getObject();
        if(xxx==&ref) {
            channelGetList.remove(*node);
            delete node;
            return;
        }
        node = channelGetList.getNext(*node);
    }
}

void PVServiceBase::removeChannelPut(ChannelPut &ref)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelPutListNode *node = channelPutList.getHead();
    while(node!=0) {
        ChannelPut * xxx = &node->getObject();
        if(xxx==&ref) {
            channelPutList.remove(*node);
            delete node;
            return;
        }
        node = channelPutList.getNext(*node);
    }
}

void PVServiceBase::removeChannelPutGet(ChannelPutGet &ref)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelPutGetListNode *node = channelPutGetList.getHead();
    while(node!=0) {
        ChannelPutGet * xxx = &node->getObject();
        if(xxx==&ref) {
            channelPutGetList.remove(*node);
            delete node;
            return;
        }
        node = channelPutGetList.getNext(*node);
    }
}

void PVServiceBase::removeChannelMonitor(Monitor &ref)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelMonitorListNode *node = channelMonitorList.getHead();
    while(node!=0) {
        Monitor * xxx = &node->getObject();
        if(xxx==&ref) {
            channelMonitorList.remove(*node);
            delete node;
            return;
        }
        node = channelMonitorList.getNext(*node);
    }
}

void PVServiceBase::removeChannelRPC(ChannelRPC &ref)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelRPCListNode *node = channelRPCList.getHead();
    while(node!=0) {
        ChannelRPC * xxx = &node->getObject();
        if(xxx==&ref) {
            channelRPCList.remove(*node);
            delete node;
            return;
        }
        node = channelRPCList.getNext(*node);
    }
}

void PVServiceBase::removeChannelArray(ChannelArray &ref)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    ChannelArrayListNode *node = channelArrayList.getHead();
    while(node!=0) {
        ChannelArray * xxx = &node->getObject();
        if(xxx==&ref) {
            channelArrayList.remove(*node);
            delete node;
            return;
        }
        node = channelArrayList.getNext(*node);
    }
}

String PVServiceBase::getRequesterName()
{
    return requester->getRequesterName();
}

void PVServiceBase::message(
        String message,
        MessageType messageType)
{
    requester->message(message,messageType);
}

ChannelProvider::shared_pointer const & PVServiceBase::getProvider()
{
    return provider;
}

String PVServiceBase::getRemoteAddress()
{
    return String("local");
}

Channel::ConnectionState PVServiceBase::getConnectionState()
{
    return Channel::CONNECTED;
}

String PVServiceBase::getChannelName()
{
    return channelName;
}

ChannelRequester::shared_pointer const & PVServiceBase::getChannelRequester()
{
    return requester;
}

bool PVServiceBase::isConnected()
{
    return true;
}

void PVServiceBase::getField(GetFieldRequester::shared_pointer const &requester,
        String subField)
{
    Status status(Status::STATUSTYPE_ERROR,
        String("client asked for illegal field"));
    requester->getDone(status,FieldConstPtr());
}

AccessRights PVServiceBase::getAccessRights(
        PVField::shared_pointer const &pvField)
{
    throw std::logic_error(String("Not Implemented"));
}

ChannelProcess::shared_pointer PVServiceBase::createChannelProcess(
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

ChannelGet::shared_pointer PVServiceBase::createChannelGet(
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

ChannelPut::shared_pointer PVServiceBase::createChannelPut(
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

ChannelPutGet::shared_pointer PVServiceBase::createChannelPutGet(
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

ChannelRPC::shared_pointer PVServiceBase::createChannelRPC(
        ChannelRPCRequester::shared_pointer const & channelRPCRequester,
        PVStructure::shared_pointer const & pvRequest)
{
    Status status(Status::STATUSTYPE_ERROR,
        String("ChannelRPC not supported"));
    channelRPCRequester->channelRPCConnect(status,
     ChannelRPC::shared_pointer(),
     PVStructure::shared_pointer(),
     BitSet::shared_pointer());
    return ChannelRPC::shared_pointer();
}

Monitor::shared_pointer PVServiceBase::createMonitor(
        MonitorRequester::shared_pointer const &monitorRequester,
        PVStructure::shared_pointer const &pvRequest)
{
    Status status(Status::STATUSTYPE_ERROR,
        String("ChannelMonitor not supported"));
    monitorRequester->monitorConnect(
        status,
        Monitor::shared_pointer(),
        StructureConstPtr());
    return Monitor::shared_pointer();
}

ChannelArray::shared_pointer PVServiceBase::createChannelArray(
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

void PVServiceBase::printInfo()
{
    printf("PVServiceBase provides access to service\n");
}

void PVServiceBase::printInfo(StringBuilder out)
{
    *out += "PVServiceBase provides access to service";
}

}}
