TOP=../../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# build an ioc application

DBD += testV3Channel.dbd
PROD_IOC += testV3Channel

# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
testV3Channel_SRCS += testV3Channel_registerRecordDeviceDriver.cpp
testV3Channel_SRCS_DEFAULT += testV3ChannelMain.cpp
testV3Channel_SRCS_vxWorks += -nil-

# The following adds support from base/src/vxWorks
testV3Channel_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

testV3Channel_LIBS += pvIOC pvData pvAccess
testV3Channel_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

