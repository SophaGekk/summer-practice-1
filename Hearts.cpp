#include <stdio.h>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <set>
#include "gameMenu.h"
#include "Hearts.h"
#include <filesystem> 
using namespace sf;

int selectedCardsIndex = 0;
std::vector<int> selected_cards(0);
Cards bito[4];
bool HeartsBroken = true;
int current_player = 0; //  Текущий игрок
int start_player = -1;
bool showMessageEscape = false; // это для проверки эскейпа
static bool show3CardSelectionMessage = false; // это для выбора 3 карт
static int selected3ButtonIndex = 0;
int selectedloseButtonIndex = 0;
//введём штрафные очки для всех 4 игроков
int penalty_points[4];

sf::Texture WinTexture;
sf::Texture LoseTexture;
bool showSaveLoadMenu = false;
int selectedSaveButtonIndex = 0;
// Флаг для отображения меню
int selectedsaveyesnobuttonindex = 0;


/// @brief раздача карт игрокам
/// @param players 
void shuffle(std::vector<Players>& players)
{
    std::vector<std::string> suits = {"HEARTS", "DIAMONDS", "SPADES", "CLUBS"};
    //основная колода 
    std::vector<Cards> deck;
    sf::Texture CardsTexture;
    for (int rank = 2; rank <= 14; ++rank) {
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


    //раздача карт на руки
    // for (int i = 0; i < 13; ++i) //для от 2
    for (int i = 0; i < 13; ++i) {
        players[0].hand.push_back(deck.back());
        deck.pop_back();
        players[1].hand.push_back(deck.back());
        deck.pop_back();
        players[2].hand.push_back(deck.back());
        deck.pop_back();
        players[3].hand.push_back(deck.back());
        deck.pop_back();
    }
}

/// @brief функция для анализа четырёх карт на столе и начисление штрафных очков
/// @param players 
/// @param bito 
void check_bito(std::vector<Players>& players, Cards bito[4])
{
    int current_points = 0;
    for (int i = 0; i < 4; i++)
    {
        if (bito[i].suit == "HEARTS") 
        {
            current_points++;
        }
        if (bito[i].suit == "SPADES" && bito[i].rank == 12)
        {
            current_points = current_points + 13; 
        }
    }
    if ((bito[0].rank >= bito[1].rank) && (bito[0].rank >= bito[2].rank) && (bito[0].rank >= bito[3].rank))
    {
        penalty_points[0] = penalty_points[0]+current_points;
        start_player = 0;
        for (int i = 0; i < 4; i++) {
                // обнулим бито
                Cards card;
                bito[i] = card;
            }
    }
    else if ((bito[1].rank >= bito[0].rank) && (bito[1].rank >= bito[2].rank) && (bito[1].rank >= bito[3].rank))
    {
        penalty_points[1] = penalty_points[1] + current_points;
        start_player = 1;
        for (int i = 0; i < 4; i++) {
                // обнулим бито
                Cards card;
                bito[i] = card;
            }
    }
    else if ((bito[2].rank >= bito[1].rank) && (bito[2].rank >= bito[0].rank) && (bito[2].rank >= bito[3].rank))
    {
        penalty_points[2] = penalty_points[2] + current_points;
        start_player = 2;
        for (int i = 0; i < 4; i++) {
                // обнулим бито
                Cards card;
                bito[i] = card;
            }
    }
    else if ((bito[3].rank >= bito[1].rank) && (bito[3].rank >= bito[2].rank) && (bito[3].rank >= bito[0].rank))
    {
        start_player = 3;
        penalty_points[3] = penalty_points[3] + current_points;
        for (int i = 0; i < 4; i++) {
                // обнулим бито
                Cards card;
                bito[i] = card;
            }
    }
    current_player = start_player; // Устанавливаем текущего игрока на победителя раунда

}

    /// @brief ход бота для передачи карт
    /// @param players 
    /// @param bito 
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
                        break;
                    }
                }
            }
            
        }
    }

/// @brief функция для определения запрещённой карты
/// @param card 
/// @param bito 
/// @param heartsBroken 
/// @return 
bool canPlayCard(const Cards& card, const Cards bito[4], bool heartsBroken) {
    if (heartsBroken || card.suit != "HEARTS") {
        return true;
    }
    return false;
}


    /// @brief функция для обычного хода бота
    /// @param players 
    /// @param num_of_player 
    /// @param bito 
    /// @param heartsBroken 
    /// @return 
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





