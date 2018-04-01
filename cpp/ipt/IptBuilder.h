#pragma once
#ifndef __IPT_BUILDER_H__
#define __IPT_BUILDER_H__

#include <map>
#include <string>
#include <vector>
#include "IptEnum.h"
#include "IptRule.h"
#include "IptRuleInput.h"

namespace kainr2 {
namespace ip_tables {

    class IptBuilder
    {
    public:
        // http://ipset.netfilter.org/iptables.man.html
        // http://ipset.netfilter.org/iptables-extensions.man.html
        static const std::map<IPT_CHAIN_TYPE_ENUM, const std::string> IPT_CHAIN_TYPE;
        static const std::map<IPT_TABLE_ENUM, const std::string>      IPT_TABLE;
        static const std::map<IPT_CHAIN_ENUM, const std::string>      IPT_CHAIN;
        static const std::map<IPT_TARGET_ENUM, const std::string>     IPT_TARGET;
        static const std::map<IPT_MODULE_ENUM, const std::string>     IPT_MODULE;

        IptBuilder(const IPT_TABLE_ENUM& table=TABLE_FILTER);
        virtual ~IptBuilder();

        IptRule build(bool showTable=true);

        // Chain actions
        IptBuilder& chainRuleAppend(const std::string& chainName);
        IptBuilder& chainRuleDelete(const std::string& chainName);
        IptBuilder& chainRuleDelete(const std::string& chainName, const int& ruleNumber);
        IptBuilder& chainRuleInsert(const std::string& chainName, const int& ruleNumber);
        IptBuilder& chainRuleReplace(const std::string& chainName, const int& ruleNumber);

        IptBuilder& chainDelete(const std::string& chainName);
        IptBuilder& chainFlush(const std::string& chainName);
        IptBuilder& chainNew(const std::string& chainName);
        IptBuilder& chainPolicy(const std::string& chainName, const std::string& option);
        IptBuilder& chainRename(const std::string& fromName, const std::string& toName);
        IptBuilder& chainZero(const std::string& chainName);
        IptBuilder& chainZero(const std::string& chainName, const int& ruleNumber);

        // Set target
        IptBuilder& targetGoto(const std::string& chainName);
        IptBuilder& targetJumpTo(const std::string& chainName);
        IptBuilder& targetAccept();
        IptBuilder& targetDrop();
        IptBuilder& targetQueue();
        IptBuilder& targetReturn();

        IptBuilder& targetAudit(const std::string& option);
        IptBuilder& targetChecksum(const std::string& option);
        IptBuilder& targetClassify(const std::string& option);
        IptBuilder& targetClusterip(const std::string& option);
        IptBuilder& targetConnmark(const std::string& option);
        IptBuilder& targetConnsecmark(const std::string& option);
        IptBuilder& targetCt(const std::string& option);
        IptBuilder& targetDnat(const std::string& option);
        IptBuilder& targetDnpt(const std::string& option);
        IptBuilder& targetDscp(const std::string& option);
        IptBuilder& targetEcn(const std::string& option);
        IptBuilder& targetHl(const std::string& option);
        IptBuilder& targetHmark(const std::string& option);
        IptBuilder& targetIdletimer(const std::string& option);
        IptBuilder& targetLed(const std::string& option);
        IptBuilder& targetLog(const std::string& option);
        IptBuilder& targetMark(const std::string& option);
        IptBuilder& targetMasquerade(const std::string& option);
        IptBuilder& targetMirror(const std::string& option);
        IptBuilder& targetNetmap(const std::string& option);
        IptBuilder& targetNflog(const std::string& option);
        IptBuilder& targetNfqueue(const std::string& option);
        IptBuilder& targetNotrack(const std::string& option);
        IptBuilder& targetRateest(const std::string& option);
        IptBuilder& targetRedirect(const std::string& option);
        IptBuilder& targetReject(const std::string& option);
        IptBuilder& targetSame(const std::string& option);
        IptBuilder& targetSecmark(const std::string& option);
        IptBuilder& targetSet(const std::string& option);
        IptBuilder& targetSnat(const std::string& option);
        IptBuilder& targetSnpt(const std::string& option);
        IptBuilder& targetTcpmss(const std::string& option);
        IptBuilder& targetTcpoptstrip(const std::string& option);
        IptBuilder& targetTee(const std::string& option);
        IptBuilder& targetTos(const std::string& option);
        IptBuilder& targetTproxy(const std::string& option);
        IptBuilder& targetTrace(const std::string& option);
        IptBuilder& targetTtl(const std::string& option);
        IptBuilder& targetUlog(const std::string& option);

        // Append matching conditions
        IptBuilder& andModule(const std::string& conditions);

        IptBuilder& moduleIpv4();
        IptBuilder& moduleIpv6();
        IptBuilder& moduleProtocol(const std::string& option);
        IptBuilder& moduleSource(const std::string& option);
        IptBuilder& moduleDestination(const std::string& option);
        IptBuilder& moduleInInterface(const std::string& option);
        IptBuilder& moduleOutInterface(const std::string& option);
        IptBuilder& moduleFragment(const std::string& option);
        IptBuilder& moduleSetCounter(const std::string& option);

