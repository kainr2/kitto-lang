#include <algorithm>
#include <iostream>
#include <sstream>
#include "IptBuilder.h"

namespace kainr2 {
namespace string_utils {
    // trim from start
    std::string ltrim(const std::string& str)
    {
        std::string s(str);
        s.erase(s.begin(),
                std::find_if(s.begin(),
                             s.end(),
                             std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
    }

    // trim from end
    std::string rtrim(const std::string& str)
    {
        std::string s(str);
        s.erase(std::find_if(s.rbegin(),
                             s.rend(),
                             std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
                s.end());
        return s;
    }

    // trim from both ends
    std::string trim(const std::string& str)
    {
        return ltrim(rtrim(str));
    }

    void to_upper(std::string& str)
    {
        for (auto & c: str)
        {
            c = static_cast<char>(std::toupper(c));
        }
    }
}
}


using namespace std;
using namespace kainr2;
using namespace kainr2::ip_tables;

//------------------------------------------------------------------------------
// PUBLIC
//------------------------------------------------------------------------------

const std::map<IPT_CHAIN_TYPE_ENUM, const std::string> IptBuilder::IPT_CHAIN_TYPE = {
    { CHAIN_TYPE_PREROUTING, "PREROUTING" },
    { CHAIN_TYPE_INPUT, "INPUT" },
    { CHAIN_TYPE_FORWARD, "FORWARD" },
    { CHAIN_TYPE_OUTPUT, "OUTPUT" },
    { CHAIN_TYPE_POSTROUTING, "POSTROUTING" },
    { CHAIN_TYPE_USER_DEFINED, "USER_DEFINED" }
};


const std::map<IPT_TABLE_ENUM, const std::string> IptBuilder::IPT_TABLE = {
    { TABLE_FILTER, "FILTER"},
    { TABLE_NAT,    "NAT"},
    { TABLE_MANGLE, "MANGLE"},
    { TABLE_RAW,    "RAW" },
    { TABLE_SECURITY, "SECURITY" }
};

const std::map<IPT_CHAIN_ENUM, const std::string> IptBuilder::IPT_CHAIN = {
    { CHAIN_RULE_APPEND,  "-A" },
    { CHAIN_RULE_DELETE,  "-D" },
    { CHAIN_RULE_INSERT,  "-I" },
    { CHAIN_RULE_REPLACE, "-R" },
    { CHAIN_DELETE, "-X" },
    { CHAIN_FLUSH,  "-F" },
    { CHAIN_NEW,    "-N" },
    { CHAIN_POLICY, "-P" },
    { CHAIN_RENAME, "-E" },
    { CHAIN_ZERO,   "-Z" }
};

const std::map<IPT_TARGET_ENUM, const std::string> IptBuilder::IPT_TARGET = {
    { TARGET_GOTO,    "-g" },
    { TARGET_JUMP,    "-j" },
    { TARGET_ACCEPT,  "-j ACCEPT" },
    { TARGET_DROP,    "-j DROP" },
    { TARGET_QUEUE,   "-j QUEUE" },
    { TARGET_RETURN,  "-j RETURN" },
    { TARGET_AUDIT, "-j AUDIT" },
    { TARGET_CHECKSUM, "-j CHECKSUM" },
    { TARGET_CLASSIFY, "-j CLASSIFY" },
    { TARGET_CLUSTERIP, "-j CLUSTERIP" },
    { TARGET_CONNMARK, "-j CONNMARK" },
    { TARGET_CONNSECMARK, "-j CONNSECMARK" },
    { TARGET_CT, "-j CT" },
    { TARGET_DNAT, "-j DNAT" },
    { TARGET_DNPT, "-j DNPT" },
    { TARGET_DSCP, "-j DSCP" },
    { TARGET_ECN, "-j ECN" },
    { TARGET_HL, "-j HL" },
    { TARGET_HMARK, "-j HMARK" },
    { TARGET_IDLETIMER, "-j IDLETIMER" },
    { TARGET_LED, "-j LED" },
    { TARGET_LOG, "-j LOG" },
    { TARGET_MARK, "-j MARK" },
    { TARGET_MASQUERADE, "-j MASQUERADE" },
    { TARGET_MIRROR, "-j MIRROR" },
    { TARGET_NETMAP, "-j NETMAP" },
    { TARGET_NFLOG, "-j NFLOG" },
    { TARGET_NFQUEUE, "-j NFQUEUE" },
    { TARGET_NOTRACK, "-j NOTRACK" },
    { TARGET_RATEEST, "-j RATEEST" },
    { TARGET_REDIRECT, "-j REDIRECT" },
    { TARGET_REJECT, "-j REJECT" },
    { TARGET_SAME, "-j SAME" },
    { TARGET_SECMARK, "-j SECMARK" },
    { TARGET_SET, "-j SET" },
    { TARGET_SNAT, "-j SNAT" },
    { TARGET_SNPT, "-j SNPT" },
    { TARGET_TCPMSS, "-j TCPMSS" },
    { TARGET_TCPOPTSTRIP, "-j TCPOPTSTRIP" },
    { TARGET_TEE, "-j TEE" },
    { TARGET_TOS, "-j TOS" },
    { TARGET_TPROXY, "-j TPROXY" },
    { TARGET_TRACE, "-j TRACE" },
    { TARGET_TTL, "-j TTL" },
    { TARGET_ULOG, "-j ULOG" }
};

const std::map<IPT_MODULE_ENUM, const std::string> IptBuilder::IPT_MODULE = {
    { MODULE_NONE,          "" },
    { MODULE_IPV4,          "-4" },
    { MODULE_IPV6,          "-6" },
    { MODULE_PROTOCOL,      "-p" },
    { MODULE_SOURCE,        "-s" },
    { MODULE_DESTINATION,   "-d" },
    { MODULE_IN_INTERFACE,  "-i" },
    { MODULE_OUT_INTERFACE, "-o" },
    { MODULE_FRAGMENT,      "-f" },
    { MODULE_SET_COUNTER,   "-c" },
    { MODULE_ADDRTYPE, "-m addrtype" },
    { MODULE_AH, "-m ah" },
    { MODULE_BPF, "-m bpf" },
    { MODULE_CLUSTER, "-m cluster" },
    { MODULE_COMMENT, "-m comment" },
    { MODULE_CONNBYTES, "-m connbytes" },
    { MODULE_CONNLABEL, "-m connlabel" },
    { MODULE_CONNLIMIT, "-m connlimit" },
    { MODULE_CONNMARK, "-m connmark" },
    { MODULE_CONNTRACK, "-m conntrack" },
    { MODULE_CPU, "-m cpu" },
    { MODULE_DCCP, "-m dccp" },
    { MODULE_DEVGROUP, "-m devgroup" },
    { MODULE_DSCP, "-m dscp" },
    { MODULE_DST, "-m dst" },
    { MODULE_ECN, "-m ecn" },
    { MODULE_ESP, "-m esp" },
    { MODULE_EUI64, "-m eui64" },
    { MODULE_FRAG, "-m frag" },
    { MODULE_HASHLIMIT, "-m hashlimit" },
    { MODULE_HBH, "-m hbh" },
    { MODULE_HELPER, "-m helper" },
    { MODULE_HL, "-m hl" },
    { MODULE_ICMP, "-m icmp" },
    { MODULE_ICMP6, "-m icmp6" },
    { MODULE_IPRANGE, "-m iprange" },
    { MODULE_IPV6HEADER, "-m ipv6header" },
    { MODULE_IPVS, "-m ipvs" },
    { MODULE_LENGTH, "-m length" },
    { MODULE_LIMIT, "-m limit" },
    { MODULE_MAC, "-m mac" },
    { MODULE_MARK, "-m mark" },
    { MODULE_MH, "-m mh" },
    { MODULE_MULTIPORT, "-m multiport" },
    { MODULE_NFACCT, "-m nfacct" },
    { MODULE_OSF, "-m osf" },
    { MODULE_OWNER, "-m owner" },
    { MODULE_PHYSDEV, "-m physdev" },
    { MODULE_PKTTYPE, "-m pkttype" },
    { MODULE_POLICY, "-m policy" },
    { MODULE_QUOTA, "-m quota" },
    { MODULE_RATEEST, "-m rateest" },
    { MODULE_REALM, "-m realm" },
    { MODULE_RECENT, "-m recent" },
    { MODULE_RPFILTER, "-m rpfilter" },
    { MODULE_RT, "-m rt" },
    { MODULE_SCTP, "-m sctp" },
    { MODULE_SET, "-m set" },
    { MODULE_SOCKET, "-m socket" },
    { MODULE_STATE, "-m state" },
    { MODULE_STATISTIC, "-m statistic" },
    { MODULE_STRING, "-m string" },
    { MODULE_TCP, "-m tcp" },
    { MODULE_TCPMSS, "-m tcpmss" },
    { MODULE_TIME, "-m time" },
    { MODULE_TOS, "-m tos" },
    { MODULE_TTL, "-m ttl" },
    { MODULE_U32, "-m u32" },
    { MODULE_UDP, "-m udp" },
    { MODULE_UNCLEAN, "-m unclean" }
};


IptBuilder::IptBuilder(const IPT_TABLE_ENUM& table)
{
    m_input.table = table;
}

IptBuilder::~IptBuilder()
{ ; }


IptBuilder& IptBuilder::chainRuleAppend(const std::string& chainName)
{
    return updateChain(__FUNCTION__, CHAIN_RULE_APPEND, chainName);
}

IptBuilder& IptBuilder::chainRuleDelete(const std::string& chainName)
{
    return updateChain(__FUNCTION__, CHAIN_RULE_DELETE, chainName);
}

IptBuilder& IptBuilder::chainRuleDelete(const std::string& chainName, const int& ruleNumber)
{
    return updateChain(__FUNCTION__, CHAIN_RULE_DELETE, chainName, ruleNumber);
}

IptBuilder& IptBuilder::chainRuleInsert(const std::string& chainName, const int& ruleNumber)
{
    return updateChain(__FUNCTION__, CHAIN_RULE_INSERT, chainName, ruleNumber);
}

IptBuilder& IptBuilder::chainRuleReplace(const std::string& chainName, const int& ruleNumber)
{
    return updateChain(__FUNCTION__, CHAIN_RULE_REPLACE, chainName, ruleNumber);
}

IptBuilder& IptBuilder::chainDelete(const std::string& chainName)
{
    return updateChain(__FUNCTION__, CHAIN_DELETE, chainName);
}

IptBuilder& IptBuilder::chainFlush(const std::string& chainName)
{
    return updateChain(__FUNCTION__, CHAIN_FLUSH, chainName);
}

IptBuilder& IptBuilder::chainNew(const std::string& chainName)
{
    return updateChain(__FUNCTION__, CHAIN_NEW, chainName);
}

IptBuilder& IptBuilder::chainPolicy(const std::string& chainName, const std::string& option)
{
    return updateChain(__FUNCTION__, CHAIN_POLICY, chainName, option);
}

IptBuilder& IptBuilder::chainRename(const std::string& fromName, const std::string& toName)
{
    return updateChain(__FUNCTION__, CHAIN_RENAME, fromName, toName);
}

IptBuilder& IptBuilder::chainZero(const std::string& chainName)
{
    return updateChain(__FUNCTION__, CHAIN_ZERO, chainName);
}

IptBuilder& IptBuilder::chainZero(const std::string& chainName, const int& ruleNumber)
{
    return updateChain(__FUNCTION__, CHAIN_ZERO, chainName, ruleNumber);
}

IptBuilder& IptBuilder::targetGoto(const std::string& chainName)
{
    return updateTarget(__FUNCTION__, TARGET_GOTO, chainName);
}

IptBuilder& IptBuilder::targetJumpTo(const std::string& chainName)
{
    return updateTarget(__FUNCTION__, TARGET_JUMP, chainName);
}

IptBuilder& IptBuilder::targetAccept()
{
    return updateTarget(__FUNCTION__, TARGET_ACCEPT, "");
}

IptBuilder& IptBuilder::targetDrop()
{
    return updateTarget(__FUNCTION__, TARGET_DROP, "");
}

IptBuilder& IptBuilder::targetQueue()
{
    return updateTarget(__FUNCTION__, TARGET_QUEUE, "");
}

IptBuilder& IptBuilder::targetReturn()
{
    return updateTarget(__FUNCTION__, TARGET_RETURN, "");
}

IptBuilder& IptBuilder::targetAudit(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_AUDIT, option);
}

IptBuilder& IptBuilder::targetChecksum(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_CHECKSUM, option);
}

IptBuilder& IptBuilder::targetClassify(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_CLASSIFY, option);
}

