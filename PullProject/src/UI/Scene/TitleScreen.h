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

class TitleScreen : public UIScreen {
public:

    void Init() override {

        auto logoImage = CreateUIObject<UIImage>(
            LoadGraph("res/Sprite/System/GameLogo.png"),
            Vector2(100, 100)
        );

        // auto start = CreateUIObject<UIButton>();
        // 
        // auto image = std::make_unique<UIImage>(
        //     LoadGraph("res/UI/StartButton.png"),
        //     Vector2(10, 400)
        // );
        // 
        // start->AddChild(std::move(image));
        // 
        // start->SetOnClick([]() {
        //     SceneManager::GetInstance().ChangeScene(SceneType::StageSelect);
        //     });

        // auto exit = CreateUIObject<UIButton>();
        // 
        // auto image2 = std::make_unique<UIImage>(
        //     LoadGraph("res/UI/Exit.png"),
        //     Vector2(850, 650)
        // );

       //  exit->AddChild(std::move(image2));
    }
};

#endif // !_TITLESCREEN_H_