#include "EffectInstance.h"
#include "EffekseerForDXLib.h"
#include "Resource/Effect/EffectResource.h"

EffectInstance::EffectInstance(std::shared_ptr<EffectResource> _effectResource)
	:InstanceBase(_effectResource)
	,playHandle(-1)
{}

void EffectInstance::Update() {
}

void EffectInstance::Render() {
}

bool EffectInstance::Play(VECTOR _pos, float _scale, VECTOR _rot) {
	// 二重再生禁止
	if (IsEffectEnd()) return false;
	// リソースが無ければ帰る
	if (!resource) return false;
	// エフェクト再生
	playHandle = PlayEffekseer3DEffect(resource->GetHandle());
	// 再生できなかったら帰る
	if (playHandle == -1) return false;
	// 座標を設定
	SetPosPlayingEffekseer3DEffect(playHandle, _pos.x, _pos.y, _pos.z);
	// 回転を設定
	SetRotationPlayingEffekseer3DEffect(playHandle, _rot.x, _rot.y, _rot.z);
	// 拡縮を設定
	SetScalePlayingEffekseer3DEffect(playHandle, _scale, _scale, _scale);

	return true;
}

void EffectInstance::Stop() {
	// 再生して無ければ帰る
	if (IsEffectEnd()) return;
	// 停止
	StopEffekseer3DEffect(playHandle);
	playHandle = -1;
}

bool EffectInstance::IsEffectEnd() {
	if (playHandle == -1) return true;
	return IsEffekseer3DEffectPlaying(playHandle);
}
