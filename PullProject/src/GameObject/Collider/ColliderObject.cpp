#include "ColliderObject.h"
#include "Component/Collider/Collider.h"
#include "Manager/TimeManager.h"

ColliderObject::ColliderObject(VECTOR _pos, float _radius, Tag _tag, float _lifeTime, std::function<void(Collider* _pOther)> _enterEvent, std::function<void(Collider* _pOther)> _stayEvent, std::function<void(Collider* _pOther)> _exitEvent)
	:GameObject(-1, _pos, _tag)
	, lifeTime(_lifeTime)
	, enterEvent(_enterEvent)
	, stayEvent(_stayEvent)
	, exitEvent(_exitEvent)
	, lifeElapsedTime(0.0f)
	, isDisable(false)
	, wantDelete(false)
{
	pCollider = std::make_unique<SphereCollider>(this, VZero, _radius);
	Start();
}

ColliderObject::ColliderObject(VECTOR _pos, VECTOR _min, VECTOR _max, Tag _tag, float _lifeTime, std::function<void(Collider* _pOther)> _enterEvent, std::function<void(Collider* _pOther)> _stayEvent, std::function<void(Collider* _pOther)> _exitEvent)
	:GameObject(-1, _pos, _tag)
	, lifeTime(_lifeTime)
	, enterEvent(_enterEvent)
	, stayEvent(_stayEvent)
	, exitEvent(_exitEvent)
	, lifeElapsedTime(0.0f)
	,isDisable(false)
	,wantDelete(false)
{
	pCollider = std::make_unique<AABBCollider>(this, _min, _max);
	Start();
}

ColliderObject::ColliderObject(VECTOR _pos, VECTOR _min, VECTOR _max, float _radius, Tag _tag, float _lifeTime, std::function<void(Collider* _pOther)> _enterEvent, std::function<void(Collider* _pOther)> _stayEvent, std::function<void(Collider* _pOther)> _exitEvent)
	:GameObject(-1, _pos, _tag)
	, lifeTime(_lifeTime)
	, enterEvent(_enterEvent)
	, stayEvent(_stayEvent)
	, exitEvent(_exitEvent)
	, lifeElapsedTime(0.0f)
	,isDisable(false)
	,wantDelete(false)
{
	pCollider = std::make_unique<CapsuleCollider>(this, _min, _max, _radius, VZero);
	Start();
}

void ColliderObject::Start(){
	GameObject::Start();

	// 0秒以下なら消えない
	isDisable = lifeTime <= 0.0f ? false : true;
}

void ColliderObject::Update(){
	GameObject::Update();

	if (!isDisable) return;

	// 生きる時間を過ぎたら消す
	if (lifeElapsedTime >= lifeTime) {
		wantDelete = true;
	}
	else {
		lifeElapsedTime += TimeManager::GetInstance().GetDeltaTime();
	}
}

void ColliderObject::Render(){
	GameObject::Render();
}

void ColliderObject::OnTriggerEnter(Collider* _pOther) {
	if (enterEvent)
		enterEvent(_pOther);
}

void ColliderObject::OnTriggerStay(Collider* _pOther) {
	if (stayEvent);
	stayEvent(_pOther);
}

void ColliderObject::OnTriggerExit(Collider* _pOther) {
	if (exitEvent)
		exitEvent(_pOther);
}
