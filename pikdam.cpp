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
    bool hasWon = false; // Добавляем переменную для отслеживания победы
    // virtual ~Player() {}
    // virtual void handleKeyPress(sf::Event& event, Player& currentPlayer, std::vector<Player>& Player, int& selectedCardIndex, std::vector<int>& selected_Card, bool& cardTaken, int& selectedButtonIndex, bool& showMessageEscape, bool& showCardSelectionMessage, bool& canSelectCards, bool& canTakeCard, bool& returnToMenu, int& selectedCardIndexRight, int& neighborIndex, sf::RenderWindow& windowss, sf::Text& message_ispair, int& playersChoseNo, bool& showMessageWait, int& shift) = 0;
};
class Button {
public:
    Button(const std::string& text, const sf::Vector2f& size, const int charSize, const sf::Color& bgColor, const sf::Color& textColor, sf::Font& font_1) {
        // Установка формы кнопки
        buttonShape.setSize(size);
        buttonShape.setFillColor(bgColor);

        // Установка текста кнопки
        buttonText.setFont(font_1);
        buttonText.setString(text);
        buttonText.setCharacterSize(charSize);
        buttonText.setFillColor(textColor);

        // Центрирование текста на кнопке
        sf::FloatRect textRect = buttonText.getLocalBounds();
        buttonText.setOrigin(textRect.left + textRect.width / 2.0f,
                             textRect.top + textRect.height / 2.0f);
        buttonText.setPosition(sf::Vector2f(size.x / 2.0f, size.y / 2.0f));
    }

    // Функция для установки позиции кнопки
    void setPosition(const sf::Vector2f& pos) {
        buttonShape.setPosition(pos);
        buttonText.setPosition(pos.x + buttonShape.getSize().x / 2.0f, pos.y + buttonShape.getSize().y / 2.0f);
    }

    // Функция для отрисовки кнопки
    void draw(sf::RenderWindow& window) {
        window.draw(buttonShape);
        window.draw(buttonText);
    }

    // // Функция для проверки, нажата ли кнопка
    // bool isClicked(const sf::Vector2i& mousePos) {
    //     return buttonShape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
    // }
    // для состояния выделения
    bool isSelected;
    // Функция для установки состояния выделения
    void setSelected(bool select) {
        isSelected = select;
        if (isSelected) {
            buttonShape.setFillColor(sf::Color::Black); // Выделенная кнопка будет черного цвета
        } else {
            buttonShape.setFillColor(sf::Color::Transparent); // Обычный цвет кнопки
        }
    }

private:
    sf::RectangleShape buttonShape;
    sf::Text buttonText;
};
// class HumanPlayer : public Player { 
// public:

// };

class Bot : public Player {
public:
    // Функция для автоматического взятия карты у игрока справа
    void takeCardFromPlayer(Player& rightPlayer) {
        if (!rightPlayer.hand.empty()) {
            std::random_device rd;
            std::mt19937 rng(rd());
            std::uniform_int_distribution<int> uni(0, rightPlayer.hand.size() - 1);

            int random_index = uni(rng); // Получаем случайный индекс
            Card cardTaken = rightPlayer.hand[random_index]; // Берем карту
            rightPlayer.hand.erase(rightPlayer.hand.begin() + random_index); // Удаляем карту из руки игрока
            hand.push_back(cardTaken); // Добавляем карту в руку бота
        }
    }

    // Функция для автоматического сброса пар карт
    void discardPairs() {
        std::set<int> indicesToDiscard;

        // Поиск пар карт
        for (size_t i = 0; i < hand.size(); ++i) {
            for (size_t j = i + 1; j < hand.size(); ++j) {
                if (hand[i].value == hand[j].value) {
                    indicesToDiscard.insert(i);
                    indicesToDiscard.insert(j);
                }
            }
        }

        // Сброс пар карт
        for (auto it = indicesToDiscard.rbegin(); it != indicesToDiscard.rend(); ++it) {
            hand.erase(hand.begin() + *it);
        }
    }
};

