/*
 *	@file	ModelManager.h
 *  @author oorui
 */

#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_

#include"../Component/Singleton.h"
#include <unordered_map>
#include <string>

/*
 *	モデルロードの一元化
 */
class ModelManager : public Singleton<ModelManager>{
	friend class Singleton<ModelManager>;
private:

	std::unordered_map<std::string, int> models;

	ModelManager() {}
	~ModelManager() = default;


public:

	/*
	 * モデルロード
	 */
	int Load(const std::string& fileName);

	/*
	 * 全削除
	 */
	void Clear();

};


#endif // !_MODELMANAGER_H_
