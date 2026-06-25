/*
 *  @file GimmickFactory.cpp
 *  @author oorui
 */

#include "GimmickFactory.h"
#include "BreakWall.h"
#include "Lever.h"
#include "BomBreakWall.h"

GimmickObject* GimmickFactory::Create(const std::string& type, int modelHandle, int triggerId, VECTOR pos, VECTOR rota) {
	// 壊れる壁生成
	if (type == "BreakWall") {

		return new BreakWall(
			triggerId,
			modelHandle,
			pos,
			rota
		);
	}

	// 爆弾で壊れる壁
	if (type == "BomBreakWall") {
		return new BomBreakWall(
			modelHandle,
			pos,
			rota
		);
	}

	return nullptr;

}
