#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <set>
#include <random>
#include <fstream> // For file operations
#include <sstream> // For string manipulation
using namespace sf;
#include "pikdam.h"
#include <memory>
void draw_for1(std::vector<std::unique_ptr<Player_>>& Player, int& selectedCardiIndex, std::vector<int>& selected_Cardi, int& selectedCardiIndexRight, int& neighborIndex, sf::RenderWindow& windowss, sf::Text& messege_winplayer, RectangleShape& back_Cardi_sprite)
{

    // Отрисовка карт для каждого игрока
    for (int playerIndex = 0; playerIndex < Player.size(); ++playerIndex) {
        // Определение позиции и поворота карт в зависимости от индекса игрока
        float rotation;
        if (playerIndex == 0) { // Игрок снизу
            rotation = 0;
        } else if (playerIndex == 1) { // Игрок справа
            rotation = 90;
        } else if (playerIndex == 2) { // Игрок сверху
            rotation = 0;
        } else if (playerIndex == 3) { // Игрок слева
            rotation = -90;
        }

        int id = 1; // Переменная для определения позиции карты в ряду
        // Если игрок выиграл, отображаем сообщение о победе
        if (Player[playerIndex]->hasWon) {
            // Определение позиции для сообщения в зависимости от индекса игрока
            sf::Vector2f messagePosition;
            if (playerIndex == 0) { // Игрок снизу
                messagePosition = sf::Vector2f(900.f, 800.f);
            } else if (playerIndex == 1) { // Игрок справа
                messagePosition = sf::Vector2f(1720.f, 540.f);
            } else if (playerIndex == 2) { // Игрок сверху
                messagePosition = sf::Vector2f(900.f, 150.f);
            } else if (playerIndex == 3) { // Игрок слева
                messagePosition = sf::Vector2f(100.f, 540.f);
            }
            messege_winplayer.setPosition(messagePosition);
            windowss.draw(messege_winplayer);
            continue; // Пропускаем отрисовку карт для выигравшего игрока
        }
        // Отрисовка карт текущего игрока
        for (size_t CardiIndex = 0; CardiIndex < Player[playerIndex]->hand.size(); ++CardiIndex) {
            Cardi& Cardi = Player[playerIndex]->hand[CardiIndex];
            if (playerIndex == 0) {
                // Атакующий игрок видит значения своих карт
                Cardi.sprite.setTexture(Cardi.texture);
                // Определение позиции карты в зависимости от её индекса 
                Cardi.sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex]->hand.size()) * id, 800));
                // Если карта выбрана, она рисуется в желтый цвет
                if (std::find(selected_Cardi.begin(), selected_Cardi.end(), CardiIndex) != selected_Cardi.end())
                {
                    Cardi.sprite.setColor(Color::Magenta);
                }
                else if (CardiIndex == selectedCardiIndex)
                {
                    Cardi.sprite.setColor(Color::Yellow);
                }
                else
                {
                    Cardi.sprite.setColor(Color::White);
                }

                windowss.draw(Cardi.sprite);
                ++id; 
                
            } else {
                // У остальных игроков карты рубашкой вверх
                back_Cardi_sprite.setRotation(rotation);
                if (playerIndex == 1) {
                    back_Cardi_sprite.setPosition(Vector2f(1820, 123 + (800 / Player[playerIndex]->hand.size()) * id));
                } else if (playerIndex == 2) {
                    back_Cardi_sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex]->hand.size()) * id, 150));
                } else if (playerIndex == 3) {
                    back_Cardi_sprite.setPosition(Vector2f(100, 200 + (800 / Player[playerIndex]->hand.size()) * id));
                }
                //neighborIndex
                // Если карта выбрана, она рисуется в желтый цвет
                if (playerIndex == neighborIndex && CardiIndex == selectedCardiIndexRight)
                {
                    back_Cardi_sprite.setFillColor(Color::Yellow);
                }
                else
                {
                    back_Cardi_sprite.setFillColor(Color::White);
                }

                windowss.draw(back_Cardi_sprite);
                id++;
            }
        }
    }
}

