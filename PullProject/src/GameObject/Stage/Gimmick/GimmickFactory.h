/*
 *	@file	GimmickFactory.h
 *  @author oorui
 */

#ifndef _GIMMICKFACTORY_H
#define _GIMMICKFACTORY_H

#include "../../../Definition/CommonModule/MyMath.h"

#include <string>

class GimmickObject;
class GimmickFactory{

public:
	/*
	 *	ギミックを生成
	 *  @param[in]	std::string& ギミックの種類 例：壊れる壁、移動床など
	 *  @param[in]	int			 モデルハンドル
	 *  @param[in]	int			 レバーのID		※レバーに対応したギミックのみ+レバー本体
	 *  @param[in]	pos			 生成する座標	例：オブジェクトに付ける場合など
	 */
	static GimmickObject* Create(
		const std::string& type,
		int modelHandle,
		int triggerId,
		VECTOR pos,
		VECTOR rota
	);


};


#endif // !_GIMMICKFACTORY_H
