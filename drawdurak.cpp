#include <SFML/Graphics.hpp>
#include <vector>
#include "durak.h"

void drawFor1People(std::vector<Playerr>& players,int& CurrentPlayerrIndex, int& selectedCardiIndex, int& neighborIndex, sf::RenderWindow& windowss, sf::Text& messageWinner, sf::RectangleShape& back_Cardi_sprite, bool attacking, bool defending, sf::Font& font_1) {
 for (int playerIndex = 0; playerIndex < players.size(); ++playerIndex) {
        int id = 1; // Переменная для определения позиции карты в ряду
        // Отрисовка карт текущего игрока
        for (size_t CardiIndex = 0; CardiIndex < players[playerIndex].hand.size(); ++CardiIndex) {
            Cardd& Cardi = players[playerIndex].hand[CardiIndex];        
            // Если игрок выиграл, отображаем сообщение о победе
            if (players[playerIndex].hasWon) {
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
                messageWinner.setPosition(messagePosition);
                windowss.draw(messageWinner);
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
                if (playerIndex == 0) {
                    // Атакующий игрок видит значения своих карт
                    Cardi.sprite.setTexture(Cardi.texture);
                    // Определение позиции карты в зависимости от её индекса 
                    Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (CardiIndex == selectedCardiIndex) {
                        Cardi.sprite.setColor(sf::Color::Yellow);
                    } else {
                        Cardi.sprite.setColor(sf::Color::White);
                    }
                    windowss.draw(Cardi.sprite);
                    ++id; 
                } else {
                    // У остальных игроков карты рубашкой вверх
                    back_Cardi_sprite.setRotation(rotation);
                    if (playerIndex == 1) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                    } else if (playerIndex == 2) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                    } else if (playerIndex == 3) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                    }
                    back_Cardi_sprite.setFillColor(sf::Color::White);
                    windowss.draw(back_Cardi_sprite);
                    id++;
                }
            }
        }       
}
void drawFor2People(std::vector<Playerr>& players, int& CurrentPlayerrIndex,int& selectedCardiIndex, int& neighborIndex, sf::RenderWindow& windowss, sf::Text& messageWinner, sf::RectangleShape& back_Cardi_sprite, bool attacking, bool defending, sf::Font& font_1) {

    if ((players[0].isAttacker) && (attacking || defending) && !players[0].isBot && !players[0].hasWon) { 
            for (int playerIndex = 0; playerIndex < players.size(); ++playerIndex) {
        int id = 1; // Переменная для определения позиции карты в ряду
        // Отрисовка карт текущего игрока
        for (size_t CardiIndex = 0; CardiIndex < players[playerIndex].hand.size(); ++CardiIndex) {
            Cardd& Cardi = players[playerIndex].hand[CardiIndex]; 
            // Если игрок выиграл, отображаем сообщение о победе
            if (players[playerIndex].hasWon) {
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
                messageWinner.setPosition(messagePosition);
                windowss.draw(messageWinner);
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
                if (playerIndex == 0) {
                    // Атакующий игрок видит значения своих карт
                    Cardi.sprite.setTexture(Cardi.texture);
                    // Определение позиции карты в зависимости от её индекса 
                    Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (CardiIndex == selectedCardiIndex) {
                        Cardi.sprite.setColor(sf::Color::Yellow);
                    } else {
                        Cardi.sprite.setColor(sf::Color::White);
                    }
                    windowss.draw(Cardi.sprite);
                    ++id; 
                } else {
                    // У остальных игроков карты рубашкой вверх
                    back_Cardi_sprite.setRotation(rotation);
                    if (playerIndex == 1) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                    } else if (playerIndex == 2) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                    } else if (playerIndex == 3) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                    }
                    back_Cardi_sprite.setFillColor(sf::Color::White);
                    windowss.draw(back_Cardi_sprite);
                    id++;
                }
            }
            }
        }
    else if ((players[1].isAttacker) && (attacking || defending) && !players[1].isBot && !players[1].hasWon) {
            for (int playerIndex = 0; playerIndex < players.size(); ++playerIndex) {
        int id = 1; // Переменная для определения позиции карты в ряду
        // Отрисовка карт текущего игрока
        for (size_t CardiIndex = 0; CardiIndex < players[playerIndex].hand.size(); ++CardiIndex) {
            Cardd& Cardi = players[playerIndex].hand[CardiIndex];
                // Если игрок выиграл, отображаем сообщение о победе
                if (players[playerIndex].hasWon) {
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
                    messageWinner.setPosition(messagePosition);
                    windowss.draw(messageWinner);
                    continue; // Пропускаем отрисовку карт для выигравшего игрока
                }
                // Определение позиции и поворота карт в зависимости от индекса игрока
                float rotation;
                if (playerIndex == 1) { // Игрок снизу
                    rotation = 0;
                } else if (playerIndex == 2) { // Игрок 1920 - 50, 1080 / 2
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
                    Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (CardiIndex == selectedCardiIndex) {
                        Cardi.sprite.setColor(sf::Color::Yellow);
                    } else {
                        Cardi.sprite.setColor(sf::Color::White);
                    }
                    windowss.draw(Cardi.sprite);
                    ++id;
                } else {
                    // У остальных игроков карты рубашкой вверх
                    back_Cardi_sprite.setRotation(rotation);
                    if (playerIndex == 2) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                    } else if (playerIndex == 3) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                    } else if (playerIndex == 0) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                    }
                    back_Cardi_sprite.setFillColor(sf::Color::White);
                    windowss.draw(back_Cardi_sprite);
                    id++;
                }
            }
        }
    }
    else if ((players[2].isDefender || (players[3].isDefender)) && (attacking || defending)) {
            for (int playerIndex = 0; playerIndex < players.size(); ++playerIndex) {
        int id = 1; // Переменная для определения позиции карты в ряду
        // Отрисовка карт текущего игрока
        for (size_t CardiIndex = 0; CardiIndex < players[playerIndex].hand.size(); ++CardiIndex) {
            Cardd& Cardi = players[playerIndex].hand[CardiIndex];
                if(!players[0].hasWon)
                {  
                    // Если игрок выиграл, отображаем сообщение о победе
                    if (players[playerIndex].hasWon) {
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
                        messageWinner.setPosition(messagePosition);
                        windowss.draw(messageWinner);
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
                        if (playerIndex == 0) {
                            // Атакующий игрок видит значения своих карт
                            Cardi.sprite.setTexture(Cardi.texture);
                            // Определение позиции карты в зависимости от её индекса 
                            Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                            // Если карта выбрана, она рисуется в желтый цвет
                            if (CardiIndex == selectedCardiIndex) {
                                Cardi.sprite.setColor(sf::Color::Yellow);
                            } else {
                                Cardi.sprite.setColor(sf::Color::White);
                            }
                            windowss.draw(Cardi.sprite);
                            ++id; 
                        } else {
                            // У остальных игроков карты рубашкой вверх
                            back_Cardi_sprite.setRotation(rotation);
                            if (playerIndex == 1) {
                                back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                            } else if (playerIndex == 2) {
                                back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                            } else if (playerIndex == 3) {
                                back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                            }
                            back_Cardi_sprite.setFillColor(sf::Color::White);
                            windowss.draw(back_Cardi_sprite);
                            id++;
                        }
                }
                else if(!players[1].hasWon)
                {
                    // Если игрок выиграл, отображаем сообщение о победе
                    if (players[playerIndex].hasWon) {
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
                        messageWinner.setPosition(messagePosition);
                        windowss.draw(messageWinner);
                        continue; // Пропускаем отрисовку карт для выигравшего игрока
                    }
                    // Определение позиции и поворота карт в зависимости от индекса игрока
                    float rotation;
                    if (playerIndex == 1) { // Игрок снизу
                        rotation = 0;
                    } else if (playerIndex == 2) { // Игрок 1920 - 50, 1080 / 2
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
                        Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (CardiIndex == selectedCardiIndex) {
                            Cardi.sprite.setColor(sf::Color::Yellow);
                        } else {
                            Cardi.sprite.setColor(sf::Color::White);
                        }
                        windowss.draw(Cardi.sprite);
                        ++id;
                    } else {
                        // У остальных игроков карты рубашкой вверх
                        back_Cardi_sprite.setRotation(rotation);
                        if (playerIndex == 2) {
                            back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                        } else if (playerIndex == 3) {
                            back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                        } else if (playerIndex == 0) {
                            back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                        }
                        back_Cardi_sprite.setFillColor(sf::Color::White);
                        windowss.draw(back_Cardi_sprite);
                        id++;
                    }

                }
            }
        }
    }
    else if ((players[0].isDefender) && (attacking || defending) && !players[0].isBot && !players[0].hasWon) {
            for (int playerIndex = 0; playerIndex < players.size(); ++playerIndex) {
        int id = 1; // Переменная для определения позиции карты в ряду
        // Отрисовка карт текущего игрока
        for (size_t CardiIndex = 0; CardiIndex < players[playerIndex].hand.size(); ++CardiIndex) {
            Cardd& Cardi = players[playerIndex].hand[CardiIndex];
                // Если игрок выиграл, отображаем сообщение о победе
                if (players[playerIndex].hasWon) {
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
                    messageWinner.setPosition(messagePosition);
                    windowss.draw(messageWinner);
                    continue; // Пропускаем отрисовку карт для выигравшего игрока
                }
                // Определение позиции и поворота карт в зависимости от индекса игрока
                float rotation;
                if (playerIndex == 0) { // Игрок снизу
                    rotation = 0;
                } else if (playerIndex == 1) { // Игрок 1920 - 50, 1080 / 2
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
                    Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (CardiIndex == selectedCardiIndex) {
                        Cardi.sprite.setColor(sf::Color::Yellow);
                    } else {
                        Cardi.sprite.setColor(sf::Color::White);
                    }
                    windowss.draw(Cardi.sprite);
                    ++id; 
                } else {
                    // У остальных игроков карты рубашкой вверх
                    back_Cardi_sprite.setRotation(rotation);
                    if (playerIndex == 1) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                    } else if (playerIndex == 2) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                    } else if (playerIndex == 3) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                    }
                    back_Cardi_sprite.setFillColor(sf::Color::White);
                    windowss.draw(back_Cardi_sprite);
                    id++;
                }
            }
        }
    }
    else if ((players[1].isDefender) && (attacking || defending) && !players[1].isBot && !players[1].hasWon) {
                    for (int playerIndex = 0; playerIndex < players.size(); ++playerIndex) {
        int id = 1; // Переменная для определения позиции карты в ряду
        // Отрисовка карт текущего игрока
        for (size_t CardiIndex = 0; CardiIndex < players[playerIndex].hand.size(); ++CardiIndex) {
            Cardd& Cardi = players[playerIndex].hand[CardiIndex];
                
                // Если игрок выиграл, отображаем сообщение о победе
                if (players[playerIndex].hasWon) {
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
                    messageWinner.setPosition(messagePosition);
                    windowss.draw(messageWinner);
                    continue; // Пропускаем отрисовку карт для выигравшего игрока
                }
                // Определение позиции и поворота карт в зависимости от индекса игрока
                float rotation;
                if (playerIndex == 1) { // Игрок снизу
                    rotation = 0;
                } else if (playerIndex == 2) { // Игрок 1920 - 50, 1080 / 2
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
                    Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (CardiIndex == selectedCardiIndex) {
                        Cardi.sprite.setColor(sf::Color::Yellow);
                    } else {
                        Cardi.sprite.setColor(sf::Color::White);
                    }
                    windowss.draw(Cardi.sprite);
                    ++id;
                } else {
                    // У остальных игроков карты рубашкой вверх
                    back_Cardi_sprite.setRotation(rotation);
                    if (playerIndex == 2) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                    } else if (playerIndex == 3) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                    } else if (playerIndex == 0) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                    }
                    back_Cardi_sprite.setFillColor(sf::Color::White);
                    windowss.draw(back_Cardi_sprite);
                    id++;
                }
            }
        }
    }
    else if ((players[2].isAttacker || (players[3].isAttacker)) && (attacking || defending)) {
                    for (int playerIndex = 0; playerIndex < players.size(); ++playerIndex) {
        int id = 1; // Переменная для определения позиции карты в ряду
        // Отрисовка карт текущего игрока
        for (size_t CardiIndex = 0; CardiIndex < players[playerIndex].hand.size(); ++CardiIndex) {
            Cardd& Cardi = players[playerIndex].hand[CardiIndex];
                if(!players[0].hasWon)
                {
                    // Если игрок выиграл, отображаем сообщение о победе
                    if (players[playerIndex].hasWon) {
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
                        messageWinner.setPosition(messagePosition);
                        windowss.draw(messageWinner);
                        continue; // Пропускаем отрисовку карт для выигравшего игрока
                    }
                    // Определение позиции и поворота карт в зависимости от индекса игрока
                    float rotation;
                    if (playerIndex == 0) { // Игрок снизу
                        rotation = 0;
                    } else if (playerIndex == 1) { // Игрок 1920 - 50, 1080 / 2
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
                        Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (CardiIndex == selectedCardiIndex) {
                            Cardi.sprite.setColor(sf::Color::Yellow);
                        } else {
                            Cardi.sprite.setColor(sf::Color::White);
                        }
                        windowss.draw(Cardi.sprite);
                        ++id; 
                    } else {
                        // У остальных игроков карты рубашкой вверх
                        back_Cardi_sprite.setRotation(rotation);
                        if (playerIndex == 1) {
                            back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                        } else if (playerIndex == 2) {
                            back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                        } else if (playerIndex == 3) {
                            back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                        }
                        back_Cardi_sprite.setFillColor(sf::Color::White);
                        windowss.draw(back_Cardi_sprite);
                        id++;
                    }
                }
                else if(!players[1].hasWon)
                {
                    // Если игрок выиграл, отображаем сообщение о победе
                    if (players[playerIndex].hasWon) {
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
                        messageWinner.setPosition(messagePosition);
                        windowss.draw(messageWinner);
                        continue; // Пропускаем отрисовку карт для выигравшего игрока
                    }
                    // Определение позиции и поворота карт в зависимости от индекса игрока
                    float rotation;
                    if (playerIndex == 1) { // Игрок снизу
                        rotation = 0;
                    } else if (playerIndex == 2) { // Игрок 1920 - 50, 1080 / 2
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
                        Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (CardiIndex == selectedCardiIndex) {
                            Cardi.sprite.setColor(sf::Color::Yellow);
                        } else {
                            Cardi.sprite.setColor(sf::Color::White);
                        }
                        windowss.draw(Cardi.sprite);
                        ++id;
                    } else {
                        // У остальных игроков карты рубашкой вверх
                        back_Cardi_sprite.setRotation(rotation);
                        if (playerIndex == 2) {
                            back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                        } else if (playerIndex == 3) {
                            back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                        } else if (playerIndex == 0) {
                            back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                        }
                        back_Cardi_sprite.setFillColor(sf::Color::White);
                        windowss.draw(back_Cardi_sprite);
                        id++;
                    }
                }
            }
        }
    }  
}
void drawFor3People(std::vector<Playerr>& players, int& CurrentPlayerrIndex,int& selectedCardiIndex, int& neighborIndex, sf::RenderWindow& windowss, sf::Text& messageWinner, sf::RectangleShape& back_Cardi_sprite, bool attacking, bool defending, sf::Font& font_1) {
    for (int playerIndex = 0; playerIndex < players.size(); ++playerIndex) {
        int id = 1; // Переменная для определения позиции карты в ряду
        // Отрисовка карт текущего игрока
        for (size_t CardiIndex = 0; CardiIndex < players[playerIndex].hand.size(); ++CardiIndex) {
            Cardd& Cardi = players[playerIndex].hand[CardiIndex];
            if ((players[0].isAttacker) && (attacking == true) && !players[0].isBot && !players[0].hasWon) {  
            // Если игрок выиграл, отображаем сообщение о победе
            if (players[playerIndex].hasWon) {
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
                messageWinner.setPosition(messagePosition);
                windowss.draw(messageWinner);
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
                if (playerIndex == 0) {
                    // Атакующий игрок видит значения своих карт
                    Cardi.sprite.setTexture(Cardi.texture);
                    // Определение позиции карты в зависимости от её индекса 
                    Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (CardiIndex == selectedCardiIndex) {
                        Cardi.sprite.setColor(sf::Color::Yellow);
                    } else {
                        Cardi.sprite.setColor(sf::Color::White);
                    }
                    windowss.draw(Cardi.sprite);
                    ++id; 
                } else {
                    // У остальных игроков карты рубашкой вверх
                    back_Cardi_sprite.setRotation(rotation);
                    if (playerIndex == 1) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                    } else if (playerIndex == 2) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                    } else if (playerIndex == 3) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                    }
                    back_Cardi_sprite.setFillColor(sf::Color::White);
                    windowss.draw(back_Cardi_sprite);
                    id++;
                }
            }
            else if ((players[1].isAttacker) && (attacking || defending) && !players[1].isBot && !players[1].hasWon) {
                // Если игрок выиграл, отображаем сообщение о победе
                if (players[playerIndex].hasWon) {
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
                    messageWinner.setPosition(messagePosition);
                    windowss.draw(messageWinner);
                    continue; // Пропускаем отрисовку карт для выигравшего игрока
                }
                // Определение позиции и поворота карт в зависимости от индекса игрока
                float rotation;
                if (playerIndex == 1) { // Игрок снизу
                    rotation = 0;
                } else if (playerIndex == 2) { // Игрок 1920 - 50, 1080 / 2
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
                    Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (CardiIndex == selectedCardiIndex) {
                        Cardi.sprite.setColor(sf::Color::Yellow);
                    } else {
                        Cardi.sprite.setColor(sf::Color::White);
                    }
                    windowss.draw(Cardi.sprite);
                    ++id;
                } else {
                    // У остальных игроков карты рубашкой вверх
                    back_Cardi_sprite.setRotation(rotation);
                    if (playerIndex == 2) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                    } else if (playerIndex == 3) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                    } else if (playerIndex == 0) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                    }
                    back_Cardi_sprite.setFillColor(sf::Color::White);
                    windowss.draw(back_Cardi_sprite);
                    id++;
                }
            }
            else if ((players[2].isAttacker) && (attacking || defending) && !players[2].isBot && !players[2].hasWon) {
                // Если игрок выиграл, отображаем сообщение о победе
                if (players[playerIndex].hasWon) {
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
                    messageWinner.setPosition(messagePosition);
                    windowss.draw(messageWinner);
                    continue; // Пропускаем отрисовку карт для выигравшего игрока
                }
                // Определение позиции и поворота карт в зависимости от индекса игрока
                float rotation;
                if (playerIndex == 2) { // Игрок снизу
                    rotation = 0;
                } else if (playerIndex == 3) { // Игрок 1920 - 50, 1080 / 2
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
                    Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (CardiIndex == selectedCardiIndex) {
                        Cardi.sprite.setColor(sf::Color::Yellow);
                    } else {
                        Cardi.sprite.setColor(sf::Color::White);
                    }
                    windowss.draw(Cardi.sprite);
                    ++id;
                } else {
                    // У остальных игроков карты рубашкой вверх
                    back_Cardi_sprite.setRotation(rotation);
                    if (playerIndex == 3) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                    } else if (playerIndex == 0) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                    } else if (playerIndex == 1) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                    }
                    back_Cardi_sprite.setFillColor(sf::Color::White);
                    windowss.draw(back_Cardi_sprite);
                    id++;
                }
            }
            else if ((players[3].isDefender) && (attacking || defending) && players[3].isBot) {
                if(!players[0].hasWon)
                {  
                    // Если игрок выиграл, отображаем сообщение о победе
                    if (players[playerIndex].hasWon) {
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
                        messageWinner.setPosition(messagePosition);
                        windowss.draw(messageWinner);
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
                        if (playerIndex == 0) {
                            // Атакующий игрок видит значения своих карт
                            Cardi.sprite.setTexture(Cardi.texture);
                            // Определение позиции карты в зависимости от её индекса 
                            Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                            // Если карта выбрана, она рисуется в желтый цвет
                            if (CardiIndex == selectedCardiIndex) {
                                Cardi.sprite.setColor(sf::Color::Yellow);
                            } else {
                                Cardi.sprite.setColor(sf::Color::White);
                            }
                            windowss.draw(Cardi.sprite);
                            ++id; 
                        } else {
                            // У остальных игроков карты рубашкой вверх
                            back_Cardi_sprite.setRotation(rotation);
                            if (playerIndex == 1) {
                                back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                            } else if (playerIndex == 2) {
                                back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                            } else if (playerIndex == 3) {
                                back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                            }
                            back_Cardi_sprite.setFillColor(sf::Color::White);
                            windowss.draw(back_Cardi_sprite);
                            id++;
                        }
                }
                else if(!players[1].hasWon)
                {
                    // Если игрок выиграл, отображаем сообщение о победе
                    if (players[playerIndex].hasWon) {
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
                        messageWinner.setPosition(messagePosition);
                        windowss.draw(messageWinner);
                        continue; // Пропускаем отрисовку карт для выигравшего игрока
                    }
                    // Определение позиции и поворота карт в зависимости от индекса игрока
                    float rotation;
                    if (playerIndex == 1) { // Игрок снизу
                        rotation = 0;
                    } else if (playerIndex == 2) { // Игрок 1920 - 50, 1080 / 2
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
                        Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (CardiIndex == selectedCardiIndex) {
                            Cardi.sprite.setColor(sf::Color::Yellow);
                        } else {
                            Cardi.sprite.setColor(sf::Color::White);
                        }
                        windowss.draw(Cardi.sprite);
                        ++id;
                    } else {
                        // У остальных игроков карты рубашкой вверх
                        back_Cardi_sprite.setRotation(rotation);
                        if (playerIndex == 2) {
                            back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                        } else if (playerIndex == 3) {
                            back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                        } else if (playerIndex == 0) {
                            back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                        }
                        back_Cardi_sprite.setFillColor(sf::Color::White);
                        windowss.draw(back_Cardi_sprite);
                        id++;
                    }

                }
                else if(!players[2].hasWon)
                {
                    // Если игрок выиграл, отображаем сообщение о победе
                    if (players[playerIndex].hasWon) {
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
                        messageWinner.setPosition(messagePosition);
                        windowss.draw(messageWinner);
                        continue; // Пропускаем отрисовку карт для выигравшего игрока
                    }
                    // Определение позиции и поворота карт в зависимости от индекса игрока
                    float rotation;
                    if (playerIndex == 2) { // Игрок снизу
                        rotation = 0;
                    } else if (playerIndex == 3) { // Игрок 1920 - 50, 1080 / 2
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
                        Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (CardiIndex == selectedCardiIndex) {
                            Cardi.sprite.setColor(sf::Color::Yellow);
                        } else {
                            Cardi.sprite.setColor(sf::Color::White);
                        }
                        windowss.draw(Cardi.sprite);
                        ++id;
                    } else {
                        // У остальных игроков карты рубашкой вверх
                        back_Cardi_sprite.setRotation(rotation);
                        if (playerIndex == 3) {
                            back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                        } else if (playerIndex == 0) {
                            back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                        } else if (playerIndex == 1) {
                            back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                        }
                        back_Cardi_sprite.setFillColor(sf::Color::White);
                        windowss.draw(back_Cardi_sprite);
                        id++;
                    }

                }
            }
            else if ((players[0].isDefender) && (attacking || defending) && !players[0].isBot && !players[0].hasWon) {
                // Если игрок выиграл, отображаем сообщение о победе
                if (players[playerIndex].hasWon) {
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
                    messageWinner.setPosition(messagePosition);
                    windowss.draw(messageWinner);
                    continue; // Пропускаем отрисовку карт для выигравшего игрока
                }
                // Определение позиции и поворота карт в зависимости от индекса игрока
                float rotation;
                if (playerIndex == 0) { // Игрок снизу
                    rotation = 0;
                } else if (playerIndex == 1) { // Игрок 1920 - 50, 1080 / 2
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
                    Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (CardiIndex == selectedCardiIndex) {
                        Cardi.sprite.setColor(sf::Color::Yellow);
                    } else {
                        Cardi.sprite.setColor(sf::Color::White);
                    }
                    windowss.draw(Cardi.sprite);
                    ++id; 
                } else {
                    // У остальных игроков карты рубашкой вверх
                    back_Cardi_sprite.setRotation(rotation);
                    if (playerIndex == 1) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                    } else if (playerIndex == 2) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                    } else if (playerIndex == 3) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                    }
                    back_Cardi_sprite.setFillColor(sf::Color::White);
                    windowss.draw(back_Cardi_sprite);
                    id++;
                }
            }
            else if ((players[1].isDefender) && (attacking || defending) && !players[1].isBot && !players[1].hasWon) {
                
                // Если игрок выиграл, отображаем сообщение о победе
                if (players[playerIndex].hasWon) {
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
                    messageWinner.setPosition(messagePosition);
                    windowss.draw(messageWinner);
                    continue; // Пропускаем отрисовку карт для выигравшего игрока
                }
                // Определение позиции и поворота карт в зависимости от индекса игрока
                float rotation;
                if (playerIndex == 1) { // Игрок снизу
                    rotation = 0;
                } else if (playerIndex == 2) { // Игрок 1920 - 50, 1080 / 2
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
                    Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (CardiIndex == selectedCardiIndex) {
                        Cardi.sprite.setColor(sf::Color::Yellow);
                    } else {
                        Cardi.sprite.setColor(sf::Color::White);
                    }
                    windowss.draw(Cardi.sprite);
                    ++id;
                } else {
                    // У остальных игроков карты рубашкой вверх
                    back_Cardi_sprite.setRotation(rotation);
                    if (playerIndex == 2) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                    } else if (playerIndex == 3) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                    } else if (playerIndex == 0) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                    }
                    back_Cardi_sprite.setFillColor(sf::Color::White);
                    windowss.draw(back_Cardi_sprite);
                    id++;
                }
            }
            else if ((players[2].isDefender) && (attacking || defending) && !players[2].isBot && !players[2].hasWon) {
                
                // Если игрок выиграл, отображаем сообщение о победе
                if (players[playerIndex].hasWon) {
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
                    messageWinner.setPosition(messagePosition);
                    windowss.draw(messageWinner);
                    continue; // Пропускаем отрисовку карт для выигравшего игрока
                }
                // Определение позиции и поворота карт в зависимости от индекса игрока
                float rotation;
                if (playerIndex == 2) { // Игрок снизу
                    rotation = 0;
                } else if (playerIndex == 3) { // Игрок 1920 - 50, 1080 / 2
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
                    Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (CardiIndex == selectedCardiIndex) {
                        Cardi.sprite.setColor(sf::Color::Yellow);
                    } else {
                        Cardi.sprite.setColor(sf::Color::White);
                    }
                    windowss.draw(Cardi.sprite);
                    ++id;
                } else {
                    // У остальных игроков карты рубашкой вверх
                    back_Cardi_sprite.setRotation(rotation);
                    if (playerIndex == 3) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                    } else if (playerIndex == 0) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                    } else if (playerIndex == 1) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                    }
                    back_Cardi_sprite.setFillColor(sf::Color::White);
                    windowss.draw(back_Cardi_sprite);
                    id++;
                }
            }
            else if ((players[3].isAttacker) && (attacking || defending) && players[3].isBot) {
                if(!players[0].hasWon)
                {
                    // Если игрок выиграл, отображаем сообщение о победе
                    if (players[playerIndex].hasWon) {
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
                        messageWinner.setPosition(messagePosition);
                        windowss.draw(messageWinner);
                        continue; // Пропускаем отрисовку карт для выигравшего игрока
                    }
                    // Определение позиции и поворота карт в зависимости от индекса игрока
                    float rotation;
                    if (playerIndex == 0) { // Игрок снизу
                        rotation = 0;
                    } else if (playerIndex == 1) { // Игрок 1920 - 50, 1080 / 2
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
                        Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (CardiIndex == selectedCardiIndex) {
                            Cardi.sprite.setColor(sf::Color::Yellow);
                        } else {
                            Cardi.sprite.setColor(sf::Color::White);
                        }
                        windowss.draw(Cardi.sprite);
                        ++id; 
                    } else {
                        // У остальных игроков карты рубашкой вверх
                        back_Cardi_sprite.setRotation(rotation);
                        if (playerIndex == 1) {
                            back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                        } else if (playerIndex == 2) {
                            back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                        } else if (playerIndex == 3) {
                            back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                        }
                        back_Cardi_sprite.setFillColor(sf::Color::White);
                        windowss.draw(back_Cardi_sprite);
                        id++;
                    }
                }
                else if(!players[1].hasWon)
                {
                    // Если игрок выиграл, отображаем сообщение о победе
                    if (players[playerIndex].hasWon) {
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
                        messageWinner.setPosition(messagePosition);
                        windowss.draw(messageWinner);
                        continue; // Пропускаем отрисовку карт для выигравшего игрока
                    }
                    // Определение позиции и поворота карт в зависимости от индекса игрока
                    float rotation;
                    if (playerIndex == 1) { // Игрок снизу
                        rotation = 0;
                    } else if (playerIndex == 2) { // Игрок 1920 - 50, 1080 / 2
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
                        Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (CardiIndex == selectedCardiIndex) {
                            Cardi.sprite.setColor(sf::Color::Yellow);
                        } else {
                            Cardi.sprite.setColor(sf::Color::White);
                        }
                        windowss.draw(Cardi.sprite);
                        ++id;
                    } else {
                        // У остальных игроков карты рубашкой вверх
                        back_Cardi_sprite.setRotation(rotation);
                        if (playerIndex == 2) {
                            back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                        } else if (playerIndex == 3) {
                            back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                        } else if (playerIndex == 0) {
                            back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                        }
                        back_Cardi_sprite.setFillColor(sf::Color::White);
                        windowss.draw(back_Cardi_sprite);
                        id++;
                    }
                }
                else if(!players[2].hasWon)
                {
                    // Если игрок выиграл, отображаем сообщение о победе
                    if (players[playerIndex].hasWon) {
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
                        messageWinner.setPosition(messagePosition);
                        windowss.draw(messageWinner);
                        continue; // Пропускаем отрисовку карт для выигравшего игрока
                    }
                    // Определение позиции и поворота карт в зависимости от индекса игрока
                    float rotation;
                    if (playerIndex == 2) { // Игрок снизу
                        rotation = 0;
                    } else if (playerIndex == 3) { // Игрок 1920 - 50, 1080 / 2
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
                        Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                        // Если карта выбрана, она рисуется в желтый цвет
                        if (CardiIndex == selectedCardiIndex) {
                            Cardi.sprite.setColor(sf::Color::Yellow);
                        } else {
                            Cardi.sprite.setColor(sf::Color::White);
                        }
                        windowss.draw(Cardi.sprite);
                        ++id;
                    } else {
                        // У остальных игроков карты рубашкой вверх
                        back_Cardi_sprite.setRotation(rotation);
                        if (playerIndex == 3) {
                            back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                        } else if (playerIndex == 0) {
                            back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                        } else if (playerIndex == 1) {
                            back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                        }
                        back_Cardi_sprite.setFillColor(sf::Color::White);
                        windowss.draw(back_Cardi_sprite);
                        id++;
                    }

                }
            }
        }
    }  
}