IptBuilder& IptBuilder::targetClusterip(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_CLUSTERIP, option);
}

IptBuilder& IptBuilder::targetConnmark(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_CONNMARK, option);
}

IptBuilder& IptBuilder::targetConnsecmark(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_CONNSECMARK, option);
}

IptBuilder& IptBuilder::targetCt(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_CT, option);
}

IptBuilder& IptBuilder::targetDnat(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_DNAT, option);
}

IptBuilder& IptBuilder::targetDnpt(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_DNPT, option);
}

IptBuilder& IptBuilder::targetDscp(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_DSCP, option);
}

IptBuilder& IptBuilder::targetEcn(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_ECN, option);
}

IptBuilder& IptBuilder::targetHl(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_HL, option);
}

IptBuilder& IptBuilder::targetHmark(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_HMARK, option);
}

IptBuilder& IptBuilder::targetIdletimer(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_IDLETIMER, option);
}

IptBuilder& IptBuilder::targetLed(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_LED, option);
}

IptBuilder& IptBuilder::targetLog(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_LOG, option);
}

IptBuilder& IptBuilder::targetMark(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_MARK, option);
}

IptBuilder& IptBuilder::targetMasquerade(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_MASQUERADE, option);
}

IptBuilder& IptBuilder::targetMirror(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_MIRROR, option);
}

