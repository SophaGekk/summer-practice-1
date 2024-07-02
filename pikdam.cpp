#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <set>
#include <random>
using namespace sf;

class Card {
public:
    int value; // 6-14 (6 to Ace)
    std::string suit; //  (Spades, Clubs, Diamonds, Hearts)
    sf::Texture texture; 
    sf::Sprite sprite; 
    bool isTaking = false; //для реализации взятия карты
};

class Player {
public:
    std::vector<Card> hand;
    bool isAttacker;
    bool isDefender;
};

bool pair_of_cards(const Card& card1, const Card& card2) {
    // Сравниваем значения карт, игнорируя масть
    if((card1.suit != "Spades" && card1.value != 12)|| (card2.suit != "Spades" && card2.value != 12))
    {
        return card1.value == card2.value;
    }
    return false;
}

// Функция для определения индекса текущего атакующего игрока
int findCurrentPlayer(std::vector<Player> players)
{
    for (int i = 0; i < players.size(); ++i)
    {
        if (players[i].isAttacker)
        {
            return i; // Возвращает индекс текущего атакующего игрока
        }
    }
    return -1; // Если атакующий игрок не найден
}
// Глобальная переменная для индекса текущего игрока
int currentPlayerIndex = 0;
int main_pikgame(sf::RenderWindow& windowss) {

    srand(time(0));

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
    std::vector<Card> deck;
    sf::Texture CardTexture;
    for (int value = 6; value <= 14; ++value) {
        for (std::string suit : suits) {
            Card card;
            card.value = value;
            card.suit = suit;
            std::string filename = "resources/" + std::to_string(value) + suit + ".png";
            if (!CardTexture.loadFromFile(filename)) { 
                std::cerr << "Ошибка загрузки текстуры: " << filename << std::endl; 
            } else if(filename != "resources/12CLUBS") //исключаем даму крести
            {
                card.texture = CardTexture; 
                card.sprite.setTexture(card.texture);
            }
            deck.push_back(card);
        }
    }

    // Перемешивание колоды
std::random_device rd;
std::mt19937 g(rd());
std::shuffle(deck.begin(), deck.end(), g);

    std::vector<Player> Player(4);
    Player[0].isAttacker = true;
    Player[0].isDefender = false;
    Player[1].isAttacker = false;
    Player[1].isDefender = false;
    Player[2].isAttacker = false;
    Player[1].isDefender = false;
    Player[3].isAttacker = false;
    Player[1].isDefender = false;

    //раздача карт на руки
    // for (int i = 0; i < 13; ++i) //для от 2
    for (int i = 0; i < 9; ++i)
    {
        Player[0].hand.push_back(deck.back());
        deck.pop_back();
        Player[1].hand.push_back(deck.back());
        deck.pop_back();
        Player[2].hand.push_back(deck.back());
        deck.pop_back();
        Player[3].hand.push_back(deck.back());
        deck.pop_back();
    }

    int selectedCardIndex = 0;
    int selectedCardIndexRight = 0;

    std::vector<int> selected_Card(0);

    
    sf::Font font_1;
    if (!font_1.loadFromFile("resources/ffont.ttf"))
    {
        std::cerr << "Ошибка загрузки шрифта!" << std::endl;
    }
    sf::Text message_ispair;
    message_ispair.setFont(font_1);
    message_ispair.setCharacterSize(24);
    message_ispair.setFillColor(sf::Color::White);
    message_ispair.setPosition(100.f, 100.f);

    // Текст вопроса
    // message_ispair.setString(L"Увы, эти карты сбросить не получиться...");
    message_ispair.setString(L"Oh, you will not be able to discard these cards...");

    sf::Text message;
    message.setFont(font_1);
    message.setCharacterSize(30);
    message.setFillColor(sf::Color::White);
    message.setPosition(100.f, 100.f);

    // Текст вопроса
    // message.setString("Есть ли что сбросить?");
     message.setString("Is there anything to reset?");
    // Создание кнопок "Да" и "Нет"
    sf::Text buttonYes;
    buttonYes.setFont(font_1);
    // buttonYes.setString("Да");
    buttonYes.setString("Yes");
    buttonYes.setCharacterSize(24);
    buttonYes.setFillColor(sf::Color::White);
    buttonYes.setPosition(200.f, 200.f);

    sf::Text buttonNo;
    buttonNo.setFont(font_1);
    // buttonNo.setString("Нет");
    buttonNo.setString("No");
    buttonNo.setCharacterSize(24);
    buttonNo.setFillColor(sf::Color::White);
    buttonNo.setPosition(400.f, 200.f);

    // Флаг для отображения текста выбора карт
    bool showCardSelectionMessage = true;
    // Индекс выбранной кнопки (0 - Да, 1 - Нет)
    int selectedButtonIndex = 0;

    sf::Text message_escape;
    message_escape.setFont(font_1);
    message_escape.setCharacterSize(24);
    message_escape.setFillColor(sf::Color::White);
    message_escape.setPosition(100.f, 100.f);

    // Текст вопроса
    // message_escape.setString(L"Вы действительно хотите выйти?");
    message_escape.setString(L"Do you really want to get out?");
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


    bool Taking_cards = false; // Флаг для проверки, взял ли игрок карту
    bool canSelectCards = true; // Можно ли выбирать карты
    bool canTakeCard = false; // Можно ли брать карту у противника
    bool cardTaken = false; // Флаг, указывающий на то, что карта была взята
    // Game Loop
    while (windowss.isOpen()) 
    {
        if(returnToMenu)
        {
            return 0;
            break;
        }
        sf::Event event;
        while (windowss.pollEvent(event))
        {
            currentPlayerIndex = findCurrentPlayer(Player);
            int neighborIndex = (currentPlayerIndex + 1) % Player.size(); // Индекс соседа справа


            switch (event.type)
            {
                case sf::Event::Closed:
                    windowss.close();
                    break;

                case sf::Event::KeyPressed:
                    if (event.key.code == Keyboard::Escape)
                    {
                        showMessageEscape = true;
                        showCardSelectionMessage = false;

                    }
                    if(canTakeCard && !cardTaken)
                    {
                        // Переход к предыдущей карте с круговым циклом
                        if(event.key.code == sf::Keyboard::Left)
                        {                        
                            if (selectedCardIndexRight  > 0)
                            {
                                --selectedCardIndexRight ;
                            }
                            else
                            {
                                selectedCardIndexRight  = Player[neighborIndex].hand.size() - 1;
                            }
                        }
                        else if(event.key.code == sf::Keyboard::Right)
                        {
                            // Переход к предыдущей карте с круговым циклом
                            if (selectedCardIndexRight  < Player[neighborIndex].hand.size() - 1)
                            {
                                ++selectedCardIndexRight ;
                            }
                            else
                            {
                                selectedCardIndexRight  = 0;
                            }
                        }
                        else if (event.key.code == sf::Keyboard::Enter) 
                        {
                            // Если не выбрана, добавляем в вектор и меняем её цвет на красный
                            selected_Card.push_back(selectedCardIndexRight);
                            // Взятие выбранной карты
                            Card selectedCard = Player[neighborIndex].hand[selectedCardIndexRight ];
                            Player[neighborIndex].hand.erase(Player[neighborIndex].hand.begin() + selectedCardIndexRight );
                            Player[currentPlayerIndex].hand.push_back(selectedCard);
                            cardTaken = true; // Установка флага взятия карты
                            selected_Card.clear(); // Очистка выбора карт
                        }
                    }
                    if (event.key.code == sf::Keyboard::Left)
                    {
                        if (showCardSelectionMessage || showMessageEscape) // Проверка, находимся ли мы в режиме выбора кнопки
                        {
                            // Переключение на предыдущую кнопку
                            selectedButtonIndex = (selectedButtonIndex - 1 + 2) % 2; // Круговой цикл
                        }
                        else
                        {
                            // Переход к предыдущей карте с круговым циклом
                            if (selectedCardIndex > 0)
                            {
                                --selectedCardIndex;
                            }
                            else
                            {
                                selectedCardIndex = Player[currentPlayerIndex].hand.size() - 1;
                            }
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Right)
                    {
                        if (showCardSelectionMessage || showMessageEscape) // Проверка, находимся ли мы в режиме выбора кнопки
                        {
                            // Переключение на следующую кнопку
                            selectedButtonIndex = (selectedButtonIndex + 1) % 2;
                        }
                        else
                        {
                            // Переход к следующей карте с круговым циклом
                            if (selectedCardIndex < Player[currentPlayerIndex].hand.size() - 1)
                            {
                                ++selectedCardIndex;
                            }
                            else
                            {
                                selectedCardIndex = 0;
                            }
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Enter)
                    {
                        if (showCardSelectionMessage && selectedButtonIndex == 0) // Проверка, выбрана ли кнопка "Да"
                        {
                            showCardSelectionMessage = false; // Сброс флага после выбора кнопки "Да"
                            selected_Card.clear(); // Очистка выбора карт
                        }
                        else if(showMessageEscape && selectedButtonIndex == 0)
                        {
                            returnToMenu = true;
                            break;
                        }
                        else if(showMessageEscape && selectedButtonIndex == 1)
                        {
                            showMessageEscape = false;
                            showCardSelectionMessage = true;
                            selected_Card.clear(); // Очистка выбора карт

                        }
                        else if (!showCardSelectionMessage && !showMessageEscape && canSelectCards )
                        {
                            //(логика выбора и пролистывания карт)
                            
                            // Проверяем, выбрана ли уже карта
                            auto it = std::find(selected_Card.begin(), selected_Card.end(), selectedCardIndex);

                            // Проверяем, выбрана ли уже карта
                            if (it == selected_Card.end())
                            {
                                // Если не выбрана, добавляем в вектор и меняем её цвет на красный
                                selected_Card.push_back(selectedCardIndex);
                                
                                // Проверка выбрано ли две карты
                                if (selected_Card.size() == 2)
                                {
                                    // Вызов функции для проверки на пару карт
                                    bool is_pair = pair_of_cards(Player[currentPlayerIndex].hand[selected_Card[0]], Player[currentPlayerIndex].hand[selected_Card[1]]);
                                    if (currentPlayerIndex != -1)
                                    {
                                        // Вывод результата
                                        if (is_pair)
                                        {
                                            // Действия, если выбранные карты являются парой
                                            // Сортировка индексов выбранных карт по убыванию
                                            std::sort(selected_Card.rbegin(), selected_Card.rend());

                                            // Удаление парных карт из руки текущего игрока
                                            for (int index : selected_Card)
                                            {
                                                Player[currentPlayerIndex].hand.erase(Player[currentPlayerIndex].hand.begin() + index);
                                            }
                                            // Возврат к выбору кнопок
                                            showCardSelectionMessage = true;

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
                                            showCardSelectionMessage = true;
                                        }
                                        
                                        // Очистка списка выбранных карт
                                        selected_Card.clear();
                                    }
                                }
                            }
                            else
                            {
                                // Если уже выбрана, удаляем из вектора и меняем её цвет на обычный
                                selected_Card.erase(it);
                                Player[0].hand[selectedCardIndex].sprite.setColor(Color::White);
                            }

                        }
                        else if(showCardSelectionMessage && selectedButtonIndex == 1)  // Если выбрано "Нет"
                        {
                            // playersChoseNo++; так надо
                            playersChoseNo =Player.size(); // Для тестирования устанавливаем, что все игроки выбрали "нет"
                            canSelectCards = false; // Блокируем выбор карт
                            showMessageWait = true; // Активация текста ожидания
                            showCardSelectionMessage = false;
                            if (playersChoseNo == Player.size()) 
                            { // Если все игроки выбрали "нет"
                                showMessageWait = false; // Деактивация текста ожидания
                                canTakeCard = true; // Позволяем взятие карты у противника
                                cardTaken = false; // Сброс флага взятия карты
                            }
                        }
                        else if (cardTaken) {
                            // Передача хода следующему игроку
                            currentPlayerIndex = (currentPlayerIndex + 1) % Player.size();
                            cardTaken = false; // Сброс флага взятия карты
                            // Обновление статуса игроков
                            for (int i = 0; i < Player.size(); ++i) {
                                Player[i].isAttacker = (i == currentPlayerIndex);
                            }
                        }
                    }
                    break;

                default:
                    break;
            }

        }
        windowss.clear(Color(50,200,150));
        windowss.draw(background);

        // // отрисовка карт
        // int id_P1 = 1; 
        // for (size_t i = 0; i < Player[0].hand.size(); ++i)
        // { 
        //     Card& card = Player[0].hand[i]; 
        //     card.sprite.setTexture(card.texture);

        //     // Определение позиции карты в зависимости от её индекса 
        //     card.sprite.setPosition(Vector2f(200 + (1300 / Player[0].hand.size()) * id_P1, 800));
            
        //     // Если карта выбрана, она рисуется в желтый цвет
        //     if (std::find(selected_Card.begin(), selected_Card.end(), i) != selected_Card.end())
        //     {
        //         card.sprite.setColor(Color::Magenta);
        //     }
        //     else if (i == selectedCardIndex)
        //     {
        //         card.sprite.setColor(Color::Yellow);
        //     }
        //     else
        //     {
        //         card.sprite.setColor(Color::White);
        //     }

        //     windowss.draw(card.sprite);
        //     ++id_P1; 
        // }   
        Texture back_card_texture;
        if (!back_card_texture.loadFromFile("resources/back.png")) { return 1; }
        RectangleShape back_card_sprite(Vector2f(77,110));
        back_card_sprite.setTexture(&back_card_texture);

        // Отрисовка карт для каждого игрока
        for (int playerIndex = 0; playerIndex < Player.size(); ++playerIndex) {
            // Определение позиции и поворота карт в зависимости от индекса игрока
            // Vector2f position;
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

            // Отрисовка карт текущего игрока
            for (size_t cardIndex = 0; cardIndex < Player[playerIndex].hand.size(); ++cardIndex) {
                Card& card = Player[playerIndex].hand[cardIndex];
                if (Player[playerIndex].isAttacker) {
                    // Атакующий игрок видит значения своих карт
                    card.sprite.setTexture(card.texture);
                    // Определение позиции карты в зависимости от её индекса 
                    card.sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex].hand.size()) * id, 800));
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (std::find(selected_Card.begin(), selected_Card.end(), cardIndex) != selected_Card.end())
                    {
                        card.sprite.setColor(Color::Magenta);
                    }
                    else if (cardIndex == selectedCardIndex)
                    {
                        card.sprite.setColor(Color::Yellow);
                    }
                    else
                    {
                        card.sprite.setColor(Color::White);
                    }

                    windowss.draw(card.sprite);
                    ++id; 
                    
                } else {
                    // У остальных игроков карты рубашкой вверх
                    back_card_sprite.setRotation(rotation);
                    if (playerIndex == 1) {
                        back_card_sprite.setPosition(Vector2f(1820, 123 + (800 / Player[playerIndex].hand.size()) * id));
                    } else if (playerIndex == 2) {
                        back_card_sprite.setPosition(Vector2f(200 + (1300 / Player[playerIndex].hand.size()) * id, 150));
                    } else if (playerIndex == 3) {
                        back_card_sprite.setPosition(Vector2f(100, 200 + (800 / Player[playerIndex].hand.size()) * id));
                    }
                    windowss.draw(back_card_sprite);
                    id++;
                }
            }
        }
        // // Отрисовка карт игрока 1 (справа)
        // int  id_P2 = 1;
        // for (Card& card : Player[1].hand) {
        //     // card.sprite.setTexture(card.texture);
        //     back_card_sprite.setRotation(90); // Поворот карты на 90 градусов
        //     back_card_sprite.setPosition(Vector2f(1820, 123 + (800 / Player[1].hand.size()) * id_P2));
        //     windowss.draw(back_card_sprite);
        //     ++id_P2;
        // }
        // // Отрисовка карт игрока 2 (сверху)
        // int id_P3 = 1;
        // for (Card& card : Player[2].hand) {
        //     back_card_sprite.setRotation(0); // Поворот карты на 90 градусов
        //     back_card_sprite.setPosition(Vector2f(200+(1300/Player[2].hand.size())*id_P3 ,150));
        //     windowss.draw(back_card_sprite);
        //     id_P3++;
        // }


        // // Отрисовка карт игрока 3 (слева)
        // int id_P4 = 1;
        // for (Card& card : Player[3].hand) {
        //     // card.sprite.setTexture(card.texture);
        //     back_card_sprite.setRotation(-90); // Поворот карты на -90 градусов
        //     back_card_sprite.setPosition(Vector2f(100, 200 + (800 / Player[3].hand.size()) * id_P4));
        //     windowss.draw(back_card_sprite);
        //     ++id_P4;
        // }

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

        if (showCardSelectionMessage)//showMessageEscape
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
            wait_message.setString("Ожидаем других игроков: " + std::to_string(Player.size() - playersChoseNo));
            // Рисование сообщения о выборе карт, если флаг установлен 
            float textX_wait_message = windowSize.x / 2.f - wait_message.getLocalBounds().width / 2.f;
            float textY_wait_message = (800 + 150) / 2.f - wait_message.getLocalBounds().height / 2.f;
            wait_message.setPosition(textX_wait_message, textY_wait_message);
        }
        
        // Drawing code for graphics can be added here
        windowss.display();
    }
    return 0;
}