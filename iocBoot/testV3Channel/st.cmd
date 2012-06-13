< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase("dbd/testV3Channel.dbd")
testV3Channel_registerRecordDeviceDriver(pdbbase)

## Load record instances
dbLoadRecords("db/test.db")

cd ${TOP}/iocBoot/${IOC}
iocInit()

epicsThreadSleep(2.0)
startV3Channel
