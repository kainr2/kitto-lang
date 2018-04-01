#pragma once
#ifndef __IPT_RULE_H__
#define __IPT_RULE_H__


#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

namespace kainr2 {
namespace ip_tables {

    struct IptRule
    {
        enum IPT_ERROR
        {
            ERROR_NONE,
            ERROR_CHAIN_NAME_MISSING,
            ERROR_NO_INPUT,
            ERROR_BAD_CHAIN_RULE_NUMBER,
            ERROR_MODULE_OR_TARGET_NOT_ALLOWED,
            ERROR_TABLE_AND_MODULE_INCOMPATIBLE,
            ERROR_TABLE_AND_TARGET_INCOMPATIBLE,
            ERROR_CHAIN_AND_MODULE_INCOMPATIBLE,
            ERROR_CHAIN_AND_TARGET_INCOMPATIBLE,
        };

        IPT_ERROR   errorCode    = ERROR_NONE;
        std::string errorMessage = "";
        std::string rule         = "";

        IptRule();
        bool hasError();
    };
}
}


#endif

