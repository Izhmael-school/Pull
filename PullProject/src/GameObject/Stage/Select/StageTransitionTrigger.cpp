#include "StageTransitionTrigger.h"
#include "../../../Component\Collider/Collider.h"
#include "../../Character/Player/PlayerCharacter.h"
#include "../../../Manager/Stage/StageManager.h"
#include "../../../Manager/Stage/GimmickObjectManager.h"
#include "Manager/SceneManager.h"
#include "Definition/CommonModule/MyMath.h"

StageTransitionTrigger::StageTransitionTrigger()
    : m_stageNo(-1){

}

void StageTransitionTrigger::OnTriggerEnter(Collider* pSelf ,Collider* other) {
    auto _other = other->GetLayer();
    
    if (_other == ColliderLayer::Player) {
        StageManager::GetInstance().SetStageID(m_stageNo);
        SceneManager::GetInstance().ChangeScene(SceneType::Game);
    }

}

