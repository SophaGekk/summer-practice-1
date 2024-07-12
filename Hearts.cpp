
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <set>
#include "HeartsBOT.h" //  Файл с логикой ботов
#include "HEARTS_turns.h"
#include "gameMenu.h"

using namespace sf;

int selectedCardsIndex = 0;
std::vector<int> selected_cards(0);
Cards bito[4];
bool HeartsBroken = true;
int current_player = 0; //  Текущий игрок
int start_player = -1;

// Функция отрисовки карт всех игроков
void draw_cards(std::vector<Players>& players, sf::RenderWindow& windowss) {
    Texture tableTexture;
    if (!tableTexture.loadFromFile("resources/table.png")) {
        std::cerr << "Ошибка загрузки текстуры стола: " << "resources/table.png" << std::endl;
    }
    RectangleShape background(Vector2f(1920, 1080));
    background.setTexture(&tableTexture);
    background.setPosition(Vector2f(0, 0));
    windowss.draw(background);

    Texture back_card_texture;
    if (!back_card_texture.loadFromFile("resources/back.png")) { return; } //  Загружаем текстуру обратной стороны карты
    RectangleShape back_card_sprite(Vector2f(77, 110));
    back_card_sprite.setTexture(&back_card_texture);
    // Отрисовка карт каждого игрока
    for (int i = 1; i < 4; ++i) {
        int id = 1; //  Индекс для позиционирования карт
        for (Cards& card : players[i].hand) {
            if (i == 0) { //  Отрисовка карт игрока 0 (игрока, который ходит)
                card.sprite.setPosition(Vector2f(200 + (1300 / players[0].hand.size()) * id, 900)); //  Позиция внизу
            } else {
                if (i == 1) {
                    back_card_sprite.setPosition(Vector2f(150, 200 + (600 / players[i].hand.size()) * id)); //  Позиция слева
                } else if (i == 2) {
                    back_card_sprite.setPosition(Vector2f(200 + (1300 / players[i].hand.size()) * id, 100)); //  Позиция сверху
                } else {
                    back_card_sprite.setPosition(Vector2f(1750, 200 + (600 / players[i].hand.size()) * id)); //  Позиция справа
                }
                windowss.draw(back_card_sprite); //  Отрисовка обратной стороны карты
            }
            id++;
        }
    }
    int id_P0 = 1;
    for (size_t i = 0; i < players[0].hand.size(); ++i) {
        Cards& card = players[0].hand[i];
        card.sprite.setTexture(card.texture);

        // Определение позиции карты в зависимости от её индекса 
        //игрок снизу


        card.sprite.setPosition(Vector2f(200 + (1300 / players[0].hand.size()) * id_P0, 900));

        // Если карта выбрана, она рисуется в желтый цвет
        if (std::find(selected_cards.begin(), selected_cards.end(), i) != selected_cards.end()) {
            card.sprite.setColor(Color::Magenta);
        } else if (i == selectedCardsIndex) {
            card.sprite.setColor(Color::Yellow);
        } else {
            card.sprite.setColor(Color::White);
        }

        windowss.draw(card.sprite);
        ++id_P0;
    }
    // далее напишем отрисовку четырех карт на столе 
    if (bito[0].suit != "") {
        bito[0].sprite.setTexture(bito[0].texture);
        bito[0].sprite.setPosition(Vector2f(920, 595));
        windowss.draw(bito[0].sprite);
    }
    if (bito[1].suit != "") {
        bito[1].sprite.setTexture(bito[1].texture);
        bito[1].sprite.setPosition(Vector2f(880, 485));
        windowss.draw(bito[1].sprite);
    }
    if (bito[2].suit != "") {
        bito[2].sprite.setTexture(bito[2].texture);
        bito[2].sprite.setPosition(Vector2f(920, 375));
        windowss.draw(bito[2].sprite);
    }
    if (bito[3].suit != "") {
        bito[3].sprite.setTexture(bito[3].texture);
        bito[3].sprite.setPosition(Vector2f(960, 485));
        windowss.draw(bito[3].sprite);
    }
}

