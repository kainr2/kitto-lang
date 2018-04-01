#pragma once
#ifndef __IPT_ENUM_H__
#define __IPT_ENUM_H__

namespace kainr2 {
namespace ip_tables {

    enum IPT_CHAIN_TYPE_ENUM
    {
        CHAIN_TYPE_PREROUTING,
        CHAIN_TYPE_INPUT,
        CHAIN_TYPE_FORWARD,
        CHAIN_TYPE_OUTPUT,
        CHAIN_TYPE_POSTROUTING,
        CHAIN_TYPE_USER_DEFINED
    };

    enum IPT_TABLE_ENUM
    {
        TABLE_NONE,
        TABLE_FILTER,
        TABLE_NAT,
        TABLE_MANGLE,
        TABLE_RAW,
        TABLE_SECURITY
    };

    enum IPT_CHAIN_ENUM
    {
        CHAIN_RULE_NONE,
        CHAIN_RULE_APPEND,
        CHAIN_RULE_DELETE,
        CHAIN_RULE_INSERT,
        CHAIN_RULE_REPLACE,
        CHAIN_DELETE,
        CHAIN_FLUSH,
        CHAIN_NEW,
        CHAIN_POLICY,
        CHAIN_RENAME,
        CHAIN_ZERO
    };

    enum IPT_TARGET_ENUM
    {
        TARGET_NONE,
        TARGET_GOTO,
        TARGET_JUMP,
        TARGET_ACCEPT,
        TARGET_DROP,
        TARGET_QUEUE,
        TARGET_RETURN,
        TARGET_AUDIT,
        TARGET_CHECKSUM,
        TARGET_CLASSIFY,
        TARGET_CLUSTERIP,
        TARGET_CONNMARK,
        TARGET_CONNSECMARK,
        TARGET_CT,
        TARGET_DNAT,
        TARGET_DNPT,
        TARGET_DSCP,
        TARGET_ECN,
        TARGET_HL,
        TARGET_HMARK,
        TARGET_IDLETIMER,
        TARGET_LED,
        TARGET_LOG,
        TARGET_MARK,
        TARGET_MASQUERADE,
        TARGET_MIRROR,
        TARGET_NETMAP,
        TARGET_NFLOG,
        TARGET_NFQUEUE,
        TARGET_NOTRACK,
        TARGET_RATEEST,
        TARGET_REDIRECT,
        TARGET_REJECT,
        TARGET_SAME,
        TARGET_SECMARK,
        TARGET_SET,
        TARGET_SNAT,
        TARGET_SNPT,
        TARGET_TCPMSS,
        TARGET_TCPOPTSTRIP,
        TARGET_TEE,
        TARGET_TOS,
        TARGET_TPROXY,
        TARGET_TRACE,
        TARGET_TTL,
        TARGET_ULOG
    };

    enum IPT_MODULE_ENUM
    {
        MODULE_NONE,
        MODULE_IPV4,
        MODULE_IPV6,
        MODULE_PROTOCOL,
        MODULE_SOURCE,
        MODULE_DESTINATION,
        MODULE_IN_INTERFACE,
        MODULE_OUT_INTERFACE,
        MODULE_FRAGMENT,
        MODULE_SET_COUNTER,
        MODULE_ADDRTYPE,
        MODULE_AH,
        MODULE_BPF,
        MODULE_CLUSTER,
        MODULE_COMMENT,
        MODULE_CONNBYTES,
        MODULE_CONNLABEL,
        MODULE_CONNLIMIT,
        MODULE_CONNMARK,
        MODULE_CONNTRACK,
        MODULE_CPU,
        MODULE_DCCP,
        MODULE_DEVGROUP,
        MODULE_DSCP,
        MODULE_DST,
        MODULE_ECN,
        MODULE_ESP,
        MODULE_EUI64,
        MODULE_FRAG,
        MODULE_HASHLIMIT,
        MODULE_HBH,
        MODULE_HELPER,
        MODULE_HL,
        MODULE_ICMP,
        MODULE_ICMP6,
        MODULE_IPRANGE,
        MODULE_IPV6HEADER,
        MODULE_IPVS,
        MODULE_LENGTH,
        MODULE_LIMIT,
        MODULE_MAC,
        MODULE_MARK,
        MODULE_MH,
        MODULE_MULTIPORT,
        MODULE_NFACCT,
        MODULE_OSF,
        MODULE_OWNER,
        MODULE_PHYSDEV,
        MODULE_PKTTYPE,
        MODULE_POLICY,
        MODULE_QUOTA,
        MODULE_RATEEST,
        MODULE_REALM,
        MODULE_RECENT,
        MODULE_RPFILTER,
        MODULE_RT,
        MODULE_SCTP,
        MODULE_SET,
        MODULE_SOCKET,
        MODULE_STATE,
        MODULE_STATISTIC,
        MODULE_STRING,
        MODULE_TCP,
        MODULE_TCPMSS,
        MODULE_TIME,
        MODULE_TOS,
        MODULE_TTL,
        MODULE_U32,
        MODULE_UDP,
        MODULE_UNCLEAN
    };

} // ipt
} // kainr2

#endif
