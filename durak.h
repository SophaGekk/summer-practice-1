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
class Card {
public:
    int value; // Значение карты (6-14)
    std::string suit; // Масть карты (HEARTS, DIAMONDS, CLUBS, SPADES)
    Texture texture; // Текстура карты
    Sprite sprite; // Спрайт карты
    // Конструктор карты
    Card(int val, const std::string& s);
};
// Класс для представления игрока
class Player {
public:
    std::vector<Card> hand; // Рука игрока
    bool isAttacker; // Является ли игрок атакующим
    bool isDefender; // Является ли игрок защищающимся
    bool isTaking; // Взял ли игрок карты из колоды
    bool cardSelected; // Выбрал ли игрок карту для атаки/защиты
    // Конструктор игрока
    Player();
};
// Функция для определения индекса текущего атакующего игрока
int findCurrentPlayer(std::vector<Player>& players);
// Функция для сортировки карт в руке игрока по значению
void sortHand(std::vector<Card>& hand);
// Функция для проверки на атаку
bool attack(std::vector<Card>& table, Card& attackCard);
// Функция для проверки на защиту
bool defense(Card& attackCard, Card& defenseCard, Card& trump);
// Функция для определения, кто первый ходит
int whoFirst (std::vector<Player>& players, Card& trump);
// Функция, которая проверяет, закончилась ли игра
bool isOver (std::vector<Card>& deck, std::vector<Player>& players);
// Функция, которая проверяет, надо ли брать карты
bool takingCards (std::vector<Player>& players);
int main_durakgame(sf::RenderWindow& windowss);