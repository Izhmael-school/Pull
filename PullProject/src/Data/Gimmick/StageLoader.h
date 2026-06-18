/*
 *	@file StageLoader.h
 *  @author oorui
 */

#ifndef _STAGELOADER_H_
#define _STAGELOADER_H_

#include <string>
class StageLoader {
public:
	static void Load(
		const std::string& fileName,
		int stageHandle
	);


};

#endif // !_STAGELOADER_H_
