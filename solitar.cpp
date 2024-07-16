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

#include "solitar.h"

using namespace sf;
bool isRedSuit(const Card& card) {
    return card.suit == "HEARTS" || card.suit == "DIAMONDS";
}

bool isBlackSuit(const Card& card) {
    return card.suit == "CLUBS" || card.suit == "SPADES";
}
bool canAddCardToColumn(int selectedCardIndex, std::vector<std::vector<Card>>& columns, std::vector<int>& selected_Card)
{
    // Получаем текущую карту
    Card currentCard = columns[selectedCardIndex].back();
    int currentCardValue = currentCard.value;

    // Получаем индекс выбранной карты
    int takeCardIndex = selected_Card.back();
    
    // Получаем выбранную карту
    Card takeCard = columns[takeCardIndex].back();    
    // Проверяем, можно ли добавить карту в конец этой колонки
    if (currentCardValue - takeCard.value == 1 && ((isRedSuit(currentCard) && isBlackSuit(takeCard)) || (isBlackSuit(currentCard) && isRedSuit(takeCard))))
    {
        return true;
    }
    return false;
}
void addCardToColumn(int selectedCardIndex, std::vector<std::vector<Card>>& columns, std::vector<int> selected_Card, std::vector<int>& opencard)
{
    // Получаем индекс выбранной карты
    int takeCardIndex = selected_Card.back();
    
    // Получаем выбранную карту
    Card takeCard = columns[takeCardIndex].back(); 

    // Удаляем выбранную карту из текущей колонки
    columns[takeCardIndex].pop_back();
    // Уменьшаем количество открытых карт, если удаляем карту из конца колонки
    if (columns[takeCardIndex].size() < opencard[takeCardIndex]) {
        opencard[takeCardIndex]--;
    }
    // Добавляем выбранную карту в конец целевой колонки
    columns[selectedCardIndex].push_back(takeCard); 
    // Увеличиваем количество открытых карт, если добавляем карту в конец колонки
    if (columns[selectedCardIndex].size() > opencard[selectedCardIndex]) {
        opencard[selectedCardIndex]++;
    }
}
void handleKeyUp(std::vector<std::vector<Card>>& columns, std::vector<int>& opencard, int& highlightedCards, bool& inStock, int selectedColumnIndex) {
    if (opencard[selectedColumnIndex] >= highlightedCards + 1) {
        highlightedCards++;
    } 
    else {
        // Логика перехода к выбору карт в колоде и ячейкам
        inStock = true;
    }
}

