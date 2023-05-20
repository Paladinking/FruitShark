#include "mainMenu.h"
#include "config.h"
#include "sharkGame.h"
#include "sound.h"

const std::string MainMenu::BUTTON_NAMES[] = {"Start Game", "Exit"};
const int MainMenu::BUTTON_WIDTH = 220;
const int MainMenu::BUTTON_HEIGHT = 110;

void MainMenu::init(WindowState *window_state) {
    SDL_SetRelativeMouseMode(SDL_FALSE);
    SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_RenderSetLogicalSize(gRenderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);
    SDL_ShowWindow(gWindow);
    State::init(window_state);
    sound::init();
    texture_handler.load_textures();
    SDL_RenderGetLogicalSize(gRenderer, &window_state->screen_width, &window_state->screen_height);
    SDL_GetRendererOutputSize(gRenderer, &window_state->window_width, &window_state->window_height);
    auto& shark_trails = get_shark_trails();
    for (int i = 0; i < INITIAL_SHARK_COUNT; ++i) {
        int x = engine::random(200, LOGICAL_WIDTH - 200);
        int y = engine::random(200, LOGICAL_HEIGHT - 200);
        int path = engine::random(0, static_cast<Sint32>(shark_trails.size()));
        int type = engine::random(0, 100);
        if (type > 80) {
            sharks.push_back(Shark::create_shark(Shark::Type::GREAT_WHITE, x, y));
        } else {
            sharks.push_back(Shark::create_shark(Shark::Type::TIGER, x, y));
        }
        sharks[i].set_trail(&shark_trails[path]);
    }

    buttons.reserve(ButtonId::TOTAL);
    for (int i = 0; i < ButtonId::TOTAL; ++i) {
        buttons.emplace_back(
                (window_state->screen_width - BUTTON_WIDTH) / 2,
                (i + 1) * window_state->screen_height / 4,
                BUTTON_WIDTH,
                BUTTON_HEIGHT,
                BUTTON_NAMES[i], 40
        );
    }
    text.emplace_back(
            (window_state->screen_width / 2- 250),
            50,
            500,
            200,
            "FRUITSHARK",
            80
    );
    text.back().set_text_color(0, 62, 110, 0xFF);
}

void MainMenu::resume() {
    SDL_SetRelativeMouseMode(SDL_FALSE);
    State::resume();
    SDL_RenderSetViewport(gRenderer, nullptr);
}

void MainMenu::button_press(int btn) {
    switch (btn) {
        case ButtonId::START_GAME:
            next_res = {StateStatus::PUSH, new SharkGame()};
            break;
        case ButtonId::EXIT:
            menu_exit();
            break;
        default:
            break;
    }
}

MainMenu::~MainMenu() {
    sound::free();
}

void MainMenu::render() {
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0xB2, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
    for (auto& shark : sharks) shark.render();
    for (auto& b : buttons) {
        b.set_hover(b.is_pressed(window_state->mouseX, window_state->mouseY));
        b.render(0, 0);
    }
    for (auto& t : text) {
        t.render(0, 0);
    }

    SDL_RenderPresent(gRenderer);
}

void MainMenu::tick(Uint64 delta, StateStatus &res) {
    Menu::tick(delta, res);
    if (delta > 0){
        for (auto& shark : sharks) shark.tick_physics(
                delta / 1000.0, get_shark_trails(),
                std::vector<Fruit>(), std::vector<Ship>());
        for (auto& shark : sharks) shark.tick_animation(delta / 1000.0);
    }

}