void draw_for2(std::vector<std::unique_ptr<Player_>>& Player, int& selectedCardiIndex, std::vector<int>& selected_Cardi, int& selectedCardiIndexRight, int& neighborIndex, sf::RenderWindow& windowss, sf::Text& messege_winplayer, RectangleShape& back_Cardi_sprite)
{
    
        // Отрисовка карт для каждого игрока
        for (int playerIndex = 0; playerIndex < Player.size(); ++playerIndex) {
            int id = 1; // Переменная для определения позиции карты в ряду
            // Если игрок выиграл, отображаем сообщение о победе
            if (Player[playerIndex]->hasWon) {
                // Определение позиции для сообщения в зависимости от индекса игрока
                sf::Vector2f messagePosition;
                if (playerIndex == 0) { // Игрок снизу
                    messagePosition = sf::Vector2f(900.f, 800.f);
                } else if (playerIndex == 1) { // Игрок справа
                    messagePosition = sf::Vector2f(1720.f, 540.f);
                } else if (playerIndex == 2) { // Игрок сверху
                    messagePosition = sf::Vector2f(900.f, 150.f);
                } else if (playerIndex == 3) { // Игрок слева
                    messagePosition = sf::Vector2f(100.f, 540.f);
                }
                messege_winplayer.setPosition(messagePosition);
                windowss.draw(messege_winplayer);
                continue; // Пропускаем отрисовку карт для выигравшего игрока
            }
            // Отрисовка карт текущего игрока
            for (size_t CardiIndex = 0; CardiIndex < Player[playerIndex]->hand.size(); ++CardiIndex) {
                Cardi& Cardi = Player[playerIndex]->hand[CardiIndex];
                if ((dynamic_cast<HumanPlayer*>(Player[0].get()) != nullptr && Player[0]->isAttacker ))
                {
                    // Определение позиции и поворота карт в зависимости от индекса игрока
                    float rotation;
                    if (playerIndex == 0) { // Игрок снизу
                        rotation = 0;
                    } else if (playerIndex == 1) { // Игрок справа
                        rotation = 90;
                    } else if (playerIndex == 2) { // Игрок сверху
                        rotation = 0;
                    } else if (playerIndex == 3) { // Игрок слева
                        rotation = -90;
                    }
                    if (playerIndex == 0) {
                        // Атакующий игрок видит значения своих карт
                        Cardi.sprite.setTexture(Cardi.texture);
                        // Определение позиции карты в зависимости от её индекса 
                        Cardi.sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex]->hand.size()) * id, 800));
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (std::find(selected_Cardi.begin(), selected_Cardi.end(), CardiIndex) != selected_Cardi.end())
                        {
                            Cardi.sprite.setColor(Color::Magenta);
                        }
                        else if (CardiIndex == selectedCardiIndex)
                        {
                            Cardi.sprite.setColor(Color::Yellow);
                        }
                        else
                        {
                            Cardi.sprite.setColor(Color::White);
                        }

                        windowss.draw(Cardi.sprite);
                        ++id; 
                        
                    } else {
                        // У остальных игроков карты рубашкой вверх
                        back_Cardi_sprite.setRotation(rotation);
                        if (playerIndex == 1) {
                            back_Cardi_sprite.setPosition(Vector2f(1820, 123 + (800 / Player[playerIndex]->hand.size()) * id));
                        } else if (playerIndex == 2) {
                            back_Cardi_sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex]->hand.size()) * id, 150));
                        } else if (playerIndex == 3) {
                            back_Cardi_sprite.setPosition(Vector2f(100, 200 + (800 / Player[playerIndex]->hand.size()) * id));
                        }
                        //neighborIndex
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (playerIndex == neighborIndex && CardiIndex == selectedCardiIndexRight)
                        {
                            back_Cardi_sprite.setFillColor(Color::Yellow);
                        }
                        else
                        {
                            back_Cardi_sprite.setFillColor(Color::White);
                        }

                        windowss.draw(back_Cardi_sprite);
                        id++;
                    }
                }
                if ((dynamic_cast<HumanPlayer*>(Player[1].get()) != nullptr && Player[1]->isAttacker ))
                {
                    // Определение позиции и поворота карт в зависимости от индекса игрока
                    float rotation;
                    if (playerIndex == 1) { // Игрок снизу
                        rotation = 0;
                    } else if (playerIndex == 2) { // Игрок справа
                        rotation = 90;
                    } else if (playerIndex == 3) { // Игрок сверху
                        rotation = 0;
                    } else if (playerIndex == 0) { // Игрок слева
                        rotation = -90;
                    }
                    if (playerIndex == 1) {
                        // Атакующий игрок видит значения своих карт
                        Cardi.sprite.setTexture(Cardi.texture);
                        // Определение позиции карты в зависимости от её индекса 
                        Cardi.sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex]->hand.size()) * id, 800));
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (std::find(selected_Cardi.begin(), selected_Cardi.end(), CardiIndex) != selected_Cardi.end())
                        {
                            Cardi.sprite.setColor(Color::Magenta);
                        }
                        else if (CardiIndex == selectedCardiIndex)
                        {
                            Cardi.sprite.setColor(Color::Yellow);
                        }
                        else
                        {
                            Cardi.sprite.setColor(Color::White);
                        }

                        windowss.draw(Cardi.sprite);
                        ++id; 
                        
                    } else {
                        // У остальных игроков карты рубашкой вверх
                        back_Cardi_sprite.setRotation(rotation);
                        if (playerIndex == 2) {
                            back_Cardi_sprite.setPosition(Vector2f(1820, 123 + (800 / Player[playerIndex]->hand.size()) * id));
                        } else if (playerIndex == 3) {
                            back_Cardi_sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex]->hand.size()) * id, 150));
                        } else if (playerIndex == 0) {
                            back_Cardi_sprite.setPosition(Vector2f(100, 200 + (800 / Player[playerIndex]->hand.size()) * id));
                        }
                        //neighborIndex
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (playerIndex == neighborIndex && CardiIndex == selectedCardiIndexRight)
                        {
                            back_Cardi_sprite.setFillColor(Color::Yellow);
                        }
                        else
                        {
                            back_Cardi_sprite.setFillColor(Color::White);
                        }

                        windowss.draw(back_Cardi_sprite);
                        id++;
                    }
                }
                
                if ((dynamic_cast<Bot*>(Player[2].get()) != nullptr && Player[2]->isAttacker ) || (dynamic_cast<Bot*>(Player[3].get()) != nullptr && Player[3]->isAttacker ) )
                {
                    // Определение позиции и поворота карт в зависимости от индекса игрока
                    float rotation;
                    if (playerIndex == 1) { // Игрок снизу
                        rotation = 0;
                    } else if (playerIndex == 2) { // Игрок справа
                        rotation = 90;
                    } else if (playerIndex == 3) { // Игрок сверху
                        rotation = 0;
                    } else if (playerIndex == 0) { // Игрок слева
                        rotation = -90;
                    }
                    if (playerIndex == 1) {
                        // Атакующий игрок видит значения своих карт
                        Cardi.sprite.setTexture(Cardi.texture);
                        // Определение позиции карты в зависимости от её индекса 
                        Cardi.sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex]->hand.size()) * id, 800));
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (std::find(selected_Cardi.begin(), selected_Cardi.end(), CardiIndex) != selected_Cardi.end())
                        {
                            Cardi.sprite.setColor(Color::Magenta);
                        }
                        else if (CardiIndex == selectedCardiIndex)
                        {
                            Cardi.sprite.setColor(Color::Yellow);
                        }
                        else
                        {
                            Cardi.sprite.setColor(Color::White);
                        }

                        windowss.draw(Cardi.sprite);
                        ++id; 
                        
                    } else {
                        // У остальных игроков карты рубашкой вверх
                        back_Cardi_sprite.setRotation(rotation);
                        if (playerIndex == 2) {
                            back_Cardi_sprite.setPosition(Vector2f(1820, 123 + (800 / Player[playerIndex]->hand.size()) * id));
                        } else if (playerIndex == 3) {
                            back_Cardi_sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex]->hand.size()) * id, 150));
                        } else if (playerIndex == 0) {
                            back_Cardi_sprite.setPosition(Vector2f(100, 200 + (800 / Player[playerIndex]->hand.size()) * id));
                        }
                        //neighborIndex
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (playerIndex == neighborIndex && CardiIndex == selectedCardiIndexRight)
                        {
                            back_Cardi_sprite.setFillColor(Color::Yellow);
                        }
                        else
                        {
                            back_Cardi_sprite.setFillColor(Color::White);
                        }

                        windowss.draw(back_Cardi_sprite);
                        id++;
                    }
                }
            }
        }
} 
void draw_for3(std::vector<std::unique_ptr<Player_>>& Player, int& selectedCardiIndex, std::vector<int>& selected_Cardi, int& selectedCardiIndexRight, int& neighborIndex, sf::RenderWindow& windowss, sf::Text& messege_winplayer, RectangleShape& back_Cardi_sprite)
{
    
        // Отрисовка карт для каждого игрока
        for (int playerIndex = 0; playerIndex < Player.size(); ++playerIndex) {
            int id = 1; // Переменная для определения позиции карты в ряду
            // Если игрок выиграл, отображаем сообщение о победе
            if (Player[playerIndex]->hasWon) {
                // Определение позиции для сообщения в зависимости от индекса игрока
                sf::Vector2f messagePosition;
                if (playerIndex == 0) { // Игрок снизу
                    messagePosition = sf::Vector2f(900.f, 800.f);
                } else if (playerIndex == 1) { // Игрок справа
                    messagePosition = sf::Vector2f(1720.f, 540.f);
                } else if (playerIndex == 2) { // Игрок сверху
                    messagePosition = sf::Vector2f(900.f, 150.f);
                } else if (playerIndex == 3) { // Игрок слева
                    messagePosition = sf::Vector2f(100.f, 540.f);
                }
                messege_winplayer.setPosition(messagePosition);
                windowss.draw(messege_winplayer);
                continue; // Пропускаем отрисовку карт для выигравшего игрока
            }
            // Отрисовка карт текущего игрока
            for (size_t CardiIndex = 0; CardiIndex < Player[playerIndex]->hand.size(); ++CardiIndex) {
                Cardi& Cardi = Player[playerIndex]->hand[CardiIndex];
                if ((dynamic_cast<HumanPlayer*>(Player[0].get()) != nullptr && Player[0]->isAttacker ))
                {
                    // Определение позиции и поворота карт в зависимости от индекса игрока
                    float rotation;
                    if (playerIndex == 0) { // Игрок снизу
                        rotation = 0;
                    } else if (playerIndex == 1) { // Игрок справа
                        rotation = 90;
                    } else if (playerIndex == 2) { // Игрок сверху
                        rotation = 0;
                    } else if (playerIndex == 3) { // Игрок слева
                        rotation = -90;
                    }
                    if (playerIndex == 0) {
                        // Атакующий игрок видит значения своих карт
                        Cardi.sprite.setTexture(Cardi.texture);
                        // Определение позиции карты в зависимости от её индекса 
                        Cardi.sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex]->hand.size()) * id, 800));
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (std::find(selected_Cardi.begin(), selected_Cardi.end(), CardiIndex) != selected_Cardi.end())
                        {
                            Cardi.sprite.setColor(Color::Magenta);
                        }
                        else if (CardiIndex == selectedCardiIndex)
                        {
                            Cardi.sprite.setColor(Color::Yellow);
                        }
                        else
                        {
                            Cardi.sprite.setColor(Color::White);
                        }

                        windowss.draw(Cardi.sprite);
                        ++id; 
                        
                    } else {
                        // У остальных игроков карты рубашкой вверх
                        back_Cardi_sprite.setRotation(rotation);
                        if (playerIndex == 1) {
                            back_Cardi_sprite.setPosition(Vector2f(1820, 123 + (800 / Player[playerIndex]->hand.size()) * id));
                        } else if (playerIndex == 2) {
                            back_Cardi_sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex]->hand.size()) * id, 150));
                        } else if (playerIndex == 3) {
                            back_Cardi_sprite.setPosition(Vector2f(100, 200 + (800 / Player[playerIndex]->hand.size()) * id));
                        }
                        //neighborIndex
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (playerIndex == neighborIndex && CardiIndex == selectedCardiIndexRight)
                        {
                            back_Cardi_sprite.setFillColor(Color::Yellow);
                        }
                        else
                        {
                            back_Cardi_sprite.setFillColor(Color::White);
                        }

                        windowss.draw(back_Cardi_sprite);
                        id++;
                    }
                }
                if ((dynamic_cast<HumanPlayer*>(Player[1].get()) != nullptr && Player[1]->isAttacker ))
                {
                    // Определение позиции и поворота карт в зависимости от индекса игрока
                    float rotation;
                    if (playerIndex == 1) { // Игрок снизу
                        rotation = 0;
                    } else if (playerIndex == 2) { // Игрок справа
                        rotation = 90;
                    } else if (playerIndex == 3) { // Игрок сверху
                        rotation = 0;
                    } else if (playerIndex == 0) { // Игрок слева
                        rotation = -90;
                    }
                    if (playerIndex == 1) {
                        // Атакующий игрок видит значения своих карт
                        Cardi.sprite.setTexture(Cardi.texture);
                        // Определение позиции карты в зависимости от её индекса 
                        Cardi.sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex]->hand.size()) * id, 800));
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (std::find(selected_Cardi.begin(), selected_Cardi.end(), CardiIndex) != selected_Cardi.end())
                        {
                            Cardi.sprite.setColor(Color::Magenta);
                        }
                        else if (CardiIndex == selectedCardiIndex)
                        {
                            Cardi.sprite.setColor(Color::Yellow);
                        }
                        else
                        {
                            Cardi.sprite.setColor(Color::White);
                        }

                        windowss.draw(Cardi.sprite);
                        ++id; 
                        
                    } else {
                        // У остальных игроков карты рубашкой вверх
                        back_Cardi_sprite.setRotation(rotation);
                        if (playerIndex == 2) {
                            back_Cardi_sprite.setPosition(Vector2f(1820, 123 + (800 / Player[playerIndex]->hand.size()) * id));
                        } else if (playerIndex == 3) {
                            back_Cardi_sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex]->hand.size()) * id, 150));
                        } else if (playerIndex == 0) {
                            back_Cardi_sprite.setPosition(Vector2f(100, 200 + (800 / Player[playerIndex]->hand.size()) * id));
                        }
                        //neighborIndex
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (playerIndex == neighborIndex && CardiIndex == selectedCardiIndexRight)
                        {
                            back_Cardi_sprite.setFillColor(Color::Yellow);
                        }
                        else
                        {
                            back_Cardi_sprite.setFillColor(Color::White);
                        }

                        windowss.draw(back_Cardi_sprite);
                        id++;
                    }
                }
                
                if ((dynamic_cast<HumanPlayer*>(Player[2].get()) != nullptr && Player[2]->isAttacker ))
                {
                    // Определение позиции и поворота карт в зависимости от индекса игрока
                    float rotation;
                    if (playerIndex == 2) { // Игрок снизу
                        rotation = 0;
                    } else if (playerIndex == 3) { // Игрок справа
                        rotation = 90;
                    } else if (playerIndex == 0) { // Игрок сверху
                        rotation = 0;
                    } else if (playerIndex == 1) { // Игрок слева
                        rotation = -90;
                    }
                    if (playerIndex == 2) {
                        // Атакующий игрок видит значения своих карт
                        Cardi.sprite.setTexture(Cardi.texture);
                        // Определение позиции карты в зависимости от её индекса 
                        Cardi.sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex]->hand.size()) * id, 800));
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (std::find(selected_Cardi.begin(), selected_Cardi.end(), CardiIndex) != selected_Cardi.end())
                        {
                            Cardi.sprite.setColor(Color::Magenta);
                        }
                        else if (CardiIndex == selectedCardiIndex)
                        {
                            Cardi.sprite.setColor(Color::Yellow);
                        }
                        else
                        {
                            Cardi.sprite.setColor(Color::White);
                        }

                        windowss.draw(Cardi.sprite);
                        ++id; 
                        
                    } else {
                        // У остальных игроков карты рубашкой вверх
                        back_Cardi_sprite.setRotation(rotation);
                        if (playerIndex == 3) {
                            back_Cardi_sprite.setPosition(Vector2f(1820, 123 + (800 / Player[playerIndex]->hand.size()) * id));
                        } else if (playerIndex == 0) {
                            back_Cardi_sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex]->hand.size()) * id, 150));
                        } else if (playerIndex == 1) {
                            back_Cardi_sprite.setPosition(Vector2f(100, 200 + (800 / Player[playerIndex]->hand.size()) * id));
                        }
                        //neighborIndex
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (playerIndex == neighborIndex && CardiIndex == selectedCardiIndexRight)
                        {
                            back_Cardi_sprite.setFillColor(Color::Yellow);
                        }
                        else
                        {
                            back_Cardi_sprite.setFillColor(Color::White);
                        }

                        windowss.draw(back_Cardi_sprite);
                        id++;
                    }
                }
                
                if ((dynamic_cast<Bot*>(Player[3].get()) != nullptr && Player[3]->isAttacker ))
                {                    // Определение позиции и поворота карт в зависимости от индекса игрока
                    float rotation;
                    if (playerIndex == 2) { // Игрок снизу
                        rotation = 0;
                    } else if (playerIndex == 3) { // Игрок справа
                        rotation = 90;
                    } else if (playerIndex == 0) { // Игрок сверху
                        rotation = 0;
                    } else if (playerIndex == 1) { // Игрок слева
                        rotation = -90;
                    }
                    if (playerIndex == 2) {
                        // Атакующий игрок видит значения своих карт
                        Cardi.sprite.setTexture(Cardi.texture);
                        // Определение позиции карты в зависимости от её индекса 
                        Cardi.sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex]->hand.size()) * id, 800));
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (std::find(selected_Cardi.begin(), selected_Cardi.end(), CardiIndex) != selected_Cardi.end())
                        {
                            Cardi.sprite.setColor(Color::Magenta);
                        }
                        else if (CardiIndex == selectedCardiIndex)
                        {
                            Cardi.sprite.setColor(Color::Yellow);
                        }
                        else
                        {
                            Cardi.sprite.setColor(Color::White);
                        }

                        windowss.draw(Cardi.sprite);
                        ++id; 
                        
                    } else {
                        // У остальных игроков карты рубашкой вверх
                        back_Cardi_sprite.setRotation(rotation);
                        if (playerIndex == 3) {
                            back_Cardi_sprite.setPosition(Vector2f(1820, 123 + (800 / Player[playerIndex]->hand.size()) * id));
                        } else if (playerIndex == 0) {
                            back_Cardi_sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex]->hand.size()) * id, 150));
                        } else if (playerIndex == 1) {
                            back_Cardi_sprite.setPosition(Vector2f(100, 200 + (800 / Player[playerIndex]->hand.size()) * id));
                        }
                        //neighborIndex
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (playerIndex == neighborIndex && CardiIndex == selectedCardiIndexRight)
                        {
                            back_Cardi_sprite.setFillColor(Color::Yellow);
                        }
                        else
                        {
                            back_Cardi_sprite.setFillColor(Color::White);
                        }

                        windowss.draw(back_Cardi_sprite);
                        id++;
                    }
                }
            }
        }
} 
void draw_for4(std::vector<std::unique_ptr<Player_>>& Player, int& selectedCardiIndex, std::vector<int>& selected_Cardi, int& selectedCardiIndexRight, int& neighborIndex, sf::RenderWindow& windowss, sf::Text& messege_winplayer, RectangleShape& back_Cardi_sprite)
{
    
        // Отрисовка карт для каждого игрока
        for (int playerIndex = 0; playerIndex < Player.size(); ++playerIndex) {
            int id = 1; // Переменная для определения позиции карты в ряду
            // Если игрок выиграл, отображаем сообщение о победе
            if (Player[playerIndex]->hasWon) {
                // Определение позиции для сообщения в зависимости от индекса игрока
                sf::Vector2f messagePosition;
                if (playerIndex == 0) { // Игрок снизу
                    messagePosition = sf::Vector2f(900.f, 800.f);
                } else if (playerIndex == 1) { // Игрок справа
                    messagePosition = sf::Vector2f(1720.f, 540.f);
                } else if (playerIndex == 2) { // Игрок сверху
                    messagePosition = sf::Vector2f(900.f, 150.f);
                } else if (playerIndex == 3) { // Игрок слева
                    messagePosition = sf::Vector2f(100.f, 540.f);
                }
                messege_winplayer.setPosition(messagePosition);
                windowss.draw(messege_winplayer);
                continue; // Пропускаем отрисовку карт для выигравшего игрока
            }
            // Отрисовка карт текущего игрока
            for (size_t CardiIndex = 0; CardiIndex < Player[playerIndex]->hand.size(); ++CardiIndex) {
                Cardi& Cardi = Player[playerIndex]->hand[CardiIndex];
                if ((dynamic_cast<HumanPlayer*>(Player[0].get()) != nullptr && Player[0]->isAttacker ))
                {
                    // Определение позиции и поворота карт в зависимости от индекса игрока
                    float rotation;
                    if (playerIndex == 0) { // Игрок снизу
                        rotation = 0;
                    } else if (playerIndex == 1) { // Игрок справа
                        rotation = 90;
                    } else if (playerIndex == 2) { // Игрок сверху
                        rotation = 0;
                    } else if (playerIndex == 3) { // Игрок слева
                        rotation = -90;
                    }
                    if (playerIndex == 0) {
                        // Атакующий игрок видит значения своих карт
                        Cardi.sprite.setTexture(Cardi.texture);
                        // Определение позиции карты в зависимости от её индекса 
                        Cardi.sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex]->hand.size()) * id, 800));
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (std::find(selected_Cardi.begin(), selected_Cardi.end(), CardiIndex) != selected_Cardi.end())
                        {
                            Cardi.sprite.setColor(Color::Magenta);
                        }
                        else if (CardiIndex == selectedCardiIndex)
                        {
                            Cardi.sprite.setColor(Color::Yellow);
                        }
                        else
                        {
                            Cardi.sprite.setColor(Color::White);
                        }

                        windowss.draw(Cardi.sprite);
                        ++id; 
                        
                    } else {
                        // У остальных игроков карты рубашкой вверх
                        back_Cardi_sprite.setRotation(rotation);
                        if (playerIndex == 1) {
                            back_Cardi_sprite.setPosition(Vector2f(1820, 123 + (800 / Player[playerIndex]->hand.size()) * id));
                        } else if (playerIndex == 2) {
                            back_Cardi_sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex]->hand.size()) * id, 150));
                        } else if (playerIndex == 3) {
                            back_Cardi_sprite.setPosition(Vector2f(100, 200 + (800 / Player[playerIndex]->hand.size()) * id));
                        }
                        //neighborIndex
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (playerIndex == neighborIndex && CardiIndex == selectedCardiIndexRight)
                        {
                            back_Cardi_sprite.setFillColor(Color::Yellow);
                        }
                        else
                        {
                            back_Cardi_sprite.setFillColor(Color::White);
                        }

                        windowss.draw(back_Cardi_sprite);
                        id++;
                    }
                }
                if ((dynamic_cast<HumanPlayer*>(Player[1].get()) != nullptr && Player[1]->isAttacker ))
                {
                    // Определение позиции и поворота карт в зависимости от индекса игрока
                    float rotation;
                    if (playerIndex == 1) { // Игрок снизу
                        rotation = 0;
                    } else if (playerIndex == 2) { // Игрок справа
                        rotation = 90;
                    } else if (playerIndex == 3) { // Игрок сверху
                        rotation = 0;
                    } else if (playerIndex == 0) { // Игрок слева
                        rotation = -90;
                    }
                    if (playerIndex == 1) {
                        // Атакующий игрок видит значения своих карт
                        Cardi.sprite.setTexture(Cardi.texture);
                        // Определение позиции карты в зависимости от её индекса 
                        Cardi.sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex]->hand.size()) * id, 800));
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (std::find(selected_Cardi.begin(), selected_Cardi.end(), CardiIndex) != selected_Cardi.end())
                        {
                            Cardi.sprite.setColor(Color::Magenta);
                        }
                        else if (CardiIndex == selectedCardiIndex)
                        {
                            Cardi.sprite.setColor(Color::Yellow);
                        }
                        else
                        {
                            Cardi.sprite.setColor(Color::White);
                        }

                        windowss.draw(Cardi.sprite);
                        ++id; 
                        
                    } else {
                        // У остальных игроков карты рубашкой вверх
                        back_Cardi_sprite.setRotation(rotation);
                        if (playerIndex == 2) {
                            back_Cardi_sprite.setPosition(Vector2f(1820, 123 + (800 / Player[playerIndex]->hand.size()) * id));
                        } else if (playerIndex == 3) {
                            back_Cardi_sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex]->hand.size()) * id, 150));
                        } else if (playerIndex == 0) {
                            back_Cardi_sprite.setPosition(Vector2f(100, 200 + (800 / Player[playerIndex]->hand.size()) * id));
                        }
                        //neighborIndex
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (playerIndex == neighborIndex && CardiIndex == selectedCardiIndexRight)
                        {
                            back_Cardi_sprite.setFillColor(Color::Yellow);
                        }
                        else
                        {
                            back_Cardi_sprite.setFillColor(Color::White);
                        }

                        windowss.draw(back_Cardi_sprite);
                        id++;
                    }
                }
                
                if ((dynamic_cast<HumanPlayer*>(Player[2].get()) != nullptr && Player[2]->isAttacker ))
                {
                    // Определение позиции и поворота карт в зависимости от индекса игрока
                    float rotation;
                    if (playerIndex == 2) { // Игрок снизу
                        rotation = 0;
                    } else if (playerIndex == 3) { // Игрок справа
                        rotation = 90;
                    } else if (playerIndex == 0) { // Игрок сверху
                        rotation = 0;
                    } else if (playerIndex == 1) { // Игрок слева
                        rotation = -90;
                    }
                    if (playerIndex == 2) {
                        // Атакующий игрок видит значения своих карт
                        Cardi.sprite.setTexture(Cardi.texture);
                        // Определение позиции карты в зависимости от её индекса 
                        Cardi.sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex]->hand.size()) * id, 800));
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (std::find(selected_Cardi.begin(), selected_Cardi.end(), CardiIndex) != selected_Cardi.end())
                        {
                            Cardi.sprite.setColor(Color::Magenta);
                        }
                        else if (CardiIndex == selectedCardiIndex)
                        {
                            Cardi.sprite.setColor(Color::Yellow);
                        }
                        else
                        {
                            Cardi.sprite.setColor(Color::White);
                        }

                        windowss.draw(Cardi.sprite);
                        ++id; 
                        
                    } else {
                        // У остальных игроков карты рубашкой вверх
                        back_Cardi_sprite.setRotation(rotation);
                        if (playerIndex == 3) {
                            back_Cardi_sprite.setPosition(Vector2f(1820, 123 + (800 / Player[playerIndex]->hand.size()) * id));
                        } else if (playerIndex == 0) {
                            back_Cardi_sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex]->hand.size()) * id, 150));
                        } else if (playerIndex == 1) {
                            back_Cardi_sprite.setPosition(Vector2f(100, 200 + (800 / Player[playerIndex]->hand.size()) * id));
                        }
                        //neighborIndex
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (playerIndex == neighborIndex && CardiIndex == selectedCardiIndexRight)
                        {
                            back_Cardi_sprite.setFillColor(Color::Yellow);
                        }
                        else
                        {
                            back_Cardi_sprite.setFillColor(Color::White);
                        }

                        windowss.draw(back_Cardi_sprite);
                        id++;
                    }
                }
                
                if ((dynamic_cast<HumanPlayer*>(Player[3].get()) != nullptr && Player[3]->isAttacker ))
                {
                    // Определение позиции и поворота карт в зависимости от индекса игрока
                    float rotation;
                    if (playerIndex == 3) { // Игрок снизу
                        rotation = 0;
                    } else if (playerIndex == 0) { // Игрок справа
                        rotation = 90;
                    } else if (playerIndex == 1) { // Игрок сверху
                        rotation = 0;
                    } else if (playerIndex == 2) { // Игрок слева
                        rotation = -90;
                    }
                    if (playerIndex == 3) {
                        // Атакующий игрок видит значения своих карт
                        Cardi.sprite.setTexture(Cardi.texture);
                        // Определение позиции карты в зависимости от её индекса 
                        Cardi.sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex]->hand.size()) * id, 800));
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (std::find(selected_Cardi.begin(), selected_Cardi.end(), CardiIndex) != selected_Cardi.end())
                        {
                            Cardi.sprite.setColor(Color::Magenta);
                        }
                        else if (CardiIndex == selectedCardiIndex)
                        {
                            Cardi.sprite.setColor(Color::Yellow);
                        }
                        else
                        {
                            Cardi.sprite.setColor(Color::White);
                        }

                        windowss.draw(Cardi.sprite);
                        ++id; 
                        
                    } else {
                        // У остальных игроков карты рубашкой вверх
                        back_Cardi_sprite.setRotation(rotation);
                        if (playerIndex == 0) {
                            back_Cardi_sprite.setPosition(Vector2f(1820, 123 + (800 / Player[playerIndex]->hand.size()) * id));
                        } else if (playerIndex == 1) {
                            back_Cardi_sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex]->hand.size()) * id, 150));
                        } else if (playerIndex == 2) {
                            back_Cardi_sprite.setPosition(Vector2f(100, 200 + (800 / Player[playerIndex]->hand.size()) * id));
                        }
                        //neighborIndex
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (playerIndex == neighborIndex && CardiIndex == selectedCardiIndexRight)
                        {
                            back_Cardi_sprite.setFillColor(Color::Yellow);
                        }
                        else
                        {
                            back_Cardi_sprite.setFillColor(Color::White);
                        }

                        windowss.draw(back_Cardi_sprite);
                        id++;
                    }
                }
            }
        }
}   
bool pair_of_Cardis(const Cardi& Cardi1, const Cardi& Cardi2) {
    if((Cardi1.suit == "SPADES" && Cardi1.value == 12) || (Cardi2.suit == "SPADES" && Cardi2.value == 12))
    {
        return false;
    
    }
    return Cardi1.value == Cardi2.value;
}
// Функция для поиска текущего игрока, чей ход необходимо выполнить
int findCurrentPlayer(const  std::vector<std::unique_ptr<Player_>>& players) {
    for (int i = 0; i < players.size(); ++i) {
        if (!players[i]->hand.empty() && !players[i]->hasWon) {
            if (players[i]->isAttacker)
            {
                return i; // Возвращает индекс текущего атакующего игрока
            }
        }
    }
    return -1; // Все игроки выиграли
}
int findNextPlayerWithCardis(const std::vector<std::unique_ptr<Player_>>& players, int currentIndex) {
    int nextIndex = (currentIndex + 1) % players.size();
    while (players[nextIndex]->hand.empty() && nextIndex != currentIndex) {
        // Пропускаем игроков без карт
        nextIndex = (nextIndex + 1) % players.size();
    }
    return nextIndex;
}