void drawFor4People(std::vector<Playerr>& players,int& CurrentPlayerrIndex, int& selectedCardiIndex, int& neighborIndex, sf::RenderWindow& windowss, sf::Text& messageWinner, sf::RectangleShape& back_Cardi_sprite, bool attacking, bool defending, sf::Font& font_1) {
    for (int playerIndex = 0; playerIndex < players.size(); ++playerIndex) {
        int id = 1; // Переменная для определения позиции карты в ряду
        // Отрисовка карт текущего игрока
        for (size_t CardiIndex = 0; CardiIndex < players[playerIndex].hand.size(); ++CardiIndex) {
            Cardd& Cardi = players[playerIndex].hand[CardiIndex];
            if ((players[0].isAttacker) && !players[0].hasWon) {
                
            // Если игрок выиграл, отображаем сообщение о победе
            if (players[playerIndex].hasWon) {
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
                messageWinner.setPosition(messagePosition);
                windowss.draw(messageWinner);
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
                if (playerIndex == 0) {
                    // Атакующий игрок видит значения своих карт
                    Cardi.sprite.setTexture(Cardi.texture);
                    // Определение позиции карты в зависимости от её индекса 
                    Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (CardiIndex == selectedCardiIndex) {
                        Cardi.sprite.setColor(sf::Color::Yellow);
                    } else {
                        Cardi.sprite.setColor(sf::Color::White);
                    }
                    windowss.draw(Cardi.sprite);
                    ++id; 
                } else {
                    // У остальных игроков карты рубашкой вверх
                    back_Cardi_sprite.setRotation(rotation);
                    if (playerIndex == 1) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                    } else if (playerIndex == 2) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                    } else if (playerIndex == 3) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                    }
                    back_Cardi_sprite.setFillColor(sf::Color::White);
                    windowss.draw(back_Cardi_sprite);
                    id++;
                }
            }
            else if ((players[1].isAttacker) && !players[1].hasWon) {
                
                // Если игрок выиграл, отображаем сообщение о победе
                if (players[playerIndex].hasWon) {
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
                    messageWinner.setPosition(messagePosition);
                    windowss.draw(messageWinner);
                    continue; // Пропускаем отрисовку карт для выигравшего игрока
                }
                // Определение позиции и поворота карт в зависимости от индекса игрока
                float rotation;
                if (playerIndex == 1) { // Игрок снизу
                    rotation = 0;
                } else if (playerIndex == 2) { // Игрок 1920 - 50, 1080 / 2
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
                    Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (CardiIndex == selectedCardiIndex) {
                        Cardi.sprite.setColor(sf::Color::Yellow);
                    } else {
                        Cardi.sprite.setColor(sf::Color::White);
                    }
                    windowss.draw(Cardi.sprite);
                    ++id;
                } else {
                    // У остальных игроков карты рубашкой вверх
                    back_Cardi_sprite.setRotation(rotation);
                    if (playerIndex == 2) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                    } else if (playerIndex == 3) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                    } else if (playerIndex == 0) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                    }
                    back_Cardi_sprite.setFillColor(sf::Color::White);
                    windowss.draw(back_Cardi_sprite);
                    id++;
                }
            }
            else if ((players[2].isAttacker)  && !players[2].hasWon) {
                
                // Если игрок выиграл, отображаем сообщение о победе
                if (players[playerIndex].hasWon) {
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
                    messageWinner.setPosition(messagePosition);
                    windowss.draw(messageWinner);
                    continue; // Пропускаем отрисовку карт для выигравшего игрока
                }
                // Определение позиции и поворота карт в зависимости от индекса игрока
                float rotation;
                if (playerIndex == 2) { // Игрок снизу
                    rotation = 0;
                } else if (playerIndex == 3) { // Игрок 1920 - 50, 1080 / 2
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
                    Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (CardiIndex == selectedCardiIndex) {
                        Cardi.sprite.setColor(sf::Color::Yellow);
                    } else {
                        Cardi.sprite.setColor(sf::Color::White);
                    }
                    windowss.draw(Cardi.sprite);
                    ++id;
                } else {
                    // У остальных игроков карты рубашкой вверх
                    back_Cardi_sprite.setRotation(rotation);
                    if (playerIndex == 3) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                    } else if (playerIndex == 0) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                    } else if (playerIndex == 1) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                    }
                    back_Cardi_sprite.setFillColor(sf::Color::White);
                    windowss.draw(back_Cardi_sprite);
                    id++;
                }
            }
            else if ((players[3].isAttacker)  && !players[3].hasWon) {
                
                // Если игрок выиграл, отображаем сообщение о победе
                if (players[playerIndex].hasWon) {
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
                    messageWinner.setPosition(messagePosition);
                    windowss.draw(messageWinner);
                    continue; // Пропускаем отрисовку карт для выигравшего игрока
                }
                // Определение позиции и поворота карт в зависимости от индекса игрока
                float rotation;
                if (playerIndex == 3) { // Игрок снизу
                    rotation = 0;
                } else if (playerIndex == 0) { // Игрок 1920 - 50, 1080 / 2
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
                    Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (CardiIndex == selectedCardiIndex) {
                        Cardi.sprite.setColor(sf::Color::Yellow);
                    } else {
                        Cardi.sprite.setColor(sf::Color::White);
                    }
                    windowss.draw(Cardi.sprite);
                    ++id;
                } else {
                    // У остальных игроков карты рубашкой вверх
                    back_Cardi_sprite.setRotation(rotation);
                    if (playerIndex == 0) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                    } else if (playerIndex == 1) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                    } else if (playerIndex == 2) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                    }
                    back_Cardi_sprite.setFillColor(sf::Color::White);
                    windowss.draw(back_Cardi_sprite);
                    id++;
                }
            }
            else if ((players[0].isDefender)  && !players[0].hasWon) {
                
                // Если игрок выиграл, отображаем сообщение о победе
                if (players[playerIndex].hasWon) {
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
                    messageWinner.setPosition(messagePosition);
                    windowss.draw(messageWinner);
                    continue; // Пропускаем отрисовку карт для выигравшего игрока
                }
                // Определение позиции и поворота карт в зависимости от индекса игрока
                float rotation;
                if (playerIndex == 0) { // Игрок снизу
                    rotation = 0;
                } else if (playerIndex == 1) { // Игрок 1920 - 50, 1080 / 2
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
                    Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (CardiIndex == selectedCardiIndex) {
                        Cardi.sprite.setColor(sf::Color::Yellow);
                    } else {
                        Cardi.sprite.setColor(sf::Color::White);
                    }
                    windowss.draw(Cardi.sprite);
                    ++id; 
                } else {
                    // У остальных игроков карты рубашкой вверх
                    back_Cardi_sprite.setRotation(rotation);
                    if (playerIndex == 1) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                    } else if (playerIndex == 2) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                    } else if (playerIndex == 3) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                    }
                    back_Cardi_sprite.setFillColor(sf::Color::White);
                    windowss.draw(back_Cardi_sprite);
                    id++;
                }
            }
            else if ((players[1].isDefender) && !players[1].hasWon) {
                
                // Если игрок выиграл, отображаем сообщение о победе
                if (players[playerIndex].hasWon) {
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
                    messageWinner.setPosition(messagePosition);
                    windowss.draw(messageWinner);
                    continue; // Пропускаем отрисовку карт для выигравшего игрока
                }
                // Определение позиции и поворота карт в зависимости от индекса игрока
                float rotation;
                if (playerIndex == 1) { // Игрок снизу
                    rotation = 0;
                } else if (playerIndex == 2) { // Игрок 1920 - 50, 1080 / 2
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
                    Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (CardiIndex == selectedCardiIndex) {
                        Cardi.sprite.setColor(sf::Color::Yellow);
                    } else {
                        Cardi.sprite.setColor(sf::Color::White);
                    }
                    windowss.draw(Cardi.sprite);
                    ++id;
                } else {
                    // У остальных игроков карты рубашкой вверх
                    back_Cardi_sprite.setRotation(rotation);
                    if (playerIndex == 2) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                    } else if (playerIndex == 3) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                    } else if (playerIndex == 0) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                    }
                    back_Cardi_sprite.setFillColor(sf::Color::White);
                    windowss.draw(back_Cardi_sprite);
                    id++;
                }
            }
            else if ((players[2].isDefender) && !players[2].hasWon) {
                
                // Если игрок выиграл, отображаем сообщение о победе
                if (players[playerIndex].hasWon) {
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
                    messageWinner.setPosition(messagePosition);
                    windowss.draw(messageWinner);
                    continue; // Пропускаем отрисовку карт для выигравшего игрока
                }
                // Определение позиции и поворота карт в зависимости от индекса игрока
                float rotation;
                if (playerIndex == 2) { // Игрок снизу
                    rotation = 0;
                } else if (playerIndex == 3) { // Игрок 1920 - 50, 1080 / 2
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
                    Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (CardiIndex == selectedCardiIndex) {
                        Cardi.sprite.setColor(sf::Color::Yellow);
                    } else {
                        Cardi.sprite.setColor(sf::Color::White);
                    }
                    windowss.draw(Cardi.sprite);
                    ++id;
                } else {
                    // У остальных игроков карты рубашкой вверх
                    back_Cardi_sprite.setRotation(rotation);
                    if (playerIndex == 3) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                    } else if (playerIndex == 0) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                    } else if (playerIndex == 1) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                    }
                    back_Cardi_sprite.setFillColor(sf::Color::White);
                    windowss.draw(back_Cardi_sprite);
                    id++;
                }
            }
            else if ((players[3].isDefender)  && !players[3].hasWon) {
                
                // Если игрок выиграл, отображаем сообщение о победе
                if (players[playerIndex].hasWon) {
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
                    messageWinner.setPosition(messagePosition);
                    windowss.draw(messageWinner);
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
                if (playerIndex == 3) {
                    // Атакующий игрок видит значения своих карт
                    Cardi.sprite.setTexture(Cardi.texture);
                    // Определение позиции карты в зависимости от её индекса 
                    Cardi.sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (CardiIndex == selectedCardiIndex) {
                        Cardi.sprite.setColor(sf::Color::Yellow);
                    } else {
                        Cardi.sprite.setColor(sf::Color::White);
                    }
                    windowss.draw(Cardi.sprite);
                    ++id;
                } else {
                    // У остальных игроков карты рубашкой вверх
                    back_Cardi_sprite.setRotation(rotation);
                    if (playerIndex == 0) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                    } else if (playerIndex == 1) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                    } else if (playerIndex == 2) {
                        back_Cardi_sprite.setPosition(sf::Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                    }
                    back_Cardi_sprite.setFillColor(sf::Color::White);
                    windowss.draw(back_Cardi_sprite);
                    id++;
                }
            }
        }
    }
}   