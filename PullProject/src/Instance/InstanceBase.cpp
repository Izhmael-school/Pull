#include "InstanceBase.h"
#include <cassert>

InstanceBase::InstanceBase(ResourcePtr _resource)
	:resource(_resource)
	,wantDelete(false)
{
#if _DEBUG
	// リソースが無ければ警告
	assert(resource && "resouce is null");
#endif
}
