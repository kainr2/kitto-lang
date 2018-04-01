#pragma once
#ifndef __IPT_MODULE_H__
#define __IPT_MODULE_H__

#include <iostream>
#include "IptEnum.h"

namespace kainr2 {
namespace ip_tables {

    struct IptModuleInput
    {
        IPT_MODULE_ENUM moduleAction = MODULE_NONE;
        std::string     moduleOption = "";

        IptModuleInput()
        { ; }

        IptModuleInput(IPT_MODULE_ENUM action)
        : moduleAction(action)
        , moduleOption("")
        { ; }

        IptModuleInput(IPT_MODULE_ENUM action, std::string option)
        : moduleAction(action)
        , moduleOption(option)
        { ; }

        bool operator()(const IptModuleInput& m) const
        {
            return m.moduleAction == moduleAction;
        }
    };

    struct IptRuleInput
    {
        IPT_TABLE_ENUM table = TABLE_NONE;

        IPT_CHAIN_ENUM chainAction = CHAIN_RULE_NONE;
        std::string    chainName   = "";
        std::string    chainOption = "";

        IPT_TARGET_ENUM targetAction = TARGET_NONE;
        std::string     targetOption = "";

        std::vector<IptModuleInput> moduleVector;
    };

}
}


#endif

