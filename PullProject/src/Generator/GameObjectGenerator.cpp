#include "GameObjectGenerator.h"

#include "Definition/CommonModule/MyJson.h"

GameObjectGenerator::GameObjectGenerator() { Start(); }

void GameObjectGenerator::Start(){
#if _DEBUG
    auto data = MyJson::LoadJsonFile(GAMEOBJECTDATA_FILEPATH);
#else
    auto data = MyJson::LoadBinary(RELEASE_GAMEOBJECTDATA_FILEPATH);
#endif
    // モデルのロード
    for (auto d : data) {
        std::string modelName = d["modelName"];
        std::string filePath = d["filePath"];
        int modelHandle = MV1LoadModel(filePath.c_str());
        modelArray.insert(std::make_pair(modelName, modelHandle));
    }
}