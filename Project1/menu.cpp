#include "Menushka.h"
#include <iostream>

Menu::Menu(sf::RenderWindow& win) : window(win) {
    if (!font.loadFromFile("C:\\Users\\pas\\Desktop\\курсач\\ArialRegular.ttf")) {
        std::cerr << "Не удалось загрузить шрифт!" << std::endl;
    }
    if (!fonTexture.loadFromFile("C:\\Users\\pas\\Desktop\\курсач\\fon.jpg")) {
        std::cerr << "Ошибка загрузки fon.jpg" << std::endl;
    }
    fonSprite.setTexture(fonTexture);

    LC_ALL(setlocale, "RU");

    playButton.setSize({ 200, 100 });
    playButton.setPosition(500, 200);
    playButton.setFillColor(sf::Color::Blue);

    playText.setFont(font);
    playText.setString(L"Играть");
    playText.setCharacterSize(24);
    playText.setFillColor(sf::Color::White);

    sf::FloatRect playBounds = playText.getLocalBounds();
    playText.setOrigin(playBounds.left + playBounds.width / 2.f,
        playBounds.top + playBounds.height / 2.f);
    playText.setPosition(
        playButton.getPosition().x + playButton.getSize().x / 2.f,
        playButton.getPosition().y + playButton.getSize().y / 2.f
    );

    rulesButton.setSize({ 200, 100 });
    rulesButton.setPosition(500, 400);
    rulesButton.setFillColor(sf::Color::Blue);

    rulesText.setFont(font);
    rulesText.setString(L"Правила");
    rulesText.setCharacterSize(24);
    rulesText.setFillColor(sf::Color::White);

    sf::FloatRect rulesBounds = rulesText.getLocalBounds();
    rulesText.setOrigin(rulesBounds.left + rulesBounds.width / 2.f,
        rulesBounds.top + rulesBounds.height / 2.f);
    rulesText.setPosition(
        rulesButton.getPosition().x + rulesButton.getSize().x / 2.f,
        rulesButton.getPosition().y + rulesButton.getSize().y / 2.f
    );

    rulesContent.setFont(font);
    rulesContent.setString(L"Правила игры:\nСоединяй шарики одного цвета\nчтобы набирать очки.\n\nКликни чтобы вернуться.");
    rulesContent.setCharacterSize(20);
    rulesContent.setFillColor(sf::Color::White);
    rulesContent.setPosition(100, 100);
}

bool Menu::isMouseOver(const sf::RectangleShape& button, const sf::Vector2f& pos) {
    return button.getGlobalBounds().contains(pos);
}

void Menu::drawMainMenu() {
    window.draw(fonSprite);
    window.draw(playButton);
    window.draw(playText);
    window.draw(rulesButton);
    window.draw(rulesText);
}

void Menu::drawRules() {
    window.draw(fonSprite);
    window.draw(rulesContent);
}

MenuState Menu::run() {
    MenuState state = MenuState::MAIN;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                return MenuState::EXIT;

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (state == MenuState::MAIN) {
                    if (isMouseOver(playButton, mousePos))
                        return MenuState::GAME;
                    if (isMouseOver(rulesButton, mousePos))
                        state = MenuState::RULES;
                }
                else if (state == MenuState::RULES) {
                    state = MenuState::MAIN;
                }
            }
        }

        window.clear(sf::Color::Black);
        if (state == MenuState::MAIN)
            drawMainMenu();
        else if (state == MenuState::RULES)
            drawRules();
        window.display();
    }

    return MenuState::EXIT;
}
