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
class Players {
public:
    std::string name; // Имя игрока
    std::vector<Cards> hand; // Рука игрока
    bool isAttacker; // Является ли игрок атакующим
    bool isDefender; // Является ли игрок защищающимся
    // Конструктор игрока
    Players() : isAttacker(false), isDefender(false), name("") {}
};
// Функция для определения индекса текущего атакующего игрока
int findCurrentPlayers(std::vector<Players>& Playerss)
{
    for (int i = 0; i < Playerss.size(); i++)
    {
        if (Playerss[i].isAttacker)
        {
            return i; // Возвращает индекс текущего атакующего игрока
        }
    }
    return -1; // Если атакующий игрок не найден
}
// Функция для проверки на атаку
bool attack(std::vector<Cards>& table, Cards& attackCards) {
    if (!table.empty()) {
        for (int i = 0; i < table.size(); i++) {
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
int whoFirst (std::vector<Players>& Playerss, Cards& trump) {
    // Словарь для хранения минимального козыря и значения для каждого игрока
    std::map<int, std::pair<int, int>> PlayersData;
    // Проход по всем игрокам
    for (int i = 0; i < Playerss.size(); i++) {
        int minTrump = 14;  // Максимальное значение козыря
        int minValue = 14; // Максимальное значение карты
        for (Cards& Cards : Playerss[i].hand) {
            if (Cards.suit == trump.suit) {
                minTrump = std::min(minTrump, Cards.value);
            }
            minValue = std::min(minValue, Cards.value);
        }
        // Сохранение данных в словарь
        PlayersData[i] = std::make_pair(minTrump, minValue);
    }
    // Определение первого игрока
    int firstPlayersIndex = 0;
    int minTrump = PlayersData[0].first;
    int minValue = PlayersData[0].second;
    for (int i = 1; i < Playerss.size(); i++) {
        if (PlayersData[i].first < minTrump) {
            minTrump = PlayersData[i].first;
            firstPlayersIndex = i;
        } else if (PlayersData[i].first == minTrump && PlayersData[i].second < minValue) {
            minValue = PlayersData[i].second;
            firstPlayersIndex = i;
        }
    }
    return firstPlayersIndex;
}
// Функция, которая проверяет, закончилась ли игра
bool isOver (std::vector<Cards>& deck, std::vector<Players>& Playerss) {
    int count = 0;
    for (Players& Players: Playerss) {
        if (Players.hand.empty()) {
            count++;
        }
    }
    if (deck.empty() && (count <= 3)) {
        return true;
    }
    return false;
}
// Глобальная переменная для индекса текущего игрока
int currentPlayersIndexs = 0;
int main_durakgame(sf::RenderWindow& windowss) {
    srand(time(0));
    // Загрузка текстуры стола
    Texture tableTexture;
    if (!tableTexture.loadFromFile("resources/table.png")) {
        std::cerr << "Ошибка загрузки текстуры стола: " << "resources/table.png" << std::endl;
        return -1;
    }
    // Создание спрайта стола
    RectangleShape background(Vector2f(1920,1080));
    background.setTexture(&tableTexture);
    background.setPosition(Vector2f(0,0));
    // Масти карт
    std::vector<std::string> suits = {"HEARTS", "DIAMONDS", "CLUBS", "SPADES"};
    // Создание колоды карт 
    std::vector<Cards> deck(36);
    sf::Texture CardssTexture;
     for (int value = 6; value < 14; value++) {
        for (const std::string& suit : suits) {
            Cards Cardss;
            Cardss.value = value;
            Cardss.suit = suit;
            std::string deckFilename = "resources/" + std::to_string(value) + suit + ".png";
            if (!CardssTexture.loadFromFile(deckFilename)) { 
                std::cerr << "Ошибка загрузки текстуры карт: " << deckFilename << std::endl; 
                return -1;
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
        std::cerr << "Ошибка загрузки текстуры колоды: resources/back.png" << std::endl;
        return -1;
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
    std::string trumpFilename = "resources/" + std::to_string(trump.value) + trump.suit + ".png";
    if (!trumpTexture.loadFromFile(trumpFilename)) {
        std::cerr << "Ошибка загрузки текстуры козыря: " << trumpFilename << std::endl;
        return -1;
    }
    trumpSprite.setTexture(trumpTexture);
    trumpSprite.setPosition(100, 400); 
    // Создание игроков
    std::vector<Players> Playerss(4);
    Playerss[0].name = "Vasya";
    Playerss[1].name = "Masha";
    Playerss[2].name = "Stepan";
    Playerss[3].name = "Katya";
    // Playerss[0].isAttacker = false;
    // Playerss[0].isDefender = false;
    // Playerss[1].isAttacker = false;
    // Playerss[1].isDefender = false;
    // Playerss[2].isAttacker = false;
    // Playerss[2].isDefender = false;
    // Playerss[3].isAttacker = false;
    // Playerss[3].isDefender = false;
    // Текст для отображения имен игроков
    sf::Font font_1;
    if (!font_1.loadFromFile("resources/ffont.ttf"))
    {
        std::cerr << "Ошибка загрузки шрифта: resources/ffont.ttf" << std::endl;
        return -1;
    }
    sf::Text PlayersTexts[4];
    for (size_t i = 0; i < 4; ++i) {
        PlayersTexts[i].setFont(font_1);
        PlayersTexts[i].setCharacterSize(24);
        PlayersTexts[i].setFillColor(sf::Color::White);
        PlayersTexts[i].setString(Playerss[i].name);
    }
    // Позиционирование имен игроков
    PlayersTexts[0].setPosition(200, 700);
    PlayersTexts[1].setPosition(1600, 200);
    PlayersTexts[2].setPosition(200, 100);
    PlayersTexts[3].setPosition(100, 600);
    // Раздача карт
    int CardssPerPlayers = 6;
    for (int i = 0; i < CardssPerPlayers; i++)
    {
        for (int j = 0; j < Playerss.size(); ++j)
        {
            if (!deck.empty()) {
                Playerss[j].hand.push_back(deck.back());
                deck.pop_back();
            }
        }
    }
    // Сообщение о выходе
    sf::Text message_escape;
    message_escape.setFont(font_1);
    message_escape.setCharacterSize(24);
    message_escape.setFillColor(sf::Color::White);
    message_escape.setPosition(100.f, 100.f);
    message_escape.setString(L"Do you really want to get out?");
    // Сообщение
    sf::Text message;
    message.setFont(font_1);
    message.setCharacterSize(30);
    message.setFillColor(sf::Color::White);
    message.setPosition(100.f, 100.f);
    // Кнопка "Да"
    sf::Text buttonYes;
    buttonYes.setFont(font_1);
    buttonYes.setString("Yes");
    buttonYes.setCharacterSize(24);
    buttonYes.setFillColor(sf::Color::White);
    buttonYes.setPosition(200.f, 200.f);
    // Кнопка "Нет"
    sf::Text buttonNo;
    buttonNo.setFont(font_1);
    buttonNo.setString("No");
    buttonNo.setCharacterSize(24);
    buttonNo.setFillColor(sf::Color::White);
    buttonNo.setPosition(400.f, 200.f);
    // Индекс выбранной кнопки (0 - Да, 1 - Нет)
    int selectedButtonIndex = 0;
    // Флаги для управления игрой
    bool showCardsSelectionMessage = true; // Флаг для отображения текста выбора карт
    bool showMessage = false; // Флаг для отображения сообщения
    bool showInvalidMove = false; // Флаг для отображения сообщения о неправильном выборе карты для атаки/защиты
    bool attacking = true; // Можно ли атаковать
    bool defending = false; // Можно ли защищаться
    // Game Loop
    int countMove = 1;
    std::vector<Cards> table(36); 
    std::vector<Cards> attackCardss(36);
    int selectedCardsIndex = 0;
    sf::Event event;
    while (windowss.isOpen()) {
        // Раздача карт
        for (auto& Players : Playerss) {
            if (Players.hand.size() < 6) {
                Players.hand.push_back(deck.back());
                deck.pop_back();
            }
        }
        // Определение текущего игрока
        if (countMove == 1) {
            currentPlayersIndexs = whoFirst(Playerss,trump);
        } else {
            currentPlayersIndexs = findCurrentPlayers(Playerss);
        }
        Playerss[currentPlayersIndexs].isAttacker = true;
        // Определение соседа справа
        int neighborIndex = (currentPlayersIndexs + 1) % Playerss.size();
        Playerss[neighborIndex].isDefender = true;
        // Проверка на конец игры
        if (deck.empty()) {
            for (int i = 0; i < Playerss.size(); i++) {
                if (Playerss[i].hand.empty()) {
                    std::cout << Playerss[i].name << " win!" << std::endl;
                }
            }
            Playerss.erase(std::remove_if(Playerss.begin(), Playerss.end(), [](const Players& p) { 
                return p.hand.empty(); 
            }), Playerss.end()); 
            if (Playerss.size() == 1) {
                std::cout << Playerss[0].name << " - durak!" << std::endl;
            }
        }
        // Обработка событий
        sf::Event event;
        while (windowss.pollEvent(event)) {
            // Закрытие окна
            if (event.type == sf::Event::Closed) windowss.close();
            // Обработка нажатия кнопок
            if (event.type == sf::Event::KeyPressed) {
                // Escape - выход
                if (event.key.code == sf::Keyboard::Escape) windowss.close();
                // Left - выбор карты справа налево
                if (event.key.code == sf::Keyboard::Left) {
                    if (selectedCardsIndex > 0) {
                        --selectedCardsIndex;
                    } else {
                        selectedCardsIndex = Playerss[currentPlayersIndexs].hand.size() - 1;
                    }
                }
                // Left - выбор карты слева направо 
                if (event.key.code == sf::Keyboard::Right) {
                    if (selectedCardsIndex < Playerss[currentPlayersIndexs].hand.size() - 1) {
                        ++selectedCardsIndex;
                    } else {
                        selectedCardsIndex = 0;
                    }
                }
                // P - пас
                if (event.key.code == sf::Keyboard::P && attacking) {
                    Playerss[currentPlayersIndexs].isAttacker = false;
                    Playerss[neighborIndex].isAttacker = true;
                    Playerss[neighborIndex].isDefender = false;
                    attacking = true; 
                    defending = false;
                    countMove++;
                }
                // T - взять карты
                if (event.key.code == sf::Keyboard::T && defending) {
                    for (Cards& Cards : table) {
                        Playerss[currentPlayersIndexs].hand.push_back(Cards);
                    }
                    Playerss[currentPlayersIndexs].isDefender = false;
                    Playerss[neighborIndex].isAttacker = true; 
                    attacking = true;
                    defending = false;
                    table.clear();
                    attackCardss.clear();
                    countMove++;
                }
                // Enter - выбор карты для атаки
                if (event.key.code == sf::Keyboard::Enter && attacking) {
                    if (attack(table, Playerss[currentPlayersIndexs].hand[selectedCardsIndex])) {
                        attackCardss.push_back(Playerss[currentPlayersIndexs].hand[selectedCardsIndex]);
                        table.push_back(Playerss[currentPlayersIndexs].hand[selectedCardsIndex]);
                        Playerss[currentPlayersIndexs].hand.erase(Playerss[currentPlayersIndexs].hand.begin() + selectedCardsIndex);
                        Playerss[currentPlayersIndexs].isAttacker = false;
                        Playerss[neighborIndex].isDefender = true;
                        attacking = false;
                        defending = true;
                        showCardsSelectionMessage = true;
                    } else {
                        showInvalidMove = true;
                        showCardsSelectionMessage = true;
                    }
                // Enter - выбор карты для защиты
                } else if (event.key.code == sf::Keyboard::Enter && defending) {
                    if (defense(attackCardss.back(), Playerss[currentPlayersIndexs].hand[selectedCardsIndex], trump)) {
                        table.push_back(Playerss[currentPlayersIndexs].hand[selectedCardsIndex]);
                        Playerss[currentPlayersIndexs].hand.erase(Playerss[currentPlayersIndexs].hand.begin() + selectedCardsIndex);
                        Playerss[currentPlayersIndexs].isAttacker = true;
                        Playerss[neighborIndex].isDefender = false;
                        attacking = true;
                        defending = false;
                        showCardsSelectionMessage = true;
                        attackCardss.clear();
                    } else {
                        showInvalidMove = true;
                        showCardsSelectionMessage = true;
                    }
                }
            }
        }
        windowss.clear(Color(50,200,150));
        windowss.draw(background);
        // Отрисовка имен игроков
        for (int i = 0; i < 4; i++) {
            if (Playerss[i].isAttacker) {
                PlayersTexts[i].setFillColor(sf::Color::Yellow); // Подсветка атакующего
            } else {
                PlayersTexts[i].setFillColor(sf::Color::White);
            }
            windowss.draw(PlayersTexts[i]);
        }
        // Отрисовка карт игроков
        Texture back_Cards_texture;
        if (!back_Cards_texture.loadFromFile("resources/back.png")) {
            std::cerr << "Ошибка загрузки текстуры рубашки карт: resources/back.png" << std::endl;
            return -1;
        }
        RectangleShape back_Cards_sprite(Vector2f(77,110));
        back_Cards_sprite.setTexture(&back_Cards_texture);
        // Отрисовка карт для каждого игрока
        for (int PlayersIndex = 0; PlayersIndex < Playerss.size(); ++PlayersIndex) {
            // Определение позиции и поворота карт в зависимости от индекса игрока
            float rotation;
            if (PlayersIndex == 0) { // Игрок снизу
                rotation = 0;
            } else if (PlayersIndex == 1) { // Игрок справа
                rotation = 90;
            } else if (PlayersIndex == 2) { // Игрок сверху
                rotation = 0;
            } else if (PlayersIndex == 3) { // Игрок слева
                rotation = -90;
            }
            int id = 1; // Переменная для определения позиции карты в ряду
            // Отрисовка карт текущего игрока
            for (int CardsIndex = 0; CardsIndex < Playerss[PlayersIndex].hand.size(); CardsIndex++) {
                Cards& Cards = Playerss[PlayersIndex].hand[CardsIndex];
                if (Playerss[PlayersIndex].isAttacker) {
                    // Атакующий игрок видит значения своих карт
                    Cards.sprite.setTexture(Cards.texture);
                    // Определение позиции карты в зависимости от её индекса 
                    Cards.sprite.setPosition(Vector2f(200 + (1300 / Playerss[PlayersIndex].hand.size()) * id, 800));
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
                    if (Playerss[PlayersIndex].isDefender) {
                        back_Cards_sprite.setPosition(Vector2f(200 + (1300 / Playerss[PlayersIndex].hand.size()) * id, 150));
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
            float buttonWidth = buttonYes.getLocalBounds().width;
            float buttonHeight = buttonYes.getLocalBounds().height;
            float buttonX = windowSize.x / 2.f - buttonWidth / 2.f; 
            float buttonY = (800 + 150) / 2.f - buttonHeight / 2.f + 50;
            buttonYes.setPosition(buttonX, buttonY);
            buttonNo.setPosition(buttonX + buttonWidth + 50, buttonY);
            // Выделение выбранной кнопки
            if (selectedButtonIndex == 0)
            {
                buttonYes.setFillColor(sf::Color::Red);
                buttonNo.setFillColor(sf::Color::White);
            } else {
                buttonYes.setFillColor(sf::Color::White);
                buttonNo.setFillColor(sf::Color::Red);
            }
            // Рисование кнопок
            windowss.draw(message);
            windowss.draw(buttonYes);
            windowss.draw(buttonNo);
        }
        // Рисование сообщения о нажатие Escape, если флаг установлен 
        float textX_message_escape = windowSize.x / 2.f - message_escape.getLocalBounds().width / 2.f;
        float textY_message_escape = (800 + 150) / 2.f - message_escape.getLocalBounds().height / 2.f;
        message_escape.setPosition(textX_message_escape, textY_message_escape);
        // Рисование сообщения о неправильном выборе, если флаг установлен
        if (showInvalidMove) {
            message.setString(L"Invalid move, please try again");
            // Рисование сообщения о выборе карт, если флаг установлен 
            float textX = windowSize.x / 2.f - message.getLocalBounds().width / 2.f;
            float textY = (800 + 150) / 2.f - message.getLocalBounds().height / 2.f;
            message.setPosition(textX, textY);
            windowss.draw(message);
        }
        windowss.display();
    }
    return 0;
}