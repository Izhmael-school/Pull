#include "GameObject/GameObject.h"
#pragma once
class StageTransitionTrigger : public GameObject{

private:
    int m_stageNo;

public:
    StageTransitionTrigger();

    void OnTriggerEnter(Collider* pSelf, Collider* other) override;

    void SetStageNo(int _n) { m_stageNo = _n;}
};

