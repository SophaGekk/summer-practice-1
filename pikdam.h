#pragma once

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <set>

using namespace sf;

class Card {
public:
    int value; // 6-14 (6 to Ace)
    std::string suit; //  (Spades, Clubs, Diamonds, Hearts)
    sf::Texture texture; 
    sf::Sprite sprite; 
    bool isTaking = false; //для реализации взятия карты
};

class Player {
public:
    std::vector<Card> hand;
    bool isAttacker;
    bool isDefender;
};


void share_Card(std::vector<Player> Player, int num_of_player, std::vector<int> selected_Card);
int findCurrentPlayer(std::vector<Player> players);
bool pair_of_cards(const Card& card1, const Card& card2);
int main_pikgame(sf::RenderWindow& window);