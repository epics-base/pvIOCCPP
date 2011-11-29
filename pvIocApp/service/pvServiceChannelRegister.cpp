/*pvServiceChannelRegister.cpp */

/* Author: Marty Kraimer */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <memory>

#include <cantProceed.h>
#include <epicsStdio.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <epicsThread.h>
#include <iocsh.h>

#include <epicsExport.h>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>
#include <pv/serverContext.h>

#include <pv/pvDatabase.h>
#include <pv/pvServiceProvider.h>

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvIOC;

static PVServiceChannelCTX *myCTX = 0;

static const iocshFuncDef startPVServiceChannelFuncDef = {
    "startPVServiceChannel", 0, 0};

static void startPVServiceChannelCallFunc(const iocshArgBuf *args)
{
    if(myCTX!=0) {
        printf("PVServiceChannel already started\n");
        return;
    }
    myCTX = new PVServiceChannelCTX();
}

static const iocshFuncDef stopPVServiceChannelFuncDef = {
    "stopPVServiceChannel", 0, 0
};
static void stopPVServiceChannelCallFunc(const iocshArgBuf *args)
{
   printf("stopPVServiceChannel\n");
   if(myCTX!=0) delete myCTX;
   myCTX = 0;
}

static void startPVServiceChannelRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&startPVServiceChannelFuncDef, startPVServiceChannelCallFunc);
    }
}

static void stopPVServiceChannelRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&stopPVServiceChannelFuncDef, stopPVServiceChannelCallFunc);
    }
}

epicsExportRegistrar(startPVServiceChannelRegister);
epicsExportRegistrar(stopPVServiceChannelRegister);