        IptBuilder& moduleAddrtype(const std::string& option);
        IptBuilder& moduleAh(const std::string& option);
        IptBuilder& moduleBpf(const std::string& option);
        IptBuilder& moduleCluster(const std::string& option);
        IptBuilder& moduleComment(const std::string& option);
        IptBuilder& moduleConnbytes(const std::string& option);
        IptBuilder& moduleConnlabel(const std::string& option);
        IptBuilder& moduleConnlimit(const std::string& option);
        IptBuilder& moduleConnmark(const std::string& option);
        IptBuilder& moduleConntrack(const std::string& option);
        IptBuilder& moduleCpu(const std::string& option);
        IptBuilder& moduleDccp(const std::string& option);
        IptBuilder& moduleDevgroup(const std::string& option);
        IptBuilder& moduleDscp(const std::string& option);
        IptBuilder& moduleDst(const std::string& option);
        IptBuilder& moduleEcn(const std::string& option);
        IptBuilder& moduleEsp(const std::string& option);
        IptBuilder& moduleEui64(const std::string& option);
        IptBuilder& moduleFrag(const std::string& option);
        IptBuilder& moduleHashlimit(const std::string& option);
        IptBuilder& moduleHbh(const std::string& option);
        IptBuilder& moduleHelper(const std::string& option);
        IptBuilder& moduleHl(const std::string& option);
        IptBuilder& moduleIcmp(const std::string& option);
        IptBuilder& moduleIcmp6(const std::string& option);
        IptBuilder& moduleIprange(const std::string& option);
        IptBuilder& moduleIpv6header(const std::string& option);
        IptBuilder& moduleIpvs(const std::string& option);
        IptBuilder& moduleLength(const std::string& option);
        IptBuilder& moduleLimit(const std::string& option);
        IptBuilder& moduleMac(const std::string& option);
        IptBuilder& moduleMark(const std::string& option);
        IptBuilder& moduleMh(const std::string& option);
        IptBuilder& moduleMultiport(const std::string& option);
        IptBuilder& moduleNfacct(const std::string& option);
        IptBuilder& moduleOsf(const std::string& option);
        IptBuilder& moduleOwner(const std::string& option);
        IptBuilder& modulePhysdev(const std::string& option);
        IptBuilder& modulePkttype(const std::string& option);
        IptBuilder& modulePolicy(const std::string& option);
        IptBuilder& moduleQuota(const std::string& option);
        IptBuilder& moduleRateest(const std::string& option);
        IptBuilder& moduleRealm(const std::string& option);
        IptBuilder& moduleRecent(const std::string& option);
        IptBuilder& moduleRpfilter(const std::string& option);
        IptBuilder& moduleRt(const std::string& option);
        IptBuilder& moduleSctp(const std::string& option);
        IptBuilder& moduleSet(const std::string& option);
        IptBuilder& moduleSocket(const std::string& option);
        IptBuilder& moduleState(const std::string& option);
        IptBuilder& moduleStatistic(const std::string& option);
        IptBuilder& moduleString(const std::string& option);
        IptBuilder& moduleTcp(const std::string& option);
        IptBuilder& moduleTcpmss(const std::string& option);
        IptBuilder& moduleTime(const std::string& option);
        IptBuilder& moduleTos(const std::string& option);
        IptBuilder& moduleTtl(const std::string& option);
        IptBuilder& moduleU32(const std::string& option);
        IptBuilder& moduleUdp(const std::string& option);
        IptBuilder& moduleUnclean(const std::string& option);

    private:
        IptRuleInput m_input;
        IptRule      m_result;


        bool hasError();
        bool isChainRuleNumberError(const int& ruleNumber, const std::string& functionName);
        bool isEmptyInputError(const std::string& input, const std::string& functionName);
        bool isChainIncompatibleError(const IPT_TABLE_ENUM& table, const std::string& chainName, const IPT_TARGET_ENUM& target, const std::vector<IptModuleInput>& matchVector);
        bool isTableIncompatibleError(const IPT_TABLE_ENUM& table, const IPT_TARGET_ENUM& target);
        bool isTargetOrMatchNotAllowedError(const IPT_CHAIN_ENUM& chainAction, const std::string& chainOption);


        IptBuilder& appendModule(const std::string& functionName, const IPT_MODULE_ENUM action, const std::string& moduleOption);
        IptBuilder& updateChain(const std::string& functionName, const IPT_CHAIN_ENUM action, const std::string& chainName);
        IptBuilder& updateChain(const std::string& functionName, const IPT_CHAIN_ENUM action, const std::string& chainName, const std::string& chainOption);
        IptBuilder& updateChain(const std::string& functionName, const IPT_CHAIN_ENUM action, const std::string& chainName, const int& ruleNumber);
        IptBuilder& updateTarget(const std::string& functionName, const IPT_TARGET_ENUM action, const std::string& targetOption);
    };
}
}


#endif