// Функция для сохранения состояния игры в файл
void saveGameState(const GameState& state, const std::string& filename) {
    std::ofstream outfile(filename);
    if (outfile.is_open()) {
        // Сохранение колоды
        outfile << state.deck.size() << std::endl;
        for (const auto& Cardi : state.deck) {
            outfile << Cardi.value << " " << Cardi.suit << std::endl;
        }
        // Сохранение игроков
        outfile << state.players.size() << std::endl;
        for (const auto& player : state.players) {
            outfile << player.hand.size() << std::endl;
            for (const auto& Cardi : player.hand) {
                outfile << Cardi.value << " " << Cardi.suit << std::endl;
            }
            outfile << player.isAttacker << " " << player.isDefender << " " << player.hasWon << std::endl;
        }
        // Сохранение других переменных состояния игры
        outfile << state.currentPlayerIndex << std::endl;
        // ... (Сохранение других переменных)
        outfile << state.selectedCardiIndex << std::endl;
        outfile << state.selectedCardiIndexRight << std::endl;
        outfile << state.selectedCardiIndex << std::endl;
        outfile << state.messege_win_player << std::endl;

        // Сохранение selected_Cardi
        outfile << state.selected_Cardi.size() << std::endl;
        for (const auto& index_Cardi : state.selected_Cardi) {
            outfile << index_Cardi << std::endl;
        }

        outfile << state.showCardiSelectionMessage << std::endl;
        outfile << state.selectedButtonIndex << std::endl;
        outfile << state.showMessageEscape << std::endl;
        outfile << state.returnToMenu << std::endl;
        outfile << state.playersChoseNo << std::endl;
        outfile << state.showMessageWait << std::endl;
        outfile << state.Taking_Cardis << std::endl;
        outfile << state.canSelectCardis << std::endl;
        outfile << state.canTakeCardi << std::endl;
        outfile << state.CardiTaken << std::endl;
        outfile << state.move_transition << std::endl;
        outfile << state.crdpair << std::endl;
        outfile << state.EndGame << std::endl;
        outfile << state.GameOver << std::endl;
        outfile << state.YouWin << std::endl;
        outfile << state.neighborIndex << std::endl;
        outfile << state.activePlayers << std::endl;
        outfile << state.selectedButtonIndex_2 << std::endl;
        
        outfile.close();
    } else {
        std::cerr << "Ошибка открытия файла для сохранения: " << filename << std::endl;
    }
}

