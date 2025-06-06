#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <vector>
#include "menushka.h"

using namespace sf;
using namespace std;

void udal(vector<vector<CircleShape>>& balls, int row, int col, Color color,
    vector<vector<bool>>& visited, vector<pair<int, int>>& ryad) {
    if (row < 0 || row >= balls.size() || col < 0 || col >= balls[row].size())
        return;

    if (visited[row][col])
        return;

    if (balls[row][col].getRadius() == 0 || balls[row][col].getFillColor() != color)
        return;

    visited[row][col] = true;
    ryad.push_back({ row, col });
    // пошук сусідів
    int dr[6] = { -1, -1, 0, 0, 1, 1 };
    int dc_even[6] = { -1, 0, -1, 1, -1, 0 };
    int dc_odd[6] = { 0, 1, -1, 1, 0, 1 };
    int* dc = (row % 2 == 0) ? dc_even : dc_odd;

    for (int i = 0; i < 6; ++i) {
        udal(balls, row + dr[i], col + dc[i], color, visited, ryad);
    }
}
void prikr(vector<vector<CircleShape>>& balls, int cols, Color colors[], int colorCount) {
    int rowIndex = 0;

    vector<CircleShape> newRow(cols);
    for (int col = 0; col < cols; col++) {
        CircleShape bubble(35.f);
        Color randomColor = colors[rand() % colorCount];
        bubble.setFillColor(randomColor);

        float x = (rowIndex % 2 == 0) ? col * 70 + 70 : col * 70 + 70 + 35;
        float y = rowIndex * 70 + 70;
        bubble.setPosition(x, y);

        newRow[col] = bubble;
    }

    balls.insert(balls.begin(), newRow);

    for (int row = 0; row < balls.size(); row++) {
        for (int col = 0; col < balls[row].size(); col++) {
            if (balls[row][col].getRadius() == 0) continue;

            float x = (row % 2 == 0) ? col * 70 + 70 : col * 70 + 70 + 35;
            float y = row * 70 + 70;
            balls[row][col].setPosition(x, y);
        }
    }
}

