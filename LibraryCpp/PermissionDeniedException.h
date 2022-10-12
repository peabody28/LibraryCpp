#pragma once
#include "Exception.h"

class PermissionDeniedException : public Exception
{
public:
	PermissionDeniedException();
};