#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <set>
#include "Hearts.h"
using namespace sf;

std::vector<int> share_card_turn(std::vector<Players>& players, int num_of_player, std::vector<int>& bot_cards);
Cards ordinary_turn(std::vector<Players>& players, int num_of_player, Cards bito[4]);
Cards ordinary_turn_without_hearts(std::vector<Players>& players, int num_of_player, Cards bito[4], bool heartsBroken);
bool canPlayCard(const Cards& card, const Cards bito[4], bool heartsBroken);
