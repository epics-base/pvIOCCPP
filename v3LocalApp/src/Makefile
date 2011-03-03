TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# Build an IOC support library

DBD += v3LocalTest.dbd

LIBRARY_IOC += v3LocalTestSupport
v3LocalTestSupport_SRCS += byteRecord.c
v3LocalTestSupport_SRCS += shortRecord.c
v3LocalTestSupport_SRCS += floatRecord.c
v3LocalTestSupport_SRCS += testV3Local.cpp
v3LocalTestSupport_LIBS += pvIoc
v3LocalTestSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================
# build an ioc application

PROD_IOC += v3LocalTest


# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
v3LocalTest_SRCS += v3LocalTest_registerRecordDeviceDriver.cpp
v3LocalTest_SRCS_DEFAULT += v3LocalTestMain.cpp
v3LocalTest_SRCS_vxWorks += -nil-


# The following adds support from base/src/vxWorks
v3LocalTest_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

v3LocalTest_LIBS += v3LocalTestSupport
v3LocalTest_LIBS += pvIoc pvData pvAccess
v3LocalTest_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

