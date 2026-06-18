/*
 *	@file ModelManager.cpp
 *  @author oorui
 */

#include "ModelManager.h"
#include <DxLib.h>

 /*
  * モデルロード
  */
int ModelManager::Load(const std::string& fileName){
	// すでにロード済みなら再利用
	auto it = models.find(fileName);

	if (it != models.end()){
		return it->second;
	}


	// モデルロード
	int handle = MV1LoadModel(fileName.c_str());

	// 保存
	models[fileName] = handle;

	return handle;

}

/*
 * 全削除
 */
void ModelManager::Clear(){

	for (auto& model : models){
		if (model.second >= 0){
			MV1DeleteModel(model.second);
		}
	}


	models.clear();

}