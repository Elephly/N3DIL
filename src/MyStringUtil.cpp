#include "MyStringUtil.h"

#include "MyDefines.h"

#include <cstdarg>
#include <cstring>

char * MyStringUtil::CopyString(const char * string)
{
	char *out = new char[strlen(string) + 1]();
	strcpy(out, string);
	return out;
}
