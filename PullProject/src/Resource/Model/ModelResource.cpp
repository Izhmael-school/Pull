#include "ModelResource.h"
#include "DxLib.h"
#include <cassert>

ModelResource::ModelResource(const std::string& _name, const std::string& _path) 
    :ResourceBase(_name,_path)
{}

ModelResource::~ModelResource() {
    if (IsLoaded())
        MV1DeleteModel(loadHandle);
}

bool ModelResource::Load() {
    // 多重読み込みしない
    if (IsLoaded())
        return true;

    // 読み込み
    loadHandle = MV1LoadModel(path.c_str());

    // 失敗
    if (!IsLoaded()) {
#if _DEBUG
        assert(false && "Failed Load Model");
        return false;
#endif
    }

    return true;
}

int ModelResource::GetDupricateHandle() {
    return MV1DuplicateModel(loadHandle);
}
