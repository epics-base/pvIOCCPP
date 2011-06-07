/* exampleContext.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/* Marty Kraimer 2011.03 
 * This creates a CAV3 context the first time a thread calls 
 * exampleContextCreate::create and then calls ca_attach_context
 * from checkContext if the caller is a thread that is not the
 * thread that called exampleContextCreate,create.
 */

#ifndef EXAMPLECONTEXT_H
#define EXAMPLECONTEXT_H

#include <map>
#include <list>

#include <epicsThread.h>

#include <pv/lock.h>
#include <pv/requester.h>


class ExampleContextCreate;

class ExampleContext {
public:
    ~ExampleContext();
    void release();
    void stop();
    void exception(epics::pvData::String message);
    void checkContext();
private:
    std::list<epicsThreadId> auxThreadList ;
    epics::pvData::Mutex mutex;
    ExampleContext(
       epics::pvData::Requester &requester);
    epics::pvData::Requester &requester;
    epicsThreadId threadId;
    struct ca_client_context *context;
    int referenceCount;
    friend class ExampleContextCreate;
};

class ExampleContextCreate {
public:
    static ExampleContext &get(epics::pvData::Requester &requester);
private:
    static void erase(epicsThreadId id);
    static std::map<epicsThreadId,ExampleContext*> contextMap;
    static epics::pvData::Mutex mutex;
    friend class ExampleContext;
};

#endif  /* EXAMPLECONTEXT_H */
