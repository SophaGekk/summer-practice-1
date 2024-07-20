#pragma once

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <set>

using namespace sf;


class Cards {
public:
    int rank; // 6-14 (6 to Ace)
    std::string suit = ""; //  (Spades, Clubs, Diamonds, Hearts)
    sf::Texture texture; 
    sf::Sprite sprite; 
    bool isTaking = false; //для реализации взятия карты
};

class Players {
public:
    std::vector<Cards> hand;
    bool isAttacker;
    bool isDefender;
    
};

class Button {
public:
    Button(const std::string& text, const sf::Vector2f& size, const int charSize, const sf::Color& bgColor, const sf::Color& textColor, sf::Font& font_1) {
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

int gameMenu(sf::RenderWindow& Play);

void share_cards(std::vector<Players>& players, int num_of_player, std::vector<int> selected_cards);
int main_hearts(sf::RenderWindow& window);
void disable_card_selection(Players& player,std::vector<int> selected_cards);
void enable_card_selection(Players& player);
void draw_cards_except_0(std::vector<Players>& players, sf::RenderWindow& windowss);
void handlePassingCards(sf::RenderWindow& windowss, std::vector<Players>& players, Event event);
void handleFirstRound(sf::RenderWindow& windowss, std::vector<Players>& players, Event event);
void handleHeratsBroken(sf::RenderWindow& windowss, std::vector<Players>& players, Event event);
void handleNormalRound(sf::RenderWindow& windowss, std::vector<Players>& players, Event event);
void check_bito(std::vector<Players>& players, Cards bito[4]);

void loadgame(std::vector<Players>& players, Cards bito[4], int& current_player, int& start_player, int penalty_points[4]);
void savegame(const std::vector<Players>& players, Cards bito[4], int current_player, int start_player, int penalty_points[4]);