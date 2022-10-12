#include "PermissionDeniedException.h"
#define PERMISSION_DENIED_FOR_THIS_ACTION "PERMISSION_DENIED_FOR_THIS_ACTION"

PermissionDeniedException::PermissionDeniedException() : Exception(PERMISSION_DENIED_FOR_THIS_ACTION)
{

}