// Функция отрисовки карт всех игроков кроме игрока 0
void draw_cards_except_0(std::vector<Players>& players, sf::RenderWindow& windowss) {
    Texture tableTexture;
    if (!tableTexture.loadFromFile("resources/table.png")) {
        std::cerr << "Ошибка загрузки текстуры стола: " << "resources/table.png" << std::endl;
    }
    RectangleShape background(Vector2f(1920, 1080));
    background.setTexture(&tableTexture);
    background.setPosition(Vector2f(0, 0));
    windowss.draw(background);

    Texture back_card_texture;
    if (!back_card_texture.loadFromFile("resources/back.png")) { return; } //  Загружаем текстуру обратной стороны карты
    RectangleShape back_card_sprite(Vector2f(77, 110));
    back_card_sprite.setTexture(&back_card_texture);
    // Отрисовка карт каждого игрока
    for (int i = 1; i < 4; ++i) {
        int id = 1; //  Индекс для позиционирования карт
        for (Cards& card : players[i].hand) {
            if (i == 0) { //  Отрисовка карт игрока 0 (игрока, который ходит)
                card.sprite.setPosition(Vector2f(200 + (1300 / players[0].hand.size()) * id, 900)); //  Позиция внизу
            } else {
                if (i == 1) {
                    back_card_sprite.setPosition(Vector2f(150, 200 + (600 / players[i].hand.size()) * id)); //  Позиция слева
                } else if (i == 2) {
                    back_card_sprite.setPosition(Vector2f(200 + (1300 / players[i].hand.size()) * id, 100)); //  Позиция сверху
                } else {
                    back_card_sprite.setPosition(Vector2f(1750, 200 + (600 / players[i].hand.size()) * id)); //  Позиция справа
                }
                windowss.draw(back_card_sprite); //  Отрисовка обратной стороны карты
            }
            id++;
        }
    }

    // далее напишем отрисовку четырех карт на столе 
    if (bito[0].suit != "") {
        bito[0].sprite.setTexture(bito[0].texture);
        bito[0].sprite.setPosition(Vector2f(920, 595));
        windowss.draw(bito[0].sprite);
    }
    if (bito[1].suit != "") {
        bito[1].sprite.setTexture(bito[1].texture);
        bito[1].sprite.setPosition(Vector2f(880, 485));
        windowss.draw(bito[1].sprite);
    }
    if (bito[2].suit != "") {
        bito[2].sprite.setTexture(bito[2].texture);
        bito[2].sprite.setPosition(Vector2f(920, 375));
        windowss.draw(bito[2].sprite);
    }
    if (bito[3].suit != "") {
        bito[3].sprite.setTexture(bito[3].texture);
        bito[3].sprite.setPosition(Vector2f(960, 485));
        windowss.draw(bito[3].sprite);
    }
}


// Функция передачи карт от одного игрока к другому
void share_cards(std::vector<Players>& players, int num_of_player, std::vector<int> selected_cards) {
    std::sort(selected_cards.rbegin(), selected_cards.rend());

    // Обработка случая, когда передача происходит от игрока 3 к игроку 0
    if (num_of_player == 0) {
        for (int i = 0; i < 3; ++i) {
            players[num_of_player].hand.push_back(players[3].hand[selected_cards[i]]);
            players[3].hand.erase(players[3].hand.begin() + selected_cards[i]);
        }
    } else {
        // Передача карт от предыдущего игрока к текущему
        for (int i = 0; i < 3; ++i) {
            players[num_of_player].hand.push_back(players[num_of_player - 1].hand[selected_cards[i]]);
        }
        for (int i = 0; i < 3; ++i) {
            players[num_of_player - 1].hand.erase(players[num_of_player - 1].hand.begin() + selected_cards[i]);
        }
    }
}
// Функция для отключения выбора карт
void disable_card_selection(Players& player, std::vector<int> selected_cards) {
    // Сбрасываем выбор всех карт
    selected_cards.clear();

    // "Затемняем" все карты
    for (auto& card : player.hand) {
        card.sprite.setColor(Color(50, 50, 50));
    }
}

// Функция для включения выбора карт
void enable_card_selection(Players& player) {
    // Восстанавливаем цвет всех карт
    for (auto& card : player.hand) {
        card.sprite.setColor(Color::White);
    }
}

// Перечисление этапов игры
enum GameState { PASSING_CARDS, FIRST_ROUND, NORMAL_ROUND, END_GAME, HEARTS_BROKEN };
GameState currentGameState = PASSING_CARDS;

