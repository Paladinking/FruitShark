#ifndef FRUITSHARK_MAINMENU_H
#define FRUITSHARK_MAINMENU_H
#include "engine/ui.h"

class MainMenu : public Menu {
public:
    /**
     * Constructs a MainMenu object.
     */
    MainMenu() : Menu() {};

    /**
     * Initializes the MainMenu, creating all buttons.
     */
    void init(WindowState* window_state) override;

    /**
     * Fixes the viewport.
     */
    void resume() override;

protected:

    void button_press(int btn) override;

private:
    static const int BUTTON_WIDTH, BUTTON_HEIGHT;

    enum ButtonId {
        START_GAME, EXIT, TOTAL
    };

    static const std::string BUTTON_NAMES[ButtonId::TOTAL];
};


#endif //FRUITSHARK_MAINMENU_H