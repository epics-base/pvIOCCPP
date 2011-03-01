/* util.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef UTIL_H
#define UTIL_H
#include <string>
#include <stdexcept>
#include <memory>

#include <pvIntrospect.h>
#include <pvData.h>
#include <noDefaultMethods.h>
#include <pvEnumerated.h>
#include <thread.h>

#include "pvDatabase.h"
#include "support.h"

namespace epics { namespace pvData { 

enum RequestResult {
    requestResultZombie,
    requestResultSuccess,
    requestResultFailure
};

}}

#endif  /* UTIL_H */
