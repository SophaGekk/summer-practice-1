
#include <stdio.h>
#include <SFML/Graphics.hpp>
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
/// @brief Функция для отрисовки игры 1 vs 3 (1 - человек, 3 - бота)
/// @param Player вектор игроков
/// @param selectedCardiIndex текущая выбранная карта
/// @param selected_Cardi вектор выбранных карт
/// @param selectedCardiIndexRight текущая выбранная карта у соседа
/// @param neighborIndex индекс игрока - соседа
/// @param windowss игровое окно
/// @param messege_winplayer текст на месте карт победителя
/// @param back_Cardi_sprite оборот карты (рубашка)
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
/// @brief Функция для отрисовки игры 2 vs 2 (2 - человека, 2 - бота)
/// @param Player вектор игроков
/// @param selectedCardiIndex текущая выбранная карта
/// @param selected_Cardi вектор выбранных карт
/// @param selectedCardiIndexRight текущая выбранная карта у соседа
/// @param neighborIndex индекс игрока - соседа
/// @param windowss игровое окно
/// @param messege_winplayer текст на месте карт победителя
/// @param back_Cardi_sprite оборот карты (рубашка)
void draw_for2(std::vector<std::unique_ptr<Player_>>& Player, int& selectedCardiIndex, std::vector<int>& selected_Cardi, int& selectedCardiIndexRight, int& neighborIndex, sf::RenderWindow& windowss, sf::Text& messege_winplayer, RectangleShape& back_Cardi_sprite)
{
    if ((dynamic_cast<HumanPlayer*>(Player[0].get()) != nullptr && Player[0]->isAttacker && (!Player[0]->hasWon)))
    {// Отрисовка карт для каждого игрока
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
            // Отрисовка карт текущего игрока
            for (size_t CardiIndex = 0; CardiIndex < Player[playerIndex]->hand.size(); ++CardiIndex) {
                Cardi& Cardi = Player[playerIndex]->hand[CardiIndex];
                // Определение позиции и поворота карт в зависимости от индекса игрока

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
    else if ((dynamic_cast<HumanPlayer*>(Player[1].get()) != nullptr && Player[1]->isAttacker && (!Player[1]->hasWon)))
    {
        // Отрисовка карт для каждого игрока
        for (int playerIndex = 0; playerIndex < Player.size(); ++playerIndex) {
            int id = 1; // Переменная для определения позиции карты в ряду
            // Если игрок выиграл, отображаем сообщение о победе
            if (Player[playerIndex]->hasWon) {
                // Определение позиции для сообщения в зависимости от индекса игрока
                sf::Vector2f messagePosition;
                if (playerIndex == 1) { // Игрок снизу
                    messagePosition = sf::Vector2f(900.f, 800.f);
                } else if (playerIndex == 2) { // Игрок справа
                    messagePosition = sf::Vector2f(1720.f, 540.f);
                } else if (playerIndex == 3) { // Игрок сверху
                    messagePosition = sf::Vector2f(900.f, 150.f);
                } else if (playerIndex == 0) { // Игрок слева
                    messagePosition = sf::Vector2f(100.f, 540.f);
                }
                messege_winplayer.setPosition(messagePosition);
                windowss.draw(messege_winplayer);
                continue; // Пропускаем отрисовку карт для выигравшего игрока
            }
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
            // Отрисовка карт текущего игрока
            for (size_t CardiIndex = 0; CardiIndex < Player[playerIndex]->hand.size(); ++CardiIndex) {
                Cardi& Cardi = Player[playerIndex]->hand[CardiIndex];
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
    else if ( !Player[0]->hasWon || (Player[0]->hasWon && Player[1]->hasWon))
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
    else if (!Player[1]->hasWon)
    {
        // Отрисовка карт для каждого игрока
        for (int playerIndex = 0; playerIndex < Player.size(); ++playerIndex) {
            int id = 1; // Переменная для определения позиции карты в ряду
            // Если игрок выиграл, отображаем сообщение о победе
            if (Player[playerIndex]->hasWon) {
                // Определение позиции для сообщения в зависимости от индекса игрока
                sf::Vector2f messagePosition;
                if (playerIndex == 1) { // Игрок снизу
                    messagePosition = sf::Vector2f(900.f, 800.f);
                } else if (playerIndex == 2) { // Игрок справа
                    messagePosition = sf::Vector2f(1720.f, 540.f);
                } else if (playerIndex == 3) { // Игрок сверху
                    messagePosition = sf::Vector2f(900.f, 150.f);
                } else if (playerIndex == 0) { // Игрок слева
                    messagePosition = sf::Vector2f(100.f, 540.f);
                }
                messege_winplayer.setPosition(messagePosition);
                windowss.draw(messege_winplayer);
                continue; // Пропускаем отрисовку карт для выигравшего игрока
            }
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
            // Отрисовка карт текущего игрока
            for (size_t CardiIndex = 0; CardiIndex < Player[playerIndex]->hand.size(); ++CardiIndex) {
                Cardi& Cardi = Player[playerIndex]->hand[CardiIndex];
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
/// @brief Функция для отрисовки игры 3 vs 1 (3 - человека, 1 - бот)
/// @param Player вектор игроков
/// @param selectedCardiIndex текущая выбранная карта
/// @param selected_Cardi вектор выбранных карт
/// @param selectedCardiIndexRight текущая выбранная карта у соседа
/// @param neighborIndex индекс игрока - соседа
/// @param windowss игровое окно
/// @param messege_winplayer текст на месте карт победителя
/// @param back_Cardi_sprite оборот карты (рубашка)
void draw_for3(std::vector<std::unique_ptr<Player_>>& Player, int& selectedCardiIndex, std::vector<int>& selected_Cardi, int& selectedCardiIndexRight, int& neighborIndex, sf::RenderWindow& windowss, sf::Text& messege_winplayer, RectangleShape& back_Cardi_sprite)
{
    if ((dynamic_cast<HumanPlayer*>(Player[0].get()) != nullptr && Player[0]->isAttacker && (!Player[0]->hasWon)))
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
    else if ((dynamic_cast<HumanPlayer*>(Player[1].get()) != nullptr && Player[1]->isAttacker ) && (!Player[1]->hasWon))
    {
        // Отрисовка карт для каждого игрока
        for (int playerIndex = 0; playerIndex < Player.size(); ++playerIndex) {
            int id = 1; // Переменная для определения позиции карты в ряду
            // Если игрок выиграл, отображаем сообщение о победе
            if (Player[playerIndex]->hasWon) {
                // Определение позиции для сообщения в зависимости от индекса игрока
                sf::Vector2f messagePosition;
                if (playerIndex == 1) { // Игрок снизу
                    messagePosition = sf::Vector2f(900.f, 800.f);
                } else if (playerIndex == 2) { // Игрок справа
                    messagePosition = sf::Vector2f(1720.f, 540.f);
                } else if (playerIndex == 3) { // Игрок сверху
                    messagePosition = sf::Vector2f(900.f, 150.f);
                } else if (playerIndex == 0) { // Игрок слева
                    messagePosition = sf::Vector2f(100.f, 540.f);
                }
                messege_winplayer.setPosition(messagePosition);
                windowss.draw(messege_winplayer);
                continue; // Пропускаем отрисовку карт для выигравшего игрока
            }
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
            // Отрисовка карт текущего игрока
            for (size_t CardiIndex = 0; CardiIndex < Player[playerIndex]->hand.size(); ++CardiIndex) {
                Cardi& Cardi = Player[playerIndex]->hand[CardiIndex];
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
    else if ((dynamic_cast<HumanPlayer*>(Player[2].get()) != nullptr && Player[2]->isAttacker ) && (!Player[2]->hasWon))
    {
        // Отрисовка карт для каждого игрока
        for (int playerIndex = 0; playerIndex < Player.size(); ++playerIndex) {
            int id = 1; // Переменная для определения позиции карты в ряду
            // Если игрок выиграл, отображаем сообщение о победе
            if (Player[playerIndex]->hasWon) {
                // Определение позиции для сообщения в зависимости от индекса игрока
                sf::Vector2f messagePosition;
                if (playerIndex == 2) { // Игрок снизу
                    messagePosition = sf::Vector2f(900.f, 800.f);
                } else if (playerIndex == 3) { // Игрок справа
                    messagePosition = sf::Vector2f(1720.f, 540.f);
                } else if (playerIndex == 0) { // Игрок сверху
                    messagePosition = sf::Vector2f(900.f, 150.f);
                } else if (playerIndex == 1) { // Игрок слева
                    messagePosition = sf::Vector2f(100.f, 540.f);
                }
                messege_winplayer.setPosition(messagePosition);
                windowss.draw(messege_winplayer);
                continue; // Пропускаем отрисовку карт для выигравшего игрока
            }
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
            // Отрисовка карт текущего игрока
            for (size_t CardiIndex = 0; CardiIndex < Player[playerIndex]->hand.size(); ++CardiIndex) {
                Cardi& Cardi = Player[playerIndex]->hand[CardiIndex];
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
    else if (!Player[0]->hasWon || (Player[0]->hasWon && Player[1]->hasWon && Player[2]->hasWon))
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
       
    else if ((!Player[1]->hasWon))
    {   
        // Отрисовка карт для каждого игрока
        for (int playerIndex = 0; playerIndex < Player.size(); ++playerIndex) {
            int id = 1; // Переменная для определения позиции карты в ряду
            // Если игрок выиграл, отображаем сообщение о победе
            if (Player[playerIndex]->hasWon) {
                // Определение позиции для сообщения в зависимости от индекса игрока
                sf::Vector2f messagePosition;
                if (playerIndex == 1) { // Игрок снизу
                    messagePosition = sf::Vector2f(900.f, 800.f);
                } else if (playerIndex == 2) { // Игрок справа
                    messagePosition = sf::Vector2f(1720.f, 540.f);
                } else if (playerIndex == 3) { // Игрок сверху
                    messagePosition = sf::Vector2f(900.f, 150.f);
                } else if (playerIndex == 0) { // Игрок слева
                    messagePosition = sf::Vector2f(100.f, 540.f);
                }
                messege_winplayer.setPosition(messagePosition);
                windowss.draw(messege_winplayer);
                continue; // Пропускаем отрисовку карт для выигравшего игрока
            }
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
            // Отрисовка карт текущего игрока
            for (size_t CardiIndex = 0; CardiIndex < Player[playerIndex]->hand.size(); ++CardiIndex) {
                Cardi& Cardi = Player[playerIndex]->hand[CardiIndex];
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
    else if ((!Player[2]->hasWon))
    {   
        // Отрисовка карт для каждого игрока
        for (int playerIndex = 0; playerIndex < Player.size(); ++playerIndex) {
            int id = 1; // Переменная для определения позиции карты в ряду
            // Если игрок выиграл, отображаем сообщение о победе
            if (Player[playerIndex]->hasWon) {
                // Определение позиции для сообщения в зависимости от индекса игрока
                sf::Vector2f messagePosition;
                if (playerIndex == 2) { // Игрок снизу
                    messagePosition = sf::Vector2f(900.f, 800.f);
                } else if (playerIndex == 3) { // Игрок справа
                    messagePosition = sf::Vector2f(1720.f, 540.f);
                } else if (playerIndex == 0) { // Игрок сверху
                    messagePosition = sf::Vector2f(900.f, 150.f);
                } else if (playerIndex == 1) { // Игрок слева
                    messagePosition = sf::Vector2f(100.f, 540.f);
                }
                messege_winplayer.setPosition(messagePosition);
                windowss.draw(messege_winplayer);
                continue; // Пропускаем отрисовку карт для выигравшего игрока
            }
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
            // Отрисовка карт текущего игрока
            for (size_t CardiIndex = 0; CardiIndex < Player[playerIndex]->hand.size(); ++CardiIndex) {
                Cardi& Cardi = Player[playerIndex]->hand[CardiIndex];
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
/// @brief Функция для отрисовки игры 4 vs 0 (4 - человека, 0 - ботов)
/// @param Player вектор игроков
/// @param selectedCardiIndex текущая выбранная карта
/// @param selected_Cardi вектор выбранных карт
/// @param selectedCardiIndexRight текущая выбранная карта у соседа
/// @param neighborIndex индекс игрока - соседа
/// @param windowss игровое окно
/// @param messege_winplayer текст на месте карт победителя
/// @param back_Cardi_sprite оборот карты (рубашка)
void draw_for4(std::vector<std::unique_ptr<Player_>>& Player, int& selectedCardiIndex, std::vector<int>& selected_Cardi, int& selectedCardiIndexRight, int& neighborIndex, sf::RenderWindow& windowss, sf::Text& messege_winplayer, RectangleShape& back_Cardi_sprite)
{
    if ((dynamic_cast<HumanPlayer*>(Player[0].get()) != nullptr && Player[0]->isAttacker)&& (!Player[0]->hasWon))
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
    else if ((dynamic_cast<HumanPlayer*>(Player[1].get()) != nullptr && Player[1]->isAttacker )&& (!Player[1]->hasWon))
    {
        // Отрисовка карт для каждого игрока
        for (int playerIndex = 0; playerIndex < Player.size(); ++playerIndex) {
            int id = 1; // Переменная для определения позиции карты в ряду
            // Если игрок выиграл, отображаем сообщение о победе
            if (Player[playerIndex]->hasWon) {
                // Определение позиции для сообщения в зависимости от индекса игрока
                sf::Vector2f messagePosition;
                if (playerIndex == 1) { // Игрок снизу
                    messagePosition = sf::Vector2f(900.f, 800.f);
                } else if (playerIndex == 2) { // Игрок справа
                    messagePosition = sf::Vector2f(1720.f, 540.f);
                } else if (playerIndex == 3) { // Игрок сверху
                    messagePosition = sf::Vector2f(900.f, 150.f);
                } else if (playerIndex == 0) { // Игрок слева
                    messagePosition = sf::Vector2f(100.f, 540.f);
                }
                messege_winplayer.setPosition(messagePosition);
                windowss.draw(messege_winplayer);
                continue; // Пропускаем отрисовку карт для выигравшего игрока
            }
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
            // Отрисовка карт текущего игрока
            for (size_t CardiIndex = 0; CardiIndex < Player[playerIndex]->hand.size(); ++CardiIndex) {
                Cardi& Cardi = Player[playerIndex]->hand[CardiIndex];
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
                
    else if ((dynamic_cast<HumanPlayer*>(Player[2].get()) != nullptr && Player[2]->isAttacker )&& (!Player[2]->hasWon))
    {
        // Отрисовка карт для каждого игрока
        for (int playerIndex = 0; playerIndex < Player.size(); ++playerIndex) {
            int id = 1; // Переменная для определения позиции карты в ряду
            // Если игрок выиграл, отображаем сообщение о победе
            if (Player[playerIndex]->hasWon) {
                // Определение позиции для сообщения в зависимости от индекса игрока
                sf::Vector2f messagePosition;
                if (playerIndex == 2) { // Игрок снизу
                    messagePosition = sf::Vector2f(900.f, 800.f);
                } else if (playerIndex == 3) { // Игрок справа
                    messagePosition = sf::Vector2f(1720.f, 540.f);
                } else if (playerIndex == 0) { // Игрок сверху
                    messagePosition = sf::Vector2f(900.f, 150.f);
                } else if (playerIndex == 1) { // Игрок слева
                    messagePosition = sf::Vector2f(100.f, 540.f);
                }
                messege_winplayer.setPosition(messagePosition);
                windowss.draw(messege_winplayer);
                continue; // Пропускаем отрисовку карт для выигравшего игрока
            }
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
            // Отрисовка карт текущего игрока
            for (size_t CardiIndex = 0; CardiIndex < Player[playerIndex]->hand.size(); ++CardiIndex) {
                Cardi& Cardi = Player[playerIndex]->hand[CardiIndex];
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
                
    else if ((dynamic_cast<HumanPlayer*>(Player[3].get()) != nullptr && Player[3]->isAttacker )&& (!Player[3]->hasWon))
    {
        // Отрисовка карт для каждого игрока
        for (int playerIndex = 0; playerIndex < Player.size(); ++playerIndex) {
            int id = 1; // Переменная для определения позиции карты в ряду
            // Если игрок выиграл, отображаем сообщение о победе
            if (Player[playerIndex]->hasWon) {
                // Определение позиции для сообщения в зависимости от индекса игрока
                sf::Vector2f messagePosition;
                if (playerIndex == 3) { // Игрок снизу
                    messagePosition = sf::Vector2f(900.f, 800.f);
                } else if (playerIndex == 0) { // Игрок справа
                    messagePosition = sf::Vector2f(1720.f, 540.f);
                } else if (playerIndex == 1) { // Игрок сверху
                    messagePosition = sf::Vector2f(900.f, 150.f);
                } else if (playerIndex == 2) { // Игрок слева
                    messagePosition = sf::Vector2f(100.f, 540.f);
                }
                messege_winplayer.setPosition(messagePosition);
                windowss.draw(messege_winplayer);
                continue; // Пропускаем отрисовку карт для выигравшего игрока
            }
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
            // Отрисовка карт текущего игрока
            for (size_t CardiIndex = 0; CardiIndex < Player[playerIndex]->hand.size(); ++CardiIndex) {
                Cardi& Cardi = Player[playerIndex]->hand[CardiIndex];
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