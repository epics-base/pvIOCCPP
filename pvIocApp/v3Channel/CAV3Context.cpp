/* v3CAContext.cpp */
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
#include <algorithm>

#include <epicsExit.h>
#include <cadef.h>
#include <db_access.h>
#include <dbDefs.h>

#include <CAV3Context.h>

using namespace epics::pvData;

extern "C" {

static void exceptionCallback(struct exception_handler_args args)
{
    CAV3Context *context = static_cast<CAV3Context *>(args.usr);   
    String message(ca_message(args.stat));
    context->exception(message);
}

static void threadExitFunc(void *arg)
{
    CAV3Context * context = static_cast<CAV3Context * >(arg);
    context->stop();
}

} //extern "C"

CAV3Context::CAV3Context(Requester &requester)
: 
  requester(requester),
  threadId(epicsThreadGetIdSelf()),
  context(0),
  referenceCount(0)
{
    SEVCHK(ca_context_create(ca_enable_preemptive_callback),
        "CAV3Context::CAV3Context calling ca_context_create");
    int status = ca_add_exception_event(exceptionCallback,this);
    if(status!=ECA_NORMAL) {
        requester.message(String(
            "ca_add_exception_event failed"),warningMessage);
    }
    context = ca_current_context();
    epicsAtThreadExit(threadExitFunc,this);
}

CAV3Context::~CAV3Context()
{
}

void CAV3Context::stop()
{
    epicsThreadId id = epicsThreadGetIdSelf();
    if(id!=threadId) {
        throw std::logic_error(String(
           "CAV3Context::stop not same thread"));
    }
    if(referenceCount!=0) {
        throw std::logic_error(String(
           "CAV3Context::stop referenceCount != 0"));
    }
    CAV3ContextCreate::erase(threadId);
    ca_context_destroy();
    delete this;
}

typedef std::list<epicsThreadId>::iterator threadListIter;

void CAV3Context::checkContext()
{
    epicsThreadId id = epicsThreadGetIdSelf();
    if(id==threadId) return;
    Lock xx(mutex);
    threadListIter iter = std::find(
       auxThreadList.begin(),auxThreadList.end(),id);
    if(iter!=auxThreadList.end()) return;
    auxThreadList.push_front(id);
    SEVCHK(ca_attach_context(context),
        "CAV3Context::checkContext calling ca_context_create");
}

void CAV3Context::release()
{
    Lock xx(mutex);
    referenceCount--;
}

void CAV3Context::exception(String message)
{
    Lock xx(mutex);
    requester.message(message,errorMessage);
}

typedef std::map<epicsThreadId,CAV3Context *>::iterator contextMapIter;

std::map<epicsThreadId,CAV3Context*>CAV3ContextCreate::contextMap;
Mutex CAV3ContextCreate::mutex;

CAV3Context & CAV3ContextCreate::get(Requester &requester)
{
    Lock xx(mutex);
    epicsThreadId id = epicsThreadGetIdSelf();
    contextMapIter iter = contextMap.find(id);
    if(iter!=contextMap.end()) {
        CAV3Context * context = iter->second;
        context->referenceCount++;
        return *context;
    }
    CAV3Context * context = new CAV3Context(requester);
    contextMap[id] = context;
    context->referenceCount++;
    return *context;
}

void CAV3ContextCreate::erase(epicsThreadId threadId)
{
    Lock xx(mutex);
    contextMap.erase(threadId);
}
