/*
 * @file Animator.cpp
 * @author Sekino
 */
#include "Animator.h"
#include <DxLib.h>

Animator::Animator(int modelHandle)
	:animationModelHandle(modelHandle)
	, pAnimations()
	, currentAnimation(-1)
	, isPlaying(false) {
}

Animator::~Animator() {
	for (auto& anim : pAnimations) {
		MV1DeleteModel(anim->animationHandle);
		anim.reset();
		anim = nullptr;
	}
	pAnimations.shrink_to_fit();
}

void Animator::Update() {
	if (currentAnimation == -1)
		return;

	// 現在のアニメーションを進める
	AnimationClip<void>* pCurrentAnim = GetAnimation(currentAnimation);

	if (pCurrentAnim == nullptr)
		return;

	// アニメーションを進める
	pCurrentAnim->playTime += pCurrentAnim->playSpeed;

	// イベントの処理
	if (pCurrentAnim->events.size() > 0) {
		for (auto& e : pCurrentAnim->events) {
			if (pCurrentAnim->playTime < e.eventTime || e.isAction) continue;
			e.isAction = true;
			e.animEvent();
		}
	}

	// 終了時間を超えたら
	if (pCurrentAnim->playTime > pCurrentAnim->totalTime) {
		// 再生フラグを落とす
		isPlaying = false;
		pCurrentAnim->playTime = 0.0f;
		// アニメーションイベントの再使用できるようにする
		for (auto& e : pCurrentAnim->events) {
			e.isAction = false;
		}
		// 再生していたアニメーションがループ再生するかどうか
		if (pCurrentAnim->isLoop)
			isPlaying = true;
		else
			return;
	}

	// アニメーションを再生
	MV1SetAttachAnimTime(animationModelHandle, 0, pCurrentAnim->playTime);
}

void Animator::Load(std::string _filePath, bool _canInterrupt, bool _isLoop) {
	int animHandle = MV1LoadModel(_filePath.c_str());
	// 読み込めてなかったらリターン
	if (animHandle == -1) return;
	std::unique_ptr<AnimationClip<>> pAnimClip;
	// ファイル内にあるアニメーション全てを読み込む
	for (int i = 0, max = MV1GetAnimNum(animHandle); i < max;i++) {
		std::string name = DeleteUnnecessaryName(MV1GetAnimName(animHandle, i));
		pAnimClip = std::make_unique<AnimationClip<>>(animHandle, name, _canInterrupt, _isLoop);
		pAnimClip->animIndex = i;
		// アニメーション群に追加
		pAnimations.push_back(std::move(pAnimClip));
	}
}

void Animator::Load(int _animHandle, bool _canInterrupt, bool _isLoop){
	// 読み込めてなかったらリターン
	if (_animHandle == -1) return;
	std::unique_ptr<AnimationClip<>> pAnimClip;
	// ファイル内にあるアニメーション全てを読み込む
	for (int i = 0, max = MV1GetAnimNum(_animHandle); i < max;i++) {
		std::string name = DeleteUnnecessaryName(MV1GetAnimName(_animHandle, i));
		pAnimClip = std::make_unique<AnimationClip<>>(_animHandle, name, _canInterrupt, _isLoop);
		pAnimClip->animIndex = i;
		// アニメーション群に追加
		pAnimations.push_back(std::move(pAnimClip));
	}
}

void Animator::Play(int _index, float _speed) {
	// 現在再生中のアニメーションの場合は処理しない
	if (_index == currentAnimation)
		return;

	// 再生するアニメーション番号に書き換える
	currentAnimation = _index;

	AnimationClip<>* pAnim = pAnimations[_index].get();

	// 再生中だったアニメーションをデタッチする
	MV1DetachAnim(animationModelHandle, 0);
	// 適応するアニメーションの再生時間の初期化
	pAnim->playTime = 0.0f;
	// 適応するアニメーションの再生速度の初期化
	pAnim->playSpeed = _speed;
	// 適応するアニメーションをアタッチする
	int attachIndex = MV1AttachAnim(animationModelHandle, pAnim->animIndex, pAnim->animationHandle, FALSE);
	// 適応したアニメーションの終了時間を初期化する
	pAnim->totalTime = MV1GetAttachAnimTotalTime(animationModelHandle, attachIndex);
	// 再生フラグを建てる
	isPlaying = true;
	// イベントフラグの初期化
	for (auto& e : pAnimations[_index]->events) {
		e.isAction = false;
	}
}

int Animator::Play(std::string _name, float _speed) {
	int size = static_cast<int>(pAnimations.size());
	for (int i = 0; i < size; i++) {
		if (pAnimations[i]->name != _name) continue;

		Play(i, _speed);
		return 0;
	}
	return -1;
}

AnimationClip<>* Animator::GetAnimation(std::string _name) const {
	int size = static_cast<int>(pAnimations.size());
	for (int i = 0; i < size; i++) {
		if (pAnimations[i]->name != _name) continue;
		return pAnimations[i].get();
	}
	return nullptr;
}

std::string Animator::DeleteUnnecessaryName(std::string _name){
	int size = static_cast<int>(_name.size());
	int useIndex = -1;
	for (int i = size; 0 < i;i--) {
		char c = _name[i];

		if (c != '|') continue;

		useIndex = i + 1;
		break;
	}

	if (useIndex == -1) return _name;

	std::string newName;

	for (int i = 0, max = size - useIndex;i < max;i++) {
		newName += _name[useIndex + i];
	}
	return newName;
}

void Animator::ChangeSpeed(int index, float _speed) {
	if (index >= pAnimations.size()) return;

	pAnimations[index]->playSpeed = _speed;
}

void Animator::ChangeSpeed(std::string animName, float _speed) {
	AnimationClip<>* anim = GetAnimation(animName);

	if (anim == nullptr) return;

	anim->playSpeed = _speed;
}

float Animator::GetAnimSpeed(int index) {
	if (index >= pAnimations.size()) return 0.0f;

	return pAnimations[index]->playSpeed;
}

float Animator::GetAnimSpeed(std::string animName) {
	int size = static_cast<int>(pAnimations.size());
	for (int i = 0; i < size; i++) {
		if (pAnimations[i]->name != animName) continue;
		return pAnimations[i]->playSpeed;
	}
	return 0.0f;
}

float Animator::GetTotalTime(std::string animName) {
	auto anim = GetAnimation(animName);
	int handle = anim->animationHandle;

	if (handle == -1) return -1;

	return  MV1GetAnimTotalTime(handle, anim->animIndex);
}

float Animator::GetTotalTime(int index){
	auto anim = GetAnimation(index);
	int handle =anim->animationHandle;

	if (handle == -1) return -1;

	return  MV1GetAnimTotalTime(handle, anim->animIndex);
}

int Animator::GetAnimationIndex(std::string animName) {
	int size = static_cast<int>(pAnimations.size());
	for (int i = 0; i < size; i++) {
		if (pAnimations[i]->name != animName) continue;
		return i;
	}
	return -1;
}

void Animator::ResetEvents(){
	for (auto& anim : pAnimations) {
		anim->ResetEvent();
	}
}
