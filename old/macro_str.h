#pragma once

#define XSTR(s) STR(s)
#define STR(s) #s

#define XCAT(x, y) CAT(x, y)
#define CAT(x, y) x##y