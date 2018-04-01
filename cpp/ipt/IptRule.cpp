#include "IptRule.h"

using namespace kainr2::ip_tables;


IptRule::IptRule()
{ ; }

bool IptRule::hasError()
{
    return (errorCode!=IPT_ERROR::ERROR_NONE);
}