/// @brief Функция отрисовки карт всех игроков
/// @param players 
/// @param windowss 
void draw_cards(std::vector<Players>& players, sf::RenderWindow& windowss, int current_player, int number_of_players) {
        sf::Font font_1; // это шрифт
    if (!font_1.loadFromFile("resources/ffont.ttf")) {
        std::cerr << "Ошибка загрузки шрифта!" << std::endl;
    }
    sf::Text text_bottom;
    text_bottom.setFont(font_1);
    text_bottom.setCharacterSize(24);
    text_bottom.setFillColor(sf::Color::White);
    text_bottom.setPosition(1920 / 2, 1080 - 50); // Центр нижней стороны

    sf::Text text_left;
    text_left.setFont(font_1);
    text_left.setCharacterSize(24);
    text_left.setFillColor(sf::Color::White);
    text_left.setPosition(50, 1080 / 2); // Центр левой стороны

    sf::Text text_top;
    text_top.setFont(font_1);
    text_top.setCharacterSize(24);
    text_top.setFillColor(sf::Color::White);
    text_top.setPosition(1920 / 2, 50); // Центр верхней стороны

    sf::Text text_right;
    text_right.setFont(font_1);
    text_right.setCharacterSize(24);
    text_right.setFillColor(sf::Color::White);
    text_right.setPosition(1920 - 50, 1080 / 2); // Центр правой стороны
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
    if (current_player < number_of_players) 
    {
        // Отрисовка карт каждого игрока
        for (int i = 0; i < 4; ++i) {
            int id = 1; //  Индекс для позиционирования карт
            for (Cards& card : players[i].hand) {
                {
                    if (i == (current_player +1 )%4) {
                        back_card_sprite.setPosition(Vector2f(150, 200 + (600 / players[i].hand.size()) * id)); //  Позиция слева
                        windowss.draw(back_card_sprite); //  Отрисовка обратной стороны карты
                    } else if (i == (current_player +2 )%4) {
                        back_card_sprite.setPosition(Vector2f(200 + (1300 / players[i].hand.size()) * id, 100)); //  Позиция сверху
                        windowss.draw(back_card_sprite); //  Отрисовка обратной стороны карты
                    } else if (i == (current_player +3 )%4){
                        back_card_sprite.setPosition(Vector2f(1750, 200 + (600 / players[i].hand.size()) * id)); //  Позиция справа
                        windowss.draw(back_card_sprite); //  Отрисовка обратной стороны карты
                    }
                    
                }
                id++;
            }
        }
        int id_P0 = 1;
        for (size_t i = 0; i < players[current_player].hand.size(); ++i) {
            Cards& card = players[current_player].hand[i];
            card.sprite.setScale(0.5f, 0.5f); 
            card.sprite.setTexture(card.texture);

            // Определение позиции карты в зависимости от её индекса 
            //игрок снизу


            card.sprite.setPosition(Vector2f(200 + (1300 / players[current_player].hand.size()) * id_P0, 900));

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
    }
    else 
    {
        for (int i = 1; i < 4; ++i) {
            int id = 1; //  Индекс для позиционирования карт
            for (Cards& card : players[i].hand) {
                if (i == 0) { //  Отрисовка карт игрока 0 (игрока, который ходит)
                    card.sprite.setPosition(Vector2f(200 + (1300 / players[0].hand.size()) * id, 900)); //  Позиция внизу
                } else {
                    if (i == 1) {
                        back_card_sprite.setPosition(Vector2f(150, 200 + (600 / players[i].hand.size()) * id)); //  Позиция слева
                        windowss.draw(back_card_sprite); //  Отрисовка обратной стороны карты
                    } else if (i == 2) {
                        back_card_sprite.setPosition(Vector2f(200 + (1300 / players[i].hand.size()) * id, 100)); //  Позиция сверху
                        windowss.draw(back_card_sprite); //  Отрисовка обратной стороны карты
                    } else {
                        back_card_sprite.setPosition(Vector2f(1750, 200 + (600 / players[i].hand.size()) * id)); //  Позиция справа
                        windowss.draw(back_card_sprite); //  Отрисовка обратной стороны карты
                    }
                    
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
    }
    
    // далее напишем отрисовку четырех карт на столе 
    if (bito[0].suit != "" && bito[0].suit != "0") {
        bito[0].sprite.setTexture(bito[0].texture);
        bito[0].sprite.setPosition(Vector2f(920, 595));
        bito[0].sprite.setScale(0.5f, 0.5f); 
        windowss.draw(bito[0].sprite);
    }
    if (bito[1].suit != "" && bito[1].suit != "0") {
        bito[1].sprite.setTexture(bito[1].texture);
        bito[1].sprite.setPosition(Vector2f(880, 485));
        bito[1].sprite.setScale(0.5f, 0.5f); 
        windowss.draw(bito[1].sprite);
    }
    if (bito[2].suit != "" && bito[2].suit != "0") {
        bito[2].sprite.setTexture(bito[2].texture);
        bito[2].sprite.setPosition(Vector2f(920, 375));
        bito[2].sprite.setScale(0.5f, 0.5f); 
        windowss.draw(bito[2].sprite);
    }
    if (bito[3].suit != "" && bito[3].suit != "0") {
        bito[3].sprite.setTexture(bito[3].texture);
        bito[3].sprite.setPosition(Vector2f(960, 485));
        bito[3].sprite.setScale(0.5f, 0.5f); 
        windowss.draw(bito[3].sprite);
    }
    // Обновление значений текстов
        text_bottom.setString("Penalty Points: " + std::to_string(penalty_points[0]));
        text_left.setString("Penalty Points: " + std::to_string(penalty_points[1]));
        text_left.setRotation(90);
        text_top.setString("Penalty Points: " + std::to_string(penalty_points[2]));
        text_right.setString("Penalty Points: " + std::to_string(penalty_points[3]));
        text_right.setRotation(90);
        if (current_player == 0){ text_bottom.setColor(Color::Green);}
        else if (current_player == 1){ text_left.setColor(Color::Green);}
        else if (current_player == 2){ text_top.setColor(Color::Green);}
        else { text_right.setColor(Color::Green); }
        windowss.draw(text_bottom);
        windowss.draw(text_left);
        windowss.draw(text_top);
        windowss.draw(text_right);
}


/// @brief  Функция отрисовки карт всех игроков кроме игрока 0
/// @param players 
/// @param windowss 
void draw_cards_except_0(std::vector<Players>& players, sf::RenderWindow& windowss) {
        sf::Font font_1; // это шрифт
    if (!font_1.loadFromFile("resources/ffont.ttf")) {
        std::cerr << "Ошибка загрузки шрифта!" << std::endl;
    }
    sf::Text text_bottom;
    text_bottom.setFont(font_1);
    text_bottom.setCharacterSize(24);
    text_bottom.setFillColor(sf::Color::White);
    text_bottom.setPosition(1920 / 2, 1080 - 50); // Центр нижней стороны

    sf::Text text_left;
    text_left.setFont(font_1);
    text_left.setCharacterSize(24);
    text_left.setFillColor(sf::Color::White);
    text_left.setPosition(50, 1080 / 2); // Центр левой стороны

    sf::Text text_top;
    text_top.setFont(font_1);
    text_top.setCharacterSize(24);
    text_top.setFillColor(sf::Color::White);
    text_top.setPosition(1920 / 2, 50); // Центр верхней стороны

    sf::Text text_right;
    text_right.setFont(font_1);
    text_right.setCharacterSize(24);
    text_right.setFillColor(sf::Color::White);
    text_right.setPosition(1920 - 50, 1080 / 2); // Центр правой стороны
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
        for (int i = 0; i < 4; ++i) {
            int id = 1; //  Индекс для позиционирования карт
            for (Cards& card : players[i].hand) {
                {
                    if (i == (current_player +1 )%4) {
                        back_card_sprite.setPosition(Vector2f(150, 200 + (600 / players[i].hand.size()) * id)); //  Позиция слева
                        windowss.draw(back_card_sprite); //  Отрисовка обратной стороны карты
                    } else if (i == (current_player +2 )%4) {
                        back_card_sprite.setPosition(Vector2f(200 + (1300 / players[i].hand.size()) * id, 100)); //  Позиция сверху
                        windowss.draw(back_card_sprite); //  Отрисовка обратной стороны карты
                    } else if (i == (current_player +3 )%4){
                        back_card_sprite.setPosition(Vector2f(1750, 200 + (600 / players[i].hand.size()) * id)); //  Позиция справа
                        windowss.draw(back_card_sprite); //  Отрисовка обратной стороны карты
                    }
                }
                id++;
            }
        }

    // далее напишем отрисовку четырех карт на столе 
    if (bito[0].suit != "" && bito[0].suit != "0") {
        bito[0].sprite.setTexture(bito[0].texture);
        bito[0].sprite.setPosition(Vector2f(920, 595));
        bito[0].sprite.setScale(0.5f, 0.5f); 
        windowss.draw(bito[0].sprite);
    }
    if (bito[1].suit != "" && bito[1].suit != "0") {
        bito[1].sprite.setTexture(bito[1].texture);
        bito[1].sprite.setPosition(Vector2f(880, 485));
        bito[1].sprite.setScale(0.5f, 0.5f); 
        windowss.draw(bito[1].sprite);
    }
    if (bito[2].suit != "" && bito[2].suit != "0") {
        bito[2].sprite.setTexture(bito[2].texture);
        bito[2].sprite.setPosition(Vector2f(920, 375));
        bito[2].sprite.setScale(0.5f, 0.5f); 
        windowss.draw(bito[2].sprite);
    }
    if (bito[3].suit != "" && bito[3].suit != "0") {
        bito[3].sprite.setTexture(bito[3].texture);
        bito[3].sprite.setPosition(Vector2f(960, 485));
        bito[3].sprite.setScale(0.5f, 0.5f); 
        windowss.draw(bito[3].sprite);
    }
    // Обновление значений текстов
        text_bottom.setString("Penalty Points: " + std::to_string(penalty_points[0]));
        text_left.setString("Penalty Points: " + std::to_string(penalty_points[1]));
        text_left.setRotation(90);
        text_top.setString("Penalty Points: " + std::to_string(penalty_points[2]));
        text_right.setString("Penalty Points: " + std::to_string(penalty_points[3]));
        text_right.setRotation(90);
        windowss.draw(text_bottom);
        windowss.draw(text_left);
        windowss.draw(text_top);
        windowss.draw(text_right);
}



/// @brief Функция передачи карт от одного игрока к другому
/// @param players 
/// @param num_of_player 
/// @param selected_cards 
void share_cards(std::vector<Players>& players, int num_of_player, std::vector<int> selected_cards) {
    // Упорядочиваем индексы выбранных карт в обратном порядке
    std::sort(selected_cards.rbegin(), selected_cards.rend());

    // Определяем номер предыдущего игрока в круговом порядке
    int previous_player = (num_of_player == 0) ? 3 : num_of_player - 1;
    //почистим карты
    for (auto& card : players[previous_player].hand) {
        card.sprite.setColor(Color::White);
    }
    // Передача карт от предыдущего игрока к текущему
    for (int i = 0; i < 3; ++i) {
        players[num_of_player].hand.push_back(players[previous_player].hand[selected_cards[i]]);
    }

    // Удаление карт из руки предыдущего игрока
    for (int i = 0; i < 3; ++i) {
        players[previous_player].hand.erase(players[previous_player].hand.begin() + selected_cards[i]);
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


/// @brief функция для хода передачи карт
/// @param windowss 
/// @param players 
/// @param event 
void handlePassingCards(sf::RenderWindow& windowss, std::vector<Players>& players, sf::Event event, int number_of_players) {
    sf::Font font_1; // это шрифт
    if (!font_1.loadFromFile("resources/ffont.ttf")) {
        std::cerr << "Ошибка загрузки шрифта!" << std::endl;
    }

    sf::Text message_3_cards; // сделаем надпись для передачи 3 карт игроком 0
    message_3_cards.setFont(font_1);
    message_3_cards.setCharacterSize(24);
    message_3_cards.setFillColor(sf::Color::White);
    message_3_cards.setPosition(700.f, 500.f);
    message_3_cards.setString(L"Do you want to pass these 3 cards?");

    sf::Text buttonYes;
    buttonYes.setFont(font_1);
    buttonYes.setString("Yes");
    buttonYes.setCharacterSize(24);
    buttonYes.setFillColor(sf::Color::White);
    buttonYes.setPosition(800.f, 600.f);

    sf::Text buttonNo;
    buttonNo.setFont(font_1);
    buttonNo.setString("No");
    buttonNo.setCharacterSize(24);
    buttonNo.setFillColor(sf::Color::White);
    buttonNo.setPosition(1000.f, 600.f);

    // Флаг для отображения текста выбора карт

    // Индекс выбранной кнопки (0 - Да, 1 - Нет)


    if (current_player < number_of_players) {
        if (show3CardSelectionMessage) {
            // Отображение сообщения и кнопок
           
            if (event.type == sf::Event::KeyPressed && showSaveLoadMenu == false) {
                if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right) {
                    selected3ButtonIndex = 1 - selected3ButtonIndex; // Переключение между кнопками
                } else if (event.key.code == sf::Keyboard::Enter && !showSaveLoadMenu) {
                    if (selected3ButtonIndex == 0) {
                        // Игрок выбрал "Yes"
                        for (Cards& card : players[0].hand) {
                            card.sprite.setColor(sf::Color::White); // тут выбранные карты красим обратно в белый
                        }
                        share_cards(players, (current_player+1)%4, selected_cards);
                        selected_cards.clear();
                        current_player = (current_player + 1) % 4;
                        show3CardSelectionMessage = false;
                        selectedCardsIndex = 0;
                    } else {
                        // Игрок выбрал "No"
                        selected_cards.clear();
                        for (Cards& card : players[0].hand) {
                            card.sprite.setColor(sf::Color::White);
                        }
                        show3CardSelectionMessage = false;
                    }
                }
            }

        } else {
            if (event.type == sf::Event::KeyPressed && showMessageEscape == false && showSaveLoadMenu == false) {
                if (event.key.code == sf::Keyboard::Left) {
                    if (selectedCardsIndex > 0) {
                        --selectedCardsIndex;
                    } else {
                        selectedCardsIndex = players[current_player].hand.size() - 1;
                    }
                } else if (event.key.code == sf::Keyboard::Right) {
                    if (selectedCardsIndex < players[current_player].hand.size() - 1) {
                        ++selectedCardsIndex;
                    } else {
                        selectedCardsIndex = 0;
                    }
                } else if (event.key.code == sf::Keyboard::Enter && !showSaveLoadMenu) {
                    auto it = std::find(selected_cards.begin(), selected_cards.end(), selectedCardsIndex);
                    if (it == selected_cards.end()) {
                        selected_cards.push_back(selectedCardsIndex);
                        players[current_player].hand[selectedCardsIndex].sprite.setColor(sf::Color::Magenta);
                        if (selected_cards.size() == 3) {
                            show3CardSelectionMessage = true;
                        }
                    } else {
                        selected_cards.erase(it);
                        players[current_player].hand[selectedCardsIndex].sprite.setColor(sf::Color::White);
                    }
                }
            }
        }
    } else {
        sf::sleep(sf::seconds(1));
        std::vector<int> bot_cards;
        share_card_turn(players, current_player, bot_cards);
        share_cards(players, (current_player + 1) % 4, bot_cards);
        current_player = (current_player + 1) % 4;
        if (current_player == 0) {
            currentGameState = FIRST_ROUND;
            // currentGameState = END_GAME;
        }
    }
}


/// @brief функция для хода игрока с картой 2-кой крести
/// @param windowss 
/// @param players 
/// @param event 
void handleFirstRound(sf::RenderWindow& windowss, std::vector<Players>& players, Event event, int number_of_players) {
    bool found_clubs = false;
    while (!found_clubs) {
        // Находим игрока с двойкой крести
        for (int player_ind = 0; player_ind < 4; player_ind++) {
            if (found_clubs) break;
            int ID = 0;
            for (Cards& card : players[player_ind].hand) {
                if (card.rank == 2 && card.suit == "CLUBS") {
                    // Если игрок - бот, он сразу кладет карту на стол
                    if (player_ind > number_of_players) {
                        current_player = player_ind;
                        bito[current_player] = card;
                        players[current_player].hand.erase(players[current_player].hand.begin() + ID);
                        current_player = (current_player + 1) % 4;
                        found_clubs = true;
                        start_player = player_ind;
                        break;
                    } else { // Игрок 0 должен выложить двойку крести
                        windowss.clear();
                        draw_cards_except_0(players, windowss);
                        // Отключаем выбор карт для игрока 0
                        disable_card_selection(players[player_ind], selected_cards);

                        // Последовательная отрисовка карт
                        for (size_t i = 0; i < players[0].hand.size(); ++i) {
                            Cards& card = players[player_ind].hand[i];
                            card.sprite.setTexture(card.texture);

                            // Определяем позицию карты
                            card.sprite.setScale(0.5f, 0.5f); 
                            card.sprite.setPosition(sf::Vector2f(320 + (1300 / players[0].hand.size()) * i, 900));

                            // Выделяем двойку крести желтым цветом
                            if (card.rank == 2 && card.suit == "CLUBS") {
                                selectedCardsIndex = i;
                                card.sprite.setColor(sf::Color::Yellow);
                            } else {
                                card.sprite.setColor(sf::Color(50, 50, 50));  // "Затемняем" остальные карты
                            }

                            windowss.draw(card.sprite);
                        }

                        windowss.display();

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
                        for (auto& card : players[player_ind].hand) {
                            card.sprite.setColor(Color::White);
                        }
                        // Выкладываем двойку крести на стол
                        bito[player_ind] = players[player_ind].hand[selectedCardsIndex];
                        players[player_ind].hand.erase(players[player_ind].hand.begin() + ID);
                        enable_card_selection(players[player_ind]); // Восстанавливаем выбор карт
                        start_player = player_ind;
                        selectedCardsIndex = player_ind;
                        current_player =( player_ind+1)%4; // Переход к следующему

                        found_clubs = true;
                        break; // Выходим из цикла поиска игрока с двойкой крести
                    }
                }
                ID++;
            }
        }
    }
   
    currentGameState = HEARTS_BROKEN;
}

/// @brief функция для хода с запрещенными картами
/// @param windowss 
/// @param players 
/// @param event 
void handleHeratsBroken(sf::RenderWindow& windowss, std::vector<Players>& players, Event event, int number_of_players) {
   
    if (current_player >= number_of_players) { //продолжаем второй ход после того как "двойка" крести оказалась на столе
        HeartsBroken = false;
        sf::sleep(sf::seconds(1));
        bito[current_player] = ordinary_turn_without_hearts(players, current_player, bito, HeartsBroken);
        current_player = (current_player + 1) % 4;
        
    } 
    else {
        if (event.type == sf::Event::KeyPressed && showMessageEscape ==false && showSaveLoadMenu == false) {
            if (event.key.code == sf::Keyboard::Left) {
                do {
                    // Переход к предыдущей карте с круговым циклом
                    if (selectedCardsIndex > 0) {
                        --selectedCardsIndex;
                    } else {
                        selectedCardsIndex = players[current_player].hand.size() - 1;
                    }
                } while (players[current_player].hand[selectedCardsIndex].suit == "HEARTS" || (players[current_player].hand[selectedCardsIndex].suit == "SPADES" && players[current_player].hand[selectedCardsIndex].rank == 12));
            } else if (event.key.code == sf::Keyboard::Right) {
                do {
                    // Переход к следующей карте с круговым циклом
                    if (selectedCardsIndex < players[current_player].hand.size() - 1) {
                        ++selectedCardsIndex;
                    } else {
                        selectedCardsIndex = 0;
                    }
                } while (players[current_player].hand[selectedCardsIndex].suit == "HEARTS" || (players[current_player].hand[selectedCardsIndex].suit == "SPADES" && players[current_player].hand[selectedCardsIndex].rank == 12));

            } else if (event.key.code == sf::Keyboard::Enter && !showSaveLoadMenu) {
                // Проверяем, выбрана ли уже карта
                auto it = std::find(selected_cards.begin(), selected_cards.end(), selectedCardsIndex);

                if (it == selected_cards.end()) {
                    // Если не выбрана, добавляем в вектор и меняем её цвет на Magenta
                    selected_cards.push_back(selectedCardsIndex);
                    players[current_player].hand[selectedCardsIndex].sprite.setColor(Color::Magenta);

                    // Проверяем, выбрана ли уже 1 карта
                    if (selected_cards.size() == 1) {
                        for (auto& card : players[current_player].hand) {
                            card.sprite.setColor(Color::White);
                        }
                        bito[current_player] = players[current_player].hand[selectedCardsIndex];
                        players[current_player].hand.erase(players[current_player].hand.begin() + selectedCardsIndex);
                        selectedCardsIndex = 0;
                        selected_cards.clear();
                        // Сбрасываем цвет всех карт

                        // Переход к следующему игроку
                        current_player =( current_player+1)%4;
                    }
                }
            }
        }
    }
    if ((bito[0].suit!="" && bito[1].suit!="" && bito[2].suit!="" && bito[3].suit!="") && (bito[0].suit!="0" && bito[1].suit!="0" && bito[2].suit!="0" && bito[3].suit!="0")) 
    {
        windowss.clear();
        draw_cards(players, windowss, current_player, number_of_players);
        check_bito(players, bito); // Проверяем бито, устанавливая current_player
        windowss.display();
        sf::sleep(sf::seconds(1));
        currentGameState = NORMAL_ROUND;
    }
}



/// @brief функция для обчного хода 
/// @param windowss 
/// @param players 
/// @param event 
void handleNormalRound(sf::RenderWindow& windowss, std::vector<Players>& players, sf::Event event, int number_of_players) {
    if (current_player < number_of_players) { 
        // Ход игрока 0
        
        if (event.type == sf::Event::KeyPressed && !showMessageEscape && showSaveLoadMenu == false) {
            switch (event.key.code) {
                case sf::Keyboard::Left:{
                    // Переход к предыдущей карте с круговым циклом
                    if (selectedCardsIndex > 0) {
                        --selectedCardsIndex;
                    } else {
                        selectedCardsIndex = players[current_player].hand.size() - 1;
                    }
                    break;}
                case sf::Keyboard::Right:{
                    // Переход к следующей карте с круговым циклом
                    if (selectedCardsIndex < players[current_player].hand.size() - 1) {
                        ++selectedCardsIndex;
                    } else {
                        selectedCardsIndex = 0;
                    }
                    break;}
                case sf::Keyboard::Enter:{
                    if (!showSaveLoadMenu)
                    {

                    
                        // Проверяем, выбрана ли уже карта
                        auto it = std::find(selected_cards.begin(), selected_cards.end(), selectedCardsIndex);
                        if (it == selected_cards.end()) {
                            // Если не выбрана, добавляем в вектор и меняем её цвет на красный
                            selected_cards.push_back(selectedCardsIndex);
                            players[current_player].hand[selectedCardsIndex].sprite.setColor(sf::Color::Magenta);

                            // Проверяем, выбрана ли уже 1 карта
                            if (selected_cards.size() == 1) {
                                // Очистка цвета всех карт
                                for (auto& card : players[current_player].hand) {
                                    card.sprite.setColor(sf::Color::White);
                                }
                                bito[current_player] = players[current_player].hand[selectedCardsIndex];
                                players[current_player].hand.erase(players[current_player].hand.begin() + selectedCardsIndex);
                                selectedCardsIndex = 0;
                                selected_cards.clear();

                                // Переход к следующему игроку
                                current_player = (current_player + 1) % 4;
                                if ((bito[0].suit!="" && bito[1].suit!="" && bito[2].suit!="" && bito[3].suit!="")&& (bito[0].suit!="0" && bito[1].suit!="0" && bito[2].suit!="0" && bito[3].suit!="0")) 
                                {
                                    windowss.clear();
                                    draw_cards(players, windowss, current_player, number_of_players);
                                    check_bito(players, bito); // Проверяем бито, устанавливая current_player
                                    windowss.display();
                                    sf::sleep(sf::seconds(1));
                                }
                            }
                        } 
                        break;
                        }
                        }
                default:{
                    break;}
            }
        } 


    } 
    else {
        // Ход бота
        sf::sleep(sf::seconds(1));
        bito[current_player] = ordinary_turn_without_hearts(players, current_player, bito, HeartsBroken);
        current_player = (current_player + 1) % 4;
        if ((bito[0].suit!="" && bito[1].suit!="" && bito[2].suit!="" && bito[3].suit!="") && (bito[0].suit!="0" && bito[1].suit!="0" && bito[2].suit!="0" && bito[3].suit!="0")) 
        {
            windowss.clear();
            draw_cards(players, windowss, current_player, number_of_players);
            check_bito(players, bito); // Проверяем бито, устанавливая current_player
            windowss.display();
            sf::sleep(sf::seconds(1));
        }
        if ((players[0].hand.size() == 0) && (players[1].hand.size() == 0) && (players[2].hand.size() == 0) && (players[3].hand.size() == 0))
        {
            currentGameState = END_GAME; 
        }

    }
}    
    
/// @brief функция для хода когда у всех игроков кончаются карты
/// @param windowss 
/// @param players 
/// @param event 
void handleEndGame(sf::RenderWindow& windowss, std::vector<Players>& players, sf::Event event, int number_of_players)
{
    if (number_of_players != 1)
    {
        if ((penalty_points[0] >= 100) || (penalty_points[1] >= 100) || (penalty_points[2] >= 100) || (penalty_points[3] >= 100))
        {
            sf::Font font_1; //это шрифт
            if (!font_1.loadFromFile("resources/arial.ttf")) {
                std::cerr << "Ошибка загрузки шрифта!" << std::endl;
            }

            sf::Text paty;
            paty.setFont(font_1);
            paty.setString(L"Победила дружба!");
            paty.setCharacterSize(50);
            paty.setFillColor(sf::Color::White);
            paty.setPosition(100.f, 1080 /2 + 100);
            RectangleShape dark_background_sprite(Vector2f(2000, 2000));
            dark_background_sprite.setFillColor(Color(0, 0, 0, 100));
            windowss.draw(dark_background_sprite);
            windowss.draw(paty);
            windowss.display();
        }
        else
        {
            current_player = 0;
            start_player = 0;
            windowss.clear();
            shuffle(players);
            draw_cards(players, windowss, current_player,number_of_players);
            windowss.display();
            sf::sleep(sf::seconds(1));
            currentGameState = PASSING_CARDS;
        }

    }
    else 
    {
        if ((penalty_points[0] >= 100) || (penalty_points[1] >= 100) || (penalty_points[2] >= 100) || (penalty_points[3] >= 100))
        {
            windowss.clear();
            sf::Font font_1; //это шрифт
            if (!font_1.loadFromFile("resources/ffont.ttf")) {
                std::cerr << "Ошибка загрузки шрифта!" << std::endl;
            }

            sf::Text buttonYes;
            buttonYes.setFont(font_1);
            // buttonYes.setString("Да");
            buttonYes.setString("Yes");
            buttonYes.setCharacterSize(24);
            buttonYes.setFillColor(sf::Color::White);
            buttonYes.setPosition(900.f, 1080 /2 + 100);

            sf::Text buttonNo;
            buttonNo.setFont(font_1);
            // buttonNo.setString("Нет");
            buttonNo.setString("No");
            buttonNo.setCharacterSize(24);
            buttonNo.setFillColor(sf::Color::White);
            buttonNo.setPosition(1000.f, 1080 /2 + 100);

            // Индекс выбранной кнопки (0 - Да, 1 - Нет)
            int selectedButtonIndex = 0;

            // Переменная для отслеживания количества игроков, выбравших "нет"
            int playersChoseNo = 0;

            sf::Text text_for_loser;
            text_for_loser.setFont(font_1);
            text_for_loser.setCharacterSize(24);
            text_for_loser.setFillColor(sf::Color::White);
            text_for_loser.setPosition(1920 / 2 - 50, 1080 /2 + 50); // Центр нижней стороны
            text_for_loser.setString("Try again?");

            //затемним экран
            RectangleShape dark_background_sprite(Vector2f(2000, 2000));
            dark_background_sprite.setFillColor(Color(0, 0, 0, 100));
            draw_cards(players, windowss,current_player,number_of_players);
            windowss.draw(dark_background_sprite);
            windowss.draw(text_for_loser);

            
            buttonYes.setFillColor(selectedloseButtonIndex == 0 ? sf::Color::Red : sf::Color::White);
            buttonNo.setFillColor(selectedloseButtonIndex == 1 ? sf::Color::Red : sf::Color::White);
            windowss.draw(buttonYes);
            windowss.draw(buttonNo);


            if ((penalty_points[0] <= penalty_points[1]) && (penalty_points[0] <= penalty_points[2]) && (penalty_points[0] <= penalty_points[3]))
            {
                // Загружаем текстуру из файла
                
                // Создаем спрайт и устанавливаем текстуру
                sf::Sprite WinSprite;
                WinSprite.setTexture(WinTexture);
                // Устанавливаем размер спрайта (примерно 500 на 500 пикселей)
                WinSprite.setPosition(650, 100);
                WinSprite.setScale(0.1f, 0.1f); 
                windowss.draw(WinSprite);
            }
            else
            {
                // Создаем спрайт и устанавливаем текстуру
                sf::Sprite LoseSprite;
                LoseSprite.setTexture(LoseTexture);
                // Устанавливаем размер спрайта (примерно 500 на 500 пикселей)
                LoseSprite.setPosition(620, 20);
                LoseSprite.setScale(1.0f, 1.0f); 
                windowss.draw(LoseSprite);
            }
            windowss.display();
        }
        else 
        {
            current_player = 0;
            start_player = 0;
            windowss.clear();
            shuffle(players);
            draw_cards(players, windowss, current_player,number_of_players);
            windowss.display();
            sf::sleep(sf::seconds(1));
            currentGameState = PASSING_CARDS;
        }
    }
    
}

/// @brief функция для сохранения игры
/// @param players 
/// @param bito 
/// @param current_player 
/// @param start_player 
/// @param penalty_points 
void savegame(const std::vector<Players>& players, Cards bito[4], int current_player, int start_player, int penalty_points[4], int number_of_players) {
    std::ofstream saveFile("resources/saveHeartsgame.txt");
    if (!saveFile.is_open()) {
        std::cerr << "Error opening file for saving!" << std::endl;
        return;
    }

    // Сохраняем карты каждого игрока
    for (const auto& player : players) {
        for (const auto& card : player.hand) {
            saveFile << card.rank << " " << card.suit << " ";
        }
        saveFile << "\n";
    }

    // Сохраняем карты массива bito
    for (int i = 0; i < 4; ++i) {
        saveFile << bito[i].suit << " " << bito[i].rank << " ";
    }
    saveFile << "\n";

    // Сохраняем текущего игрока и стартового игрока
    saveFile << current_player << "\n";
    saveFile << start_player << "\n";

    // Сохраняем очки штрафа
    for (int i = 0; i < 4; ++i) {
        saveFile << penalty_points[i] << "\n";
    }

    saveFile << number_of_players << "\n";

    saveFile.close();
}

/// @brief функция для загрузки сохранения игры
/// @param players 
/// @param bito 
/// @param current_player 
/// @param start_player 
/// @param penalty_points 
void loadgame(std::vector<Players>& players, Cards bito[4], int& current_player, int& start_player, int penalty_points[4], int number_of_players) {
    std::ifstream loadFile("resources/saveHeartsgame.txt");
    if (!loadFile.is_open()) {
        std::cerr << "Error opening file for loading!" << std::endl;
        return;
    }
    std::vector<std::string> suits = {"HEARTS", "DIAMONDS", "SPADES", "CLUBS"};
    players.clear(); // Очищаем текущий список игроков перед загрузкой
    players.resize(4); // Убеждаемся, что у нас 4 игрока

    std::string line;
    sf::Texture CardsTexture;
    // Загружаем карты каждого игрока
    for (auto& player : players) {
        player.hand.clear();
        if (std::getline(loadFile, line)) {
            std::istringstream iss(line);
            int rank;
            std::string suit;
            while (iss >> rank >> suit) {
                Cards card;
                card.rank = rank;
                card.suit = suit;
                // Загрузка текстуры и установка спрайта
                
                std::string filename = "resources/" + std::to_string(rank) + suit + ".png";
                if (!CardsTexture.loadFromFile(filename)) {
                    std::cerr << "Ошибка загрузки текстуры: " << filename << std::endl;
                } else {
                    card.texture = CardsTexture;
                    card.sprite.setTexture(card.texture);
                }
                player.hand.push_back(card);
            }
        }
    }

    // Загружаем карты массива bito
    if (std::getline(loadFile, line)) {
        std::istringstream iss(line);
        int rank;
        std::string suit;
        for (int i = 0; i < 4; ++i) {
            iss >> suit >> rank;
            bito[i].suit = suit;
            bito[i].rank = rank;
            if (suit != "0" && !(rank > 14))
            {
                std::string filename = "resources/" + std::to_string(rank) + suit + ".png";
                if (!CardsTexture.loadFromFile(filename)) {
                    std::cerr << "Ошибка загрузки текстуры: " << filename << std::endl;
                } else  {
                    bito[i].texture = CardsTexture;
                    bito[i].sprite.setTexture(bito[i].texture);
                }
            }
        }
    }

    // Загружаем текущего игрока
    loadFile >> current_player;

    // Загружаем стартового игрока
    loadFile >> start_player;

    // Загружаем очки штрафа
    for (int i = 0; i < 4; ++i) {
        loadFile >> penalty_points[i];
    }

    loadFile >> number_of_players;

    loadFile.close();
}

/// @brief главная функция игры в червы для обработки ходов игроков и отрисовки
/// @param windowss 
/// @return 
int main_hearts(sf::RenderWindow& windowss, int number_of_players) {

    if (!WinTexture.loadFromFile("resources/YouWin.png")) {
        std::cerr << "Ошибка загрузки шрифта YouLose.png!" << std::endl; // Ошибка загрузки текстуры
    }
    if (!LoseTexture.loadFromFile("resources/YouLose.png")) {
        std::cerr << "Ошибка загрузки шрифта YouLose.png!" << std::endl; // Ошибка загрузки текстуры
    }    
    // srand(time(0));

    sf::Font font_1; //это шрифт
    if (!font_1.loadFromFile("resources/ffont.ttf")) {
        std::cerr << "Ошибка загрузки шрифта!" << std::endl;
    }

    // сделаем сохранения
    // Текст для элементов меню
    sf::Text saveText("Save Game", font_1, 24);
    saveText.setFillColor(sf::Color::White);
    saveText.setPosition(800.f, 500.f);

    sf::Text loadText("Load Game", font_1, 24);
    loadText.setFillColor(sf::Color::White);
    loadText.setPosition(1020.f, 500.f);




    sf::Text message_3_cards; // сделаем надпись для передачи 3 карт игроком 0
    message_3_cards.setFont(font_1);
    message_3_cards.setCharacterSize(24);
    message_3_cards.setFillColor(sf::Color::White);
    message_3_cards.setPosition(700.f,500.f);

    // Текст вопроса
    // message_escape.setString(L"Вы действительно хотите выйти?");
    message_3_cards.setString(L"Do you want to pass theese 3 cards?");


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


    sf::Text message_escape;
    message_escape.setFont(font_1);
    message_escape.setCharacterSize(24);
    message_escape.setFillColor(sf::Color::White);
    message_escape.setPosition(700.f,500.f);

    // Текст вопроса
    // message_escape.setString(L"Вы действительно хотите выйти?");
    message_escape.setString(L"Do you really want to get out?");
    // Флаг для отображения текста выхода из игры

    // Флаг для отображения текста выхода в меню
    bool returnToMenu = false;


    std::vector<Players> players(4);

    shuffle(players);


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

                // Обработка нажатия на кнопку "Save Game"
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::S) {
                showSaveLoadMenu = true;
                saveText.setFillColor(sf::Color::Red);
                loadText.setFillColor(sf::Color::White);
                selectedSaveButtonIndex = 0; // Выбираем "Save Game" по умолчанию
            }
            // Обработка нажатия на кнопку "Load Game"
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::L) {
                showSaveLoadMenu = true;
                loadText.setFillColor(sf::Color::Red);
                saveText.setFillColor(sf::Color::White);
                selectedSaveButtonIndex = 1; // Выбираем "Load Game" по умолчанию
            }

            // Обработка событий для меню сохранения/загрузки
            if (showSaveLoadMenu) {
                if (selectedSaveButtonIndex == 0)
                {
                    if (event.key.code == Keyboard::Left) {
                            // selectedsaveyesnobuttonindex = (selectedsaveyesnobuttonindex - 1 + 2) % 2;
                            selectedsaveyesnobuttonindex = 0;
                            
                        } else if (event.key.code == Keyboard::Right) {
                            // selectedsaveyesnobuttonindex = (selectedsaveyesnobuttonindex + 1) % 2;
                            selectedsaveyesnobuttonindex = 1;
                            
                        } else if (event.key.code == Keyboard::Enter) {
                            
                            if (selectedsaveyesnobuttonindex == 0) {
                                savegame(players, bito, current_player, start_player, penalty_points, number_of_players);
                                showSaveLoadMenu = false;
                                break;
                            } else {
                                // Скрываем текстовое сообщение
                                showSaveLoadMenu = false;
                                // Сбрасываем цвет кнопок
                                buttonYes.setFillColor(Color::White);
                                buttonNo.setFillColor(Color::White);
                            }
                        }
                    buttonYes.setFillColor(selectedsaveyesnobuttonindex == 0 ? sf::Color::Red : sf::Color::White);
                    buttonNo.setFillColor(selectedsaveyesnobuttonindex == 1 ? sf::Color::Red : sf::Color::White);
                }
                else if (selectedSaveButtonIndex == 1)
                {
                    
                    if (event.key.code == Keyboard::Left) {
                            selectedsaveyesnobuttonindex = 0;
                            
                        } else if (event.key.code == Keyboard::Right) {
                            selectedsaveyesnobuttonindex = 1;
                            
                        } else if (event.key.code == Keyboard::Enter) {
                            if (selectedsaveyesnobuttonindex == 0) {
                                loadgame(players, bito,current_player, start_player, penalty_points, number_of_players);
                                currentGameState = NORMAL_ROUND;
                                showSaveLoadMenu = false;
                                break;
                            } else {
                                // Скрываем текстовое сообщение
                                showSaveLoadMenu = false;
                                // Сбрасываем цвет кнопок
                                buttonYes.setFillColor(Color::White);
                                buttonNo.setFillColor(Color::White);
                            }
                        }
                    buttonYes.setFillColor(selectedsaveyesnobuttonindex == 0 ? sf::Color::Red : sf::Color::White);
                    buttonNo.setFillColor(selectedsaveyesnobuttonindex == 1 ? sf::Color::Red : sf::Color::White);
                }
                // ... (Код аналогичен обработке событий для меню выхода)
                // В зависимости от выбранной кнопки вызывайте saveGame() или loadGame()

            }


            // Обработка событий для текстового сообщения
           if (showMessageEscape) {
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Left) {
                        selectedButtonIndex = (selectedButtonIndex - 1 + 2) % 2;
                        
                    } else if (event.key.code == Keyboard::Right) {
                        selectedButtonIndex = (selectedButtonIndex + 1) % 2;
                        
                    } else if (event.key.code == Keyboard::Enter) {
                        if (selectedButtonIndex == 0) {
                            showMessageEscape = false;
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
            windowss.clear();
            // Выбор состояния и его обработка
            switch (currentGameState) {
            case PASSING_CARDS:
                handlePassingCards(windowss, players, event, number_of_players);
                break;
            case FIRST_ROUND:
                handleFirstRound(windowss, players, event, number_of_players);
                break;
            case HEARTS_BROKEN:
                handleHeratsBroken(windowss, players, event, number_of_players);
                break;
            case NORMAL_ROUND:
                handleNormalRound(windowss, players, event, number_of_players);
                break;
            case END_GAME:
                handleEndGame(windowss, players, event, number_of_players);
                break;
            }

            

            // Отрисовка текущего состояния
            switch (currentGameState) {
            case PASSING_CARDS:
                draw_cards(players, windowss, current_player, number_of_players);
                if (show3CardSelectionMessage) //отрисовываем сообщение о выборе 3 карт
            {
                 // Обновление цвета кнопок в зависимости от выбранной
                buttonYes.setFillColor(selected3ButtonIndex == 0 ? sf::Color::Red : sf::Color::White);
                buttonNo.setFillColor(selected3ButtonIndex == 1 ? sf::Color::Red : sf::Color::White);
                windowss.draw(message_3_cards);
                windowss.draw(buttonYes);
                windowss.draw(buttonNo);
            }
                break;
            case FIRST_ROUND:
                draw_cards(players, windowss, current_player, number_of_players);
                
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
                    draw_cards(players, windowss, current_player, number_of_players);
                }
                break;
            case NORMAL_ROUND:
                draw_cards(players, windowss, current_player, number_of_players);
                break;
            case END_GAME:
                if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right) {
                    selectedloseButtonIndex = 1 - selectedloseButtonIndex; // Переключение между кнопками
                } else if (event.key.code == sf::Keyboard::Enter) {
                    if (selectedloseButtonIndex == 0) {
                        // Игрок выбрал "Yes"
                        for (Cards& card : players[0].hand) {
                            card.sprite.setColor(sf::Color::White); // тут выбранные карты красим обратно в белый
                                shuffle(players);
                                penalty_points[0] = 0;
                                penalty_points[1] = 0;
                                penalty_points[2] = 0;
                                penalty_points[3] = 0;
                                currentGameState = PASSING_CARDS;
                        }
                    }
                     else {
                        return 0;
                    }
                }
            }
                break;
            }
            
            // Отрисовка текстового сообщения, если оно активно
            if (showMessageEscape) {
                if (selectedButtonIndex == 0)
                {
                    // Изменяем цвет выделенной кнопки
                        buttonYes.setFillColor(Color::Red);
                        buttonNo.setFillColor(Color::White);
                }
                else
                {
                    buttonYes.setFillColor(Color::White);
                    buttonNo.setFillColor(Color::Red);
                }
                windowss.draw(message_escape);
                windowss.draw(buttonYes);
                windowss.draw(buttonNo);
            }


            // Отрисовка меню сохранения/загрузки
        if (showSaveLoadMenu) {
            RectangleShape dark_background_sprite(Vector2f(2000, 2000));
            dark_background_sprite.setFillColor(Color(0, 0, 0, 100));
            windowss.draw(dark_background_sprite);
            windowss.draw(saveText);
            windowss.draw(loadText);
            windowss.draw(buttonYes);
            windowss.draw(buttonNo);
        }
            windowss.display();
        }
    }
    return 0;
}