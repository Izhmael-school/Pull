/*
 *	@file	PlayerActionEnum.h
 *	@author Riku
 */

#ifndef _PLAYERACTIONENUM_H_
#define _PLAYERACTIONENUM_H_

enum class PlayerAction {
	Invalid = -1,
	Move,
	Jump,
	ArmExtend,
	CatchCancel,
	CameraMove,
	LockOn,

	Max
};

#endif // !_PLAYERACTIONENUM_H_
