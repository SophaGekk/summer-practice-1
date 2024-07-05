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
class cards {
public:
    int value; // Значение карты (6-14)
    std::string suit; // Масть карты (HEARTS, DIAMONDS, CLUBS, SPADES)
    Texture texture; // Текстура карты
    Sprite sprite; // Спрайт карты
    // Конструктор карты
    // cards(int val, const std::string& s) : value(val), suit(s) {
    //     // Загружаем текстуру карты
    //     std::string filename = "resources/" + std::to_string(value) + s + ".png"; 
    //     if (!texture.loadFromFile(filename)) {
    //         std::cerr << "Ошибка загрузки текстуры: " << filename << std::endl;
    //     } else {
    //         sprite.setTexture(texture);
    //     }
    // }
};
// Класс для представления игрока
class Player {
public:
    std::vector<cards> hand; // Рука игрока
    bool isAttacker; // Является ли игрок атакующим
    bool isDefender; // Является ли игрок защищающимся
    bool isTaking; // Взял ли игрок карты из колоды
    bool cardsSelected; // Выбрал ли игрок карту для атаки/защиты
    // Конструктор игрока
    Player() : isAttacker(false), isDefender(false), isTaking(false), cardsSelected(false) {}
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
// Функция для сортировки карт в руке игрока по значению
void sortHand(std::vector<cards>& hand) {
    std::sort(hand.begin(), hand.end(), [](const cards& a, const cards& b) {
        return a.value < b.value;
    });
}
// Функция для проверки на атаку
bool attack(std::vector<cards>& table, cards& attackcards) {
    if (!table.empty()) {
        for (int i = 0; i < table.size(); ++i) {
            if (attackcards.value == table[i].value) {
                return true;
            }
        }
    } 
    return false;
}
// Функция для проверки на защиту
bool defense(cards& attackcards, cards& defensecards, cards& trump) {
    if (((defensecards.value > attackcards.value) && (defensecards.suit == attackcards.suit)) || ((defensecards.suit == trump.suit) && (defensecards.suit != trump.suit))) {
        return true;
    }
    return false;
}
// Функция для определения, кто первый ходит
int whoFirst (std::vector<Player>& players, cards& trump) {
    // Словарь для хранения минимального козыря и значения для каждого игрока
    std::map<int, std::pair<int, int>> playerData;
    // Проход по всем игрокам
    for (int i = 0; i < players.size(); ++i) {
        int minTrump = 15;  // Максимальное значение козыря
        int minValue = 15; // Максимальное значение карты
        for (const cards& cards : players[i].hand) {
            if (cards.suit == trump.suit) {
                minTrump = std::min(minTrump, cards.value);
            }
            minValue = std::min(minValue, cards.value);
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
bool isOver (std::vector<cards>& deck, std::vector<Player>& players) {
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
// Функция, которая проверяет, надо ли брать карты
bool takingcardss (std::vector<Player>& players) {
    for (const Player player: players) {
        if (player.isTaking == false) {
            return false;
        }
    }
    return true;
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
    std::vector<cards> deck;
    sf::Texture cardsTexture;
     for (int value = 6; value <= 14; ++value) {
        for (const std::string& suit : suits) {
            // Исключаем даму крести
            if (value == 12 && suit == "CLUBS") {
                continue; // Пропускаем добавление карты "Дама Пик"
            }
            cards cards;
            cards.value = value;
            cards.suit = suit;
            std::string filename = "resources/" + std::to_string(value) + suit + ".png";
            if (!cardsTexture.loadFromFile(filename)) { 
                std::cerr << "Ошибка загрузки текстуры: " << filename << std::endl; 
            } else {
                cards.texture = cardsTexture; 
                cards.sprite.setTexture(cards.texture);
                deck.push_back(cards);
            }
        }
    }
    // for (int value = 6; value <= 14; ++value) {
    //     for (const std::string& suit : suits) {
    //         deck.push_back(cards(value, suit));
    //     }
    // }
    // Перемешивание колоды
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
    // Выбор козыря
    cards trump = deck.back();
    deck.pop_back();
    // Возврат козыря в начало колоды
    deck.insert(deck.begin(), trump);
    // Создание игроков
    std::vector<Player> players(4);
    players[0].isAttacker = true;
    players[0].isDefender = false;
    players[1].isAttacker = false;
    players[1].isDefender = false;
    players[2].isAttacker = false;
    players[2].isDefender = false;
    players[3].isAttacker = false;
    players[3].isDefender = false;
    // Раздача карт
    int cardssPerPlayer = 6;
    for (int i = 0; i < cardssPerPlayer; ++i)
    {
        for (int j = 0; j < players.size(); ++j)
        {
            if (!deck.empty()) {
                players[j].hand.push_back(deck.back());
                players[j].isTaking = true;
                deck.pop_back();
            }
        }
    }
    // Сортировка карт в руках игроков
    for (Player& player : players) {
        sortHand(player.hand);
    }
    sf::Font font_1;
    if (!font_1.loadFromFile("resources/ffont.ttf"))
    {
        std::cerr << "Ошибка загрузки шрифта!" << std::endl;
    }
    int selectedcardsIndex = 0;
    int selectedcardsIndexRight = 0;
    // Сообщения о возможности атаковать, выход
    sf::Text message_can_attack;
    message_can_attack.setFont(font_1);
    message_can_attack.setCharacterSize(24);
    message_can_attack.setFillColor(sf::Color::White);
    message_can_attack.setPosition(100.f, 100.f);
    message_can_attack.setString(L"Cannot attack with this cards");
    sf::Text message_can_defend;
    message_can_defend.setFont(font_1);
    message_can_defend.setCharacterSize(24);
    message_can_defend.setFillColor(sf::Color::White);
    message_can_defend.setPosition(100.f, 100.f);
    message_can_defend.setString(L"Cannot defend with this cards");
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
    // Кнопки "Да" и "Нет"
    sf::Text buttonYes;
    buttonYes.setFont(font_1);
    buttonYes.setString("Yes");
    buttonYes.setCharacterSize(24);
    buttonYes.setFillColor(sf::Color::White);
    buttonYes.setPosition(200.f, 200.f);
    sf::Text buttonNo;
    buttonNo.setFont(font_1);
    buttonNo.setString("No");
    buttonNo.setCharacterSize(24);
    buttonNo.setFillColor(sf::Color::White);
    buttonNo.setPosition(400.f, 200.f);
    // Кнопки "Пас" и "Бито"
    sf::Text button_pass;
    button_pass.setFont(font_1);
    button_pass.setString("Pass");
    button_pass.setCharacterSize(24);
    button_pass.setFillColor(sf::Color::White);
    button_pass.setPosition(200.f, 200.f);
    sf::Text button_bito;
    button_bito.setFont(font_1);
    button_bito.setString("Bito");
    button_bito.setCharacterSize(24);
    button_bito.setFillColor(sf::Color::White);
    button_bito.setPosition(400.f, 200.f);
    // Индекс выбранной кнопки (0 - Да, 1 - Нет)
    int selectedButtonIndex = 0;
    // Флаги для управления игрой
    bool showcardsSelectionMessage = true; // Флаг для отображения текста выбора карт
    bool showMessageEscape = false; // Флаг для отображения текста выхода из игры
    bool returnToMenu = false; // Флаг для отображения текста выхода в меню
    bool showMessageWait = false; // Флаг для отображения текста ожидания игроков
    bool showMessage = false; // Флаг для отображения сообщения
    bool showInvalidMove = false; // Флаг для отображения сообщения о неправильном выборе карты для атаки/защиты
    bool showTakecardsMessage = false; // Флаг для отображения сообщения о взятии карты
    bool can_attack = true; // Можно ли атаковать
    bool can_defend = false; // Можно ли защищаться
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
    bool cardsTaken = false;
    std::vector<cards> table; 
    std::vector<cards> attackcardss;
    std::vector<int> selected_cards;
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
            // Определение текущего игрока
            if (countMove == 1) {
                currentPlayerIndexs = whoFirst(players,trump);
            } else {currentPlayerIndexs = findCurrentPlayer(players);}
            int neighborIndex = (currentPlayerIndexs + 1) % players.size(); // Индекс соседа справа
            switch (event.type)
            {
                case sf::Event::Closed:
                    windowss.close();
                    break;
                case sf::Event::KeyPressed:
                    // Проверка надо ли брать карты
                    if (takingcardss(players) == false) {
                        for (int j = 0; j < players.size(); ++j)
                        {
                            while (!deck.empty() && players[j].hand.size() < 6) {
                                players[j].hand.push_back(deck.back());
                                players[j].isTaking = true;
                                deck.pop_back();
                            }
                        }
                    }
                    if (event.key.code == Keyboard::Escape)
                    {
                        showMessageEscape = true;
                        showcardsSelectionMessage = false;
                    }
                    // Выбор карты
                    if(event.key.code == sf::Keyboard::Left)
                    {                        
                        if (selectedcardsIndex > 0)
                        {
                            --selectedcardsIndex;
                        } else {
                            selectedcardsIndex = players[currentPlayerIndexs].hand.size() - 1;
                        }
                    }
                    else if(event.key.code == sf::Keyboard::Right)
                    {
                        // Переход к предыдущей карте с круговым циклом
                        if (selectedcardsIndex < players[currentPlayerIndexs].hand.size() - 1)
                        {
                            ++selectedcardsIndex;
                        } else {
                            selectedcardsIndex = 0;
                        }
                    }
                    // Выбор карты для атаки/защиты
                    else if (event.key.code == sf::Keyboard::Enter && !table.empty() && !cardsTaken)
                    {
                        if (can_attack && !players[currentPlayerIndexs].cardsSelected) 
                        {
                            // Атака
                            if (attack(table, players[currentPlayerIndexs].hand[selectedcardsIndex])) {
                                attackcardss.push_back(players[currentPlayerIndexs].hand[selectedcardsIndex]);
                                table.push_back(players[currentPlayerIndexs].hand[selectedcardsIndex]);
                                players[currentPlayerIndexs].hand.erase(players[currentPlayerIndexs].hand.begin() + selectedcardsIndex);
                                can_attack = false;
                                can_defend = true;
                                showcardsSelectionMessage = false;
                                showMessage = false;
                                showInvalidMove = false;
                                showMessageWait = true;
                                playersChoseNo = 0;
                                cardsTaken = true;
                            } else {
                                showInvalidMove = true;
                                showcardsSelectionMessage = true;
                            }
                        } 
                        else if (can_defend && !players[currentPlayerIndexs].cardsSelected) 
                        {
                            // Защита
                            if (defense(attackcardss.back(), players[currentPlayerIndexs].hand[selectedcardsIndex], trump)) {
                                table.push_back(players[currentPlayerIndexs].hand[selectedcardsIndex]);
                                players[currentPlayerIndexs].hand.erase(players[currentPlayerIndexs].hand.begin() + selectedcardsIndex);
                                players[neighborIndex].isDefender = false;
                                players[currentPlayerIndexs].isDefender = true;
                                players[currentPlayerIndexs].isAttacker = true;
                                can_attack = true;
                                can_defend = false;
                                showcardsSelectionMessage = false;
                                showMessage = false;
                                showInvalidMove = false;
                                showMessageWait = true;
                                playersChoseNo = 0;
                                cardsTaken = true;
                            } else {
                                showInvalidMove = true;
                                showcardsSelectionMessage = true;
                            }
                        }
                    }
                    // Взятие карты, если колода не пуста
                    else if (event.key.code == sf::Keyboard::Enter && cardsTaken && !deck.empty())
                    {
                        players[currentPlayerIndexs].hand.push_back(deck.back());
                        deck.pop_back();
                        cardsTaken = false;
                        can_attack = true;
                        can_defend = false;
                        showTakecardsMessage = false;
                    }
                    // Переход к следующему игроку, если атака завершена
                    else if (event.key.code == sf::Keyboard::Enter && cardsTaken && deck.empty())
                    {
                        // Переход к следующему игроку, если карта была взята или колода пуста
                        players[currentPlayerIndexs].isAttacker = false;
                        players[neighborIndex].isAttacker = true;
                        can_attack = true;
                        can_defend = false;
                        table.clear();
                        attackcardss.clear();
                        selected_cards.clear();
                        showcardsSelectionMessage = false;
                        showMessage = false;
                        showInvalidMove = false;
                        showMessageWait = false;
                        playersChoseNo = 0;
                        countMove++;
                        cardsTaken = false;
                        // Переход к следующему игроку
                    } 
                    // Если игрок не атакует, а карты на столе есть
                    else if (event.key.code == sf::Keyboard::Enter && !table.empty() && can_attack)
                    {
                        players[currentPlayerIndexs].isAttacker = false;
                        players[neighborIndex].isAttacker = true;
                        can_attack = true;
                        can_defend = false;
                        table.clear();
                        attackcardss.clear();
                        selected_cards.clear();
                        showcardsSelectionMessage = false;
                        showMessage = false;
                        showInvalidMove = false;
                        showMessageWait = false;
                        playersChoseNo = 0;
                        countMove++;
                        cardsTaken = false;
                        // Переход к следующему игроку
                    }
                    //Если на столе нет карт, а колода пуста, то игрок может взять карты, если у него не все карты сброшены
                    else if (event.key.code == sf::Keyboard::Enter && table.empty() && deck.empty() && !players[currentPlayerIndexs].hand.empty())
                    {
                        // Переход к следующему игроку, если карта была взята или колода пуста
                        players[currentPlayerIndexs].isAttacker = false;
                        players[neighborIndex].isAttacker = true;
                        can_attack = true;
                        can_defend = false;
                        table.clear();
                        attackcardss.clear();
                        selected_cards.clear();
                        showcardsSelectionMessage = false;
                        showMessage = false;
                        showInvalidMove = false;
                        showMessageWait = false;
                        playersChoseNo = 0;
                        countMove++;
                        cardsTaken = false;
                        // Переход к следующему игроку
                    }
                    else if (event.key.code == sf::Keyboard::Enter && table.empty() && deck.empty() && players[currentPlayerIndexs].hand.empty())
                    {
                        // Переход к следующему игроку, если карта была взята или колода пуста
                        players[currentPlayerIndexs].isAttacker = false;
                        players[neighborIndex].isAttacker = true;
                        can_attack = true;
                        can_defend = false;
                        table.clear();
                        attackcardss.clear();
                        selected_cards.clear();
                        showcardsSelectionMessage = false;
                        showMessage = false;
                        showInvalidMove = false;
                        showMessageWait = false;
                        playersChoseNo = 0;
                        countMove++;
                        cardsTaken = false;
                        // Переход к следующему игроку
                    }
                    // Если у игрока нет подходящих карт для атаки
                    else if (event.key.code == sf::Keyboard::Enter && can_attack && !players[currentPlayerIndexs].cardsSelected)
                    {
                        // Игрок не атакует, передавая ход следующему
                        players[currentPlayerIndexs].isAttacker = false;
                        players[neighborIndex].isAttacker = true;
                        can_attack = true;
                        can_defend = false;
                        table.clear();
                        attackcardss.clear();
                        selected_cards.clear();
                        showcardsSelectionMessage = false;
                        showMessage = false;
                        showInvalidMove = false;
                        showMessageWait = false;
                        playersChoseNo = 0;
                        countMove++;
                        cardsTaken = false;
                    }
                    // Если игрок не защищается, а карты на столе есть
                    else if (event.key.code == sf::Keyboard::Enter && can_defend && !players[currentPlayerIndexs].cardsSelected)
                    {
                        // Игрок не защищается, передавая ход следующему
                        players[currentPlayerIndexs].isDefender = false;
                        players[neighborIndex].isAttacker = true;
                        can_attack = true;
                        can_defend = false;
                        table.clear();
                        attackcardss.clear();
                        selected_cards.clear();
                        showcardsSelectionMessage = false;
                        showMessage = false;
                        showInvalidMove = false;
                        showMessageWait = false;
                        playersChoseNo = 0;
                        countMove++;
                        cardsTaken = false;
                    }
                    // Обработка кнопок "Да" и "Нет"
                    else if (event.key.code == sf::Keyboard::Enter && (showMessageEscape || showcardsSelectionMessage))
                    {
                        if (selectedButtonIndex == 0) // "Да"
                        {
                            // Выход из игры или продолжение игры
                            if (showMessageEscape) {
                                returnToMenu = true;
                                showMessageEscape = false;
                            } else if (showcardsSelectionMessage) {
                                // Выбор карты
                                if (!players[currentPlayerIndexs].cardsSelected) {
                                    // Установка флага выбора карты
                                    players[currentPlayerIndexs].cardsSelected = true;
                                    selected_cards.push_back(selectedcardsIndex);
                                    if (selected_cards.size() == 4) {
                                        selected_cards.clear();
                                    }
                                }
                                // Сброс флага выбора карты
                                players[currentPlayerIndexs].cardsSelected = false;
                                showcardsSelectionMessage = false;
                            }
                        } else if (selectedButtonIndex == 1) // "Нет"
                        {
                            // Сброс флагов
                            showMessageEscape = false;
                            showcardsSelectionMessage = false;
                            showTakecardsMessage = false;
                        }
                    }
                    // Обработка кнопок "Пас" и "Бито"
                    else if (event.key.code == sf::Keyboard::Enter && (showTakecardsMessage))
                    {
                        if (selectedButtonIndex == 0) // "Пас"
                        {
                            // Игрок берет карты
                            players[currentPlayerIndexs].isAttacker = false;
                            players[neighborIndex].isAttacker = true;
                            can_attack = true;
                            can_defend = false;
                            table.clear();
                            attackcardss.clear();
                            selected_cards.clear();
                            showcardsSelectionMessage = false;
                            showMessage = false;
                            showInvalidMove = false;
                            showMessageWait = false;
                            playersChoseNo = 0;
                            countMove++;
                            cardsTaken = false;
                            // Переход к следующему игроку
                        } else if (selectedButtonIndex == 1) // "Бито"
                        {
                            // Игрок берет карты
                            players[currentPlayerIndexs].isAttacker = false;
                            players[neighborIndex].isAttacker = true;
                            can_attack = true;
                            can_defend = false;
                            table.clear();
                            attackcardss.clear();
                            selected_cards.clear();
                            showcardsSelectionMessage = false;
                            showMessage = false;
                            showInvalidMove = false;
                            showMessageWait = false;
                            playersChoseNo = 0;
                            countMove++;
                            cardsTaken = false;
                            // Переход к следующему игроку
                        }
                    }
                    break;
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        // Обработка клика по кнопкам "Да" и "Нет"
                        if (showMessageEscape || showcardsSelectionMessage) {
                            sf::Vector2f mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                            if (buttonYes.getGlobalBounds().contains(mousePos)) {
                                selectedButtonIndex = 0; // "Да"
                            } else if (buttonNo.getGlobalBounds().contains(mousePos)) {
                                selectedButtonIndex = 1; // "Нет"
                            }
                        }
                        // Обработка клика по кнопкам "Пас" и "Бито"
                        else if (showTakecardsMessage) {
                            sf::Vector2f mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                            if (button_pass.getGlobalBounds().contains(mousePos)) {
                                selectedButtonIndex = 0; // "Пас"
                            } else if (button_bito.getGlobalBounds().contains(mousePos)) {
                                selectedButtonIndex = 1; // "Бито"
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
        // Отрисовка карт игроков
        Texture back_cards_texture;
        if (!back_cards_texture.loadFromFile("resources/back.png")) { return 1; }
        RectangleShape back_cards_sprite(Vector2f(77,110));
        back_cards_sprite.setTexture(&back_cards_texture);
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
            for (size_t cardsIndex = 0; cardsIndex < players[playerIndex].hand.size(); ++cardsIndex) {
                cards& cards = players[playerIndex].hand[cardsIndex];
                if (players[playerIndex].isAttacker) {
                    // Атакующий игрок видит значения своих карт
                    cards.sprite.setTexture(cards.texture);
                    // Определение позиции карты в зависимости от её индекса 
                    cards.sprite.setPosition(Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 800));
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (std::find(selected_cards.begin(), selected_cards.end(), cardsIndex) != selected_cards.end())
                    {
                        cards.sprite.setColor(Color::Magenta);
                    } else if (cardsIndex == selectedcardsIndex)
                    {
                        cards.sprite.setColor(Color::Yellow);
                    } else {
                        cards.sprite.setColor(Color::White);
                    }
                    windowss.draw(cards.sprite);
                    ++id; 
                } else {
                    // У остальных игроков карты рубашкой вверх
                    back_cards_sprite.setRotation(rotation);
                    if (playerIndex == 1) {
                        back_cards_sprite.setPosition(Vector2f(1820, 123 + (800 / players[playerIndex].hand.size()) * id));
                    } else if (playerIndex == 2) {
                        back_cards_sprite.setPosition(Vector2f(200 + (1300 / players[playerIndex].hand.size()) * id, 150));
                    } else if (playerIndex == 3) {
                        back_cards_sprite.setPosition(Vector2f(100, 200 + (800 / players[playerIndex].hand.size()) * id));
                    }
                    windowss.draw(back_cards_sprite);
                    id++;
                }
            }
        }
        // Рисование сообщения о выборе карт, если флаг установлен 
        sf::Vector2u windowSize = windowss.getSize();
        if (showcardsSelectionMessage) {
            if (can_attack) {
                message.setString(L"Select cards for attack");
            } else if (can_defend) {
                message.setString(L"Select cards for defend");
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
        if (showMessageEscape)
        {
            windowss.draw(message_escape);
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
            windowss.draw(buttonYes);
            windowss.draw(buttonNo);
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
        // Отображение текста ожидания, если флаг активен
        if (showMessageWait) {
            wait_message.setString("Ожидаем других игроков: " + std::to_string(players.size() - playersChoseNo));
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