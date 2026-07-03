#include "AudioManager.h"
#include "Manager/AudioResourceManager.h"
#include "Instance/Audio/AudioInstance.h"

AudioManager::AudioManager(AudioResourceManager& _resourceManager)
	:pAudioResourceManager(_resourceManager)
	, instances() {
}

AudioPtr AudioManager::Play(const std::string& _audioName, float _volume, bool _isLoop, const VECTOR& _pos, float _distance) {
	// リソースを管理するクラス
	auto resource = pAudioResourceManager.GetResource(_audioName);
	// リソースが無ければ帰る
	if (!resource) return nullptr;
	// インスタンスの生成
	auto instance = std::make_shared<AudioInstance>(resource, _volume, _isLoop, _distance);
	// 再生失敗したら帰る
	if (!instance->Play(_pos)) return nullptr;
	// インスタンスを管理下に追加
	instances.push_back(instance);
	return instance;
}

void AudioManager::Update() {
	for (auto& instance : instances) {
		instance->Update();
	}

	// 再生が終わったら消す
	std::erase_if(instances, [](AudioPtr _instance) {
		return _instance->IsAudioEnd();
	});
}

void AudioManager::Render() {
}

void AudioManager::Clean() {
	StopAll();
	instances.clear();
	instances.shrink_to_fit();
}

void AudioManager::StopAll() {
	for (auto& instance : instances) {
		instance->Stop();
	}
}
