#include "MainGameScreen.h"
#include "Definition/CommonModule/MyString.h"
#include "Game/GameData.h"

void MainGameScreen::Update(float deltaTime, const UIInput& input) {
	scoreCount->SetText(MyString::ToZeroPadded(GameData::GetScore(),8));
}

