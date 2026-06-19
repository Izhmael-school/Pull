/*
 *  @file GimmickFactory.cpp
 *  @author oorui
 */

#include "GimmickFactory.h"
#include "BreakWall.h"
#include "Lever.h"
GimmickObject* GimmickFactory::Create(const std::string& type, int modelHandle, int triggerId, VECTOR pos, VECTOR rota)
{
	// 壊れる壁生成
	if (type == "BreakWall") {

		return new BreakWall(
			triggerId,
			modelHandle,
			pos,
			rota
		);
	}

	return nullptr;

}
