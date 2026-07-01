#include "AudioResource.h"
#include "DxLib.h"
#include "Definition/CommonModule/MyString.h"
#include <cassert>

AudioResource::AudioResource(const std::string& _name, const std::string& _path, bool _is3D) 
	:ResourceBase(_name, _path)
	, is3D(_is3D)
{}

AudioResource::~AudioResource() {
	if (IsLoaded())
		DeleteSoundMem(loadHandle);
}

bool AudioResource::Load() {
	if (IsLoaded()) return false;

	// 3Dにするか
	SetCreate3DSoundFlag(static_cast<int>(is3D));

	// 読み込み
	loadHandle = LoadSoundMem(path.c_str());

	if (!IsLoaded()) {
		// 失敗
#if _DEBUG
		std::string fail = MyString::MergeString("Failed Load Audio:", name);
		assert(false && fail.c_str());
#endif // _DEBUG
		return false;
	}

	return true;
}
