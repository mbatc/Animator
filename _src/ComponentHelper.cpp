#include "ComponentHelper.h"

#include <cstring>

COMPONENTID COMPONENT::GetType(char* strID)
{
	if (!strcmp(strID, COMP_ID_CNULL))
		return COMPONENTID::CNULL;

	return COMPONENTID::CERROR;
}