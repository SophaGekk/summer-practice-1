#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <set>
using namespace sf;

class Cards {
public:
    int rank; // 6-14 (6 to Ace)
    std::string suit; //  (Spades, Clubs, Diamonds, Hearts)
    sf::Texture texture; 
    sf::Sprite sprite; 
    bool isTaking = false; //для реализации взятия карты
};

class Players {
public:
    std::vector<Cards> hand;
    bool isAttacker;
    bool isDefender;
};


void share_cards(std::vector<Players> players, int num_of_player, std::vector<int> selected_cards)
{
    if(num_of_player == 0)
    {
        players[num_of_player-1].hand.push_back(players[num_of_player].hand[selected_cards[0]]);
        players[num_of_player-1].hand.push_back(players[num_of_player].hand[selected_cards[1]]);
        players[num_of_player-1].hand.push_back(players[num_of_player].hand[selected_cards[3]]);
    }
    else
    {
        players[3].hand.push_back(players[0].hand[selected_cards[0]]);
        players[3].hand.push_back(players[0].hand[selected_cards[1]]);
        players[3].hand.push_back(players[0].hand[selected_cards[3]]);
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

int selectedCardsIndex = 0;
std::vector<int> selected_cards(0);
 //   int trumpSuit = deck.back().suit; // Suit of the last card in deck is the trump suit

// Game Loop
while (windowss.isOpen()) {
    Event event;
    while (windowss.pollEvent(event)) 
    {
    // Game logic goes here
    switch(event.type)
    {
    // закрытие окна
        case sf::Event::Closed:
            windowss.close();
            break;
        case Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Left) 
            {
                // Выбираем предыдущую карту (если возможно)
                if (selectedCardsIndex > 0) 
                {
                    --selectedCardsIndex;
                }
            }
                
            else if (event.key.code == sf::Keyboard::Right) 
            {
                // Выбираем следующую карту (если возможно)
                if (selectedCardsIndex < players[0].hand.size() - 1) 
                {
                    ++selectedCardsIndex;
                }
            } 
            else if (event.key.code == Keyboard::Enter) {
                        // Добавляем выбранную карту в вектор selected_cards
                        selected_cards.push_back(selectedCardsIndex);
                    }
            break;
        case Event::KeyReleased:
            if (event.key.code == sf::Keyboard::Left) {}
            else if (event.key.code == sf::Keyboard::Right){}
            else if (event.key.code == sf::Keyboard::Enter){}
        default:
        break;
    }

    windowss.clear(Color(50,200,150));
     windowss.draw(background);
    // int id_P1 = 1;
    // for (Cards& card : players[0].hand) {
        
    //     card.sprite.setTexture(card.texture);
    //     card.sprite.setPosition(Vector2f(200+(1300/players[0].hand.size())*id_P1 ,800));
    //     windowss.draw(card.sprite);
    //     id_P1++;
    // }
    
    
        // Отрисовка карты игрока
        int id_P1 = 1; 
        for (size_t i = 0; i < players[0].hand.size(); ++i)
        { 
            Cards& card = players[0].hand[i]; card.sprite.setTexture(card.texture);
            // Определение позиции карты в зависимости от её индекса 
            card.sprite.setPosition(Vector2f(200 + (1300 / players[0].hand.size()) * id_P1, 800)); 
            // Если карта выбрана, она рисуется в желтый цвет 
            if (i == selectedCardsIndex) 
            { 
                if(i!=0){card.sprite.setColor(Color::Magenta); }
                else if((i == 0) && (std::find(selected_cards.begin(), selected_cards.end(), i) == selected_cards.end()))
                {
                    card.sprite.setColor(Color::Magenta); 
                }
            } 
            else if (std::find(selected_cards.begin(), selected_cards.end(), i) == selected_cards.end()) 
            { 
            card.sprite.setColor(Color::White);
             // сбрасываем цвет для не выбранных карт
            } 
            windowss.draw(card.sprite);
            ++id_P1; 
        }
    int id_P2 = 1;
    Texture back_card_texture;
    if (!back_card_texture.loadFromFile("resources/back.png")) { return 1; }
    RectangleShape back_card_sprite(Vector2f(77,110));
    back_card_sprite.setTexture(&back_card_texture);

    for (Cards& card : players[1].hand) {
        back_card_sprite.setPosition(Vector2f(200+(1300/players[0].hand.size())*id_P2 ,150));
        windowss.draw(back_card_sprite);
        id_P2++;
    }
    // Drawing code for graphics can be added here
    windowss.display();
    }
}
return 0;
}