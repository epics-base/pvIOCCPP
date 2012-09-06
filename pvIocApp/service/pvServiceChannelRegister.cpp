/*pvServiceChannelRegister.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */

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

#include <pv/pvServiceProvider.h>

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvIOC;

static PVServiceChannelCTX *myCTX = 0;

static const iocshFuncDef startPVServiceChannelFuncDef = {
    "startPVServiceChannel", 0, 0};

extern "C" void startPVServiceChannel(const iocshArgBuf *args)
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
extern "C" void stopPVServiceChannel(const iocshArgBuf *args)
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
        iocshRegister(&startPVServiceChannelFuncDef, startPVServiceChannel);
    }
}

static void stopPVServiceChannelRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&stopPVServiceChannelFuncDef, stopPVServiceChannel);
    }
}

epicsExportRegistrar(startPVServiceChannelRegister);
epicsExportRegistrar(stopPVServiceChannelRegister);
