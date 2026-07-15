/*
 *  @file   TitleScreen.h
 *  @author oorui
 */

#include "../UIScreen.h"
#include "../Button/UIButton.h"
#include "../Image/UIImage.h"
#include "../UIManager.h"
#include "../../Manager/SceneManager.h"

class TitleScreen : public UIScreen {
public:

    void Init() override {
        auto start = CreateUIObject<UIButton>();

        auto image = std::make_unique<UIImage>(
            LoadGraph("res/UI/Start.png"),
            Vector2(850, 500)
        );

        start->AddChild(std::move(image));

        start->SetOnClick([]() {
            SceneManager::GetInstance().ChangeScene(SceneType::StageSelect);
            });

        auto exit = CreateUIObject<UIButton>();

        auto image2 = std::make_unique<UIImage>(
            LoadGraph("res/UI/Exit.png"),
            Vector2(850, 650)
        );

        exit->AddChild(std::move(image2));
    }
};