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
class Cardss {
public:
    int value; // Значение карты (6-14)
    std::string suit; // Масть карты (HEARTS, DIAMONDS, CLUBS, SPADES)
    Texture texture; // Текстура карты
    Sprite sprite; // Спрайт карты
    // Конструктор карты
    Cardss(int val, const std::string& s);
};
// Класс для представления игрока
class Player {
public:
    std::vector<Cardss> hand; // Рука игрока
    bool isAttacker; // Является ли игрок атакующим
    bool isDefender; // Является ли игрок защищающимся
    bool isTaking; // Взял ли игрок карты из колоды
    bool CardssSelected; // Выбрал ли игрок карту для атаки/защиты
    // Конструктор игрока
    Player();
};
// Функция для определения индекса текущего атакующего игрока
int findCurrentPlayer(std::vector<Player>& players);
// Функция для сортировки карт в руке игрока по значению
void sortHand(std::vector<Cardss>& hand);
// Функция для проверки на атаку
bool attack(std::vector<Cardss>& table, Cardss& attackCardss);
// Функция для проверки на защиту
bool defense(Cardss& attackCardss, Cardss& defenseCardss, Cardss& trump);
// Функция для определения, кто первый ходит
int whoFirst (std::vector<Player>& players, Cardss& trump);
// Функция, которая проверяет, закончилась ли игра
bool isOver (std::vector<Cardss>& deck, std::vector<Player>& players);
// Функция, которая проверяет, надо ли брать карты
bool takingCardsss (std::vector<Player>& players);
int main_durakgame(sf::RenderWindow& windowss);