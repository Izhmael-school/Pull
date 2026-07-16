#include "ShadowMap.h"
#include "DxLib.h"
#include "Manager/Stage/StageManager.h"
#include "Manager/PlayerManager.h"
#include "GameObject/GameObject.h"
#include "GameObject/Character/Character.h"

ShadowMap::ShadowMap()
	: shadowMapHandle(-1) {
}

ShadowMap::~ShadowMap() {
}

void ShadowMap::SetUp() {
	// シャドウマップの生成
	shadowMapHandle = MakeShadowMap(1024, 1024);

	// 描画エリアの設定
	// ※キャラやステージのサイズに合わせて適宜調整してください
	SetShadowMapDrawArea(
		shadowMapHandle,
		VGet(-1000.0f, -1000.0f, -1000.0f),
		VGet(1000.0f, 1000.0f, 1000.0f)
	);

	// 影のチラつき（シャドウバイアス）を調整する設定（必要に応じてコメントアウトを解除）
	SetShadowMapAdjustDepth(shadowMapHandle, 0.0005f);
}

void ShadowMap::Start() {
}

void ShadowMap::Update() {
	// ライトの方向をシャドウマップに同期
	SetShadowMapLightDirection(shadowMapHandle, VNorm(VGet(-1.0f, -3.0f, -1.0f)));
}

// ★ステップ1: シャドウマップに「影（深度）」を描き込む処理
void ShadowMap::Render() {
	if (shadowMapHandle == -1) return;


	auto player = PlayerManager::GetInstance().GetPlayer();
	
	
	if (player) {
		VECTOR pos = player->GetPosition();

		SetShadowMapDrawArea(
			shadowMapHandle,
			VSub(pos, VGet(600, 600, 600)),
			VAdd(pos, VGet(600, 600, 600))
		);
	}


	// シャドウマップへの描画準備
	ShadowMap_DrawSetup(shadowMapHandle);

	// 影を落としたい3Dモデルを描画
	/*auto stage = StageManager::GetInstance().GetCurrentStage();
	if (stage) {
		MV1DrawModel(stage->GetStageModelHandle());
	}*/

	if (player) {
		MV1DrawModel(player->GetModelHandle());
	}

	// シャドウマップへの描画終了
	ShadowMap_DrawEnd();
}

void ShadowMap::Apply() {
	if (shadowMapHandle == -1) return;

	// 0番目のライト（ディレクショナルライト）にこのシャドウマップを適用する
	SetUseShadowMap(0, shadowMapHandle);
}

// 使用が終わったら無効化する処理（描画パスの最後に呼ぶと安全）
void ShadowMap::Disable() {
	SetUseShadowMap(0, -1);
}

void ShadowMap::RoundShadowMap() {}
void ShadowMap::SpotShadowMap() {}