void handlePassingCards(sf::RenderWindow& windowss, std::vector<Players>& players, Event event) {
    if (current_player == 0) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Left) {
                // Переход к предыдущей карте с круговым циклом
                if (selectedCardsIndex > 0) {
                    --selectedCardsIndex;
                } else {
                    selectedCardsIndex = players[0].hand.size() - 1;
                }
            } else if (event.key.code == sf::Keyboard::Right) {
                // Переход к следующей карте с круговым циклом
                if (selectedCardsIndex < players[0].hand.size() - 1) {
                    ++selectedCardsIndex;
                } else {
                    selectedCardsIndex = 0;
                }
            } else if (event.key.code == sf::Keyboard::Enter) {
                // Проверяем, выбрана ли уже карта
                auto it = std::find(selected_cards.begin(), selected_cards.end(), selectedCardsIndex);

                if (it == selected_cards.end()) {
                    // Если не выбрана, добавляем в вектор и меняем её цвет на красный
                    selected_cards.push_back(selectedCardsIndex);
                    players[0].hand[selectedCardsIndex].sprite.setColor(Color::Magenta);

                    // Проверяем, выбраны ли уже 3 карты
                    if (selected_cards.size() == 3) {
                        for (Cards& card : players[0].hand) {
                            card.sprite.setColor(Color::White);
                        }
                        // Вызываем функцию share_cards, чтобы передать карты
                        share_cards(players, 1, selected_cards);
                        // Очищаем вектор selected_cards
                        selected_cards.clear();


// Сбрасываем цвет всех карт

                        // Переход к следующему игроку
                        current_player = (current_player + 1) % 4;
                    }
                } else {
                    // Если уже выбрана, удаляем из вектора и меняем её цвет на обычный
                    selected_cards.erase(it);
                    players[0].hand[selectedCardsIndex].sprite.setColor(Color::White);
                }
            }
        }
    } else {
        sf::sleep(sf::seconds(1));
        std::vector<int> bot_cards(0);
        share_card_turn(players, current_player, bot_cards);
        share_cards(players, (current_player + 1) % 4, bot_cards);
        current_player = (current_player + 1) % 4;
        if (current_player == 0) {
            currentGameState = FIRST_ROUND;
        }
    }
}
void handleFirstRound(sf::RenderWindow& windowss, std::vector<Players>& players, Event event) {
    bool found_clubs = false;
    while (found_clubs != true) {
        // Находим игрока с двойкой крести
        for (int player_ind = 0; player_ind < 4; player_ind++) {

            int ID = 0;
            for (Cards& card : players[player_ind].hand) {
                if (card.rank == 6 && card.suit == "CLUBS") {
                    // Если игрок - бот, он сразу кладет карту на стол
                    if (player_ind != 0) {
                        current_player = player_ind;
                        bito[current_player] = card;
                        players[current_player].hand.erase(players[current_player].hand.begin() + ID);
                        current_player = (current_player + 1) % 4;
                        found_clubs = true;
                        start_player = player_ind;
                        break;
                    } else // Игрок 0 должен выложить двойку крести
                    {
                        windowss.clear();
                        draw_cards_except_0(players, windowss);
                        // Отключаем выбор карт для игрока 0
                        disable_card_selection(players[0], selected_cards);

                        // Последовательная отрисовка карт

                    for (size_t i = 0; i < players[0].hand.size(); ++i) {
                        Cards& card = players[0].hand[i];
                        card.sprite.setTexture(card.texture);

                        // Определяем позицию карты
                        card.sprite.setPosition(Vector2f(320 + (1300 / players[0].hand.size()) * i, 900));

                        // Выделяем двойку крести желтым цветом
                        if (card.rank == 6 && card.suit == "CLUBS") {
                            selectedCardsIndex = i;
                            card.sprite.setColor(Color::Yellow);
                        } else {
                            card.sprite.setColor(Color(50, 50, 50));  // "Затемняем" остальные карты
                        }

                        windowss.draw(card.sprite);
                        windowss.display();
                    }

                        // Ждем нажатия Enter
                        bool enterPressed = false;
                        while (!enterPressed) {
                            sf::Event event;
                            while (windowss.pollEvent(event)) {
                                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                                    enterPressed = true;
                                }
                            }
                        }

                        // Выкладываем двойку крести на стол
                        bito[0] = players[0].hand[selectedCardsIndex];
                        selectedCardsIndex = 0;
                        current_player = 1; // Переход к следующем


                        enable_card_selection(players[0]); // Восстанавливаем выбор карт
                        players[0].hand.erase(players[0].hand.begin() + ID);
                        found_clubs = true;
                        start_player = 0;
                        break; // Выходим из цикла поиска игрока с двойкой крести
                    }
                }
                ID++;
            }
        }
    }
    currentGameState = HEARTS_BROKEN;
}

