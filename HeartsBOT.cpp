#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <set>
#include "Hearts.h"
// #include "HEARTS_turns.h"
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

bool canPlayCard(const Cards& card, const Cards bito[4], bool heartsBroken) {
    if (heartsBroken || card.suit != "HEARTS") {
        return true;
    }
    return false;
}

Cards ordinary_turn_without_hearts(std::vector<Players>& players, int num_of_player, Cards bito[4], bool heartsBroken) {
    Cards chosenCard;
    Players& currentPlayer = players[num_of_player];

    // Sort player’s hand by rank to facilitate card selection
    std::sort(currentPlayer.hand.begin(), currentPlayer.hand.end(), [](const Cards& a, const Cards& b) {
        return a.rank < b.rank;
    });

    for (auto it = currentPlayer.hand.begin(); it != currentPlayer.hand.end(); ++it) {
        if (canPlayCard(*it, bito, heartsBroken)) {
            chosenCard = *it;
            currentPlayer.hand.erase(it);
            return chosenCard;
        }
    }

    // Fallback: in case no card can be played (should not occur usually)
    chosenCard = currentPlayer.hand.back();
    currentPlayer.hand.pop_back();
    return chosenCard;
}

Cards ordinary_turn(std::vector<Players>& players, int num_of_player, Cards bito[4])
{
    Cards Bito_0 = bito[0];
    Cards Bito_1 = bito[1];
    Cards Bito_2 = bito[2];
    Cards Bito_3 = bito[3];
    Cards card_for_bito;
    int ID = 0;
    for (Cards& card : players[num_of_player].hand)
    {
        
        if ((card.rank < Bito_0.rank && Bito_0.suit != "")||(card.rank < Bito_1.rank && Bito_1.suit != "")|| (card.rank < Bito_2.rank && Bito_2.suit != "")||(card.rank < Bito_3.rank && Bito_3.suit != ""))
        {
            card_for_bito = card;
            ID++;
        }
        else
        {
            if (card_for_bito.suit == ""){card_for_bito = card; }
        }
    }
    return card_for_bito;
}