IptBuilder& IptBuilder::targetNetmap(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_NETMAP, option);
}

IptBuilder& IptBuilder::targetNflog(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_NFLOG, option);
}

IptBuilder& IptBuilder::targetNfqueue(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_NFQUEUE, option);
}

IptBuilder& IptBuilder::targetNotrack(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_NOTRACK, option);
}

IptBuilder& IptBuilder::targetRateest(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_RATEEST, option);
}

IptBuilder& IptBuilder::targetRedirect(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_REDIRECT, option);
}

IptBuilder& IptBuilder::targetReject(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_REJECT, option);
}

IptBuilder& IptBuilder::targetSame(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_SAME, option);
}

IptBuilder& IptBuilder::targetSecmark(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_SECMARK, option);
}

IptBuilder& IptBuilder::targetSet(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_SET, option);
}

IptBuilder& IptBuilder::targetSnat(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_SNAT, option);
}

IptBuilder& IptBuilder::targetSnpt(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_SNPT, option);
}

IptBuilder& IptBuilder::targetTcpmss(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_TCPMSS, option);
}

IptBuilder& IptBuilder::targetTcpoptstrip(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_TCPOPTSTRIP, option);
}

IptBuilder& IptBuilder::targetTee(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_TEE, option);
}

IptBuilder& IptBuilder::targetTos(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_TOS, option);
}

