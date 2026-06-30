#include "InstanceBase.h"
#include <cassert>

InstanceBase::InstanceBase(std::shared_ptr<ResourceBase> _resource) 
	:resource(_resource)
	,wantDelete(false)
{
#if _DEBUG
	// リソースが無ければ警告
	assert(resource && "resouce is null");
#endif
}