void handleHeratsBroken(sf::RenderWindow& windowss, std::vector<Players>& players, Event event) {
    if (current_player != 0) { //продолжаем второй ход после того как "двойка" крести оказалась на столе
        HeartsBroken = false;
        bito[current_player] = ordinary_turn_without_hearts(players, current_player, bito, HeartsBroken);
        current_player = (current_player + 1) % 4;
        sf::sleep(sf::seconds(1));
    } else {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Left) {
                do {
                    // Переход к предыдущей карте с круговым циклом
                    if (selectedCardsIndex > 0) {
                        --selectedCardsIndex;
                    } else {
                        selectedCardsIndex = players[0].hand.size() - 1;
                    }
                } while (players[0].hand[selectedCardsIndex].suit == "HEARTS" || (players[0].hand[selectedCardsIndex].suit == "SPADES" && players[0].hand[selectedCardsIndex].rank == 12));
            } else if (event.key.code == sf::Keyboard::Right) {
                do {
                    // Переход к следующей карте с круговым циклом
                    if (selectedCardsIndex < players[0].hand.size() - 1) {
                        ++selectedCardsIndex;
                    } else {
                        selectedCardsIndex = 0;
                    }
                } while (players[0].hand[selectedCardsIndex].suit == "HEARTS" || (players[0].hand[selectedCardsIndex].suit == "SPADES" && players[0].hand[selectedCardsIndex].rank == 12));

            } else if (event.key.code == sf::Keyboard::Enter) {
                // Проверяем, выбрана ли уже карта
                auto it = std::find(selected_cards.begin(), selected_cards.end(), selectedCardsIndex);

                if (it == selected_cards.end()) {
                    // Если не выбрана, добавляем в вектор и меняем её цвет на Magenta
                    selected_cards.push_back(selectedCardsIndex);
                    players[0].hand[selectedCardsIndex].sprite.setColor(Color::Magenta);

                    // Проверяем, выбрана ли уже 1 карта
                    if (selected_cards.size() == 1) {
                        for (auto& card : players[0].hand) {
                            card.sprite.setColor(Color::White);
                        }
                        bito[0] = players[0].hand[selectedCardsIndex];
                        players[0].hand.erase(players[0].hand.begin() + selectedCardsIndex);
                        selectedCardsIndex = 0;
                        selected_cards.clear();
                        // Сбрасываем цвет всех карт

                        // Переход к следующему игроку
                        current_player = 1;
                    }
                }
            }
        }
    }
    if (current_player == (start_player + 1) % 4) {
        draw_cards(players, windowss);
        currentGameState = NORMAL_ROUND;
    }

}

void handleNormalRound(sf::RenderWindow& windowss, std::vector<Players>& players, Event event) {
    if (current_player == 0) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Left) {
                // Переход к предыдущей карте с кру


                if (selectedCardsIndex > 0) {
                    --selectedCardsIndex;
                } else {
                    selectedCardsIndex = players[0].hand.size() - 1;
                }
            } else if (event.key.code == sf::Keyboard::Right) {
                // Переход к следующей карте с круговым циклом
                if (selectedCardsIndex < players[0].hand.size() - 1) {
                    ++selectedCardsIndex;
                } else {
                    selectedCardsIndex = 0;
                }
            } else if (event.key.code == sf::Keyboard::Enter) {
                // Проверяем, выбрана ли уже карта
                auto it = std::find(selected_cards.begin(), selected_cards.end(), selectedCardsIndex);

                if (it == selected_cards.end()) {
                    // Если не выбрана, добавляем в вектор и меняем её цвет на красный
                    selected_cards.push_back(selectedCardsIndex);
                    players[0].hand[selectedCardsIndex].sprite.setColor(Color::Magenta);

                    // Проверяем, выбран ли уже 1
                    if (selected_cards.size() == 1) {
                        for (auto& card : players[0].hand) {
                            card.sprite.setColor(Color::White);
                        }
                        bito[0] = players[0].hand[selectedCardsIndex];
                        players[0].hand.erase(players[0].hand.begin() + selectedCardsIndex);
                        selectedCardsIndex = 0;
                        selected_cards.clear();
                        // Сбрасываем цвет всех карт

                        // Переход к следующему игроку
                        current_player = (current_player + 1) % 4;
                    }
                } else {
                    // Если уже выбрана, удаляем из вектора и меняем её цвет на обычный
                    selected_cards.erase(it);
                    players[0].hand[selectedCardsIndex].sprite.setColor(Color::White);
                }
            }
        }
    } else {
        sf::sleep(sf::seconds(1));
        bito[current_player] = ordinary_turn(players, current_player, bito);
        current_player = (current_player + 1) % 4;
        if (current_player == start_player) {
            for (int i = 0; i < 4; i++) {
                // обнулим бито
                Cards card;
                bito[i] = card;
            }
        }
    }
}