IptBuilder& IptBuilder::targetTproxy(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_TPROXY, option);
}

IptBuilder& IptBuilder::targetTrace(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_TRACE, option);
}

IptBuilder& IptBuilder::targetTtl(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_TTL, option);
}

IptBuilder& IptBuilder::targetUlog(const std::string& option)
{
    return updateTarget(__FUNCTION__, TARGET_ULOG, option);
}


IptBuilder& IptBuilder::andModule(const std::string& conditions)
{
    return appendModule(__FUNCTION__, MODULE_NONE, conditions);
}

IptBuilder& IptBuilder::moduleIpv4()
{
    return appendModule(__FUNCTION__, MODULE_IPV4, "");
}

IptBuilder& IptBuilder::moduleIpv6()
{
    return appendModule(__FUNCTION__, MODULE_IPV6, "");
}

IptBuilder& IptBuilder::moduleProtocol(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_PROTOCOL, option);
}

IptBuilder& IptBuilder::moduleSource(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_SOURCE, option);
}

IptBuilder& IptBuilder::moduleDestination(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_DESTINATION, option);
}

IptBuilder& IptBuilder::moduleInInterface(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_IN_INTERFACE, option);
}

IptBuilder& IptBuilder::moduleOutInterface(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_OUT_INTERFACE, option);
}

IptBuilder& IptBuilder::moduleFragment(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_FRAGMENT, option);
}

IptBuilder& IptBuilder::moduleSetCounter(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_SET_COUNTER, option);
}

IptBuilder& IptBuilder::moduleAddrtype(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_ADDRTYPE, option);
}

IptBuilder& IptBuilder::moduleAh(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_AH, option);
}

IptBuilder& IptBuilder::moduleBpf(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_BPF, option);
}

IptBuilder& IptBuilder::moduleCluster(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_CLUSTER, option);
}

IptBuilder& IptBuilder::moduleComment(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_COMMENT, option);
}

IptBuilder& IptBuilder::moduleConnbytes(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_CONNBYTES, option);
}

IptBuilder& IptBuilder::moduleConnlabel(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_CONNLABEL, option);
}

