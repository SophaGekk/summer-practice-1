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

void share_cards(std::vector<Players>& players, int num_of_player, std::vector<int> selected_cards);
int main_hearts(sf::RenderWindow& window);
void disable_card_selection(Players& player,std::vector<int> selected_cards);
void enable_card_selection(Players& player);