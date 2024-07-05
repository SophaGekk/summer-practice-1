#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <set>
#include "HeartsBOT.h" //  Файл с логикой ботов

using namespace sf;



int selectedCardsIndex = 0;
std::vector<int> selected_cards(0);
Cards bito[4];

// Функция отрисовки карт всех игроков
void draw_cards(std::vector<Players>& players, sf::RenderWindow& windowss) {
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
    int id_P0 =1;
    for (size_t i = 0; i < players[0].hand.size(); ++i)
    { 
    Cards& card = players[0].hand[i]; 
    card.sprite.setTexture(card.texture);

    // Определение позиции карты в зависимости от её индекса 
    //игрок снизу
    card.sprite.setPosition(Vector2f(200 + (1300 / players[0].hand.size()) * id_P0, 900));
    
    // Если карта выбрана, она рисуется в желтый цвет
    if (std::find(selected_cards.begin(), selected_cards.end(), i) != selected_cards.end())
    {
        card.sprite.setColor(Color::Magenta);
    }
    else if (i == selectedCardsIndex)
    {
        card.sprite.setColor(Color::Yellow);
    }
    else
    {
        card.sprite.setColor(Color::White);
    }

    windowss.draw(card.sprite);
    ++id_P0; 
    }
    // далее напишем отрисовку четырех карт на столе 
    if (bito[0].suit != "")
    {bito[0].sprite.setTexture(bito[0].texture); bito[0].sprite.setPosition(Vector2f(920, 595)) ; windowss.draw(bito[0].sprite);}
    if (bito[1].suit != "")
    {bito[1].sprite.setTexture(bito[1].texture); bito[1].sprite.setPosition(Vector2f(920, 595)) ; windowss.draw(bito[1].sprite);}
    if (bito[2].suit != "")
    {bito[2].sprite.setTexture(bito[2].texture); bito[2].sprite.setPosition(Vector2f(920, 595)) ; windowss.draw(bito[2].sprite);}
    if (bito[3].suit != "")
    {bito[3].sprite.setTexture(bito[3].texture); bito[3].sprite.setPosition(Vector2f(920, 595)) ; windowss.draw(bito[3].sprite);}
}
// //функция для отрисовки четырёх карт в серидине экрана
// void draw_bito(int num_of_player,  sf::RenderWindow& windowss, Cards& card)
// {
//     if (num_of_player == 0){ card.sprite.setPosition(Vector2f(920, 595)) ; windowss.draw(card.sprite);}
//     if (num_of_player == 1){ card.sprite.setPosition(Vector2f(880, 485)) ; windowss.draw(card.sprite);}
//     if (num_of_player == 2){ card.sprite.setPosition(Vector2f(880, 375)) ; windowss.draw(card.sprite);}
//     if (num_of_player == 3){ card.sprite.setPosition(Vector2f(960, 485)) ; windowss.draw(card.sprite);}
// }

// Функция передачи карт от одного игрока к другому
void share_cards(std::vector<Players>& players, int num_of_player, std::vector<int> selected_cards) {
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
            players[num_of_player - 1].hand.erase(players[num_of_player - 1].hand.begin() + selected_cards[i]);
        }
    }
}
// Функция для отключения выбора карт
void disable_card_selection(Players& player, std::vector<int> selected_cards)
{
    // Сбрасываем выбор всех карт
    selected_cards.clear();

    // "Затемняем" все карты
    for (auto& card : player.hand)
    {
        card.sprite.setColor(Color::Black);
    }
}

