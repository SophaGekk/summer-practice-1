#ifndef PIKDAM_H
#define PIKDAML_H


#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <set>
#include <random>

#include <fstream> // For file operations
#include <sstream> // For string manipulation
using namespace sf;

class Cardi {
public:
    int value; // 6-14 (6 to Ace)
    std::string suit; //  (Spades, Clubs, Diamonds, Hearts)
    sf::Texture texture; 
    sf::Sprite sprite; 
    bool isTaking = false; //для реализации взятия карты
};

class Player_ {
public:
    virtual ~Player_() {} // Виртуальный деструктор
    std::vector<Cardi> hand;
    bool isAttacker;
    bool isDefender;
    bool hasWon = false; // Добавляем переменную для отслеживания победы
};

class Button {
public:
    Button(const std::wstring& text, const sf::Vector2f& size, const int charSize, const sf::Color& bgColor, const sf::Color& textColor, sf::Font& font_1) {
        // Установка формы кнопки
        buttonShape.setSize(size);
        buttonShape.setFillColor(bgColor);

        // Установка текста кнопки
        buttonText.setFont(font_1);
        buttonText.setString(text);
        buttonText.setCharacterSize(charSize);
        buttonText.setFillColor(textColor);

        // Центрирование текста на кнопке
        sf::FloatRect textRect = buttonText.getLocalBounds();
        buttonText.setOrigin(textRect.left + textRect.width / 2.0f,
                             textRect.top + textRect.height / 2.0f);
        buttonText.setPosition(sf::Vector2f(size.x / 2.0f, size.y / 2.0f));
    }

    // Функция для установки позиции кнопки
    void setPosition(const sf::Vector2f& pos) {
        buttonShape.setPosition(pos);
        buttonText.setPosition(pos.x + buttonShape.getSize().x / 2.0f, pos.y + buttonShape.getSize().y / 2.0f);
    }

    // Функция для отрисовки кнопки
    void draw(sf::RenderWindow& window) {
        window.draw(buttonShape);
        window.draw(buttonText);
    }

    // для состояния выделения
    bool isSelected;
    // Функция для установки состояния выделения
    void setSelected(bool select) {
        isSelected = select;
        if (isSelected) {
            buttonShape.setFillColor(sf::Color::Black); // Выделенная кнопка будет черного цвета
        } else {
            buttonShape.setFillColor(sf::Color::Transparent); // Обычный цвет кнопки
        }
    }

private:
    sf::RectangleShape buttonShape;
    sf::Text buttonText;
};


class HumanPlayer : public Player_ { 
public:
    virtual ~HumanPlayer() {} // Виртуальный деструктор
};

class Bot : public Player_ {
public:
    virtual ~Bot() {} // Виртуальный деструктор
    // Функция для автоматического взятия карты у игрока справа
    void takeCardiFromPlayer(Player_& rightPlayer) {
        if (!rightPlayer.hand.empty()) {
            std::random_device rd;
            std::mt19937 rng(rd());
            std::uniform_int_distribution<int> uni(0, rightPlayer.hand.size() - 1);

            int random_index = uni(rng); // Получаем случайный индекс
            Cardi CardiTaken = rightPlayer.hand[random_index]; // Берем карту
            rightPlayer.hand.erase(rightPlayer.hand.begin() + random_index); // Удаляем карту из руки игрока
            hand.push_back(CardiTaken); // Добавляем карту в руку бота
        }
    }
    void disCardiPairs() {
        std::map<int, std::vector<size_t>> valueToIndices;

        // Сопоставление значений карт с их индексами
        for (size_t i = 0; i < hand.size(); ++i) {
            valueToIndices[hand[i].value].push_back(i);
        }

        std::set<int> indicesToDisCardi;

        // Поиск пар карт, исключая тройки и пары дам с дамой пик
        for (const auto& pair : valueToIndices) {
            const auto& indices = pair.second;
            if (indices.size() == 2) { // Убедитесь, что есть только две карты одного ранга
                // Проверка на пару дам, включая даму пик
                if (pair.first == 12) { 
                    bool hasSpades = false;
                    for (size_t index : indices) {
                        if (hand[index].suit == "SPADES") { 
                            hasSpades = true;
                            break;
                        }
                    }
                    if (!hasSpades) {
                        for (size_t index : indices) {
                            indicesToDisCardi.insert(index);
                        }
                    }
                } else {
                    for (size_t index : indices) {
                        indicesToDisCardi.insert(index);
                    }
                }
            }
        }

        // Удаление пар карт из руки
        std::vector<Cardi> newHand;
        for (size_t i = 0; i < hand.size(); ++i) {
            if (indicesToDisCardi.find(i) == indicesToDisCardi.end()) {
                newHand.push_back(hand[i]);
            }
        }
        hand = newHand;
    }
};
// Структура для хранения состояния игры
struct GameState {
    std::vector<Cardi> deck;
    std::vector<Player_> players;
    int currentPlayerIndex;
    // ... (Add other variables related to game state)
    int selectedCardiIndex;
    int selectedCardiIndexRight;
    std::vector<int> selected_Cardi;
    bool messege_win_player;
    // Флаг для отображения текста выбора карт
    bool showCardiSelectionMessage;
    // Индекс выбранной кнопки (0 - Да, 1 - Нет)
    int selectedButtonIndex;
    // Флаг для отображения текста выхода из игры
    bool showMessageEscape;
    // Флаг для отображения текста выхода в меню
    bool returnToMenu;
    // Переменная для отслеживания количества игроков, выбравших "нет"
    int playersChoseNo; 
    bool showMessageWait;
    bool Taking_Cardis; // Флаг для проверки, взял ли игрок карту
    bool canSelectCardis; // Можно ли выбирать карты
    bool canTakeCardi; // Можно ли брать карту у противника
    bool CardiTaken; // Флаг, указывающий на то, что карта была взята
    bool move_transition;
    bool crdpair;
    bool EndGame; //окончание игры
    bool GameOver; //проигрыш
    bool YouWin; //выигрыш
    int neighborIndex;
    int activePlayers;
    int selectedButtonIndex_2; // Индекс выбранной кнопки

    std::map<std::string, sf::Texture> textures; 
};

// void draw_for1(std::vector<std::unique_ptr<Player_>>& Player, int& selectedCardiIndex, std::vector<int>& selected_Cardi, int& selectedCardiIndexRight, int& neighborIndex, sf::RenderWindow& windowss, sf::Text& messege_winplayer, RectangleShape& back_Cardi_sprite);
int main_pikgame(sf::RenderWindow& window, int& number_of_players);

#endif 