void handleKeyDown(std::vector<std::vector<Card>>& columns, std::vector<int>& opencard, int& highlightedCards, bool& inStock, int selectedColumnIndex) {
    if (opencard[selectedColumnIndex] >= highlightedCards - 1 && highlightedCards > 1) {
        highlightedCards--;
    } 
    else {
        // Логика перехода к выбору карт в колоде и ячейкам
        inStock = false;
        highlightedCards = 1; // Сбрасываем количество выделенных карт
    }
}
int main_solitare(sf::RenderWindow& windowss) {

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

    std::vector<std::vector<Card>> columns(7);
    std::vector<Card> stock;
    std::vector<int> opencard(columns.size(), 1); // Инициализируем вектор нулями    
    int highlightedCards = 1; // Начальное количество выделенных карт
    int selectedColumnIndex = 0; // Индекс текущей колонки
    int selectedCard_stock = 0;
    std::vector<int> selected_Card_stock(0);

    // Раздача карт в колонны
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j <= i; ++j) {
            Card card = deck.back();
            deck.pop_back();
            columns[i].push_back(card);
        }
    }

    // Оставшиеся карты идут в колоду
    while (!deck.empty()) {
        Card card = deck.back();
        deck.pop_back();
        stock.push_back(card);
    }
    
    
    int selectedCardIndex = 0;
    bool inStock = false; // Флаг для отслеживания, находимся ли мы в колоде карт
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

    bool EndGame = false; //окончание игры
    bool GameOver = false; //проигрыш
    bool YouWin = false; //выигрыш

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
    Button menuButton("Return to menu", sf::Vector2f(400, 50), 30, sf::Color::Transparent, sf::Color::White, font_1);
    Button restartButton("Restart Game", sf::Vector2f(400, 50), 30, sf::Color::Transparent, sf::Color::White, font_1);
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
        sf::Event event;
        while (windowss.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed && event.key.code == Keyboard::Escape)
            {
                showMessageEscape = true;
            }
            // if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F5) {
            //     // Сохранение состояния игры
            //     saveGameState(gameState, saveFilename);
            //     std::cout << "Состояние игры сохранено в " << saveFilename << std::endl;
                
            // }
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
                        // returnToMenu = true;
                        return 0;
                        break;
                    }
                    else if(selectedButtonIndex_2 == 1)// Проверка, выбрана ли кнопка "Restart"
                    {
                        // Здесь код для перезапуска игры
                        // Сброс всех игровых переменных и состояний
                        // Пересоздание колоды карт и перераспределение карт игрокам
                        // Сброс флагов состояния игры
                        EndGame = false;
                        GameOver = false;
                        YouWin = false;
                        // Перезапуск основного игрового цикла
                        main_solitare(windowss);  return 0; break;
                    }

                }
            }
            if (event.type == sf::Event::KeyPressed && (showMessageEscape))
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
                    if(showMessageEscape && selectedButtonIndex == 0)
                    {
                        returnToMenu = true;
                        break;
                    }
                    else if(showMessageEscape && selectedButtonIndex == 1)
                    {
                        showMessageEscape = false;
                        selected_Card.clear(); // Очистка выбора карт

                    }
                }
            } 
            else if (inStock)
            {
                if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)
                {                        
                    // selectedCard_stock = (selectedCard_stock - 1 + 2) % 2;
                    selectedCard_stock = 1;
                }
                else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
                {                        
                    // selectedCard_stock = (selectedCard_stock + 1) % 2;
                    selectedCard_stock = 0;
                } 
                else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
                {
                    if (selectedCard_stock == 0)
                    {
                        // Проверяем, выбрана ли уже карта
                        auto it = std::find(selected_Card_stock.begin(), selected_Card_stock.end(), selectedColumnIndex);

                        // Проверяем, выбрана ли уже карта
                        if (it == selected_Card_stock.end() && selected_Card_stock.size() == 0)
                        {
                            // Если не выбрана, добавляем в вектор и меняем её цвет на красный
                            selected_Card_stock.push_back(selectedColumnIndex);
                            columns[selectedColumnIndex][selectedColumnIndex].sprite.setColor(sf::Color::Red);
                        }
                        // Проверяем, можно ли добавить карту в конец этой колонки, если она не выделена
                        else if (canAddCardToColumn(selectedColumnIndex, columns, selected_Card_stock))
                        {
                            // Добавляем карту в конец колонки
                            addCardToColumn(selectedColumnIndex, columns, selected_Card_stock, opencard);
                            selected_Card_stock.clear();
                        }
                        else
                        {
                            // Если уже выбрана, удаляем из вектора и меняем её цвет на обычный
                            selected_Card.erase(it);
                            columns[selectedColumnIndex][selectedColumnIndex].sprite.setColor(sf::Color::White);
                        }
                    }
                    else if(selectedCard_stock == 1)
                    {
                        if (!stock.empty()) {
                            selectedCardIndex++;
                            if (selectedCardIndex >= stock.size()) {
                                selectedCardIndex = 0; // Сброс индекса, если он превышает размер колоды
                            }
                        }
                    }
                }
                else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
                {
                    inStock = false;
                }
            }
            
            else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left && !inStock)
            {                        
                if (selectedColumnIndex > 0)
                {
                    --selectedColumnIndex;
                }
                else
                {
                    selectedColumnIndex = columns.size() - 1; // Переход к последней колонке
                }
            } 

            else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right && !inStock)
            {
                if (selectedColumnIndex < columns.size() - 1)
                {
                    ++selectedColumnIndex;
                }
                else
                {
                    selectedColumnIndex = 0; // Переход к первой колонке
                }
            } 
            else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && !inStock)
            {    
                // Проверяем, выбрана ли уже карта
                auto it = std::find(selected_Card.begin(), selected_Card.end(), selectedColumnIndex);

                // Проверяем, выбрана ли уже карта
                if (it == selected_Card.end() && selected_Card.size() == 0)
                {
                    // Если не выбрана, добавляем в вектор и меняем её цвет на красный
                    selected_Card.push_back(selectedColumnIndex);
                    columns[selectedColumnIndex][selectedColumnIndex].sprite.setColor(sf::Color::Red);
                }
                // Проверяем, можно ли добавить карту в конец этой колонки, если она не выделена
                else if (canAddCardToColumn(selectedColumnIndex, columns, selected_Card))
                {
                    // Добавляем карту в конец колонки
                    addCardToColumn(selectedColumnIndex, columns, selected_Card, opencard);
                    selected_Card.clear();
                }
                else
                {
                    // Если уже выбрана, удаляем из вектора и меняем её цвет на обычный
                    selected_Card.erase(it);
                    columns[selectedColumnIndex][selectedColumnIndex].sprite.setColor(sf::Color::White);
                }
            } 
            else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
            {
                handleKeyUp(columns, opencard, highlightedCards, inStock, selectedColumnIndex);
            }
            else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
            {
                handleKeyDown(columns, opencard, highlightedCards, inStock, selectedColumnIndex);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
            {
                if(showMessageEscape && selectedButtonIndex == 0)
                {
                    returnToMenu = true;
                    break;
                }
                else if(showMessageEscape && selectedButtonIndex == 1)
                {
                    showMessageEscape = false;
                    selected_Card.clear(); // Очистка выбора карт

                }
            }
            if(returnToMenu)
            {
                return 0; break;
            }
        }
        // if (EndGame) {
        //     //окончание игры 
        //     if(Player[0].hand.empty())
        //     {
        //         YouWin = true;
        //     }
        //     else
        //     {
        //         GameOver = true;
        //     }
        //     showMessageEscape = false;
        // }

        windowss.clear(Color(50,200,150));
        windowss.draw(background);

        sf::Texture back_card_texture;
        if (!back_card_texture.loadFromFile("resources/back.png")) { return 1; }
        sf::RectangleShape back_card_sprite(sf::Vector2f(77, 110));
        back_card_sprite.setTexture(&back_card_texture);

        // Отрисовка карт в колоннах
        for (int columnIndex = 0; columnIndex < columns.size(); ++columnIndex) {
            for (int cardIndex = 0; cardIndex < columns[columnIndex].size(); ++cardIndex) {
                Card& card = columns[columnIndex][cardIndex];
                card.sprite.setPosition(sf::Vector2f(200 + columnIndex * 200, 350 + cardIndex * 30));
                card.sprite.setScale(1.0f, 1.0f); // Уменьшение размера спрайта 

                if (cardIndex >= columns[columnIndex].size() - opencard[columnIndex]) {
                    card.sprite.setTexture(card.texture); // Лицевая сторона карты
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (std::find(selected_Card.begin(), selected_Card.end(), cardIndex) != selected_Card.end()) {
                        card.sprite.setColor(sf::Color::Magenta);
                    } else if (columnIndex == selectedColumnIndex && cardIndex >= columns[columnIndex].size() - highlightedCards) {
                        card.sprite.setColor(sf::Color::Yellow);
                    } else {
                        card.sprite.setColor(sf::Color::White);
                    }
                    windowss.draw(card.sprite);
                } else {
                    back_card_sprite.setPosition(sf::Vector2f(200 + columnIndex * 200, 350 + cardIndex * 30));
                    back_card_sprite.setScale(1.0f, 1.0f); // Уменьшение размера спрайта 
                    windowss.draw(back_card_sprite);
                }
            }
        }
        // Отрисовка карт в колоде
        for (int cardIndex = 0; cardIndex < stock.size(); ++cardIndex) {
            if (cardIndex == selectedCardIndex) {
                Card& card_deck = stock[selectedCardIndex];
                card_deck.sprite.setPosition(sf::Vector2f(200, 150));
                card_deck.sprite.setScale(1.0f, 1.0f); // Уменьшение размера спрайта 
                card_deck.sprite.setTexture(card_deck.texture); // Лицевая сторона карты

                if(inStock && (selectedCard_stock == 0))
                {
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (std::find(selected_Card_stock.begin(), selected_Card_stock.end(), cardIndex) != selected_Card_stock.end()) {
                        card_deck.sprite.setColor(sf::Color::Magenta);
                    } else if (cardIndex == selectedCard_stock) {
                        card_deck.sprite.setColor(sf::Color::Yellow);
                    } else {
                        card_deck.sprite.setColor(sf::Color::White);
                    }
                }
                windowss.draw(card_deck.sprite);
            } 
            else
            {
                back_card_sprite.setPosition(sf::Vector2f(100, 150));
                if(inStock && (selectedCard_stock == 1))
                {
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (cardIndex == selectedCard_stock) {
                        back_card_sprite.setFillColor(sf::Color::Yellow);
                    } else {
                        back_card_sprite.setFillColor(sf::Color::White);
                    }
                }
                windowss.draw(back_card_sprite);
            }
        }

        // // Отрисовка карт в колоде
        // for (int cardIndex = 0; cardIndex < stock.size(); ++cardIndex) {
        //     // if (cardIndex == selectedCardIndex) {
        //     //     back_card_sprite.setFillColor(sf::Color(255, 255, 0, 128)); // Полупрозрачный желтый цвет для выделения
        //     // } else {
        //     //     back_card_sprite.setFillColor(sf::Color::Transparent); // Прозрачный цвет для остальных карт
        //     // }

        //     back_card_sprite.setPosition(sf::Vector2f(100, 150));
        //     windowss.draw(back_card_sprite);
        // }

        // Отрисовка 4 окошек для сбора карт по мастям
        for (int i = 0; i < 4; ++i) {
            sf::RectangleShape foundation(sf::Vector2f(77, 110));
            foundation.setPosition(sf::Vector2f(600 + i * 100, 150));
            foundation.setOutlineColor(sf::Color::White);
            foundation.setOutlineThickness(2);
            foundation.setFillColor(sf::Color::Transparent);
            windowss.draw(foundation);
        }

  
        // Рисование сообщения о выборе карт, если флаг установлен 
        sf::Vector2u windowSize = windowss.getSize();

        float buttonWidth = buttonYes.getLocalBounds().width;
        float buttonHeight = buttonYes.getLocalBounds().height;

        float buttonX = windowSize.x / 2.f - buttonWidth / 2.f; 
        float buttonY = (800 + 150) / 2.f - buttonHeight / 2.f + 50;
        buttonYes.setPosition(buttonX, buttonY);

        buttonNo.setPosition(buttonX + buttonWidth + 50, buttonY);

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
        // Drawing code for graphics can be added here

        // Обновление объекта GameState
        // updateGameState(gameState); // Добавьте здесь функцию обновления состояния игры
        // Сохранение изменений в GameState (если необходимо)
        // Например, если вы хотите сохранять состояние игры каждую секунду:
        // if (gameTimer.getElapsedTime().asSeconds() >= 1.0f) {
        //     saveGameState(gameState, saveFilename);
        //     gameTimer.restart();
        // }
        windowss.display();
        
    }
    return 0;
}
