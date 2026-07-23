#include "TitleScreen.h"

void TitleScreen::Update(float deltaTime, const UIInput& input) {

    startText->SetSelected(currentSelect == 0);
    endText->SetSelected(currentSelect == 1);
    // タイマー更新
    colorTimer += deltaTime;

    // 0.3秒ごとに色を切り替える
    if (colorTimer >= 0.9f) {
        colorTimer = 0.0f;
        isWhite = !isWhite;
    }

    // スタイル取得
    UITextStyle startStyle = startText->GetStyle();
    UITextStyle endStyle = endText->GetStyle();

    // 通常色
    startStyle.normalColor = white;
    endStyle.normalColor = white;

    // どちらが選択されているか
    if (currentSelect == 0) {
        startStyle.selectedColor = isWhite ? white : red;
    }
    else {
        endStyle.selectedColor = isWhite ? white : red;
    }

    startText->SetStyle(startStyle);
    endText->SetStyle(endStyle);
}