IptBuilder& IptBuilder::moduleConnlimit(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_CONNLIMIT, option);
}

IptBuilder& IptBuilder::moduleConnmark(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_CONNMARK, option);
}

IptBuilder& IptBuilder::moduleConntrack(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_CONNTRACK, option);
}

IptBuilder& IptBuilder::moduleCpu(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_CPU, option);
}

IptBuilder& IptBuilder::moduleDccp(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_DCCP, option);
}

IptBuilder& IptBuilder::moduleDevgroup(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_DEVGROUP, option);
}

IptBuilder& IptBuilder::moduleDscp(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_DSCP, option);
}

IptBuilder& IptBuilder::moduleDst(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_DST, option);
}

IptBuilder& IptBuilder::moduleEcn(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_ECN, option);
}

IptBuilder& IptBuilder::moduleEsp(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_ESP, option);
}

IptBuilder& IptBuilder::moduleEui64(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_EUI64, option);
}

IptBuilder& IptBuilder::moduleFrag(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_FRAG, option);
}

IptBuilder& IptBuilder::moduleHashlimit(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_HASHLIMIT, option);
}

IptBuilder& IptBuilder::moduleHbh(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_HBH, option);
}

IptBuilder& IptBuilder::moduleHelper(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_HELPER, option);
}

IptBuilder& IptBuilder::moduleHl(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_HL, option);
}

IptBuilder& IptBuilder::moduleIcmp(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_ICMP, option);
}

IptBuilder& IptBuilder::moduleIcmp6(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_ICMP6, option);
}

IptBuilder& IptBuilder::moduleIprange(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_IPRANGE, option);
}

IptBuilder& IptBuilder::moduleIpv6header(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_IPV6HEADER, option);
}

IptBuilder& IptBuilder::moduleIpvs(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_IPVS, option);
}

IptBuilder& IptBuilder::moduleLength(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_LENGTH, option);
}

IptBuilder& IptBuilder::moduleLimit(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_LIMIT, option);
}

IptBuilder& IptBuilder::moduleMac(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_MAC, option);
}

IptBuilder& IptBuilder::moduleMark(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_MARK, option);
}

IptBuilder& IptBuilder::moduleMh(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_MH, option);
}

IptBuilder& IptBuilder::moduleMultiport(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_MULTIPORT, option);
}

IptBuilder& IptBuilder::moduleNfacct(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_NFACCT, option);
}

IptBuilder& IptBuilder::moduleOsf(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_OSF, option);
}

IptBuilder& IptBuilder::moduleOwner(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_OWNER, option);
}

IptBuilder& IptBuilder::modulePhysdev(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_PHYSDEV, option);
}

IptBuilder& IptBuilder::modulePkttype(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_PKTTYPE, option);
}

IptBuilder& IptBuilder::modulePolicy(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_POLICY, option);
}

IptBuilder& IptBuilder::moduleQuota(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_QUOTA, option);
}

IptBuilder& IptBuilder::moduleRateest(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_RATEEST, option);
}

IptBuilder& IptBuilder::moduleRealm(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_REALM, option);
}

IptBuilder& IptBuilder::moduleRecent(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_RECENT, option);
}

IptBuilder& IptBuilder::moduleRpfilter(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_RPFILTER, option);
}

IptBuilder& IptBuilder::moduleRt(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_RT, option);
}

IptBuilder& IptBuilder::moduleSctp(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_SCTP, option);
}

IptBuilder& IptBuilder::moduleSet(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_SET, option);
}

IptBuilder& IptBuilder::moduleSocket(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_SOCKET, option);
}

IptBuilder& IptBuilder::moduleState(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_STATE, option);
}

IptBuilder& IptBuilder::moduleStatistic(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_STATISTIC, option);
}

IptBuilder& IptBuilder::moduleString(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_STRING, option);
}

IptBuilder& IptBuilder::moduleTcp(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_TCP, option);
}

IptBuilder& IptBuilder::moduleTcpmss(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_TCPMSS, option);
}

IptBuilder& IptBuilder::moduleTime(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_TIME, option);
}

IptBuilder& IptBuilder::moduleTos(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_TOS, option);
}

IptBuilder& IptBuilder::moduleTtl(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_TTL, option);
}

IptBuilder& IptBuilder::moduleU32(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_U32, option);
}

IptBuilder& IptBuilder::moduleUdp(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_UDP, option);
}

