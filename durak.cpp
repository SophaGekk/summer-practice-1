#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <set>
#include <random>
using namespace sf;
// Класс для представления карты
class Cards {
public:
    int value; // Значение карты (6-14)
    std::string suit; // Масть карты (HEARTS, DIAMONDS, CLUBS, SPADES)
    Texture texture; // Текстура карты
    Sprite sprite; // Спрайт карты
};
// Класс для представления игрока
class Player {
public:
    std::string name; // Имя игрока
    std::vector<Cards> hand; // Рука игрока
    bool isAttacker; // Является ли игрок атакующим
    bool isDefender; // Является ли игрок защищающимся
    // Конструктор игрока
    Player() : isAttacker(false), isDefender(false), name("") {}
};
// Функция для определения индекса текущего атакующего игрока
int findCurrentPlayer(std::vector<Player>& players)
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
// Функция для проверки на атаку
bool attack(std::vector<Cards>& table, Cards& attackCards) {
    if (!table.empty()) {
        for (int i = 0; i < table.size(); ++i) {
            if (attackCards.value == table[i].value) {
                return true;
            }
        }
    } 
    return false;
}
// Функция для проверки на защиту
bool defense(Cards& attackCards, Cards& defenseCards, Cards& trump) {
    if ((defenseCards.value > attackCards.value && defenseCards.suit == attackCards.suit) ||
        (defenseCards.suit == trump.suit && attackCards.suit != trump.suit)) {
        return true;
    }
    return false;
}
// Функция для определения, кто первый ходит
int whoFirst (std::vector<Player>& players, Cards& trump) {
    // Словарь для хранения минимального козыря и значения для каждого игрока
    std::map<int, std::pair<int, int>> playerData;
    // Проход по всем игрокам
    for (int i = 0; i < players.size(); ++i) {
        int minTrump = 15;  // Максимальное значение козыря
        int minValue = 15; // Максимальное значение карты
        for (const Cards& Cards : players[i].hand) {
            if (Cards.suit == trump.suit) {
                minTrump = std::min(minTrump, Cards.value);
            }
            minValue = std::min(minValue, Cards.value);
        }
        // Сохранение данных в словарь
        playerData[i] = std::make_pair(minTrump, minValue);
    }
    // Определение первого игрока
    int firstPlayerIndex = 0;
    int minTrump = playerData[0].first;
    int minValue = playerData[0].second;
    for (int i = 1; i < players.size(); ++i) {
        if (playerData[i].first < minTrump) {
            minTrump = playerData[i].first;
            firstPlayerIndex = i;
        } else if (playerData[i].first == minTrump && playerData[i].second < minValue) {
            minValue = playerData[i].second;
            firstPlayerIndex = i;
        }
    }
    return firstPlayerIndex;
}
// Функция, которая проверяет, закончилась ли игра
bool isOver (std::vector<Cards>& deck, std::vector<Player>& players) {
    int count = 0;
    for (const Player& player: players) {
        if (player.hand.empty()) {
            count++;
        }
    }
    if (deck.empty() && (count <= 3)) {
        return true;
    }
    return false;
}
// Глобальная переменная для индекса текущего игрока
int currentPlayerIndexs = 0;
int main_durakgame(sf::RenderWindow& windowss) {
    srand(time(0));
    // Загрузка текстуры стола
    Texture tableTexture;
    if (!tableTexture.loadFromFile("resources/table.png")) {
        std::cerr << "Ошибка загрузки текстуры стола: " << "resources/table.png" << std::endl;
        return 1; 
    }
    // Создание спрайта стола
    RectangleShape background(Vector2f(1920,1080));
    background.setTexture(&tableTexture);
    background.setPosition(Vector2f(0,0));
    // Масти карт
    std::vector<std::string> suits = {"HEARTS", "DIAMONDS", "CLUBS", "SPADES"};
    // Создание колоды карт 
    std::vector<Cards> deck;
    sf::Texture CardssTexture;
     for (int value = 6; value <= 14; ++value) {
        for (const std::string& suit : suits) {
            // Исключаем даму крести
            if (value == 12 && suit == "CLUBS") {
                continue; // Пропускаем добавление карты "Дама Пик"
            }
            Cards Cardss;
            Cardss.value = value;
            Cardss.suit = suit;
            std::string filename = "resources/" + std::to_string(value) + suit + ".png";
            if (!CardssTexture.loadFromFile(filename)) { 
                std::cerr << "Ошибка загрузки текстуры: " << filename << std::endl; 
            } else {
                Cardss.texture = CardssTexture; 
                Cardss.sprite.setTexture(Cardss.texture);
                deck.push_back(Cardss);
            }
        }
    }
    // Отрисовка колоды
    sf::Sprite deckSprite;
    Texture deckTexture;
    if (!deckTexture.loadFromFile("resources/back.png")) {
        std::cerr << "Ошибка загрузки текстуры колоды: " << "resources/table.png" << std::endl;
        return 1; 
    }
    deckSprite.setTexture(deckTexture);
    deckSprite.setPosition(100, 400); 
    // Перемешивание колоды
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
    // Выбор козыря
    Cards trump = deck.back();
    deck.pop_back();
    // Возврат козыря в начало колоды
    deck.insert(deck.begin(), trump);
    // Отрисовка козыря
    sf::Sprite trumpSprite;
    Texture trumpTexture;
    std::string filename = "resources/" + std::to_string(trump.value) + trump.suit + ".png";
    if (!trumpTexture.loadFromFile(filename)) {
        std::cerr << "Ошибка загрузки текстуры козыря: " << "resources/table.png" << std::endl;
        return 1; 
    }
    trumpSprite.setTexture(trumpTexture);
    trumpSprite.setPosition(100, 400); 
    // Создание игроков
    std::vector<Player> players(4);
    players[0].name = "Vasya";
    players[1].name = "Masha";
    players[2].name = "Stepan";
    players[3].name = "Katya";
    players[0].isAttacker = true;
    players[0].isDefender = true;
    players[1].isAttacker = false;
    players[1].isDefender = false;
    players[2].isAttacker = false;
    players[2].isDefender = false;
    players[3].isAttacker = false;
    players[3].isDefender = false;
    // Текст для отображения имен игроков
    sf::Font font_1;
    if (!font_1.loadFromFile("resources/ffont.ttf"))
    {
        std::cerr << "Ошибка загрузки шрифта!" << std::endl;
    }
    sf::Text playerTexts[4];
    for (size_t i = 0; i < 4; ++i) {
        playerTexts[i].setFont(font_1);
        playerTexts[i].setCharacterSize(24);
        playerTexts[i].setFillColor(sf::Color::White);
        playerTexts[i].setString(players[i].name);
    }
    // Позиционирование имен игроков
    playerTexts[0].setPosition(200, 700);
    playerTexts[1].setPosition(1600, 200);
    playerTexts[2].setPosition(200, 100);
    playerTexts[3].setPosition(100, 600);
    // Раздача карт
    int CardssPerPlayer = 6;
    for (int i = 0; i < CardssPerPlayer; ++i)
    {
        for (int j = 0; j < players.size(); ++j)
        {
            if (!deck.empty()) {
                players[j].hand.push_back(deck.back());
                deck.pop_back();
            }
        }
    }
    // Сообщения о возможности атаковать, выход
    sf::Text message_can_attack;
    message_can_attack.setFont(font_1);
    message_can_attack.setCharacterSize(24);
    message_can_attack.setFillColor(sf::Color::White);
    message_can_attack.setPosition(100.f, 100.f);
    message_can_attack.setString(L"Cannot attack with this Cards");
    sf::Text message_can_defend;
    message_can_defend.setFont(font_1);
    message_can_defend.setCharacterSize(24);
    message_can_defend.setFillColor(sf::Color::White);
    message_can_defend.setPosition(100.f, 100.f);
    message_can_defend.setString(L"Cannot defend with this Cards");
    sf::Text message_escape;
    message_escape.setFont(font_1);
    message_escape.setCharacterSize(24);
    message_escape.setFillColor(sf::Color::White);
    message_escape.setPosition(100.f, 100.f);
    message_escape.setString(L"Do you really want to get out?");
    sf::Text message;
    message.setFont(font_1);
    message.setCharacterSize(30);
    message.setFillColor(sf::Color::White);
    message.setPosition(100.f, 100.f);
    // Кнопки "Пас" и "Бито"
    sf::Text passButton;
    passButton.setFont(font_1);
    passButton.setString("Pass");
    passButton.setCharacterSize(24);
    passButton.setFillColor(sf::Color::White);
    passButton.setPosition(200.f, 200.f);
    sf::Text takeCardssButton;
    takeCardssButton.setFont(font_1);
    takeCardssButton.setString("Bito");
    takeCardssButton.setCharacterSize(24);
    takeCardssButton.setFillColor(sf::Color::White);
    takeCardssButton.setPosition(400.f, 200.f);
    // Индекс выбранной кнопки (0 - Да, 1 - Нет)
    int selectedButtonIndex = 0;
    // Флаги для управления игрой
    bool showCardsSelectionMessage = true; // Флаг для отображения текста выбора карт
    bool showMessageEscape = false; // Флаг для отображения текста выхода из игры
    bool returnToMenu = false; // Флаг для отображения текста выхода в меню
    bool showMessageWait = false; // Флаг для отображения текста ожидания игроков
    bool showMessage = false; // Флаг для отображения сообщения
    bool showInvalidMove = false; // Флаг для отображения сообщения о неправильном выборе карты для атаки/защиты
    bool showTakeCardsMessage = false; // Флаг для отображения сообщения о взятии карты
    bool attacking = true; // Можно ли атаковать
    bool defending = false; // Можно ли защищаться
    int playersChoseNo = 0; // Переменная для отслеживания количества игроков, выбравших "нет"
    // Код для отображения сообщения "Ожидаем других игроков: количество"
    sf::Text wait_message;
    wait_message.setFont(font_1);
    wait_message.setCharacterSize(30);
    wait_message.setFillColor(sf::Color::White);
    wait_message.setPosition(100.f, 100.f);
    int shift = 0; // Начальное значение смещения
    // Game Loop
    int countMove = 1;
    bool CardsTaken = false;
    std::vector<Cards> table; 
    std::vector<Cards> attackCardss;
    int selectedCardsIndex = 0;
    while (windowss.isOpen()) {
        if(returnToMenu) {
            return 0;
            break;
        }
        sf::Event event;
        while (windowss.pollEvent(event)) {
            // Раздача карт, если надо
            for (auto& player : players) {
                if (player.hand.size() < 6) {
                    player.hand.push_back(deck.back());
                    deck.pop_back();
                }
            }
            // Определение текущего игрока
            if (countMove == 1) {
                currentPlayerIndexs = whoFirst(players,trump);
            } else {
                currentPlayerIndexs = findCurrentPlayer(players);
            }
            // Определение соседа справа
            int neighborIndex = (currentPlayerIndexs + 1) % players.size(); 
            switch (event.type) {
                case sf::Event::Closed:
                    windowss.close();
                    break;
                case sf::Event::KeyPressed:
                    // Обработка кнопки "Escape"
                    if (event.key.code == Keyboard::Escape) {
                        showMessageEscape = true;
                        showCardsSelectionMessage = false;
                    }
                    // Обработка кнопок "Pass", "Take Cardss"
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            // Получить глобальные координаты курсора мыши
                            sf::Vector2i mousePos = sf::Mouse::getPosition(windowss);
                            // Проверка нажатия кнопки "Pass"
                            if (passButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && attacking) {
                                players[currentPlayerIndexs].isAttacker = false;
                                players[neighborIndex].isAttacker = true;
                                players[neighborIndex].isDefender = false;
                                attacking = true; 
                                defending = false;
                                countMove++;
                            }
                            // Проверка нажатия кнопки "Take Cardss"
                            if (takeCardssButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && defending) {
                                for (auto& Cards : table) {
                                    players[currentPlayerIndexs].hand.push_back(Cards);
                                }
                                players[currentPlayerIndexs].isDefender = false;
                                players[neighborIndex].isAttacker = true; 
                                attacking = true;
                                defending = false;
                                table.clear();
                                attackCardss.clear();
                                countMove++;
                            }
                        }
                    }
                    // Выбор карты
                    if(event.key.code == sf::Keyboard::Left)
                    {                        
                        if (selectedCardsIndex > 0)
                        {
                            --selectedCardsIndex;
                        } else {
                            selectedCardsIndex = players[currentPlayerIndexs].hand.size() - 1;
                        }
                    }
                    if(event.key.code == sf::Keyboard::Right)
                    {
                        // Переход к предыдущей карте с круговым циклом
                        if (selectedCardsIndex < players[currentPlayerIndexs].hand.size() - 1)
                        {
                            ++selectedCardsIndex;
                        } else {
                            selectedCardsIndex = 0;
                        }
                    }
                    // Выбор карты для атаки/защиты
                    if (event.key.code == sf::Keyboard::Enter)
                    {
                        if (attacking) 
                        {
                            // Атака
                            if (attack(table, players[currentPlayerIndexs].hand[selectedCardsIndex])) {
                                attackCardss.push_back(players[currentPlayerIndexs].hand[selectedCardsIndex]);
                                table.push_back(players[currentPlayerIndexs].hand[selectedCardsIndex]);
                                players[currentPlayerIndexs].hand.erase(players[currentPlayerIndexs].hand.begin() + selectedCardsIndex);
                                // Переход хода к защищающемуся игроку
                                players[currentPlayerIndexs].isAttacker = false;
                                players[neighborIndex].isDefender = true;
                                attacking = false;
                                defending = true;
                                showCardsSelectionMessage = true;
                            } else {
                                showInvalidMove = true;
                                showCardsSelectionMessage = true;
                            } 
                        }    
                        else if (defending) 
                        {
                            // Защита
                            if (defense(attackCardss.back(), players[currentPlayerIndexs].hand[selectedCardsIndex], trump)) {
                                table.push_back(players[currentPlayerIndexs].hand[selectedCardsIndex]);
                                players[currentPlayerIndexs].hand.erase(players[currentPlayerIndexs].hand.begin() + selectedCardsIndex);
                                // Переход хода к атакующему игроку
                                players[currentPlayerIndexs].isAttacker = true;
                                players[neighborIndex].isDefender = false;
                                attacking = true;
                                defending = false;
                                showCardsSelectionMessage = true;
                            } else {
                                showInvalidMove = true;
                                showCardsSelectionMessage = true;
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
        // Отрисовка имен игроков
        for (size_t i = 0; i < 4; ++i) {
            if (players[i].isAttacker) {
                playerTexts[i].setFillColor(sf::Color::Yellow); // Подсветка атакующего
            } else {
                playerTexts[i].setFillColor(sf::Color::White);
            }
            windowss.draw(playerTexts[i]);
        }
        // Отрисовка карт игроков
        Texture back_Cards_texture;
        if (!back_Cards_texture.loadFromFile("resources/back.png")) { return 1; }
        RectangleShape back_Cards_sprite(Vector2f(77,110));
        back_Cards_sprite.setTexture(&back_Cards_texture);
        // Отрисовка карт для каждого игрока
        for (int playerIndex = 0; playerIndex < players.size(); ++playerIndex) {
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
            // Отрисовка карт текущего игрока
            for (size_t CardsIndex = 0; CardsIndex < players[playerIndex].hand.size(); ++CardsIndex) {
                Cards& Cards = players[playerIndex].hand[CardsIndex];
                if (players[playerIndex].isAttacker) {
                    // Атакующий игрок видит значения своих карт
                    Cards.sprite.setTexture(Cards.texture);
                    // Определение позиции карты в зависимости от её индекса 
                    Cards.sprite.setPosition(Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (CardsIndex != selectedCardsIndex)
                    {
                        Cards.sprite.setColor(Color::Magenta);
                    } else if (CardsIndex == selectedCardsIndex)
                    {
                        Cards.sprite.setColor(Color::Yellow);
                    } else {
                        Cards.sprite.setColor(Color::White);
                    }
                    windowss.draw(Cards.sprite);
                    ++id; 
                } else {
                    back_Cards_sprite.setRotation(rotation);
                    if (players[playerIndex].isDefender) {
                        back_Cards_sprite.setPosition(Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                    }
                    windowss.draw(back_Cards_sprite);
                    id++;
                }
            }
        }
        // Рисование сообщения о выборе карт, если флаг установлен 
        sf::Vector2u windowSize = windowss.getSize();
        if (showCardsSelectionMessage) {
            if (attacking) {
                message.setString(L"Select Cards for attack");
            } else if (defending) {
                message.setString(L"Select Cards for defend");
            }
            float textX = windowSize.x / 2.f - message.getLocalBounds().width / 2.f;
            float textY = (800 + 150) / 2.f - message.getLocalBounds().height / 2.f;
            message.setPosition(textX, textY);
            float buttonWidth = passButton.getLocalBounds().width;
            float buttonHeight = passButton.getLocalBounds().height;
            float buttonX = windowSize.x / 2.f - buttonWidth / 2.f; 
            float buttonY = (800 + 150) / 2.f - buttonHeight / 2.f + 50;
            passButton.setPosition(buttonX, buttonY);
            takeCardssButton.setPosition(buttonX + buttonWidth + 50, buttonY);
            // Выделение выбранной кнопки
            if (selectedButtonIndex == 0)
            {
                passButton.setFillColor(sf::Color::Red);
                takeCardssButton.setFillColor(sf::Color::White);
            } else {
                passButton.setFillColor(sf::Color::White);
                takeCardssButton.setFillColor(sf::Color::Red);
            }
            // Рисование кнопок
            windowss.draw(message);
            windowss.draw(passButton);
            windowss.draw(takeCardssButton);
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
                passButton.setFillColor(sf::Color::Red);
                takeCardssButton.setFillColor(sf::Color::White);
            } else {
                passButton.setFillColor(sf::Color::White);
                takeCardssButton.setFillColor(sf::Color::Red);
            }
            // Рисование кнопок
            windowss.draw(passButton);
            windowss.draw(takeCardssButton);
        }
        // Рисование сообщения о неправильном выборе, если флаг установлен
        if (showInvalidMove) {
            message.setString(L"Invalid move, please try again");
            // Рисование сообщения о выборе карт, если флаг установлен 
            float textX = windowSize.x / 2.f - message.getLocalBounds().width / 2.f;
            float textY = (800 + 150) / 2.f - message.getLocalBounds().height / 2.f;
            message.setPosition(textX, textY);
            windowss.draw(message);
        }
        // Drawing code for graphics can be added here
        windowss.display();
    }
    return 0;
}