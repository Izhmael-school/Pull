/*
 *  @file   TitleScreen.h
 *  @author oorui
 */

#ifndef _TITLESCREEN_H_
#define _TITLESCREEN_H_

#include "../UIScreen.h"
#include "../Button/UIButton.h"
#include "../Image/UIImage.h"
#include "../UIManager.h"
#include "../../Manager/SceneManager.h"
#include "../Text/UIText.h"

class TitleScreen : public UIScreen {
    UIText* startText = nullptr;        // スタートテキスト
    UIText* endText = nullptr;          // ゲーム終了テキスト

    float colorTimer = 0.0f;            // 色切り替え用のタイマー
    bool isWhite = false;               // 選択されているかどうか

    int currentSelect = 0;              // 現在選択されているテキスト
public:

    void Init() override {

        // フォントを追加
        AddFontResourceEx(
            "res/UI/FONT/Kaisotai-Next-UP-B.ttf",
            FR_PRIVATE,
            NULL
        );

        // タイトルロゴを生成
        auto logoImage = CreateUIObject<UIImage>(
            LoadGraph("res/Sprite/System/GameLogo.png"),
            Vector2(100, 100)
        );
        
        // テキストを生成
        UITextStyle style;
        style.fontName = "Kaisotai-Next-UP-B";
        style.fontSize = 150;
        style.fontThickness = 5;
        style.normalColor = white;
        style.outLineColor = black;

        // スタートテキスト生成
        startText = CreateUIObject<UIText>(
            "GAME START",
            style,
            Vector2(650, 650)
        );
        
        // ゲーム終了テキスト生成
        endText = CreateUIObject<UIText>(
            "EXIT",
            style,
            Vector2(850, 850)
        );


        startText->SetSelected(true);
        endText->SetSelected(false);

    }

    void Update(float deltaTime, const UIInput& input) override;

    /*
     *	選択されているものを表示
     */
    void SetSelected(int select) {
        currentSelect = select;
    }
    
    /*
     *  現在選択されているモノを取得
     */
    int GetSelectText() const { return currentSelect; }
};

#endif // !_TITLESCREEN_H_