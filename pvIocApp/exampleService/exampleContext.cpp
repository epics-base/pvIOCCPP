/* ExampleContext.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/* Marty Kraimer 2011.03 
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

#include <exampleContext.h>

using namespace epics::pvData;

extern "C" {

static void exceptionCallback(struct exception_handler_args args)
{
    ExampleContext *context = static_cast<ExampleContext *>(args.usr);   
    String message(ca_message(args.stat));
    context->exception(message);
}

static void threadExitFunc(void *arg)
{
    ExampleContext * context = static_cast<ExampleContext * >(arg);
    context->stop();
}

} //extern "C"

ExampleContext::ExampleContext(Requester &requester)
: 
  requester(requester),
  threadId(epicsThreadGetIdSelf()),
  context(0),
  referenceCount(0)
{
    SEVCHK(ca_context_create(ca_enable_preemptive_callback),
        "ExampleContext::ExampleContext calling ca_context_create");
    int status = ca_add_exception_event(exceptionCallback,this);
    if(status!=ECA_NORMAL) {
        requester.message(String(
            "ca_add_exception_event failed"),warningMessage);
    }
    context = ca_current_context();
    epicsAtThreadExit(threadExitFunc,this);
}

ExampleContext::~ExampleContext()
{
}

void ExampleContext::stop()
{
    epicsThreadId id = epicsThreadGetIdSelf();
    if(id!=threadId) {
        throw std::logic_error(String(
           "ExampleContext::stop not same thread"));
    }
    if(referenceCount!=0) {
        throw std::logic_error(String(
           "ExampleContext::stop referenceCount != 0"));
    }
    ExampleContextCreate::erase(threadId);
    ca_context_destroy();
    delete this;
}

typedef std::list<epicsThreadId>::iterator threadListIter;

void ExampleContext::checkContext()
{
    epicsThreadId id = epicsThreadGetIdSelf();
    if(id==threadId) return;
    Lock xx(mutex);
    threadListIter iter = std::find(
       auxThreadList.begin(),auxThreadList.end(),id);
    if(iter!=auxThreadList.end()) return;
    auxThreadList.push_front(id);
    SEVCHK(ca_attach_context(context),
        "ExampleContext::checkContext calling ca_context_create");
}

void ExampleContext::release()
{
    Lock xx(mutex);
    referenceCount--;
}

void ExampleContext::exception(String message)
{
    Lock xx(mutex);
    requester.message(message,errorMessage);
}

typedef std::map<epicsThreadId,ExampleContext *>::iterator contextMapIter;

std::map<epicsThreadId,ExampleContext*>ExampleContextCreate::contextMap;
Mutex ExampleContextCreate::mutex;

ExampleContext & ExampleContextCreate::get(Requester &requester)
{
    Lock xx(mutex);
    epicsThreadId id = epicsThreadGetIdSelf();
    contextMapIter iter = contextMap.find(id);
    if(iter!=contextMap.end()) {
        ExampleContext * context = iter->second;
        context->referenceCount++;
        return *context;
    }
    ExampleContext * context = new ExampleContext(requester);
    contextMap[id] = context;
    context->referenceCount++;
    return *context;
}

void ExampleContextCreate::erase(epicsThreadId threadId)
{
    Lock xx(mutex);
    contextMap.erase(threadId);
}
