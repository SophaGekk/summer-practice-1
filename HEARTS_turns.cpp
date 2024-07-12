#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <set>
#include "HeartsBOT.h" //  Файл с логикой ботов
#include "Hearts.h"


void first_round_share(int& first_round, std::vector<Players>& players, int current_player, sf::RenderWindow& windowss)
{
    if (current_player != 0) {
        std::vector<int> bot_cards(0);
        share_card_turn(players, current_player, bot_cards);
        share_cards(players, (current_player + 1) % 4, bot_cards);
        current_player = (current_player + 1) % 4;
        
    }
}

// void second_round_clubs(int& first_round, std::vector<Players>& players, int current_player, sf::RenderWindow& windowss, Cards bito[4])
// {
//      bool found_clubs = false;
//     // Находим игрока с двойкой крести
//     for (int player_ind = 0; player_ind < 4; player_ind++)
//     {
//         if (found_clubs == true)
//         { 
//             break;
//         }
        
//         int ID = 0;
//         for (Cards& card : players[player_ind].hand)
//         {
//             if (card.rank == 6 && card.suit == "CLUBS")
//             {
//                 // Если игрок - бот, он сразу кладет карту на стол
//                 if (player_ind != 0)
//                 {
//                     current_player = player_ind;
//                     bito[current_player] = card;
//                     players[current_player].hand.erase(players[current_player].hand.begin() + ID);
//                     current_player = (current_player + 1) % 4;
//                     found_clubs = true;
//                     break;
//                 }
//                 else // Игрок 0 должен выложить двойку крести
//                 {
//                     // Отключаем выбор карт для игрока 0
//                     disable_card_selection(players[0], selected_cards);
                    
//                     // Последовательная отрисовка карт
//                     windowss.clear(Color::White);
//                     windowss.draw(background);
//                     draw_cards_except_0(players, windowss);

//                     for (size_t i = 0; i < players[0].hand.size(); ++i)
//                     {
//                         Cards& card = players[0].hand[i];
//                         card.sprite.setTexture(card.texture);
                        
//                         // Определяем позицию карты
//                         card.sprite.setPosition(Vector2f(320 + (1300 / players[0].hand.size()) * i, 900));
                        
//                         // Выделяем двойку крести желтым цветом
//                         if (card.rank == 6 && card.suit == "CLUBS")
//                         {
//                             selectedCardsIndex = i;
//                             card.sprite.setColor(Color::Yellow);
//                         }
//                         else
//                         {
//                             card.sprite.setColor(Color(50, 50, 50));  // "Затемняем" остальные карты
//                         }
                        
//                         windowss.draw(card.sprite);
//                     }
                    
//                     // Финальная отрисовка перед ожиданием
//                     windowss.display();

//                     // Ждем нажатия Enter
//                     bool enterPressed = false;
//                     while (!enterPressed)
//                     {
//                         sf::Event event;
//                         while (windowss.pollEvent(event))
//                         {
//                             if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
//                             {
//                                 enterPressed = true;
//                             }
//                         }
//                     }

//                     // Выкладываем двойку крести на стол
//                     bito[0] = players[0].hand[selectedCardsIndex]; 
//                     selectedCardsIndex = 0;
//                     current_player = 1; // Переход к следующему игроку
//                     enable_card_selection(players[0]); // Восстанавливаем выбор карт
//                     players[0].hand.erase(players[0].hand.begin() + ID);
//                     found_clubs = true;
//                     break; // Выходим из цикла поиска игрока с двойкой крести
//                 }
//             }
//             ID++;
//         }
//     }

//     // if (current_player != 0) { //продолжаем второй ход после того как "двойка" крести оказалась на столе
//     //             HeartsBroken = false;
//     //             bito[current_player] = ordinary_turn_without_hearts(players, current_player, bito, HeartsBroken);
//     //             current_player = (current_player + 1) % 4;
//     //             // Последовательная отрисовка карт
//     //             windowss.clear(Color::White);
//     //             windowss.draw(background);
//     //             draw_cards(players, windowss);
//     //             sf::sleep(sf::seconds(1));
//     //         }
//         // else
//         // {
//         //     // Ждем нажатия Enter
//         //             bool enterPressed = false;
//         //             while (!enterPressed)
//         //             {
//         //                 // Отрисовка карт
//         //                 windowss.clear(Color::White);
//         //                 windowss.draw(background);
//         //                 draw_cards(players, windowss);
//         //                 windowss.display();
//         //                 sf::Event event;
//         //                 while (windowss.pollEvent(event))
//         //                 {
//         //                     if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
//         //                     {
//         //                         enterPressed = true;
//         //                     }
//         //                 }
//         //             }
//         // }
//     }
