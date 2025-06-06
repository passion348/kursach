#include "Menushka.h"
#include <iostream>

using namespace sf;

Menu::Menu(RenderWindow& win) : window(win) {
    if (!font.loadFromFile("C:\\Users\\pas\\Desktop\\курсач\\ArialRegular.ttf")) {
        std::cerr << "Не удалось загрузить шрифт!" << std::endl;
    }
    if (!fonTexture.loadFromFile("C:\\Users\\pas\\Desktop\\курсач\\fon.jpg")) {
        std::cerr << "Ошибка загрузки fon.jpg" << std::endl;
    }
    fonSprite.setTexture(fonTexture);

    LC_ALL(setlocale, "RU");

    gameTitle.setFont(font);
    gameTitle.setString(L"Шарики");
    gameTitle.setStyle(Text::Bold);
    gameTitle.setCharacterSize(126);  
    gameTitle.setFillColor(Color::Magenta);  
    FloatRect titleBounds = gameTitle.getLocalBounds();
    gameTitle.setOrigin(titleBounds.left + titleBounds.width / 2.f, titleBounds.top + titleBounds.height / 2.f);
    gameTitle.setPosition(window.getSize().x / 2.f, 100);

    playButton.setSize({ 200, 100 });
    playButton.setPosition(500, 200);
    playButton.setFillColor(Color::Blue);

    playText.setFont(font);
    playText.setString(L"Грати");
    playText.setCharacterSize(24);
    playText.setFillColor(Color::White);
    FloatRect playBounds = playText.getLocalBounds();
    playText.setOrigin(playBounds.left + playBounds.width / 2.f, playBounds.top + playBounds.height / 2.f);
    playText.setPosition( playButton.getPosition().x + playButton.getSize().x / 2.f, playButton.getPosition().y + playButton.getSize().y / 2.f);

    rulesButton.setSize({ 200, 100 });
    rulesButton.setPosition(500, 400);
    rulesButton.setFillColor(Color::Blue);

    rulesText.setFont(font);
    rulesText.setString(L"Правила");
    rulesText.setCharacterSize(24);
    rulesText.setFillColor(Color::White);
    FloatRect rulesBounds = rulesText.getLocalBounds();
    rulesText.setOrigin(rulesBounds.left + rulesBounds.width / 2.f, rulesBounds.top + rulesBounds.height / 2.f);
    rulesText.setPosition( rulesButton.getPosition().x + rulesButton.getSize().x / 2.f, rulesButton.getPosition().y + rulesButton.getSize().y / 2.f);

    rulesContent.setFont(font);
    rulesContent.setString(L"Правила гри:\n\nСполучай кульки одного кольору, щоб отримати очки\nДля пострілу націлься в потрібне місце та натисни Пробіл\n\nРахунок очок:\n3 кульки - 100 очок\nКожна слідуюча + 50 (4 = 150, 5 = 200 i тд.\n\nПовернутися до меню");
    rulesContent.setCharacterSize(20);
    rulesContent.setFillColor(Color::Black);
    rulesContent.setPosition(100, 100);

    exitButton.setSize({ 100, 50 });
    exitButton.setPosition(550, 700);
    exitButton.setFillColor(Color::Black);

    exitText.setFont(font);
    exitText.setString(L"Вийти");
    exitText.setCharacterSize(18);
    exitText.setFillColor(Color::White);
    FloatRect exitBounds = exitText.getLocalBounds();
    exitText.setOrigin(exitBounds.left + exitBounds.width / 2.f, exitBounds.top + exitBounds.height / 2.f);
    exitText.setPosition(exitButton.getPosition().x + exitButton.getSize().x / 2.f, exitButton.getPosition().y + exitButton.getSize().y / 2.f);

}

bool Menu::isMouseOver(const RectangleShape& button, const Vector2f& pos) {
    return button.getGlobalBounds().contains(pos);
}

void Menu::drawMainMenu() {
    window.draw(fonSprite);
    window.draw(playButton);
    window.draw(playText);
    window.draw(rulesButton);
    window.draw(rulesText);
    window.draw(gameTitle);
    window.draw(exitButton);
    window.draw(exitText);
}

void Menu::drawRules() {
    window.draw(fonSprite);
    window.draw(rulesContent);
}

MenuState Menu::run() {
    MenuState state = MenuState::MAIN;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                return MenuState::EXIT;

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                if (state == MenuState::MAIN) {
                    if (isMouseOver(playButton, mousePos))
                        return MenuState::GAME;
                    if (isMouseOver(rulesButton, mousePos))
                        state = MenuState::RULES;
                    if (isMouseOver(exitButton, mousePos)) {
                        window.close();
                        state = MenuState::EXIT;
                    }
                }
                else if (state == MenuState::RULES) {
                    state = MenuState::MAIN;
                }
            }
        }

        window.clear(Color::Black);
        if (state == MenuState::MAIN)
            drawMainMenu();
        else if (state == MenuState::RULES)
            drawRules();
        window.display();
    }

    return MenuState::EXIT;
}
