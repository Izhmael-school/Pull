#include "EffectManager.h"
#include "Manager/EffectResourceManager.h"
#include "EffekseerForDXLib.h"

EffectManager::EffectManager(EffectResourceManager& _resourceManager)
    :pEffectResourceManager(_resourceManager)
    ,instances()
{
}

EffectPtr EffectManager::Play(const std::string& _effectName, const VECTOR& _pos, float _scale, const VECTOR& _rot) {
    // リソースの取得
    auto resource = pEffectResourceManager.GetResource(_effectName);
    // リソースが無ければ帰る
    if (!resource) return nullptr;
    // インスタンスの生成
    auto instance = std::make_shared<EffectInstance>(resource);
    // 再生失敗したら帰る
    if (!instance->Play(_pos, _scale, _rot)) return nullptr;
    // 再生できたら配列に入れる
    instances.push_back(instance);

    return instance;
}

void EffectManager::Update() {
    // Effekseerの更新
    UpdateEffekseer3D();

    // インスタンスの更新
    for (auto& instance : instances) {
        instance->Update();
    }

    // 再生が終わったら消す
    std::erase_if(instances, [](EffectPtr _instance) {
        return _instance->IsEffectEnd();
    });
}

void EffectManager::Render() {
    // Effekseerの描画
    DrawEffekseer3D();
}

void EffectManager::Clean() {
    for (auto& instance : instances) {
        instance->Stop();
    }
    instances.clear();
    instances.shrink_to_fit();
}