IptBuilder& IptBuilder::moduleUnclean(const std::string& option)
{
    return appendModule(__FUNCTION__, MODULE_UNCLEAN, option);
}

//------------------------------------------------------------------------------
// PRIVATE
//------------------------------------------------------------------------------
bool IptBuilder::hasError()
{
    return m_result.hasError();
}

bool IptBuilder::isChainRuleNumberError(const int& ruleNumber, const std::string& functionName)
{
    if (ruleNumber < 1)
    {
        m_result.errorCode = IptRule::ERROR_BAD_CHAIN_RULE_NUMBER;
        m_result.errorMessage = "[ERROR][" + functionName + "] Invalid Rule number: " + std::to_string(ruleNumber);
        return true;
    }
    return false;
}

bool IptBuilder::isEmptyInputError(const std::string& input, const std::string& functionName)
{
    if (input.empty())
    {
        m_result.errorCode = IptRule::ERROR_NO_INPUT;
        m_result.errorMessage = "[ERROR][" + functionName + "] Empty input";
        return true;
    }
    return false;
}

bool IptBuilder::isChainIncompatibleError(const IPT_TABLE_ENUM& table, const std::string& chainName, const IPT_TARGET_ENUM& target, const std::vector<IptModuleInput>& matchVector)
{
    bool isValid = true;

    // Check for chain-x compatibility
    IPT_CHAIN_TYPE_ENUM chainType = CHAIN_TYPE_USER_DEFINED;
    if (IPT_CHAIN_TYPE.at(CHAIN_TYPE_PREROUTING)==chainName)
    {
        chainType = CHAIN_TYPE_PREROUTING;
    }
    else if (IPT_CHAIN_TYPE.at(CHAIN_TYPE_INPUT)==chainName)
    {
        chainType = CHAIN_TYPE_INPUT;
    }
    else if (IPT_CHAIN_TYPE.at(CHAIN_TYPE_FORWARD)==chainName)
    {
        chainType = CHAIN_TYPE_FORWARD;
    }
    else if (IPT_CHAIN_TYPE.at(CHAIN_TYPE_OUTPUT)==chainName)
    {
        chainType = CHAIN_TYPE_OUTPUT;
    }
    else if (IPT_CHAIN_TYPE.at(CHAIN_TYPE_POSTROUTING)==chainName)
    {
        chainType = CHAIN_TYPE_POSTROUTING;
    }

    // Check for chain-target compatibility
    switch (target)
    {
    case TARGET_DNAT:
        isValid = false;
        if (chainType==CHAIN_TYPE_USER_DEFINED
        ||  chainType==CHAIN_TYPE_PREROUTING
        ||  chainType==CHAIN_TYPE_INPUT)
        {
            isValid = (table==TABLE_NAT);
        }
        break;
    case TARGET_MASQUERADE:
        isValid = (chainType==CHAIN_TYPE_POSTROUTING);
        break;
    case TARGET_MIRROR:
        isValid = (chainType==CHAIN_TYPE_USER_DEFINED
            || chainType==CHAIN_TYPE_PREROUTING
            || chainType==CHAIN_TYPE_INPUT
            || chainType==CHAIN_TYPE_FORWARD);
        break;
    case TARGET_REDIRECT:
        isValid = false;
        if (chainType==CHAIN_TYPE_USER_DEFINED
        ||  chainType==CHAIN_TYPE_PREROUTING
        ||  chainType==CHAIN_TYPE_OUTPUT)
        {
            isValid = (table==TABLE_NAT);
        }
        break;
    case TARGET_REJECT:
        isValid = (chainType==CHAIN_TYPE_USER_DEFINED
            || chainType==CHAIN_TYPE_PREROUTING
            || chainType==CHAIN_TYPE_FORWARD
            || chainType==CHAIN_TYPE_OUTPUT);
        break;
    case TARGET_SNAT:
        isValid = (chainType==CHAIN_TYPE_USER_DEFINED
            || chainType==CHAIN_TYPE_INPUT
            || chainType==CHAIN_TYPE_POSTROUTING);
        break;
    case TARGET_TPROXY:
        isValid = false;
        if (chainType==CHAIN_TYPE_USER_DEFINED
        ||  chainType==CHAIN_TYPE_PREROUTING)
        {
            isValid = (table==TABLE_MANGLE);
        }
        break;
    default:
        isValid = true;
        break;
    }

    if (!isValid)
    {
        m_result.errorCode = IptRule::ERROR_CHAIN_AND_TARGET_INCOMPATIBLE;
        m_result.errorMessage = "[ERROR] Chain-target incompatible: [CHAIN: " + chainName + "][TARGET: " + IPT_TARGET.at(target) + "]";
        return true;
    }

    // Check for chain-match compatibility
    isValid = true;
    std::string matchName = "";
    if (std::find_if(matchVector.begin(), matchVector.end(), IptModuleInput(MODULE_EUI64)) != matchVector.end())
    {
        matchName = IPT_MODULE.at(MODULE_EUI64);
        isValid = (chainType==CHAIN_TYPE_PREROUTING
            || chainType==CHAIN_TYPE_INPUT
            || chainType==CHAIN_TYPE_FORWARD);
    }
    else if (std::find_if(matchVector.begin(), matchVector.end(), IptModuleInput(MODULE_MAC)) != matchVector.end())
    {
        matchName = IPT_MODULE.at(MODULE_MAC);
        isValid = (chainType==CHAIN_TYPE_PREROUTING
            || chainType==CHAIN_TYPE_INPUT
            || chainType==CHAIN_TYPE_FORWARD);
    }
    else if (std::find_if(matchVector.begin(), matchVector.end(), IptModuleInput(MODULE_OWNER)) != matchVector.end())
    {
        matchName = IPT_MODULE.at(MODULE_OWNER);
        isValid = (chainType==CHAIN_TYPE_OUTPUT
            || chainType==CHAIN_TYPE_POSTROUTING);
    }
    else if (std::find_if(matchVector.begin(), matchVector.end(), IptModuleInput(MODULE_RPFILTER)) != matchVector.end())
    {
        matchName = IPT_MODULE.at(MODULE_RPFILTER);
        isValid = (chainType==CHAIN_TYPE_PREROUTING);
    }

    if (!isValid)
    {
        m_result.errorCode = IptRule::ERROR_CHAIN_AND_MODULE_INCOMPATIBLE;
        m_result.errorMessage = "[ERROR] Chain-match incompatible: [CHAIN: " + chainName + "][MATCH: " + matchName + "]";
        return true;
    }

    return false;
}