// Функция для загрузки состояния игры из файла
GameState loadGameState(const std::string& filename) {
    GameState state;
    std::ifstream infile(filename);
    if (infile.is_open()) {
        // Загрузка колоды
        int deckSize;
        infile >> deckSize;
        state.deck.resize(deckSize);
        for (auto& Cardi : state.deck) {
            int value;
            std::string suit;
            infile >> value >> suit;
            Cardi.value = value;
            Cardi.suit = suit;
            // Загрузка текстуры карты (вам нужно реализовать эту часть)
        }
        // Загрузка игроков
        int playerCount;
        infile >> playerCount;
        state.players.resize(playerCount);
        for (auto& player : state.players) {
            int handSize;
            infile >> handSize;
                        player.hand.resize(handSize);
            for (auto& Cardi : player.hand) {
                int value;
                std::string suit;
                infile >> value >> suit;
                Cardi.value = value;
                Cardi.suit = suit;
                // Загрузка текстуры карты (вам нужно реализовать эту часть)
            }
            infile >> player.isAttacker >> player.isDefender >> player.hasWon;
        }
        // Загрузка других переменных состояния игры
        infile >> state.currentPlayerIndex;
        // ... (Загрузка других переменных)
        infile >> state.selectedCardiIndex;
        infile >> state.selectedCardiIndexRight;
        infile >> state.selectedCardiIndex;
        infile >> state.messege_win_player;
/////////////////////////////////////////////////////
        // Сохранение selected_Cardi
        int selected_CardiSize;
        infile >> selected_CardiSize;
        state.selected_Cardi.resize(selected_CardiSize);
        for (int i = 0; i < selected_CardiSize; ++i) {
            int index_Cardi;
            infile >> index_Cardi;
            state.selected_Cardi[i] = index_Cardi; 
        }
        infile >> state.showCardiSelectionMessage;
        infile >> state.selectedButtonIndex;
        infile >> state.showMessageEscape;
        infile >> state.returnToMenu;
        infile >> state.playersChoseNo;
        infile >> state.showMessageWait;
        infile >> state.Taking_Cardis;
        infile >> state.canSelectCardis;
        infile >> state.canTakeCardi;
        infile >> state.CardiTaken;
        infile >> state.move_transition;
        infile >> state.crdpair;
        infile >> state.EndGame;
        infile >> state.GameOver;
        infile >> state.YouWin;
        infile >> state.neighborIndex;
        infile >> state.activePlayers;
        infile >> state.selectedButtonIndex_2;

        infile.close();
    } else {
        std::cerr << "Ошибка открытия файла для загрузки: " << filename << std::endl;
    }
    return state;
}
int main_pikgame(sf::RenderWindow& windowss, int& number_of_players_people) {
    int currentPlayerIndex = 0;
    int number_of_players = 4;
    srand(time(0));
    bool PlayerHuman = false;
    Texture tableTexture;
    if (!tableTexture.loadFromFile("resources/table.png")) {
        std::cerr << "Ошибка загрузки текстуры стола: " << "resources/table.png" << std::endl;
        return 1; 
    }
    RectangleShape background(Vector2f(1920,1080));
    background.setTexture(&tableTexture);
    background.setPosition(Vector2f(0,0));

    GameState gameState; // Создание объекта для хранения состояния игры

    // Загрузка предыдущего состояния игры из файла (если он существует)
    std::string saveFilename = "pikgame_save.txt";
    gameState = loadGameState(saveFilename);

    std::vector<std::string> suits = {"HEARTS", "DIAMONDS", "CLUBS", "SPADES"};
    // основная колода 
    std::vector<Cardi> deck;
    sf::Texture CardiTexture;
    
    std::vector<std::unique_ptr<Player_>> Player;

    // Создание игроков-людей и ботов
    for (int i = 0; i < number_of_players_people; ++i) {
        Player.push_back(std::make_unique<HumanPlayer>());
    }
    for (int i = 0; i < number_of_players - number_of_players_people; ++i) {
        Player.push_back(std::make_unique<Bot>());
    }
    // Если файл не найден, начинаем новую игру
    if (gameState.deck.empty() && gameState.players.empty()) 
    {
        for (int value = 6; value <= 14; ++value) {
            for (const std::string& suit : suits) {
                // Исключаем даму крести
                if (value == 12 && suit == "CLUBS") {
                    continue; // Пропускаем добавление карты "Дама Крести"
                }
                Cardi Cardi;
                Cardi.value = value;
                Cardi.suit = suit;
                std::string filename = "resources/" + std::to_string(value) + suit + ".png";
                if (!CardiTexture.loadFromFile(filename)) { 
                    std::cerr << "Ошибка загрузки текстуры: " << filename << std::endl; 
                } else {
                    Cardi.texture = CardiTexture; 
                    Cardi.sprite.setTexture(Cardi.texture);
                    Cardi.sprite.setScale(0.6f, 0.6f); // Уменьшение размера спрайта 
                    deck.push_back(Cardi);
                }
            }
        }

        // Перемешивание колоды
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(deck.begin(), deck.end(), g);

        Player[0]->isAttacker = true;
        Player[0]->isDefender = false;
        for (int i = 1; i < Player.size(); ++i) {
            Player[i]->isAttacker = false;
            Player[i]->isDefender = false;
        }
        //раздача карт на руки
        for (int i = 0; i < 9; ++i)
        {
            for (auto& player : Player) {
                if (deck.empty()) {
                    std::cerr << "Колода пуста. Невозможно продолжить раздачу карт." << std::endl;
                    break; // Выход из цикла, если колода пуста
                }
                player->hand.push_back(deck.back());
                deck.pop_back();
            }
        }
        gameState.currentPlayerIndex = 0; // Начало с первого игрока
        for (auto& player : Player) {
            if (auto bot = dynamic_cast<Bot*>(player.get())) {
                bot->disCardiPairs();
            }
        }
    }
    else {
        // Загрузка состояния из файла
        // Инициализация графики, шрифтов, текстур и других элементов игры
        for (auto& Cardi : gameState.deck) {
            std::string filename = "resources/" + std::to_string(Cardi.value) + Cardi.suit + ".png";
            sf::Texture texture;
            if (!texture.loadFromFile(filename)) {
                std::cerr << "Ошибка загрузки текстуры: " << filename << std::endl;
            } else {
                gameState.textures[filename] = texture;
            }
        }

        // Установка текстур и спрайтов для карт в руках игроков
        for (auto& player : gameState.players) {
            for (auto& Cardi : player.hand) {
                std::string filename = "resources/" + std::to_string(Cardi.value) + Cardi.suit + ".png";
                Cardi.texture = gameState.textures[filename];
                Cardi.sprite.setTexture(Cardi.texture);
            }
        }
    }
    
    // Перечисление этапов игры
    enum GameState { PASSING_CARDS, ROUND};
    GameState currentGameState = PASSING_CARDS;
    int selectedCardiIndex = 0;
    int selectedCardiIndexRight = 0;

    std::vector<int> selected_Cardi(0);
    sf::Font font_1;
    if (!font_1.loadFromFile("resources/arial.ttf"))
    {
        std::cerr << "Ошибка загрузки шрифта!" << std::endl;
    }
    bool messege_win_player = false;
    sf::Text messege_winplayer;
    messege_winplayer.setFont(font_1);
    messege_winplayer.setCharacterSize(24);
    messege_winplayer.setFillColor(sf::Color::White);
    messege_winplayer.setPosition(100.f, 100.f);

    // Текст вопроса
    messege_winplayer.setString(L"Игрок выиграл");

    sf::Text message_ispair;
    message_ispair.setFont(font_1);
    message_ispair.setCharacterSize(24);
    message_ispair.setFillColor(sf::Color::White);
    message_ispair.setPosition(100.f, 100.f);

    // Текст вопроса
    message_ispair.setString(L"Увы, эти карты сбросить не получиться...");
    // message_ispair.setString(L"Oh, you will not be able to disCardi these Cardis...");

    sf::Text message;
    message.setFont(font_1);
    message.setCharacterSize(30);
    message.setFillColor(sf::Color::White);
    message.setPosition(100.f, 100.f);

    // Текст вопроса
    message.setString(L"Есть ли что сбросить?");
    //  message.setString("Is there anything to reset?");
    // Создание кнопок "Да" и "Нет"
    sf::Text buttonYes;
    buttonYes.setFont(font_1);
    buttonYes.setString(L"Да");
    // buttonYes.setString("Yes");
    buttonYes.setCharacterSize(24);
    buttonYes.setFillColor(sf::Color::White);
    buttonYes.setPosition(200.f, 200.f);

    sf::Text buttonNo;
    buttonNo.setFont(font_1);
    buttonNo.setString(L"Нет");
    // buttonNo.setString("No");
    buttonNo.setCharacterSize(24);
    buttonNo.setFillColor(sf::Color::White);
    buttonNo.setPosition(400.f, 200.f);

    // Флаг для отображения текста выбора карт
    bool showCardiSelectionMessage = true;
    // Индекс выбранной кнопки (0 - Да, 1 - Нет)
    int selectedButtonIndex = 0;

    sf::Text message_escape;
    message_escape.setFont(font_1);
    message_escape.setCharacterSize(24);
    message_escape.setFillColor(sf::Color::White);
    message_escape.setPosition(100.f, 100.f);

    // Текст вопроса
    message_escape.setString(L"Вы действительно хотите выйти?");
    // message_escape.setString(L"Do you really want to get out?");
    // Флаг для отображения текста выхода из игры
    bool showMessageEscape = false;
    // Флаг для отображения текста выхода в меню
    bool returnToMenu = false;

    // Переменная для отслеживания количества игроков, выбравших "нет"
    int playersChoseNo = 0; 
    
    // Флаг для отображения текста ожидания игроков
    bool showMessageWait = false;
    // Здесь код для отображения сообщения "Ожидаем других игроков: количество"
    sf::Text wait_message;
    wait_message.setFont(font_1);
    wait_message.setCharacterSize(30);
    wait_message.setFillColor(sf::Color::White);
    wait_message.setPosition(100.f, 100.f);


    bool Taking_Cardis = false; // Флаг для проверки, взял ли игрок карту
    bool canSelectCardis = true; // Можно ли выбирать карты
    bool canTakeCardi = false; // Можно ли брать карту у противника
    bool CardiTaken = false; // Флаг, указывающий на то, что карта была взята

    bool move_transition = false;
    bool crdpair =false;
    bool EndGame = false; //окончание игры
    bool GameOver = false; //проигрыш
    bool YouWin = false; //выигрыш
    int neighborIndex = 1;
    int activePlayers = 0;

    // Создаем затемнение фона
    sf::RectangleShape backgroundFade;
    backgroundFade.setSize(sf::Vector2f(windowss.getSize().x, windowss.getSize().y));
    backgroundFade.setFillColor(sf::Color(0, 0, 0, 150)); // Полупрозрачный черный цвет

    // Загружаем изображения
    sf::Texture youWinTexture, gameOverTexture;
    if (!youWinTexture.loadFromFile("resources/YouWin.png")) {
        std::cerr << "Ошибка загрузки YouWin!" << std::endl;
    }
    if (!gameOverTexture.loadFromFile("resources/YouLose.png")) {
        std::cerr << "Ошибка загрузки YouLose!" << std::endl;
    }


    sf::Sprite endGameSprite;

    // Создаем кнопки 
    Button menuButton(L"Вернуться в меню", sf::Vector2f(400, 50), 30, sf::Color::Transparent, sf::Color::White, font_1);
    Button restartButton(L"Перезапустить игру", sf::Vector2f(400, 50), 30, sf::Color::Transparent, sf::Color::White, font_1);
    // Установка позиции кнопок
    menuButton.setPosition(sf::Vector2f(770, 640));
    restartButton.setPosition(sf::Vector2f(770, 740));
    int selectedButtonIndex_2 = 0; // Индекс выбранной кнопки
    std::vector<Button> buttons = {menuButton, restartButton}; // Вектор всех кнопок
    sf::Clock gameTimer; // Создание таймера

    // Game Loop
    while (windowss.isOpen()) 
    {
        // Обновляем индекс текущего игрока
        currentPlayerIndex = findCurrentPlayer(Player);
        neighborIndex =findNextPlayerWithCardis(Player, currentPlayerIndex); // Индекс соседа справа
        sf::Event event;
        while (windowss.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed && event.key.code == Keyboard::Escape)
            {
                showMessageEscape = true;
                showCardiSelectionMessage = false;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F5) {
                // Сохранение состояния игры
                saveGameState(gameState, saveFilename);
                std::cout << "Состояние игры сохранено в " << saveFilename << std::endl;
                
            }
            if (event.type == sf::Event::KeyPressed && EndGame)
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    // Переключение на предыдущую кнопку
                    selectedButtonIndex_2 = (selectedButtonIndex_2 - 1 + 2) % 2;
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    // Переключение на следующую кнопку
                    selectedButtonIndex_2 = (selectedButtonIndex_2 + 1) % 2;
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    if (selectedButtonIndex_2 == 0) // Проверка, выбрана ли кнопка "Menu"
                    {
                        returnToMenu = true;
                        // Очистка вектора игроков перед возвратом в меню
                        // Player.clear();
                        // return 0;
                        // break;
                    }
                    else if(selectedButtonIndex_2 == 1)// Проверка, выбрана ли кнопка "Restart"
                    {
                        EndGame = false;
                        GameOver = false;
                        YouWin = false;
                        // Перезапуск основного игрового цикла
                        main_pikgame(windowss, number_of_players_people);  return 0; break;
                    }

                }
            }
            if (currentPlayerIndex != -1 && dynamic_cast<HumanPlayer*>(Player[currentPlayerIndex].get()) != nullptr)//(currentPlayerIndex == 0) //пока только 0 человек
            {
            if (event.type == sf::Event::KeyPressed && (showCardiSelectionMessage || showMessageEscape))
            {
                // Проверка нажатия клавиш для переключения между кнопками "Да" и "Нет"

                    if (event.key.code == sf::Keyboard::Left)
                    {
                        // Переключение на предыдущую кнопку
                        selectedButtonIndex = (selectedButtonIndex - 1 + 2) % 2;
                    }
                    else if (event.key.code == sf::Keyboard::Right)
                    {
                        // Переключение на следующую кнопку
                        selectedButtonIndex = (selectedButtonIndex + 1) % 2;
                    }
                    else if (event.key.code == sf::Keyboard::Enter)
                    {
                        if (showCardiSelectionMessage && selectedButtonIndex == 0) // Проверка, выбрана ли кнопка "Да"
                        {
                            showCardiSelectionMessage = false; // Сброс флага после выбора кнопки "Да"
                            selected_Cardi.clear(); // Очистка выбора карт
                        }
                        else if(showMessageEscape && selectedButtonIndex == 0)
                        {
                            returnToMenu = true;
                            break;
                        }
                        else if(showMessageEscape && selectedButtonIndex == 1)
                        {
                            showMessageEscape = false;
                            showCardiSelectionMessage = true;
                            selected_Cardi.clear(); // Очистка выбора карт

                        }
                        else if (showCardiSelectionMessage && selectedButtonIndex == 1 && CardiTaken) // Проверка, выбрана ли кнопка "Нет"
                        {
                            showCardiSelectionMessage = false; // Сброс флага после выбора кнопки "Нет"
                            selected_Cardi.clear(); // Очистка выбора карт
                            move_transition = true;
                        }
                        else if(showCardiSelectionMessage && selectedButtonIndex == 1 && !canTakeCardi )  // Если выбрано "Нет"
                        {
                            // playersChoseNo++; так надо
                            playersChoseNo = Player.size(); // Для тестирования устанавливаем, что все игроки выбрали "нет"
                            canSelectCardis = false; // Блокируем выбор карт
                            showMessageWait = true; // Активация текста ожидания
                            showCardiSelectionMessage = false;
                            if (playersChoseNo == Player.size()) 
                            { // Если все игроки выбрали "нет"
                                showMessageWait = false; // Деактивация текста ожидания
                                canTakeCardi = true; // Позволяем взятие карты у противника
                            }
                        }
                }
            }  
            // Переход к предыдущей карте с круговым циклом
            else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left && (canTakeCardi && !CardiTaken))
            {                        
                if (selectedCardiIndexRight  > 0)
                {
                    --selectedCardiIndexRight ;
                }
                else
                {
                    selectedCardiIndexRight  = Player[neighborIndex]->hand.size() - 1;
                }
            }
            else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right && (canTakeCardi && !CardiTaken))
            {
                // Переход к предыдущей карте с круговым циклом
                if (selectedCardiIndexRight  < Player[neighborIndex]->hand.size() - 1)
                {
                    ++selectedCardiIndexRight ;
                }
                else
                {
                    selectedCardiIndexRight  = 0;
                }
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && (canTakeCardi && !CardiTaken)) 
            {
                // Если не выбрана, добавляем в вектор и меняем её цвет на красный
                selected_Cardi.push_back(selectedCardiIndexRight);
                // Взятие выбранной карты
                Cardi selectedCardi = Player[neighborIndex]->hand[selectedCardiIndexRight ];
                Player[neighborIndex]->hand.erase(Player[neighborIndex]->hand.begin() + selectedCardiIndexRight );
                Player[currentPlayerIndex]->hand.push_back(selectedCardi);
                CardiTaken = true; // Установка флага взятия карты
                if(Player[currentPlayerIndex]->isAttacker)////////
                {
                    showCardiSelectionMessage = true;  
                } 
                else
                {
                        showCardiSelectionMessage = false;  
                } 
                selectedCardiIndexRight = 0;
                //     // Обновление индекса после удаления
                selected_Cardi.clear(); // Очистка выбора карт
                crdpair = true;

            }
        
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left && crdpair)
            {
                // Переход к предыдущей карте с круговым циклом
                if (selectedCardiIndex > 0)
                {
                    --selectedCardiIndex;
                }
                else
                {
                    selectedCardiIndex = Player[currentPlayerIndex]->hand.size() - 1;
                }
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right && crdpair)
            {
                // Переход к следующей карте с круговым циклом
                if (selectedCardiIndex < Player[currentPlayerIndex]->hand.size() - 1)
                {
                    ++selectedCardiIndex;
                }
                else
                {
                    selectedCardiIndex = 0;
                }
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && crdpair)
            {
                if (showCardiSelectionMessage && selectedButtonIndex == 0) // Проверка, выбрана ли кнопка "Да"
                {
                    showCardiSelectionMessage = false; // Сброс флага после выбора кнопки "Да"
                    selected_Cardi.clear(); // Очистка выбора карт
                }
                else if (showCardiSelectionMessage && selectedButtonIndex == 1 && CardiTaken) // Проверка, выбрана ли кнопка "Нет"
                {
                    showCardiSelectionMessage = false; // Сброс флага после выбора кнопки "Нет"
                    selected_Cardi.clear(); // Очистка выбора карт
                    move_transition = true;
                }
                else if(showMessageEscape && selectedButtonIndex == 0)
                {
                    returnToMenu = true;
                    break;
                }
                else if(showMessageEscape && selectedButtonIndex == 1)
                {
                    showMessageEscape = false;
                    // showCardiSelectionMessage = true;
                    selected_Cardi.clear(); // Очистка выбора карт

                }
                else if (!showCardiSelectionMessage && !showMessageEscape)
                {
                    //(логика выбора и пролистывания карт)
                    
                    // Проверяем, выбрана ли уже карта
                    auto it = std::find(selected_Cardi.begin(), selected_Cardi.end(), selectedCardiIndex);

                    // Проверяем, выбрана ли уже карта
                    if (it == selected_Cardi.end())
                    {
                        // Если не выбрана, добавляем в вектор и меняем её цвет на красный
                        selected_Cardi.push_back(selectedCardiIndex);
                        
                        // Проверка выбрано ли две карты
                        if (selected_Cardi.size() == 2)
                        {
                            // Вызов функции для проверки на пару карт
                            bool is_pair = pair_of_Cardis(Player[currentPlayerIndex]->hand[selected_Cardi[0]], Player[currentPlayerIndex]->hand[selected_Cardi[1]]);
                            if (currentPlayerIndex != -1)
                            {
                                // Вывод результата
                                if (is_pair)
                                {
                                    // Действия, если выбранные карты являются парой
                                    // Сортировка индексов выбранных карт по убыванию
                                    std::sort(selected_Cardi.rbegin(), selected_Cardi.rend());

                                    // Удаление парных карт из руки текущего игрока
                                    for (int index : selected_Cardi)
                                    {
                                        Player[currentPlayerIndex]->hand.erase(Player[currentPlayerIndex]->hand.begin() + index);
                                    }
                                    // Обновление индекса после удаления
                                    if (selectedCardiIndex >= Player[currentPlayerIndex]->hand.size() &&  !Player[currentPlayerIndex]->hand.empty())
                                    {
                                        selectedCardiIndex = Player[currentPlayerIndex]->hand.size() - 1;
                                    }
                                    // Возврат к выбору кнопок
                                    showCardiSelectionMessage = true;
                                    selectedCardiIndex = 0;

                                }
                                else
                                {
                                    // Действия, если выбранные карты не являются парой

                                    // Отображение сообщения в течение 1.5 секунды
                                    sf::Clock clock; // Создание таймера
                                    while(clock.getElapsedTime() < sf::seconds(1.5))
                                    {
                                        // Обработка событий
                                        windowss.pollEvent(event);

                                        sf::Vector2u windowSize = windowss.getSize();
                                        float textX_ispair = windowSize.x / 2.f - message_ispair.getLocalBounds().width / 2.f;
                                        float textY_ispair = (800 + 150) / 2.f - message_ispair.getLocalBounds().height / 2.f;
                                        message_ispair.setPosition(textX_ispair, textY_ispair);
                                        // Здесь код для отрисовки сообщения на экране
                                        windowss.draw(message_ispair);
                                        // Обновление окна
                                        windowss.display();
                                    }

                                    // Показ выбора кнопок
                                    showCardiSelectionMessage = true;
                                }
                                
                                // Очистка списка выбранных карт
                                selected_Cardi.clear();
                            }
                        }
                    }
                    else
                    {
                        // Если уже выбрана, удаляем из вектора и меняем её цвет на обычный
                        selected_Cardi.erase(it);
                        Player[currentPlayerIndex]->hand[selectedCardiIndex].sprite.setColor(Color::White);
                    }
                }
            }          
            else if (currentPlayerIndex != -1 && dynamic_cast<HumanPlayer*>(Player[currentPlayerIndex].get()) != nullptr)//(currentPlayerIndex == 0) //пока только 0 человек
            {
            switch (event.type)
            {
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Left )
                    {
                        // Переход к предыдущей карте с круговым циклом
                        if (selectedCardiIndex > 0)
                        {
                            --selectedCardiIndex;
                        }
                        else
                        {
                            selectedCardiIndex = Player[currentPlayerIndex]->hand.size() - 1;
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Right)
                    {
                        // Переход к следующей карте с круговым циклом
                        if (selectedCardiIndex < Player[currentPlayerIndex]->hand.size() - 1)
                        {
                            ++selectedCardiIndex;
                        }
                        else
                        {
                            selectedCardiIndex = 0;
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Enter)
                    {
                        if (!showCardiSelectionMessage && !showMessageEscape)
                        {
                            //(логика выбора и пролистывания карт)
                            
                            // Проверяем, выбрана ли уже карта
                            auto it = std::find(selected_Cardi.begin(), selected_Cardi.end(), selectedCardiIndex);

                            // Проверяем, выбрана ли уже карта
                            if (it == selected_Cardi.end())
                            {
                                // Если не выбрана, добавляем в вектор и меняем её цвет на красный
                                selected_Cardi.push_back(selectedCardiIndex);
                                
                                // Проверка выбрано ли две карты
                                if (selected_Cardi.size() == 2)
                                {
                                    // Вызов функции для проверки на пару карт
                                    bool is_pair = pair_of_Cardis(Player[currentPlayerIndex]->hand[selected_Cardi[0]], Player[currentPlayerIndex]->hand[selected_Cardi[1]]);
                                    if (currentPlayerIndex != -1)
                                    {
                                        // Вывод результата
                                        if (is_pair)
                                        {
                                            // Действия, если выбранные карты являются парой
                                            // Сортировка индексов выбранных карт по убыванию
                                            std::sort(selected_Cardi.rbegin(), selected_Cardi.rend());

                                            // Удаление парных карт из руки текущего игрока
                                            for (int index : selected_Cardi)
                                            {
                                                Player[currentPlayerIndex]->hand.erase(Player[currentPlayerIndex]->hand.begin() + index);
                                            }
                                            // Возврат к выбору кнопок
                                            showCardiSelectionMessage = true;

                                        }
                                        else
                                        {
                                            // Действия, если выбранные карты не являются парой

                                            // Отображение сообщения в течение 1.5 секунды
                                            sf::Clock clock; // Создание таймера
                                            while(clock.getElapsedTime() < sf::seconds(1.5))
                                            {
                                                // Обработка событий
                                                windowss.pollEvent(event);

                                                sf::Vector2u windowSize = windowss.getSize();
                                                float textX_ispair = windowSize.x / 2.f - message_ispair.getLocalBounds().width / 2.f;
                                                float textY_ispair = (800 + 150) / 2.f - message_ispair.getLocalBounds().height / 2.f;
                                                message_ispair.setPosition(textX_ispair, textY_ispair);
                                                // Здесь код для отрисовки сообщения на экране
                                                windowss.draw(message_ispair);
                                                // Обновление окна
                                                windowss.display();
                                            }

                                            // Показ выбора кнопок
                                            showCardiSelectionMessage = true;
                                        }
                                        
                                        // Очистка списка выбранных карт
                                        selected_Cardi.clear();
                                    }
                                }
                            }
                            else
                            {
                                // Если уже выбрана, удаляем из вектора и меняем её цвет на обычный
                                selected_Cardi.erase(it);
                                Player[currentPlayerIndex]->hand[selectedCardiIndex].sprite.setColor(Color::White);
                            }
                        }
                    }
                    break;

                default:
                    break;
                }
            }
        }
        }
        if(returnToMenu)
        {
            Player.clear();
            return 0; break;
        }
        if (EndGame) {
            //окончание игры 
            if(Player[0]->hand.empty())
            {
                YouWin = true;
            }
            else
            {
                GameOver = true;
            }
            showCardiSelectionMessage = false;
            showMessageEscape = false;
        }
        if(move_transition)
        {
            do {
                // Передача хода следующему игроку
                currentPlayerIndex = (currentPlayerIndex + 1) % Player.size();
            } while (Player[currentPlayerIndex]->hand.empty()); // Проверка, есть ли у игрока карты

            CardiTaken = false; // Сброс флага взятия карты

            // Обновление статуса игроков
            for (int i = 0; i < Player.size(); ++i) {
                Player[i]->isAttacker = (i == currentPlayerIndex);
            }
            move_transition =false;
        }
        // Если текущий игрок - бот, выполняем его действия
        if (currentPlayerIndex != -1 && dynamic_cast<Bot*>(Player[currentPlayerIndex].get()) != nullptr ) {
            Bot* currentBot =  dynamic_cast<Bot*>(Player[currentPlayerIndex].get());;
            neighborIndex = findNextPlayerWithCardis(Player, currentPlayerIndex);
            currentBot->takeCardiFromPlayer(*Player[neighborIndex].get());
            currentBot->disCardiPairs();
            move_transition = true; // Передаем ход следующему игроку
            if(neighborIndex == 0)//(currentPlayerIndex == 3)
            {
                move_transition = false;
                canTakeCardi = true;
                CardiTaken = false;

                // currentPlayerIndex = (currentPlayerIndex + 1) % Player.size();
                currentPlayerIndex = 0;
                // Обновление статуса игроков
                for (int i = 0; i < Player.size(); ++i) {
                    Player[i]->isAttacker = (i == currentPlayerIndex);
                }
            }
        }
        // if (Player[currentPlayerIndex]->hand.empty()) {
        //     Player[currentPlayerIndex]->hasWon = true; // Игрок выиграл
        // }
        activePlayers = 0;
        for (const auto& player : Player) {
            if (!player->hand.empty()) {
                activePlayers++;
            }
        }

        if (activePlayers == 1 || Player[0]->hand.empty()) ////////
        {
            // Только один игрок остался с картами, игра окончена
            EndGame = true;
        }
        windowss.clear(Color(50,200,150));
        windowss.draw(background);

        // Отрисовка карт игроков

        Texture back_Cardi_texture;
        if (!back_Cardi_texture.loadFromFile("resources/back.png")) { return 1; }
        RectangleShape back_Cardi_sprite(Vector2f(77,110));
        back_Cardi_sprite.setTexture(&back_Cardi_texture);
        if (number_of_players_people == 1) {draw_for1(Player, selectedCardiIndex, selected_Cardi, selectedCardiIndexRight, neighborIndex, windowss, messege_winplayer, back_Cardi_sprite);}
        if (number_of_players_people == 2) {draw_for2(Player, selectedCardiIndex, selected_Cardi, selectedCardiIndexRight, neighborIndex, windowss, messege_winplayer, back_Cardi_sprite);}
        if (number_of_players_people == 3) {draw_for3(Player, selectedCardiIndex, selected_Cardi, selectedCardiIndexRight, neighborIndex, windowss, messege_winplayer, back_Cardi_sprite);}
        if (number_of_players_people == 4) {draw_for4(Player, selectedCardiIndex, selected_Cardi, selectedCardiIndexRight, neighborIndex, windowss, messege_winplayer, back_Cardi_sprite);}

        // Рисование сообщения о выборе карт, если флаг установлен 
        sf::Vector2u windowSize = windowss.getSize();

        // Рисование сообщения о выборе карт, если флаг установлен 
        float textX = windowSize.x / 2.f - message.getLocalBounds().width / 2.f;
        float textY = (800 + 150) / 2.f - message.getLocalBounds().height / 2.f;
        message.setPosition(textX, textY);

        float buttonWidth = buttonYes.getLocalBounds().width;
        float buttonHeight = buttonYes.getLocalBounds().height;

        float buttonX = windowSize.x / 2.f - buttonWidth / 2.f; 
        float buttonY = (800 + 150) / 2.f - buttonHeight / 2.f + 50;
        buttonYes.setPosition(buttonX, buttonY);

        buttonNo.setPosition(buttonX + buttonWidth + 50, buttonY);

        if (showCardiSelectionMessage)//showMessageEscape
        {
            windowss.draw(message);
            // Выделение выбранной кнопки
            if (selectedButtonIndex == 0)
            {
                buttonYes.setFillColor(sf::Color::Red);
                buttonNo.setFillColor(sf::Color::White);
            }
            else
            {
                buttonYes.setFillColor(sf::Color::White);
                buttonNo.setFillColor(sf::Color::Red);
            }

            // Рисование кнопок
            windowss.draw(buttonYes);
            windowss.draw(buttonNo);
        }
        // Рисование сообщения о нажатие Escape, если флаг установлен 
        float textX_message_escape = windowSize.x / 2.f - message_escape.getLocalBounds().width / 2.f;
        float textY_message_escape = (800 + 150) / 2.f - message_escape.getLocalBounds().height / 2.f;
        message_escape.setPosition(textX_message_escape, textY_message_escape);
        if (showMessageEscape)
        {
            windowss.draw(message_escape);
            // Выделение выбранной кнопки
            if (selectedButtonIndex == 0)
            {
                buttonYes.setFillColor(sf::Color::Red);
                buttonNo.setFillColor(sf::Color::White);
            }
            else
            {
                buttonYes.setFillColor(sf::Color::White);
                buttonNo.setFillColor(sf::Color::Red);
            }

            // Рисование кнопок
            windowss.draw(buttonYes);
            windowss.draw(buttonNo);
        }
        // Отображение текста ожидания, если флаг активен
        if (showMessageWait) {
            wait_message.setString(L"Ожидаем других игроков: " + std::to_string(Player.size() - playersChoseNo));
            // Рисование сообщения о выборе карт, если флаг установлен 
            float textX_wait_message = windowSize.x / 2.f - wait_message.getLocalBounds().width / 2.f;
            float textY_wait_message = (800 + 150) / 2.f - wait_message.getLocalBounds().height / 2.f;
            wait_message.setPosition(textX_wait_message, textY_wait_message);
        }
        if (EndGame) {
            // Определяем, какое изображение использовать
            if (GameOver) {
                endGameSprite.setTexture(gameOverTexture);
                endGameSprite.setPosition(620, 20);
                endGameSprite.setScale(1.0f, 1.0f); // Уменьшение размера спрайта 
            } else if (YouWin) {
                // Устанавливаем текстуру для спрайта
                endGameSprite.setTexture(youWinTexture);
                endGameSprite.setPosition(600, 170);
                endGameSprite.setScale(0.1f, 0.1f); // Уменьшение размера спрайта 
            }
            // Отрисовка затемнения фона
            windowss.draw(backgroundFade);

            // Отрисовка изображения
            windowss.draw(endGameSprite);
                        // Выделение выбранной кнопки
            if (selectedButtonIndex_2 == 0)
            {
                menuButton.setSelected(true);
                restartButton.setSelected(false);
            }
            else
            {
                menuButton.setSelected(false);
                restartButton.setSelected(true);
            }
            // Отрисовка кнопок
            menuButton.draw(windowss);
            restartButton.draw(windowss);
        }
        // Например, если вы хотите сохранять состояние игры каждую секунду:
        if (gameTimer.getElapsedTime().asSeconds() >= 1.0f) {
            saveGameState(gameState, saveFilename);
            gameTimer.restart();
        }
        windowss.display();
    }
    return 0;
}
