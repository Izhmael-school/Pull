#include "ModelResource.h"
#include "DxLib.h"
#include "Definition/CommonModule/MyString.h"
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

    if (!IsLoaded()) {
    // 失敗
#if _DEBUG
        std::string fail = MyString::MergeString("Failed Load Model:", name);
        assert(false && fail.c_str());
#endif
        return false;
    }

    return true;
}

int ModelResource::GetDupricateHandle() {
    return MV1DuplicateModel(loadHandle);
}
