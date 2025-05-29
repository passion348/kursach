#pragma once
#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>

using namespace sf;

enum class MenuState { MAIN, RULES, GAME, EXIT };

class Menu {
public:
    Menu(RenderWindow& window);
    MenuState run(); 

private:
    RenderWindow& window;
    Font font;
    RectangleShape playButton;
    RectangleShape rulesButton;
    Text playText;
    Text rulesText;
    Text rulesContent;
    Texture fonTexture;
    Sprite fonSprite;
    Text gameTitle;

    bool isMouseOver(const RectangleShape& button, const Vector2f& mousePos);
    void drawMainMenu();
    void drawRules();
};

#endif
