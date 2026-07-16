#include "MainGameScreen.h"
#include "Definition/CommonModule/MyString.h"

void MainGameScreen::Update(float deltaTime, const UIInput& input) {
	coinCount->SetText(std::to_string(GameData::GetCoin()));
	scoreCount->SetText(MyString::ToZeroPadded(GameData::GetScore(),8));
}