bool IptBuilder::isTableIncompatibleError(const IPT_TABLE_ENUM& table, const IPT_TARGET_ENUM& target)
{
    bool isValid = true;

    // check for table-target compatibility
    switch (target)
    {
    case TARGET_DNAT:
    case TARGET_MASQUERADE:
    case TARGET_NETMAP:
    case TARGET_REDIRECT:
    case TARGET_SNAT:
        isValid = (table==TABLE_NAT);
        break;
    case TARGET_CHECKSUM:
    case TARGET_DNPT:
    case TARGET_DSCP:
    case TARGET_ECN:
    case TARGET_HL:
    case TARGET_SNPT:
    case TARGET_TOS:
    case TARGET_TPROXY:
    case TARGET_TTL:
        isValid = (table==TABLE_MANGLE);
        break;
    case TARGET_CT:
    case TARGET_NOTRACK:
    case TARGET_TRACE:
        isValid = (table==TABLE_RAW);
        break;
    case TARGET_CONNSECMARK:
    case TARGET_SECMARK:
        isValid = (table==TABLE_SECURITY || table==TABLE_MANGLE);
        break;
    default:
        isValid = true;
        break;
    }

    if (!isValid)
    {
        m_result.errorCode = IptRule::ERROR_TABLE_AND_TARGET_INCOMPATIBLE;
        m_result.errorMessage = "[ERROR] Table-target incompatible: [TABLE: " + IPT_TABLE.at(table) + "][TARGET: " + IPT_TARGET.at(target) + "]";
        return true;
    }

    return false;
}


// For some chain actions, targets and matches are not allowed.
bool IptBuilder::isTargetOrMatchNotAllowedError(const IPT_CHAIN_ENUM& chainAction, const std::string& chainOption)
{
    // If CHAIN DELETE has no rule number, then target/match are allowed
    if (chainAction==CHAIN_RULE_DELETE && chainOption.empty())
    {
        return false;
    }

    switch (chainAction)
    {
    case CHAIN_RULE_DELETE:
    case CHAIN_DELETE:
    case CHAIN_FLUSH:
    case CHAIN_NEW:
    case CHAIN_RENAME:
    case CHAIN_ZERO:
        // If contain a target or a match
        if ((m_input.targetAction!=TARGET_NONE) || !m_input.moduleVector.empty())
        {
            m_result.errorCode = IptRule::ERROR_MODULE_OR_TARGET_NOT_ALLOWED;
            m_result.errorMessage = "[ERROR] The given chain action does not allowed target and/or match";
            return true;
        }
        break;
    default:
        break;
    }

    return false;
}


