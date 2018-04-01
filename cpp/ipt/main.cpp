/*
 * main.cpp
 *
 *  Created on: Jan 3, 2018
 *      Author: kittipong
 */

#include <iostream>
#include "IptBuilder.h"

using namespace std;
using namespace kainr2::ip_tables;

int main()
{
    /**
     * Hello World!!!
     * 1: -t FILTER -N IPO
     * 2: -t FILTER -A INPUT -4 -p tcp -j DROP
     * 3: -t MANGLE -A QOS -i wan_name -j CONNMARK --restore-mark --mask 0xf0
     * 4: -t MANGLE -A QOS -i wan_name -m addrtype --src-type PROHIBIT -j CONNMARK --restore-mark --mask 0xf00
     */
    cout << "Hello World!!!" << endl;

    IptRule output = IptBuilder().chainNew("IPO").build();
    cout << "1: " << output.rule << endl;

    output = IptBuilder().chainRuleAppend("INPUT").targetDrop().andModule("-4 -p tcp ").build();
    cout << "2: " << output.rule << endl;

    output = IptBuilder(TABLE_MANGLE).chainRuleAppend("QOS").moduleInInterface("wan_name").targetConnmark("--restore-mark --mask 0xf00").build();
    cout << "3: " << output.rule << endl;

    output = IptBuilder(TABLE_MANGLE).chainRuleAppend("QOS").moduleInInterface("wan_name").moduleAddrtype("--src-type PROHIBIT").targetConnmark("--restore-mark --mask 0xf00").build();
    cout << "4: " << output.rule << endl;


    return 0;
}
