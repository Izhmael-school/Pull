#include "CaughtObject.h"

void CaughtObject::CaughtUpdate(){
	switch (currentState){
	case CaughtState::Catch:
		CatchStart();
		break;
	case CaughtState::Catching:
		Catching();
		break;
	case CaughtState::Throw:
		ThrowStart();
		break;
	case CaughtState::Throwing:
		Throwing();
		break;
	}
}

void CaughtObject::CatchStart(){
	ChangeCaughtState(CaughtState::Catching);
}

void CaughtObject::Catching(){}

void CaughtObject::ThrowStart(){
	ChangeCaughtState(CaughtState::Throwing);
}

void CaughtObject::Throwing(){}

void CaughtObject::HitObject(){}

void CaughtObject::ChangeCaughtState(CaughtState _state){
	currentState = _state;
}
