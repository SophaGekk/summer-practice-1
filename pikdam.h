#pragma once

#include<Windows.h>
#include <C:/Users/Sopha/Desktop/proj/resources/SFML-2.6.1/include/SFML/Graphics.hpp>
#include<C:/Users/Sopha/Desktop/proj/resources/SFML-2.6.1/include/SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace sf;

// Структура для представления карты
struct Card {
    sf::Texture texture;
    sf::Sprite sprite;
    int value;
    sf::String suit;
    
    bool operator==(const Card& other);
};

// Класс для представления колоды карт
class Deck {
public:
    Deck();

    // Перетасовка колоды
    void shuffle();

    // Раздача карт
    std::vector<std::vector<Card>> deal(int numPlayers);

private:
    std::vector<Card> cards;
    const std::string suits[4] = {"HEARTS", "DIAMONDS", "CLUBS", "SPADES"};
};

// Класс для представления игрока
class Player {
public:
    Player(int id, std::vector<Card>& hand) : id(id), hand(hand) {}

    // Проверка на наличие пар
    bool hasPairs();

    // Проверка на наличие Пиковой Дамы
    bool hasQueenOfSpades(const Card& card);

    // Сброс пары
    std::pair<Card, Card> discardPair(const Card& card1, const Card& card2);

    // Получение карты от другого игрока
    void receiveCard(Card card);
    // Вывод информации об игроке
    void printInfo();

    int getId();

    std::vector<Card>& getHand();

private:
    int id;
    std::vector<Card> hand;
};

// Функция для отрисовки игры
void drawGame(sf::RenderWindow& window, std::vector<Player>& players, std::vector<std::pair<Card, Card>>& discardedPairs);

int main_pikgame(sf::RenderWindow& window);