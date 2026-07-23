#include "StageTransitionTrigger.h"
#include "../../../Component\Collider/Collider.h"
#include "../../Character/Player/PlayerCharacter.h"
#include "../../../Manager/Stage/StageManager.h"
#include "../../../Manager/Stage/GimmickObjectManager.h"
#include "Manager/SceneManager.h"

StageTransitionTrigger::StageTransitionTrigger()
    : m_stageNo(0){

}

void StageTransitionTrigger::OnTriggerEnter(Collider* pSelf ,Collider* other) {
    auto _other = other->GetGameObject();
    auto player = dynamic_cast<PlayerCharacter*>(_other);
    if (player) {
        StageManager::GetInstance().SetStageID(m_stageNo);
        SceneManager::GetInstance().ChangeScene(SceneType::Game);
    }

}