// Функция для включения выбора карт
void enable_card_selection(Players& player)
{
    // Восстанавливаем цвет всех карт
    for (auto& card : player.hand)
    {
        card.sprite.setColor(Color::White);
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
    RectangleShape background(Vector2f(1920,1080));
    background.setTexture(&tableTexture);
    background.setPosition(Vector2f(0,0));


    std::vector<std::string> suits = {"HEARTS", "DIAMONDS", "CLUBS", "SPADES"};
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
    for (int i = 0; i < 9; ++i)
    {
        players[0].hand.push_back(deck.back());
        deck.pop_back();
        players[1].hand.push_back(deck.back());
        deck.pop_back();
        players[2].hand.push_back(deck.back());
        deck.pop_back();
        players[3].hand.push_back(deck.back());
        deck.pop_back();
    }


int current_player = 0; //  Текущий игрок
int first_round = 0; //  Флаг для первого раунда (обмен карт)

Event event;
Event main_game;
// Game Loop
while (windowss.isOpen()) {
    
    while (windowss.pollEvent(event)) 
    {
    // Game logic goes here
switch (event.type)
{
    case sf::Event::Closed:
        windowss.close();
        break;

    case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::Left) 
        {
            // Переход к предыдущей карте с круговым циклом
            if (selectedCardsIndex > 0) 
            {
                --selectedCardsIndex;
            }
            else 
            {
                selectedCardsIndex = players[0].hand.size() - 1;
            }
        }
        else if (event.key.code == sf::Keyboard::Right) 
        {
            // Переход к следующей карте с круговым циклом
            if (selectedCardsIndex < players[0].hand.size() - 1) 
            {
                ++selectedCardsIndex;
            }
            else 
            {
                selectedCardsIndex = 0;
            }
        } 
        else if (event.key.code == sf::Keyboard::Enter) 
        {
            // Проверяем, выбрана ли уже карта
            auto it = std::find(selected_cards.begin(), selected_cards.end(), selectedCardsIndex);
            
            if (it == selected_cards.end())
            {
                // Если не выбрана, добавляем в вектор и меняем её цвет на красный
                selected_cards.push_back(selectedCardsIndex);
                players[0].hand[selectedCardsIndex].sprite.setColor(Color::Magenta);

                // Проверяем, выбраны ли уже 3 карты
                if ( first_round == 0 and selected_cards.size() == 3) {
                    for (auto& card : players[0].hand) {
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
            }
            else
            {
                // Если уже выбрана, удаляем из вектора и меняем её цвет на обычный
                selected_cards.erase(it);
                players[0].hand[selectedCardsIndex].sprite.setColor(Color::White);
            }
        }
        break;

    case sf::Event::KeyReleased:
        if (event.key.code == sf::Keyboard::Left) {}
        if (event.key.code == sf::Keyboard::Right) {}
        if (event.key.code == sf::Keyboard::Enter) {}
        break;

    default:
        break;
}


// Отрисовка карт
windowss.clear(Color::White);
windowss.draw(background);
draw_cards(players, windowss);
windowss.display();


if (first_round == 0) {
    // Первый раунд (обмен карт)
    
    if (current_player != 0) {
        sf::sleep(sf::seconds(1)); 
        std::vector<int> bot_cards(0);
        share_card_turn(players, current_player, bot_cards);
        share_cards(players, (current_player + 1) % 4, bot_cards);
        current_player = (current_player + 1) % 4;
        
        // Если все игроки сделали первый ход, переходим ко второму раунду
        if (current_player == 0) {
            windowss.clear(Color::White);
            windowss.draw(background);
            draw_cards(players, windowss);
            windowss.display();
            sf::sleep(sf::seconds(1));
            first_round++;
        }
    }
} 


// Программа для второго хода с двойкой крести
if (first_round == 1) // first_round - индекс хода
{
    bool found_clubs = false;
    // Находим игрока с двойкой крести
    for (int player_ind = 0; player_ind < 4; player_ind++)
    {
        
        if (found_clubs == true)
        { break;}
        int ID = 0;
        for (Cards& card : players[player_ind].hand)
        {
            if (card.rank == 6 && card.suit == "CLUBS") 
            {
                // Если игрок - бот, он сразу кладет карту на стол
                if (player_ind != 0) 
                {
                    current_player = player_ind;
                    // draw_bito(current_player, windowss, card); 
                    bito[current_player] = card;
                    // players[current_player].hand.erase(players[current_player].hand.begin() + ID);
                    current_player = (current_player + 1) % 4;
                    first_round++;
                    found_clubs = true;
                    break; 
                }
                else // Игрок 0 должен выложить двойку крести
                {
                    // Отключаем выбор карт для игрока 0
                    disable_card_selection(players[0], selected_cards); 

                    // Отображаем только двойку крести
                    for (size_t i = 0; i < players[0].hand.size(); ++i)
                    {
                        Cards& card = players[0].hand[i];
                        card.sprite.setTexture(card.texture);
                        
                        // Определяем позицию карты
                        card.sprite.setPosition(Vector2f(200 + (1300 / players[0].hand.size()) * i, 900));
                        
                        // Выделяем двойку крести желтым цветом
                        if (card.rank == 6 and card.suit == "CLUBS")
                        {
                            card.sprite.setColor(Color::Yellow);
                            bito[0] = card;
                        }
                        else
                        {
                            card.sprite.setColor(Color::Black); // "Затемняем" остальные карты
                        }
                        windowss.draw(card.sprite);
                    }

                    // Ждем нажатия Enter
                    while (true)
                    {
                        sf::Event event;
                        while (windowss.pollEvent(event))
                        {
                            if (event.type == sf::Event::KeyPressed)
                            {
                                if (event.key.code == sf::Keyboard::Enter) 
                                {
                                    // Выкладываем двойку крести на стол
                                    current_player = 1; // Переход к следующему игроку
                                    enable_card_selection(players[0]); // Восстанавливаем выбор карт
                                    first_round++;
                                    found_clubs = true;
                                    break; 
                                }
                            }
                        }
                        // windowss.display();
                    }
                    break; // Выходим из цикла поиска игрока с двойкой крести
                }
            }
            ID++;
        }
    }
    break;
}

    }
}
return 0;
}