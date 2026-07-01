#include "EffectInstance.h"
#include "EffekseerForDXLib.h"
#include "Resource/Effect/EffectResource.h"

EffectInstance::EffectInstance(std::shared_ptr<EffectResource> _effectResource)
	:InstanceBase(_effectResource)
	, playHandle(-1) {
}

void EffectInstance::Update() {
	GameObject::Update();

	VECTOR pos = GetPosition();
	SetPosPlayingEffekseer3DEffect(playHandle, pos.x, pos.y, pos.z);
	VECTOR rot = GetRotation();
	SetRotationPlayingEffekseer3DEffect(playHandle, rot.x, rot.y, rot.z);
	VECTOR scale = GetScale();
	SetScalePlayingEffekseer3DEffect(playHandle, scale.x, scale.y, scale.z);
}

void EffectInstance::Render() {
	GameObject::Render();
}

bool EffectInstance::Play(VECTOR _pos, float _scale, VECTOR _rot) {
	// 二重再生禁止
	if (!IsEffectEnd()) return false;
	// リソースが無ければ帰る
	if (!resource) return false;
	// エフェクト再生
	playHandle = PlayEffekseer3DEffect(resource->GetHandle());
	// 再生できなかったら帰る
	if (playHandle == -1) return false;
	auto transform = GetTransform();
	// 座標を設定
	SetPosPlayingEffekseer3DEffect(playHandle, _pos.x, _pos.y, _pos.z);
	transform->SetPosition(_pos);
	// 回転を設定
	SetRotationPlayingEffekseer3DEffect(playHandle, _rot.x, _rot.y, _rot.z);
	transform->SetRotation(_rot);
	// 拡縮を設定
	SetScalePlayingEffekseer3DEffect(playHandle, _scale, _scale, _scale);
	transform->SetScale(_scale);	
	
	return true;
}

void EffectInstance::Stop() {
	// 再生して無ければ帰る
	if (IsEffectEnd()) return;
	// 停止
	StopEffekseer3DEffect(playHandle);
	playHandle = -1;
}

const bool EffectInstance::IsEffectEnd() const {
	return IsEffekseer3DEffectPlaying(playHandle) == -1;
}
