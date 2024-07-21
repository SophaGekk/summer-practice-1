#pragma once
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <set>
#include <random>
using namespace sf;
// Класс для представления карты
class Cardd {
public:
    int value; // Значение карты (6-14)
    std::string suit; // Масть карты (HEARTS, DIAMONDS, CLUBS, SPADES)
    Texture texture; // Текстура карты
    Sprite sprite; // Спрайт карты
    // Конструктор карты
    // Cardd(int val, const std::string& s);
};
// Класс для представления игрока
class Playerr {
public:
    std::string name; // Имя игрока
    std::vector<Cardd> hand; // Рука игрока
    bool isAttacker = false; // Является ли игрок атакующим
    bool isDefender = false; // Является ли игрок защищающимся
    bool hasWon = false;
    // Конструктор игрока
    Playerr() : isAttacker(false), isDefender(false), hasWon(false), name("") {}
};
class Buttonn {
private:
    sf::RectangleShape ButtonnShape;
    sf::Text ButtonnText;
public:
    Buttonn(const std::string& text, const sf::Vector2f& size, const int charSize, const sf::Color& bgColor, const sf::Color& textColor, sf::Font& font_1) {
        // Установка формы кнопки
        ButtonnShape.setSize(size);
        ButtonnShape.setFillColor(bgColor);
        // Установка текста кнопки
        ButtonnText.setFont(font_1);
        ButtonnText.setString(text);
        ButtonnText.setCharacterSize(charSize);
        ButtonnText.setFillColor(textColor);
        // Центрирование текста на кнопке
        sf::FloatRect textRect = ButtonnText.getLocalBounds();
        ButtonnText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        ButtonnText.setPosition(sf::Vector2f(size.x / 2.0f, size.y / 2.0f));
    }
    // Функция для установки позиции кнопки
    void setPosition(const sf::Vector2f& pos) {
        ButtonnShape.setPosition(pos);
        ButtonnText.setPosition(pos.x + ButtonnShape.getSize().x / 2.0f, pos.y + ButtonnShape.getSize().y / 2.0f);
    }
    // Функция для отрисовки кнопки
    void draw(sf::RenderWindow& window) {
        window.draw(ButtonnShape);
        window.draw(ButtonnText);
    }
    // Для состояния выделения
    bool isSelected;
    // Функция для установки состояния выделения
    void setSelected(bool select) {
        isSelected = select;
        if (isSelected) {
            ButtonnShape.setFillColor(sf::Color::Black); // Выделенная кнопка будет черного цвета
        } else {
            ButtonnShape.setFillColor(sf::Color::Transparent); // Обычный цвет кнопки
        }
    }
};

int main_durakgame(sf::RenderWindow& windowss);