#include "ColliderObjectManager.h"
#include "Component/Collider/Collider.h"
#include "GameObject/Collider/ColliderObject.h"

ColliderObjectManager::ColliderObjectManager()
{
}

ColliderObjectManager::~ColliderObjectManager()
{
}

void ColliderObjectManager::Update(){

	// 更新
	for (auto& c : aliveColliderObject) {
		if (c == nullptr) continue;
		c->Update();
	}

	// 削除
	std::erase_if(aliveColliderObject, [this](std::unique_ptr<ColliderObject>& col) {
		// 消すフラグが立ったら消す
		if (!col->WantDelete()) return false;
		col.reset();
		return true;
		});
}

void ColliderObjectManager::Render(){
	for (auto& c : aliveColliderObject) {
		if (c == nullptr) continue;

		c->Render();
	}
}

void ColliderObjectManager::CreateSphere(VECTOR _pos, float _radius, Tag _tag, float _lifeTime, std::function<void(Collider* _pOther)> _enterEvent, std::function<void(Collider* _pOther)> _stayEvent, std::function<void(Collider* _pOther)> _exitEvent){
	std::unique_ptr<ColliderObject> sphere = std::make_unique<ColliderObject>(_pos, _radius, _tag, _lifeTime, _enterEvent, _stayEvent, _exitEvent);
	aliveColliderObject.emplace_back(std::move(sphere));
}

void ColliderObjectManager::CreateAABB(VECTOR _pos, VECTOR _min, VECTOR _max, Tag _tag, float _lifeTime, std::function<void(Collider* _pOther)> _enterEvent, std::function<void(Collider* _pOther)> _stayEvent, std::function<void(Collider* _pOther)> _exitEvent){
	std::unique_ptr<ColliderObject> aabb = std::make_unique<ColliderObject>(_pos, _min,_max, _tag, _lifeTime, _enterEvent, _stayEvent, _exitEvent);
	aliveColliderObject.emplace_back(std::move(aabb));
}

void ColliderObjectManager::CreateCapsule(VECTOR _pos, VECTOR _min, VECTOR _max, float _radius, Tag _tag, float _lifeTime, std::function<void(Collider* _pOther)> _enterEvent, std::function<void(Collider* _pOther)> _stayEvent, std::function<void(Collider* _pOther)> _exitEvent){
	std::unique_ptr<ColliderObject> capsule = std::make_unique<ColliderObject>(_pos, _min, _max,_radius, _tag, _lifeTime, _enterEvent, _stayEvent, _exitEvent);
	aliveColliderObject.emplace_back(std::move(capsule));
}
