#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <set>
#include "Hearts.h"
using namespace sf;

std::vector<int> share_card_turn(std::vector<Players>& players, int num_of_player, std::vector<int>& bot_cards)
{
    if (players[num_of_player].hand.size() > 0)
    {
        for (int card_rank = 14; card_rank > 6; card_rank --)
        {
            for (int i = 0; i < players[num_of_player].hand.size(); i++)
            {
                if (bot_cards.size() != 3)
                {
                   if (players[num_of_player].hand[i].rank == card_rank)
                    {
                        bot_cards.push_back(i);
                    } 
                }
                else
                {
                    return bot_cards;
                }
            }
        }
        
    }
}

