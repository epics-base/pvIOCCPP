TOP=../../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# Build an IOC support library
#


DBD += testLocalChannelProvider.dbd

#LIBRARY_IOC += testLocalChannelProvider
#testLocalChannelProviderSupport_SRCS += testLocalChannelProvider.cpp
#testLocalChannelProviderSupport_LIBS += pvData
#testLocalChannelProviderSupport_LIBS += pvAccess
#testLocalChannelProviderSupport_LIBS += pvIOC
#testLocalChannelProviderSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================
# build an ioc application

PROD_IOC += testLocalChannelProvider


# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
testLocalChannelProvider_SRCS += testLocalChannelProvider_registerRecordDeviceDriver.cpp
testLocalChannelProvider_SRCS_DEFAULT += testLocalChannelProviderMain.cpp
testLocalChannelProvider_SRCS_vxWorks += -nil-


# The following adds support from base/src/vxWorks
testLocalChannelProvider_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

testLocalChannelProvider_LIBS += pvIOC pvData pvAccess
#testLocalChannelProvider_LIBS += testLocalChannelProviderSupport
testLocalChannelProvider_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