int main_hearts(sf::RenderWindow& windowss) {

    srand(time(0));

    //  RenderWindow windowss(VideoMode(1920, 1080), L"Червы");
  //RenderWindow windowss(VideoMode::getDesktopMode(),  L"Червы", Style::Fullscreen);

    Texture tableTexture;
    if (!tableTexture.loadFromFile("resources/table.png")) {
        std::cerr << "Ошибка загрузки текстуры стола: " << "resources/table.png" << std::endl;
        return 1;
    }
    RectangleShape background(Vector2f(1920, 1080));
    background.setTexture(&tableTexture);
    background.setPosition(Vector2f(0, 0));

    sf::Font font_1; //это шрифт
    if (!font_1.loadFromFile("resources/ffont.ttf")) {
        std::cerr << "Ошибка загрузки шрифта!" << std::endl;
    }

    sf::Text message_escape;
    message_escape.setFont(font_1);
    message_escape.setCharacterSize(24);
    message_escape.setFillColor(sf::Color::White);
    message_escape.setPosition(700.f,500.f);

    // Текст вопроса
    // message_escape.setString(L"Вы действительно хотите выйти?");
    message_escape.setString(L"Do you really want to get out?");
    // Флаг для отображения текста выхода из игры
    bool showMessageEscape = false;
    // Флаг для отображения текста выхода в меню
    bool returnToMenu = false;

    sf::Text buttonYes;
    buttonYes.setFont(font_1);
    // buttonYes.setString("Да");
    buttonYes.setString("Yes");
    buttonYes.setCharacterSize(24);
    buttonYes.setFillColor(sf::Color::White);
    buttonYes.setPosition(800.f, 600.f);

    sf::Text buttonNo;
    buttonNo.setFont(font_1);
    // buttonNo.setString("Нет");
    buttonNo.setString("No");
    buttonNo.setCharacterSize(24);
    buttonNo.setFillColor(sf::Color::White);
    buttonNo.setPosition(1000.f, 600.f);

    // Флаг для отображения текста выбора карт
    bool showCardSelectionMessage = true;
    // Индекс выбранной кнопки (0 - Да, 1 - Нет)
    int selectedButtonIndex = 0;

    // Переменная для отслеживания количества игроков, выбравших "нет"
    int playersChoseNo = 0;

    std::vector<std::string> suits = {"HEARTS", "DIAMONDS", "SPADES", "CLUBS"};
    //основная колода 
    std::vector<Cards> deck;
    sf::Texture CardsTexture;
    for (int rank = 6; rank <= 14; ++rank) {
        for (std::string suit : suits) {
            Cards card;
            card.rank = rank;
            card.suit = suit;
            std::string filename = "resources/" + std::to_string(rank) + suit + ".png";
            if (!CardsTexture.loadFromFile(filename)) {
                std::cerr << "Ошибка загрузки текстуры: " << filename << std::endl;
            } else {
                card.texture = CardsTexture;
                card.sprite.setTexture(card.texture);
                // RectangleShape CardsShape(Vector2f(77,108));
                // CardsShape.setTexture(&CardsTexture);
            }
            deck.push_back(card);
        }
    }

    std::random_shuffle(deck.begin(), deck.end());

    std::vector<Players> players(4);
    players[0].isAttacker = true;
    players[0].isDefender = false;
    players[1].isAttacker = false;
    players[1].isDefender = true;
    players[2].isAttacker = false;
    players[1].isDefender = true;
    players[3].isAttacker = false;
    players[1].isDefender = true;

    //раздача карт на руки
    // for (int i = 0; i < 13; ++i) //для от 2
    for (int i = 0; i < 9; ++i) {
        players[0].hand.push_back(deck.back());
        deck.pop_back();
        players[1].hand.push_back(deck.back());
        deck.pop_back();
        players[2].hand.push_back(deck.back());
        deck.pop_back();
        players[3].hand.push_back(deck.back());
        deck.pop_back();
    }



    int first_round = 0; //  Флаг для первого раунда (обмен карт)

    Event event;
    Event main_game;
    // Game Loop
    while (windowss.isOpen()) {

        while (windowss.pollEvent(event)) {

            // Обработка событий
            if (event.type == Event::Closed) {
                windowss.close();
            } else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                // Показывает текстовое сообщение с запросом "Вы точно хотите выйти?"
                showMessageEscape = true;
                selectedButtonIndex = 0;
            }

            // Обработка событий для текстового сообщения
            if (showMessageEscape) {
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Left) {
                        selectedButtonIndex = 0;
                        // Изменяем цвет выделенной кнопки
                        buttonYes.setFillColor(Color::Red);
                        buttonNo.setFillColor(Color::White);
                    } else if (event.key.code == Keyboard::Right) {
                        selectedButtonIndex = 1;
                        // Изменяем цвет выделенной кнопки
                        buttonYes.setFillColor(Color::White);
                        buttonNo.setFillColor(Color::Red);
                    } else if (event.key.code == Keyboard::Enter) {
                        if (selectedButtonIndex == 0) {
                            // Выходим из игры
                            // windowss.close();
                            return 0;  
                            break;
                        } else {
                            // Скрываем текстовое сообщение
                            showMessageEscape = false;
                            // Сбрасываем цвет кнопок
                            buttonYes.setFillColor(Color::White);
                            buttonNo.setFillColor(Color::White);
                        }
                    }
                }
            }

            // Выбор состояния и его обработка
            switch (currentGameState) {
            case PASSING_CARDS:
                handlePassingCards(windowss, players, event);
                break;
            case FIRST_ROUND:
                handleFirstRound(windowss, players, event);
                break;
            case HEARTS_BROKEN:
                handleHeratsBroken(windowss, players, event);
                break;
                // case NORMAL_ROUND:
                //     handleNormalRound(windowss, players, event);
                //     break;
                // case END_GAME:
                //     // TODO: добавить обработку конца игры
                //     break;
            }

            windowss.clear();

            // Отрисовка текущего состояния
            switch (currentGameState) {
            case PASSING_CARDS:
                draw_cards(players, windowss);
                break;
            case FIRST_ROUND:
                draw_cards(players, windowss);
                break;
            case HEARTS_BROKEN:
                if (current_player == 0) {
                    draw_cards_except_0(players, windowss);
                    for (size_t i = 0; i < players[0].hand.size(); ++i) {
                        Cards& card = players[0].hand[i];
                        card.sprite.setTexture(card.texture);
                        if ((players[0].hand[i].suit == "HEARTS") or ((players[0].hand[i].suit == "SPADES") and (players[0].hand[i].rank == 12))) {
                            card.sprite.setColor(Color(50, 50, 50)); // затемняем запрещенные карты

                        } else if (i == selectedCardsIndex) {
                            card.sprite.setColor(Color::Yellow);
                        } else {
                            card.sprite.setColor(Color::White);
                        }
                        // Определяем позицию карты
                        card.sprite.setPosition(Vector2f(320 + (1300 / players[0].hand.size()) * i, 900));
                        windowss.draw(card.sprite);

                    }
                } else {
                    draw_cards(players, windowss);
                }
                break;
            case NORMAL_ROUND:
                draw_cards(players, windowss);
                break;
                // case END_GAME:
                //     // TODO: добавить отрисовку конца игры
                //     break;
            }

            // Отрисовка текстового сообщения, если оно активно
            if (showMessageEscape) {
                windowss.draw(message_escape);
                windowss.draw(buttonYes);
                windowss.draw(buttonNo);
            }

            windowss.display();
        }
    }
    return 0;
}