bool pair_of_cards(const Card& card1, const Card& card2) {
    if((card1.suit == "SPADES" && card1.value == 12) || (card2.suit == "SPADES" && card2.value == 12))
    {
        return false;
    
    }
    return card1.value == card2.value;
}
// Функция для поиска текущего игрока, чей ход необходимо выполнить
int findCurrentPlayer(const std::vector<Player>& players) {
    for (int i = 0; i < players.size(); ++i) {
        if (!players[i].hand.empty() && !players[i].hasWon) {
            if (players[i].isAttacker)
            {
                return i; // Возвращает индекс текущего атакующего игрока
            }
        }
    }
    return -1; // Все игроки выиграли
}
int findNextPlayerWithCards(const std::vector<Player>& players, int currentIndex) {
    int nextIndex = (currentIndex + 1) % players.size();
    while (players[nextIndex].hand.empty() && nextIndex != currentIndex) {
        // Пропускаем игроков без карт
        nextIndex = (nextIndex + 1) % players.size();
    }
    return nextIndex;
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
    // основная колода 
    std::vector<Card> deck;
    sf::Texture CardTexture;
    for (int value = 6; value <= 14; ++value) {
        for (const std::string& suit : suits) {
            // Исключаем даму крести
            if (value == 12 && suit == "CLUBS") {
                continue; // Пропускаем добавление карты "Дама Пик"
            }
            Card card;
            card.value = value;
            card.suit = suit;
            std::string filename = "resources/" + std::to_string(value) + suit + ".png";
            if (!CardTexture.loadFromFile(filename)) { 
                std::cerr << "Ошибка загрузки текстуры: " << filename << std::endl; 
            } else {
                card.texture = CardTexture; 
                card.sprite.setTexture(card.texture);
                deck.push_back(card);
            }
        }
    }

    // Перемешивание колоды
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);


    // Инициализация игроков
    // std::vector<Player> Player;
    // Player.push_back(HumanPlayer()); // Предполагаем, что первый игрок - человек
    // for (int i = 1; i < 4; ++i) {
    //     Player.push_back(Bot()); // Остальные игроки - боты
    // }
    std::vector<Player> Player(4);
    Player[0].isAttacker = true;
    Player[0].isDefender = false;
    Player[1].isAttacker = false;
    Player[1].isDefender = false;
    Player[2].isAttacker = false;
    Player[2].isDefender = false;
    Player[3].isAttacker = false;
    Player[3].isDefender = false;
    Bot& bot_1 = static_cast<Bot&>(Player[1]);
    Bot& bot_2 = static_cast<Bot&>(Player[2]);
    Bot& bot_3 = static_cast<Bot&>(Player[3]);

    //раздача карт на руки
    // for (int i = 0; i < 13; ++i) //для от 2
    for (int i = 0; i < 9; ++i)
    {
        if (deck.empty()) {
            std::cerr << "Колода пуста. Невозможно продолжить раздачу карт." << std::endl;
            break; // Выход из цикла, если колода пуста
        }
        Player[0].hand.push_back(deck.back());
        deck.pop_back();

        if (deck.empty()) {
            std::cerr << "Колода пуста. Невозможно продолжить раздачу карт." << std::endl;
            break;
        }
        Player[1].hand.push_back(deck.back());
        deck.pop_back();

        if (deck.empty()) {
            std::cerr << "Колода пуста. Невозможно продолжить раздачу карт." << std::endl;
            break;
        }
        Player[2].hand.push_back(deck.back());
        deck.pop_back();

        if (deck.empty()) {
            std::cerr << "Колода пуста. Невозможно продолжить раздачу карт." << std::endl;
            break;
        }
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
    bool messege_win_player = false;
    sf::Text messege_winplayer;
    messege_winplayer.setFont(font_1);
    messege_winplayer.setCharacterSize(24);
    messege_winplayer.setFillColor(sf::Color::White);
    messege_winplayer.setPosition(100.f, 100.f);

    // Текст вопроса
    messege_winplayer.setString(L"The Player Won ");

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
    // int shift = 0; // Начальное значение смещения


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
    // RectangleShape gameOverTexture(Vector2f(300,200));
    // RectangleShape youWinTexture(Vector2f(300,200));
    if (!youWinTexture.loadFromFile("resources/YouWin.png")) {
        std::cerr << "Ошибка загрузки YouWin!" << std::endl;
    }
    if (!gameOverTexture.loadFromFile("resources/GameOver.png")) {
        std::cerr << "Ошибка загрузки GameOver!" << std::endl;
    }


    sf::Sprite endGameSprite;
    endGameSprite.setPosition(760, 270);
    

    // Создаем кнопки 
    Button menuButton("Return to menu", sf::Vector2f(400, 50), 30, sf::Color::Transparent, sf::Color::White, font_1);
    Button restartButton("Restart Game", sf::Vector2f(400, 50), 30, sf::Color::Transparent, sf::Color::White, font_1);
    // Установка позиции кнопок
    menuButton.setPosition(sf::Vector2f(760, 440));
    restartButton.setPosition(sf::Vector2f(760, 540));
    int selectedButtonIndex_2 = 0; // Индекс выбранной кнопки
    std::vector<Button> buttons = {menuButton, restartButton}; // Вектор всех кнопок

///////////// боты скидывают карты
    bot_1.discardPairs();
    bot_2.discardPairs();
    bot_3.discardPairs();
    // Game Loop
    while (windowss.isOpen()) 
    {
        // Обновляем индекс текущего игрока
        currentPlayerIndex = findCurrentPlayer(Player);
        neighborIndex =findNextPlayerWithCards(Player, currentPlayerIndex);// (currentPlayerIndex + 1) % Player.size(); // Индекс соседа справа
        sf::Event event;
        while (windowss.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed && event.key.code == Keyboard::Escape)
            {
                showMessageEscape = true;
                showCardSelectionMessage = false;
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
                        return 0; break;
                    }
                    else if(selectedButtonIndex_2 == 1)// Проверка, выбрана ли кнопка "Restart"
                    {

                    }

                }

                // if (event.key.code == sf::Keyboard::Up)
                // {
                //     if (selectedButtonIndex_2 > 0) {
                //         buttons[selectedButtonIndex_2].setSelected(false);
                //         selectedButtonIndex_2--;
                //         buttons[selectedButtonIndex_2].setSelected(true);
                //     }
                // }
                // else if (event.key.code == sf::Keyboard::Down) {
                //     if (selectedButtonIndex_2 < buttons.size() - 1) {
                //         buttons[selectedButtonIndex_2].setSelected(false);
                //         selectedButtonIndex_2++;
                //         buttons[selectedButtonIndex_2].setSelected(true);
                //     }
                // } 
                // else if (event.key.code == sf::Keyboard::Enter) {
                //     // Обработка нажатия выбранной кнопки
                //     // if (buttons[selectedButtonIndex].isClicked(sf::Mouse::getPosition(windowss))) {
                //     //     // Действие для выбранной кнопки
                //     // }
                //     if (selectedButtonIndex_2 == 0) // Проверка, выбрана ли кнопка "Да"
                //     {
                //     }
                //     else if(selectedButtonIndex_2 == 1)
                //     {

                //     }
                // }

            }
            if (event.type == sf::Event::KeyPressed && (showCardSelectionMessage || showMessageEscape))
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
                        else if (showCardSelectionMessage && selectedButtonIndex == 1 && cardTaken) // Проверка, выбрана ли кнопка "Нет"
                        {
                            showCardSelectionMessage = false; // Сброс флага после выбора кнопки "Нет"
                            selected_Card.clear(); // Очистка выбора карт
                            move_transition = true;
                        }
                        else if(showCardSelectionMessage && selectedButtonIndex == 1 && !canTakeCard )  // Если выбрано "Нет"
                        {
                            // playersChoseNo++; так надо
                            playersChoseNo = Player.size(); // Для тестирования устанавливаем, что все игроки выбрали "нет"
                            canSelectCards = false; // Блокируем выбор карт
                            showMessageWait = true; // Активация текста ожидания
                            showCardSelectionMessage = false;
                            if (playersChoseNo == Player.size()) 
                            { // Если все игроки выбрали "нет"
                                showMessageWait = false; // Деактивация текста ожидания
                                canTakeCard = true; // Позволяем взятие карты у противника
                                // cardTaken = false; // Сброс флага взятия карты
                            }
                        }
                }
            }  
            // Переход к предыдущей карте с круговым циклом
            else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left && (canTakeCard && !cardTaken))
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
            else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right && (canTakeCard && !cardTaken))
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
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && (canTakeCard && !cardTaken)) 
            {
                // Если не выбрана, добавляем в вектор и меняем её цвет на красный
                selected_Card.push_back(selectedCardIndexRight);
                // Взятие выбранной карты
                Card selectedCard = Player[neighborIndex].hand[selectedCardIndexRight ];
                Player[neighborIndex].hand.erase(Player[neighborIndex].hand.begin() + selectedCardIndexRight );
                Player[currentPlayerIndex].hand.push_back(selectedCard);
                cardTaken = true; // Установка флага взятия карты
                if(Player[0].isAttacker)
                {
                    showCardSelectionMessage = true;  
                } 
                else
                {
                        showCardSelectionMessage = false;  
                } 

                selected_Card.clear(); // Очистка выбора карт
                crdpair = true;

            }
        
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left && crdpair)
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
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right && crdpair)
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
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && crdpair)
            {
                if (showCardSelectionMessage && selectedButtonIndex == 0) // Проверка, выбрана ли кнопка "Да"
                {
                    showCardSelectionMessage = false; // Сброс флага после выбора кнопки "Да"
                    selected_Card.clear(); // Очистка выбора карт
                }
                else if (showCardSelectionMessage && selectedButtonIndex == 1 && cardTaken) // Проверка, выбрана ли кнопка "Нет"
                {
                    showCardSelectionMessage = false; // Сброс флага после выбора кнопки "Нет"
                    selected_Card.clear(); // Очистка выбора карт
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
                    // showCardSelectionMessage = true;
                    selected_Card.clear(); // Очистка выбора карт

                }
                else if (!showCardSelectionMessage && !showMessageEscape)
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
            }          
            else if (currentPlayerIndex == 0) //пока только 0 человек
            {
            switch (event.type)
            {
                // case sf::Event::Closed:
                //     windowss.close();
                //     break;
                case sf::Event::KeyPressed:
                
    //             if (dynamic_cast<HumanPlayer*>(Player[currentPlayerIndex]) != nullptr) {
    //                 // Если текущий игрок - человек
    //                 handleHumanKeyPress(event, Player[currentPlayerIndex], Player, selectedCardIndex, selected_Card, cardTaken, selectedButtonIndex, showMessageEscape, showCardSelectionMessage, canSelectCards, canTakeCard, returnToMenu, selectedCardIndexRight, neighborIndex, windowss, message_ispair, playersChoseNo, showMessageWait, shift);
    //             } else if (dynamic_cast<Bot*>(Player[currentPlayerIndex]) != nullptr) {
    //                 // Если текущий игрок - бот
    //                 handleBotKeyPress(event, Player[currentPlayerIndex], Player, selectedCardIndex, selected_Card, cardTaken, selectedButtonIndex, showMessageEscape, showCardSelectionMessage, canSelectCards, canTakeCard, returnToMenu);
    //             }
    //             break;

    //         default:
    //             break;
    //     }
    // }
                    if (event.key.code == sf::Keyboard::Left )
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
                    else if (event.key.code == sf::Keyboard::Right)
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
                    else if (event.key.code == sf::Keyboard::Enter)
                    {
                        if (!showCardSelectionMessage && !showMessageEscape)
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
                    }
                    break;

                default:
                    break;
                }
            }
        }
        if(returnToMenu)
        {
            return 0; break;
        }
        if(move_transition)
        {
            do {
                // Передача хода следующему игроку
                currentPlayerIndex = (currentPlayerIndex + 1) % Player.size();
            } while (Player[currentPlayerIndex].hand.empty()); // Проверка, есть ли у игрока карты

            cardTaken = false; // Сброс флага взятия карты

            // Обновление статуса игроков
            for (int i = 0; i < Player.size(); ++i) {
                Player[i].isAttacker = (i == currentPlayerIndex);
            }
        }
        // Если текущий игрок - бот, выполняем его действия
        if (currentPlayerIndex != 0) {
            Bot& currentBot = static_cast<Bot&>(Player[currentPlayerIndex]);
            neighborIndex = findNextPlayerWithCards(Player, currentPlayerIndex);
            currentBot.takeCardFromPlayer(Player[neighborIndex]);
            currentBot.discardPairs();
            move_transition = true; // Передаем ход следующему игроку
            if(currentPlayerIndex == 3)
            {
                move_transition = false;
                canTakeCard = true;
                cardTaken = false;

                currentPlayerIndex = (currentPlayerIndex + 1) % Player.size();
                // Обновление статуса игроков
                for (int i = 0; i < Player.size(); ++i) {
                    Player[i].isAttacker = (i == currentPlayerIndex);
                }
            }
        }
        if (Player[currentPlayerIndex].hand.empty()) {
            Player[currentPlayerIndex].hasWon = true; // Игрок выиграл
        }
        activePlayers = 0;
        for (const auto& player : Player) {
            if (!player.hand.empty()) {
                activePlayers++;
            }
        }

        if (activePlayers == 1 || Player[0].hand.empty()) {
            // Только один игрок остался с картами, игра окончена
            EndGame = true;
        }
                // Проверяем, не закончилась ли игра
        if (EndGame) {
            //окончание игры 
            if(Player[0].hand.empty())
            {
                YouWin = true;
            }
            else
            {
                GameOver = true;
            }
            showCardSelectionMessage = false;
            showMessageEscape = false;
        }
        windowss.clear(Color(50,200,150));
        windowss.draw(background);

        // Отрисовка карт игроков

        Texture back_card_texture;
        if (!back_card_texture.loadFromFile("resources/back.png")) { return 1; }
        RectangleShape back_card_sprite(Vector2f(77,110));
        back_card_sprite.setTexture(&back_card_texture);
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
            if (Player[playerIndex].hasWon) {
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
            for (size_t cardIndex = 0; cardIndex < Player[playerIndex].hand.size(); ++cardIndex) {
                Card& card = Player[playerIndex].hand[cardIndex];
                if (playerIndex == 0)//Player[playerIndex].isAttacker
                {
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
         // Если игра закончилась
        if (EndGame) {
            // Определяем, какое изображение использовать
            if (GameOver) {
                endGameSprite.setTexture(gameOverTexture);
                
                // Определяем прямоугольник размером 300x200 пикселей
                sf::IntRect textureRect(0, 0, 300, 200);

                // Устанавливаем область текстуры для спрайта
                endGameSprite.setTextureRect(textureRect);
            } else if (YouWin) {
                // Устанавливаем текстуру для спрайта
                endGameSprite.setTexture(youWinTexture);

                // Определяем прямоугольник размером 300x200 пикселей
                sf::IntRect textureRect(0, 0, 300, 200);

                // Устанавливаем область текстуры для спрайта
                endGameSprite.setTextureRect(textureRect);
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
        // Drawing code for graphics can be added here
        windowss.display();
        
    }
    return 0;
}
