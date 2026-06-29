#include "ResourceBase.h"

ResourceBase::ResourceBase(const std::string& _name, const std::string& _path)
    :name(_name)
    ,path(_path)
    ,loadHandle(-1)
{}