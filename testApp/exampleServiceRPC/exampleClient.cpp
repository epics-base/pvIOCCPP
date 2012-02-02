/* exampleClient.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/* Author Marty Kraimer 2012.01 */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>

#include <epicsThread.h>
#include <epicsAssert.h>
#include <epicsExit.h>

#include <pv/CDRMonitor.h>
#include "ezchannelRPC.h"

using namespace epics::pvData;
using namespace epics::pvAccess;


static FILE *fd = 0;
static FILE *auxfd = 0;

static void dump(EZChannelRPC::shared_pointer const & channelRPC)
{
    printf("%s\n",channelRPC->getMessage().c_str());
}

void exampleClient()
{
    String builder;
    FieldCreate * fieldCreate = getFieldCreate();
    PVDataCreate * pvDataCreate = getPVDataCreate();

    // Create an argument to pass to service
    int n = 3;
    FieldConstPtrArray fields = new FieldConstPtr[5];
    fields[0] = fieldCreate->createScalar("function",pvString);
    fields[1] = fieldCreate->createScalarArray("names",pvString);
    fields[2] = fieldCreate->createScalarArray("values",pvString);
    PVStructure::shared_pointer pvArgument = PVStructure::shared_pointer(
        pvDataCreate->createPVStructure(0,"NTNameValue",n,fields));
    PVString *pvfunction = pvArgument->getStringField("function");
    PVStringArray *pvnames = static_cast<PVStringArray *>
        (pvArgument->getScalarArrayField("names",pvString));
    PVStringArray *pvvalues = static_cast<PVStringArray *>
        (pvArgument->getScalarArrayField("values",pvString));
    pvfunction->put("example");
    int nnameValue = 2;
    String names[] = {"name0","name1"};
    String values[] = {"value0","value1"};
    pvnames->put(0,2,names,0);
    pvvalues->put(0,2,values,0);

    // create a channelRPC and connect
    EZChannelRPC::shared_pointer channelRPC = 
         EZChannelRPC::shared_pointer(new EZChannelRPC("serviceRPC"));
    bool result = channelRPC->connect(1.0);
    if(!result) {dump(channelRPC); return;}

    //make a request
    PVStructure::shared_pointer pvResponse
        = channelRPC->request(pvArgument,false);
    if(pvResponse.get()==0) {dump(channelRPC); return;}

builder.clear();
pvResponse->toString(&builder);
printf("\nresponse\n%s\n",builder.c_str());

    // now look for expected data from service
    PVInt *pvSeverity = pvResponse->getIntField("alarm.severity");
    if(pvSeverity==0) exit(1);
    if(pvSeverity->get()!=0) {
        printf("alarm severity not 0\n");
    }
    PVDoubleArray * pvposition = static_cast<PVDoubleArray *>
        (pvResponse->getScalarArrayField("position",pvDouble));
    if(pvposition!=0) {
        builder.clear();
        pvposition->toString(&builder);
        printf("\nposition\n%s\n",builder.c_str());
      
    }
    // can look for other data
    channelRPC->destroy();
}

int main(int argc,char *argv[])
{
    char *fileName = 0;
    if(argc>1) fileName = argv[1];
    fd = stdout;
    if(fileName!=0 && fileName[0]!=0) {
        fd = fopen(fileName,"w+");
    }
    char *auxFileName = 0;
    if(argc>2) auxFileName = argv[2];
    auxfd = stdout;
    if(auxFileName!=0 && auxFileName[0]!=0) {
        auxfd = fopen(auxFileName,"w+");
    }
    ClientFactory::start();
    exampleClient();
    ClientFactory::stop();
    epicsThreadSleep(2.0);
    epicsExitCallAtExits();
    CDRMonitor::get().show(stdout,true);
    return (0);
}