int main() {
    LC_ALL(setlocale, "RU");
    srand(static_cast<unsigned int>(time(0)));

    RenderWindow window(VideoMode(1200, 800), "kursach");

    Texture fonTexture;
    Sprite fonSprite;

    if (!fonTexture.loadFromFile("C:\\Users\\pas\\Desktop\\курсач\\fon.jpg")) {
        std::cerr << "Ошибка загрузки fon.jpg" << std::endl;
    }
    fonSprite.setTexture(fonTexture);

    Menu menu(window);
    MenuState state = menu.run();

    if (state != MenuState::GAME) {
        return 0;
    }

    CircleShape pushka1(40.f, 50);
    pushka1.setFillColor(Color::Cyan);
    pushka1.setPosition(550, 690);

    CircleShape pushka2(30.f, 4);
    pushka2.setFillColor(Color::Cyan);
    pushka2.setPosition(590, 660);
    pushka2.rotate(45);

    vector<vector<CircleShape>> balls;
    int rows = 5;
    int cols = 15;
    Color colors[4] = { Color::Green, Color::Yellow, Color::Red, Color::Blue };

    balls.resize(rows);
    for (int row = 0; row < rows; row++) {
        balls[row].resize(cols);
        for (int col = 0; col < cols; col++) {
            balls[row][col] = CircleShape(35.f);
            balls[row][col].setFillColor(colors[rand() % 4]);

            if (row % 2 == 0) {
                balls[row][col].setPosition(col * 70 + 70, row * 70 + 70);
            }
            else {
                balls[row][col].setPosition(col * 70 + 70 + 35, row * 70 + 70);
            }
        }
    }

    CircleShape yadro(35.f);
    yadro.setFillColor(colors[rand() % 4]);
    yadro.setPosition(555, 600);

    bool ismoving = false;
    Vector2f yadronapr(0.f, 0.f);
    float yadroSpeed = 0.3f;
    int noMatch = 0;
    bool gameOver = false;

    int score = 0;
    Font scoreFont;
    if (!scoreFont.loadFromFile("C:\\Users\\pas\\Desktop\\курсач\\ArialRegular.ttf")) {
        cout << "Ошибка загрузки шрифта!" << endl;
    }
    Text scoreText;
    scoreText.setFont(scoreFont);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(550, 20);
    scoreText.setString(L"Очки: 0");

    RectangleShape restartButton(Vector2f(200, 100));
    restartButton.setFillColor(Color::Red);
    restartButton.setPosition(500, 500);

    Text restartText;
    restartText.setFont(scoreFont);
    restartText.setString(L"Грати знову");
    restartText.setCharacterSize(24);
    restartText.setFillColor(Color::Black);
    FloatRect textBounds = restartText.getLocalBounds();
    restartText.setOrigin(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2);
    restartText.setPosition(restartButton.getPosition().x + restartButton.getSize().x / 2, restartButton.getPosition().y + restartButton.getSize().y / 2);

    RectangleShape exitGameButton(Vector2f(100, 50));
    exitGameButton.setFillColor(Color::Black);
    exitGameButton.setPosition(550, 700);

    Text exitGameText;
    exitGameText.setFont(scoreFont);
    exitGameText.setString(L"Вийти");
    exitGameText.setCharacterSize(18);
    exitGameText.setFillColor(Color::White);
    FloatRect exitGameBounds = exitGameText.getLocalBounds();
    exitGameText.setOrigin(exitGameBounds.left + exitGameBounds.width / 2, exitGameBounds.top + exitGameBounds.height / 2);
    exitGameText.setPosition(exitGameButton.getPosition().x + exitGameButton.getSize().x / 2, exitGameButton.getPosition().y + exitGameButton.getSize().y / 2);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
                if (!ismoving) {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                    Vector2f yadroPos = yadro.getPosition() + Vector2f(yadro.getRadius(), yadro.getRadius());

                    Vector2f napr = mousePos - yadroPos;
                    float length = sqrt(napr.x * napr.x + napr.y * napr.y);

                    if (length != 0) {
                        yadronapr = napr / length;
                        ismoving = true;
                    }
                }
            }
        }

        if (gameOver) {
            window.clear(Color::Black);

            Text text(L"Кiнець гри", scoreFont, 100);
            text.setFillColor(Color::Red);
            text.setPosition(350, 300);
            window.draw(text);
            window.draw(restartButton);
            window.draw(restartText);
            window.draw(exitGameButton);
            window.draw(exitGameText);
            window.display();

            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                    if (restartButton.getGlobalBounds().contains(mousePos)) {
                        balls.clear();
                        for (int row = 0; row < rows; row++) {
                            vector<CircleShape> newRow(cols);
                            for (int col = 0; col < cols; col++) {
                                newRow[col] = CircleShape(35.f);
                                newRow[col].setFillColor(colors[rand() % 4]);
                                if (row % 2 == 0)
                                    newRow[col].setPosition(col * 70 + 70, row * 70 + 70);
                                else
                                    newRow[col].setPosition(col * 70 + 70 + 35, row * 70 + 70);
                            }
                            balls.push_back(newRow);
                        }

                        yadro.setPosition(555, 600);
                        yadro.setFillColor(colors[rand() % 4]);
                        ismoving = false;
                        yadronapr = Vector2f(0.f, 0.f);
                        noMatch = 0;
                        gameOver = false;
                        score = 0;
                        scoreText.setString(L"Очки: 0");
                    }
                    if (exitGameButton.getGlobalBounds().contains(mousePos)) {
                        window.close();
                    }
                }
            }

            continue;
        }

        if (ismoving) {
            yadro.move(yadronapr * yadroSpeed);

            if (yadro.getPosition().y < 0 || yadro.getPosition().y > 800 ||
                yadro.getPosition().x < 0 || yadro.getPosition().x > 1200) {
                yadro.setPosition(555, 600);
                yadro.setFillColor(colors[rand() % 4]);
                ismoving = false;
            }
            for (int row = 0; row < balls.size(); row++) {
                for (int col = 0; col < balls[row].size(); col++) {
                    if (balls[row][col].getRadius() == 0) continue;

                    FloatRect ballBounds = balls[row][col].getGlobalBounds();
                    FloatRect yadroBounds = yadro.getGlobalBounds();

                    if (ballBounds.intersects(yadroBounds)) {
                        ismoving = false;

                        Color shotColor = yadro.getFillColor();

                        int newRow = row + 1;
                        int newCol;
                        float targetX = balls[row][col].getPosition().x;
                        newCol = (row % 2 == 0) ? (targetX - 70) / 70 : (targetX - 105) / 70;

                        while (balls.size() <= newRow) {
                            balls.emplace_back(cols);
                        }
                        if (newCol >= 0 && newCol < cols) {
                            CircleShape newBubble(35.f);
                            newBubble.setFillColor(shotColor);
                            float newX = (newRow % 2 == 0) ? newCol * 70 + 70 : newCol * 70 + 70 + 35;
                            float newY = newRow * 70 + 70;
                            newBubble.setPosition(newX, newY);
                            balls[newRow][newCol] = newBubble;

                            vector<vector<bool>> visited(balls.size(), vector<bool>(cols, false));
                            vector<pair<int, int>> ryad;
                            udal(balls, newRow, newCol, shotColor, visited, ryad);

                            if (ryad.size() >= 3) {
                                for (size_t i = 0; i < ryad.size(); ++i) {
                                    int r = ryad[i].first;
                                    int c = ryad[i].second;
                                    balls[r][c] = CircleShape(0.f);
                                }
                                int earned = 100 + (ryad.size() - 3) * 50;
                                score += earned;
                                scoreText.setString(L"Очки: " + to_string(score));
                                noMatch = 0;
                            }
                            else {
                                noMatch++;
                                if (noMatch >= 3) {
                                    prikr(balls, cols, colors, 4);
                                    noMatch = 0;
                                }
                            }
                        }

                        yadro.setPosition(555, 600);
                        yadro.setFillColor(colors[rand() % 4]);
                        break;
                    }
                }
                if (!ismoving) break;
            }
        }
        for (int row = 0; row < balls.size(); row++) {
            for (int col = 0; col < balls[row].size(); col++) {
                if (balls[row][col].getRadius() > 0 &&
                    balls[row][col].getPosition().y + 70 >= 800) {
                    gameOver = true;
                    break;
                }
            }
            if (gameOver) break;
        }
        window.clear(Color::Black);
        window.draw(fonSprite);

        for (int row = 0; row < balls.size(); row++) {
            for (int col = 0; col < balls[row].size(); col++) {
                if (balls[row][col].getRadius() > 0)
                    window.draw(balls[row][col]);
            }
        }
        window.draw(pushka1);
        window.draw(pushka2);
        window.draw(yadro);
        window.draw(scoreText);
        window.display();
    }

    return 0;
}
