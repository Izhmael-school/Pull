#include "Object.h"

void Object::Start() {}

Object::Object(Tag _tag)
	:transform()
	,tag(_tag)
{}

Object::~Object(){}

void Object::Update() {
	transform.Update();
}

void Object::Render() {}

void Object::Setup() {}

void Object::Teardown() {}

//void Object::OnTriggerEnter(Collider* _pOther) {}
//
//void Object::OnTriggerStay(Collider* _pOther) {}
//
//void Object::OnTriggerExit(Collider* _pOther) {}
