#pragma once
#include "Exception.h"
#define PERMISSION_DENIED_FOR_THIS_ACTION "PERMISSION_DENIED_FOR_THIS_ACTION"

class PermissionDeniedException : public Exception
{
public:
	PermissionDeniedException() : Exception(PERMISSION_DENIED_FOR_THIS_ACTION)
	{

	}
};