IptBuilder& IptBuilder::appendModule(const std::string& functionName, const IPT_MODULE_ENUM action, const std::string& moduleOption)
{
    if (!hasError())
    {
        // If given as-is, or an option-less action.  Otherwise, must have some options.
        if (action==MODULE_NONE || action==MODULE_IPV4 || action==MODULE_IPV6
        ||  !isEmptyInputError(moduleOption, functionName))
        {
            IptModuleInput input;
            input.moduleAction = action;
            input.moduleOption = string_utils::trim(moduleOption);
            m_input.moduleVector.push_back(input);
        }
    }
    return *this;
}


IptBuilder& IptBuilder::updateChain(const std::string& functionName, const IPT_CHAIN_ENUM action, const std::string& chainName)
{
    if (!hasError() && !isEmptyInputError(chainName, functionName))
    {
        m_input.chainAction = action;
        m_input.chainName = string_utils::trim(chainName);
    }
    return *this;
}

IptBuilder& IptBuilder::updateChain(const std::string& functionName, const IPT_CHAIN_ENUM action, const std::string& chainName, const std::string& chainOption)
{
    if (!hasError() && !isEmptyInputError(chainName, functionName) && !isEmptyInputError(chainOption, functionName))
    {
        m_input.chainAction = action;
        m_input.chainName = string_utils::trim(chainName);
        m_input.chainOption = string_utils::trim(chainOption);
    }
    return *this;
}

IptBuilder& IptBuilder::updateChain(const std::string& functionName, const IPT_CHAIN_ENUM action, const std::string& chainName, const int& ruleNumber)
{
    if (!hasError() && !isEmptyInputError(chainName, functionName) && !isChainRuleNumberError(ruleNumber, functionName))
    {
        m_input.chainAction = action;
        m_input.chainName = string_utils::trim(chainName);
        m_input.chainOption = std::to_string(ruleNumber);
    }
    return *this;
}

IptBuilder& IptBuilder::updateTarget(const std::string& functionName, const IPT_TARGET_ENUM action, const std::string& targetOption)
{
    if (!hasError())
    {
        m_input.targetAction = action;
        m_input.targetOption = string_utils::trim(targetOption);
    }
    return *this;
}

IptRule IptBuilder::build(bool showTable)
{
    if (m_input.chainAction==CHAIN_RULE_NONE)
    {
        m_result.errorCode = IptRule::ERROR_CHAIN_NAME_MISSING;
        m_result.errorMessage = "[ERROR] No Chain rule specified.";
        return m_result;
    }

    if (hasError()
    ||  isTargetOrMatchNotAllowedError(m_input.chainAction, m_input.chainOption)
    ||  isTableIncompatibleError(m_input.table, m_input.targetAction)
    ||  isChainIncompatibleError(m_input.table, m_input.chainName, m_input.targetAction, m_input.moduleVector))
    {
        return m_result;
    }

    // Create the final rule string
    std::stringstream ss;
    if (showTable)
    {
        ss << "-t" << " " << IPT_TABLE.at(m_input.table) << " ";
    }

    // Append chain name
    ss << IPT_CHAIN.at(m_input.chainAction) << " " << m_input.chainName << " ";
    if (!m_input.chainOption.empty())
    {
        ss << m_input.chainOption << " ";
    }

    // Append match/module section
    if (!m_input.moduleVector.empty())
    {
        for (const IptModuleInput& match : m_input.moduleVector)
        {
            if (match.moduleAction==MODULE_NONE)
            {
                ss << match.moduleOption << " ";
            }
            else
            {
                ss << IPT_MODULE.at(match.moduleAction) << " " << match.moduleOption << " ";
            }
        }
    }

    // Append target section
    if (m_input.targetAction!=TARGET_NONE)
    {
        ss << IPT_TARGET.at(m_input.targetAction) << " ";
        if (!m_input.targetOption.empty())
        {
            ss << m_input.targetOption << " ";
        }
    }

    m_result.rule = string_utils::trim(ss.str());
    return m_result;
}
