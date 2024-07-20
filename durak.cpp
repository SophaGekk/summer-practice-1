#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <set>
#include <random>
#include <fstream> // For file operations
#include <sstream> // For string manipulation
#include "durak.h"
using namespace sf;

void savegame(std::vector<Playerr>& players, int& currentPlayerIndex, int& neighborIndex, int& move, const std::vector<Cardd>& attackCards, const std::vector<Cardd>& defendingCards, const std::vector<Cardd>& table, const std::vector<Cardd>& deck, Cardd& trump, bool& attacking, bool& defending) {
    std::ofstream saveFile("resources/saveDurakGame.txt");
    if (!saveFile.is_open()) {
        std::cerr << "Ошибка загрузки: resources/saveDurakGame.txt" << std::endl;
        return;
    }
    // Сохраняем карты каждого игрока
    for (const auto& player : players) {
        for (const auto& card : player.hand) {
            saveFile << card.value << " " << card.suit << " ";
        }
        saveFile << "\n";
    }
    saveFile <<attacking << "\n";
    saveFile <<defending << "\n";
    //isAttacker и hasWon
    if(attacking)
    {
        saveFile <<players[currentPlayerIndex].isAttacker << "\n";
        saveFile <<players[neighborIndex].isDefender << "\n";
        
    }
    else if (defending)
    {
        saveFile <<players[neighborIndex].isAttacker << "\n";
        saveFile <<players[currentPlayerIndex].isDefender << "\n";
    }
    
    for (int i = 0; i < players.size(); ++i) {
        saveFile <<players[i].hasWon << "\n";
    }
    // Сохраняем атакующие карты
    for (int i = 0; i < attackCards.size(); ++i) {
        saveFile << attackCards[i].suit << " " << attackCards[i].value << " ";
    }
    saveFile << "\n";
    // Сохраняем защищающиеся карты
    for (int i = 0; i < defendingCards.size(); ++i) {
        saveFile << defendingCards[i].suit << " " << defendingCards[i].value << " ";
    }
    saveFile << "\n";
    // Сохраняем карты, которые лежат на столе
    for (int i = 0; i < table.size(); ++i) {
        saveFile << table[i].suit << " " << table[i].value << " ";
    }
    saveFile << "\n";
    // Сохраняем колоду карт
    for (int i = 0; i < deck.size(); ++i) {
        saveFile << deck[i].suit << " " << deck[i].value << " ";
    }
    saveFile << "\n";
    // Сохраняем козырь
    saveFile << trump.suit << " " << trump.value << " ";
    saveFile << "\n";
    // Сохраняем индексы текущего игрока и его соседа
    saveFile << currentPlayerIndex << "\n";
    saveFile << neighborIndex << "\n";
    // Сохраняем номер раунда
    saveFile << move << "\n";
}
void loadgame(std::vector<Playerr>& players, int& currentPlayerIndex, int& neighborIndex, int& move, std::vector<Cardd>& attackCards, std::vector<Cardd>& defendingCards, std::vector<Cardd>& table, std::vector<Cardd>& deck, Cardd& trump, bool& attacking, bool& defending) {
    std::ifstream loadFile("resources/saveDurakGame.txt");
    if (!loadFile.is_open()) {
        std::cerr << "Ошибка загрузки: resources/saveDurakGame.txt" << std::endl;
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
                Cardd card;
                card.value = rank;
                card.suit = suit;
                // Загрузка текстуры и установка спрайта
                std::string filename = "resources/" + std::to_string(rank) + suit + ".png";
                if (!CardsTexture.loadFromFile(filename)) {
                    std::cerr << "Ошибка загрузки: " << filename << std::endl;
                } else {
                    card.texture = CardsTexture;
                    card.sprite.setTexture(card.texture);
                    card.sprite.setScale(0.6f, 0.6f); // Уменьшение размера спрайта 
                }
                player.hand.push_back(card);
            }
        }
    }
    loadFile >> attacking;
    loadFile >> defending;
    if(attacking)
    {
        loadFile >>players[currentPlayerIndex].isAttacker;
        loadFile >>players[neighborIndex].isDefender;
        
    }
    else if (defending)
    {
        loadFile >>players[neighborIndex].isAttacker;
        loadFile >>players[currentPlayerIndex].isDefender;
    }
    for (int i = 0; i < players.size(); ++i) {

        loadFile >>players[i].hasWon;
    }
    // Загружаем карты массива attackCards
    if (std::getline(loadFile, line)) {
        std::istringstream iss(line);
        int rank;
        std::string suit;
        for (int i = 0; i < attackCards.size(); ++i) {
            iss >> suit >> rank;
            attackCards[i].suit = suit;
            attackCards[i].value = rank;
            if (suit != "0") {
                std::string filename = "resources/" + std::to_string(rank) + suit + ".png";
                if (!CardsTexture.loadFromFile(filename)) {
                    std::cerr << "Ошибка загрузки: " << filename << std::endl;
                } else  {
                    attackCards[i].texture = CardsTexture;
                    attackCards[i].sprite.setTexture(attackCards[i].texture);
                }
            }
        }
    }
    // Загружаем карты массива defendingCards
    if (std::getline(loadFile, line)) {
        std::istringstream iss(line);
        int rank;
        std::string suit;
        for (int i = 0; i < defendingCards.size(); ++i) {
            iss >> suit >> rank;
            defendingCards[i].suit = suit;
            defendingCards[i].value = rank;
            if (suit != "0") {
                std::string filename = "resources/" + std::to_string(rank) + suit + ".png";
                if (!CardsTexture.loadFromFile(filename)) {
                    std::cerr << "Ошибка загрузки: " << filename << std::endl;
                } else  {
                    defendingCards[i].texture = CardsTexture;
                    defendingCards[i].sprite.setTexture(defendingCards[i].texture);
                }
            }
        }
    }
    // Загружаем карты массива table
    if (std::getline(loadFile, line)) {
        std::istringstream iss(line);
        int rank;
        std::string suit;
        for (int i = 0; i < table.size(); ++i) {
            iss >> suit >> rank;
            table[i].suit = suit;
            table[i].value = rank;
            if (suit != "0") {
                std::string filename = "resources/" + std::to_string(rank) + suit + ".png";
                if (!CardsTexture.loadFromFile(filename)) {
                    std::cerr << "Ошибка загрузки: " << filename << std::endl;
                } else  {
                    table[i].texture = CardsTexture;
                    table[i].sprite.setTexture(table[i].texture);
                }
            }
        }
    }
    // Загружаем карты массива deck
    if (std::getline(loadFile, line)) {
        std::istringstream iss(line);
        int rank;
        std::string suit;
        for (int i = 0; i < deck.size(); ++i) {
            iss >> suit >> rank;
            deck[i].suit = suit;
            deck[i].value = rank;
            if (suit != "0") {
                std::string filename = "resources/" + std::to_string(rank) + suit + ".png";
                if (!CardsTexture.loadFromFile(filename)) {
                    std::cerr << "Ошибка загрузки: " << filename << std::endl;
                } else  {
                    deck[i].texture = CardsTexture;
                    deck[i].sprite.setTexture(deck[i].texture);
                }
            }
        }
    }
    //Загружаем козырь
    if (std::getline(loadFile, line)) {
        std::istringstream iss(line);
        int rank;
        std::string suit;
        iss >> suit >> rank;
        trump.suit = suit;
        trump.value = rank;
        if (suit != "0") {
            std::string filename = "resources/" + std::to_string(rank) + suit + ".png";
            if (!CardsTexture.loadFromFile(filename)) {
                std::cerr << "Ошибка загрузки: " << filename << std::endl;
            } else  {
                trump.texture = CardsTexture;
                trump.sprite.setTexture(trump.texture);
            }
        }
    }
    // Загружаем индексы текущего игрока и его соседа
    loadFile >> currentPlayerIndex;
    loadFile >> neighborIndex;
    // Загружаем номер раунда
    loadFile >> move;
    loadFile.close();
}
// Функция для поиска текущего игрока, чей ход необходимо выполнить
int FindCurrentPlayerr_isAttacker(const std::vector<Playerr>& Playerrs) {
    for (int i = 0; i < Playerrs.size(); ++i) {
        if (!Playerrs[i].hand.empty() && !Playerrs[i].hasWon) {
            if (Playerrs[i].isAttacker)
            {
                return i; // Возвращает индекс текущего атакующего игрока
            }
        }
    }
    return -1; // Все игроки выиграли
}
// Функция для поиска текущего игрока, чей ход необходимо выполнить
int FindCurrentPlayerr_isDefender(const std::vector<Playerr>& Playerrs) {
    for (int i = 0; i < Playerrs.size(); ++i) {
        if (!Playerrs[i].hand.empty() && !Playerrs[i].hasWon) {
            if (Playerrs[i].isDefender)
            {
                return i; // Возвращает индекс текущего атакующего игрока
            }
        }
    }
    return -1; // Все игроки выиграли
}
int FindNextPlayerrWithCardds(const std::vector<Playerr>& Playerrs, int currentIndex) {
    int nextIndex = (currentIndex + 1) % Playerrs.size();
    while (Playerrs[nextIndex].hand.empty() && nextIndex != currentIndex) {
        // Пропускаем игроков без карт
        nextIndex = (nextIndex + 1) % Playerrs.size();
    }
    return nextIndex;
}
// Функция для проверки на атаку
bool attack(std::vector<Cardd>& table, Cardd& attackCardd) {
    if (table.empty()) {
        return true; // Можно атаковать любой картой, если стол пустой
    } else if (!table.empty()) {
        for (int i = 0; i < table.size(); i++) {
            if (attackCardd.value == table[i].value) {
                return true; // Можно атаковать картой с таким же значением, что и на столе
            }
        }
        return false; // Нельзя атаковать картой с другим значением, если стол не пустой
    }
}
// Функция для проверки на защиту
bool defense(Cardd& attackCardd, Cardd& defenseCardd, Cardd& trump) {
    if ((defenseCardd.value > attackCardd.value && defenseCardd.suit == attackCardd.suit) ||
        (defenseCardd.suit == trump.suit && attackCardd.suit != trump.suit)) {
        return true;
    }
    return false;
}
// Функция для определения, кто первый ходит
int whoFirst (std::vector<Playerr>& Playerrs, Cardd& trump) {
    // Словарь для хранения минимального козыря и значения для каждого игрока
    std::map<int, std::pair<int, int>> PlayerrData;
    // Проход по всем игрокам
    for (int i = 0; i < Playerrs.size(); i++) {
        int minTrump = 20;  // Максимальное значение козыря
        int minValue = 20; // Максимальное значение карты
        for (Cardd& Cardd : Playerrs[i].hand) {
            if (Cardd.suit == trump.suit) {
                minTrump = std::min(minTrump, Cardd.value);
            }
            minValue = std::min(minValue, Cardd.value);
        }
        // Сохранение данных в словарь
        PlayerrData[i] = std::make_pair(minTrump, minValue);
    }
    // Определение первого игрока
    int firstPlayerrIndexWithTrump = 0;
    int firstPlayerrIndex = 0;
    int minTrump = PlayerrData[0].first;
    int minValue = PlayerrData[0].second;
    for (int i = 1; i < Playerrs.size(); i++) {
        if (PlayerrData[i].first < minTrump) {
            minTrump = PlayerrData[i].first;
            firstPlayerrIndexWithTrump = i;
        } 
        else if (PlayerrData[i].second < minValue) {
            minValue = PlayerrData[i].second;
            firstPlayerrIndex = i;
        }
    }
    if (minTrump == 20) {
        return firstPlayerrIndex;
    } else {
        return firstPlayerrIndexWithTrump;
    } 
}
// Раздача карт
void dealingCards (std::vector<Cardd>& decks, std::vector<Playerr>& players) {   
    for (auto& Playerr : players) {
        if (!decks.empty()) {
            if (Playerr.hand.size() < 6) {
                Playerr.hand.push_back(decks.back());
                decks.pop_back();
            }
        } else {
            break;
        }
    }
}
// Глобальная переменная для индекса текущего игрока
int CurrentPlayerrIndex = 0;
// Глобальная переменная для индекса соседа справа текущего игрока
int neighborIndex = 1;
// Глобальная переменная для индекса промежуточного игрока (для переключения между атакующим и защищающимся)
int middle = -1;
// Глобальная переменная для подсчета раундов
int move = 1;
int who_move_transition(const std::vector<Playerr>& players, int& currentIndex, int& number_of_players_people){
    int nextIndex = (currentIndex + 1) % number_of_players_people;
    while (players[nextIndex].hand.empty() && nextIndex != currentIndex) {
        // Пропускаем игроков без карт
        nextIndex = (nextIndex + 1) % number_of_players_people;
    }
    return nextIndex;
}
int main_durakgame(sf::RenderWindow& windowss) {
    int number_of_players_people=4;
    int activePlayers = 0;
    int People_win = 0;
    srand(time(0));
    // Загрузка шрифта
    sf::Font font_1;
    if (!font_1.loadFromFile("resources/arial.ttf")) {
        std::cerr << "Ошибка загрузки: resources/arial.ttf" << std::endl;
        return -1;
    }
    // Загрузка текстуры стола
    Texture tableTexture;
    if (!tableTexture.loadFromFile("resources/table.png")) {
        std::cerr << "Ошибка загрузки: resources/table.png" << std::endl;
        return -1;
    }
        // Создаем затемнение фона
    sf::RectangleShape Black_who;
    Black_who.setSize(sf::Vector2f(windowss.getSize().x, windowss.getSize().y));
    Black_who.setFillColor(sf::Color::Black); //черный цвет

    sf::Text message_who;
    message_who.setFont(font_1);
    message_who.setCharacterSize(35);
    message_who.setFillColor(sf::Color::White);
    message_who.setPosition(100.f, 100.f);

    // Текст вопроса
    message_who.setString(L"Нажмите Enter");

    sf::Text message_who_move_transition;
    message_who_move_transition.setFont(font_1);
    message_who_move_transition.setCharacterSize(35);
    message_who_move_transition.setFillColor(sf::Color::White);
    message_who_move_transition.setPosition(100.f, 100.f);
    // Создание спрайта стола
    RectangleShape background(Vector2f(1920,1080));
    background.setTexture(&tableTexture);
    background.setPosition(Vector2f(0,0));
    // Масти карт
    std::vector<std::string> suits = {"HEARTS", "DIAMONDS", "CLUBS", "SPADES"};
    // основная колода 
    std::vector<Cardd> decks;
    decks.reserve(36); // Ограничение на 36 ячеек
    for (int value = 6; value <= 14; ++value) {
        for (const std::string& suit : suits) {
            Cardd Cardd;
            Cardd.value = value;
            Cardd.suit = suit;
            std::string filename = "resources/" + std::to_string(value) + suit + ".png";
            sf::Texture CarddTexture;
            if (!CarddTexture.loadFromFile(filename)) { 
                std::cerr << "Ошибка загрузки: " << filename << std::endl; 
            } else {
                Cardd.texture = CarddTexture; 
                Cardd.sprite.setTexture(Cardd.texture);
                Cardd.sprite.setScale(0.6f, 0.6f); // Уменьшение размера спрайта 
                decks.push_back(Cardd);
            }
        }
    }
    // Перемешивание колоды
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(decks.begin(), decks.end(), g);
    // Выбор козыря
    Cardd trump = decks.back();
    decks.pop_back();
    // Возврат козыря в начало колоды
    decks.insert(decks.begin(), trump);
    // Создание игроков
    std::vector<Playerr> Playerrs(4);
    Playerrs[0].name = "Василий";
    Playerrs[1].name = "Мария";
    Playerrs[2].name = "Степан";
    Playerrs[3].name = "Екатерина";
    Playerrs[0].isAttacker = false;
    Playerrs[1].isAttacker = false;
    Playerrs[2].isAttacker = false;
    Playerrs[3].isAttacker = false;
    Playerrs[0].isDefender = false;
    Playerrs[1].isDefender = false;
    Playerrs[2].isDefender = false;
    Playerrs[3].isDefender = false;
    Playerrs[0].hasWon = false;
    Playerrs[1].hasWon = false;
    Playerrs[2].hasWon = false;
    Playerrs[3].hasWon = false;
    // Текст для сохранения игры
    sf::Text saveText(L"Сохранить игру", font_1, 24);
    saveText.setFillColor(sf::Color::White);
    saveText.setPosition(800.f, 500.f);
    // Текст для загрузки игры
    sf::Text loadText(L"Загрузить игру", font_1, 24);
    loadText.setFillColor(sf::Color::White);
    loadText.setPosition(1020.f, 500.f);
    // Сообщение о победе игрока
    bool messageWin = false;
    sf::Text messageWinner;
    messageWinner.setFont(font_1);
    messageWinner.setCharacterSize(24);
    messageWinner.setFillColor(sf::Color::White);
    messageWinner.setPosition(100.f, 100.f);
    messageWinner.setString(L"Вы выйграли!");
    // Сообщение о выборе карт
    sf::Text messageReset;
    messageReset.setFont(font_1);
    messageReset.setCharacterSize(30);
    messageReset.setFillColor(sf::Color::White);
    messageReset.setPosition(100.f, 100.f);
    messageReset.setString(L"Есть ещё что-нибудь выбрать?");
    // Кнопки "Да" и "Нет"
    // Индекс выбранной кнопки (0 - Да, 1 - Нет)
    sf::Text ButtonnYes;
    ButtonnYes.setFont(font_1);
    ButtonnYes.setString(L"Да");
    ButtonnYes.setCharacterSize(24);
    ButtonnYes.setFillColor(sf::Color::White);
    ButtonnYes.setPosition(200.f, 200.f);
    sf::Text ButtonnNo;
    ButtonnNo.setFont(font_1);
    ButtonnNo.setString(L"Нет");
    ButtonnNo.setCharacterSize(24);
    ButtonnNo.setFillColor(sf::Color::White);
    ButtonnNo.setPosition(400.f, 200.f);
    // Сообщение о выходе из игры
    sf::Text message_escape;
    message_escape.setFont(font_1);
    message_escape.setCharacterSize(24);
    message_escape.setFillColor(sf::Color::White);
    message_escape.setPosition(100.f, 100.f);
    message_escape.setString(L"Вы действительно хотите выйти из игры?");
    // Сообщение о неверном выборе карты
    sf::Text messageInvalid;
    messageInvalid.setFont(font_1);
    messageInvalid.setCharacterSize(24);
    messageInvalid.setFillColor(sf::Color::White);
    messageInvalid.setPosition(100.f, 100.f);
    messageInvalid.setString(L"Неверная карта, выберите другую!");
    //Кнопки "Да" и "Нет" для сохранения и загрузки
    sf::Text buttonYes;
    buttonYes.setFont(font_1);
    buttonYes.setString(L"Да");
    buttonYes.setCharacterSize(24);
    buttonYes.setFillColor(sf::Color::White);
    buttonYes.setPosition(800.f, 600.f);
    sf::Text buttonNo;
    buttonNo.setFont(font_1);
    buttonNo.setString(L"Нет");
    buttonNo.setCharacterSize(24);
    buttonNo.setFillColor(sf::Color::White);
    buttonNo.setPosition(1000.f, 600.f);
    // Флаги для иправления игрой
    bool showMessageEscape = false; // Флаг для отображения текста выхода из игры
    bool showInvalidMessage = false; // Флаг для отображения текста о неправильном выборе карты
    bool showCarddSelectionMessage = false; // Флаг для отображения текста выбора карт
    bool returnToMenu = false; // Флаг для отображения текста выхода в меню
    bool canTakeCardd = false; // Можно ли выбирать карту
    bool moveTransition = false; // Переход хода между раундами
    bool moveTransitionInMove = false; // Переход хода внутри раунда
    bool EndGame = false; // Окончание игры
    bool GameOver = false; // Проигрыш
    bool YouWin = false; // Выигрыш
    bool showSaveLoadMenu = false; // // Флаг для отображения текста сохранения
    bool attacking = false;
    bool defending = false;
    int selectedButtonnIndex = 0; // Индекс выбранной кнопки для "Yes" и "No"
    int selectedButtonnIndex_2 = 0; // Индекс выбранной кнопки для "Return to menu" и "Restart Game"
    int selectedSaveButtonIndex = 0; // Индекс выбранной кнопки для "Save Game" и "Load Game"
    int selectedsaveyesnobuttonindex = 0;
    int shift = 0; // Листались ли карты
    // Создаем затемнение фона
    sf::RectangleShape backgroundFade;
    backgroundFade.setSize(sf::Vector2f(windowss.getSize().x, windowss.getSize().y));
    backgroundFade.setFillColor(sf::Color(0, 0, 0, 150)); // Полупрозрачный черный цвет
    // Загрузка изображения поражения и победы
    sf::Texture youWinTexture, gameOverTexture;
    if (!youWinTexture.loadFromFile("resources/YouWin.png")) {
        std::cerr << "Ошибка загрузки: resources/YouWin.png" << std::endl;
    }
    if (!gameOverTexture.loadFromFile("resources/YouLose.png")) {
        std::cerr << "Ошибка загрузки: resources/YouLose.png" << std::endl;
    }
    sf::Sprite endGameSprite;
    // Создаем кнопки "Return to menu" и "Restart Game"
    Buttonn menuButtonn("Вернуться в меню", sf::Vector2f(400, 50), 30, sf::Color::Transparent, sf::Color::White, font_1);
    Buttonn restartButtonn("Перезапустить игру", sf::Vector2f(400, 50), 30, sf::Color::Transparent, sf::Color::White, font_1);
    // Установка позиции кнопок "Return to menu" и "Restart Game"
    menuButtonn.setPosition(sf::Vector2f(770, 640));
    restartButtonn.setPosition(sf::Vector2f(770, 740));
    std::vector<Buttonn> Buttonns = {menuButtonn, restartButtonn}; // Вектор всех кнопок
    // Логика игры
    std::vector<Cardd> table; // Все карты, которые лежат на столе
    std::vector<Cardd> attackCardds; // Карты атакующего для отрисовки
    std::vector<Cardd> defendingCardds; // Карты защищаегося для отрисовки и логики игры
    Cardd attackCardd; // атакующая карта 
    int selectedCarddIndex = 0;
    // Проставление флагов
    attacking = true;
    canTakeCardd = true; // Можно выбрать карту для атаки
    showCarddSelectionMessage = true;
    int first = 0;
    while(first != 6)
    {
        first++;
        dealingCards (decks, Playerrs);
    }
    //Определение текущего игрока
    CurrentPlayerrIndex = whoFirst(Playerrs,trump);
    Playerrs[CurrentPlayerrIndex].isAttacker = true;
    // Определение соседа справа
    neighborIndex = FindNextPlayerrWithCardds(Playerrs, CurrentPlayerrIndex);
    // Определение соседа справа
    Playerrs[neighborIndex].isDefender = true;

    while (windowss.isOpen()) {
        // Обработка событий
        sf::Event event;
        while (windowss.pollEvent(event)) {
            // Escape - выход
            if (event.type == sf::Event::KeyPressed && event.key.code == Keyboard::Escape) {
                showCarddSelectionMessage = false;
                showMessageEscape = true;
            }
             // S - сохранить игру
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::S) {
                showSaveLoadMenu = true;
                saveText.setFillColor(sf::Color::Red);
                loadText.setFillColor(sf::Color::White);
                selectedSaveButtonIndex = 0; // Выбираем "Save Game" по умолчанию
            }
            // L - загрузить игру
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::L) {
                showSaveLoadMenu = true;
                loadText.setFillColor(sf::Color::Red);
                saveText.setFillColor(sf::Color::White);
                selectedSaveButtonIndex = 1; // Выбираем "Load Game" по умолчанию
            }
            // Обработка событий для меню сохранения/загрузки
            if (showSaveLoadMenu) {
                if (selectedSaveButtonIndex == 0) {
                    if (event.key.code == Keyboard::Left) {
                        selectedsaveyesnobuttonindex = 0;
                    } else if (event.key.code == Keyboard::Right) {
                        selectedsaveyesnobuttonindex = 1;
                    } else if (event.key.code == Keyboard::Enter) {
                        if (selectedsaveyesnobuttonindex == 0) {
                            savegame(Playerrs, CurrentPlayerrIndex, neighborIndex, move, attackCardds, defendingCardds, table, decks, trump, attacking, defending);
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
                } else if (selectedSaveButtonIndex == 1) {
                    if (event.key.code == Keyboard::Left) {
                        selectedsaveyesnobuttonindex = 0;
                    } else if (event.key.code == Keyboard::Right) {
                        selectedsaveyesnobuttonindex = 1;
                    } else if (event.key.code == Keyboard::Enter) {
                        if (selectedsaveyesnobuttonindex == 0) {
                            loadgame(Playerrs, CurrentPlayerrIndex, neighborIndex, move, attackCardds, defendingCardds, table, decks, trump, attacking, defending);
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
            }
            // Конец игры
            if (event.type == sf::Event::KeyPressed && EndGame) {
                showCarddSelectionMessage = false;
                // Up и Down - переключение между кнопками "Menu" и "Restart"
                if (event.key.code == sf::Keyboard::Up) {
                    selectedButtonnIndex_2 = (selectedButtonnIndex_2 - 1 + 2) % 2;
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    selectedButtonnIndex_2 = (selectedButtonnIndex_2 + 1) % 2;
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    // Проверка, выбрана ли кнопка "Menu"
                    if (selectedButtonnIndex_2 == 0) {
                        return 0; 
                        break;
                    }
                    // Проверка, выбрана ли кнопка "Restart"
                    else if(selectedButtonnIndex_2 == 1) {
                        EndGame = false;
                        GameOver = false;
                        YouWin = false;
                        CurrentPlayerrIndex = 0;
                        canTakeCardd = false;
                        // Перезапуск основного игрового цикла
                        main_durakgame(windowss);  
                        return 0; 
                        break;
                    }
                }
            }
            // Обработка кнопок "Да", "Нет"
            if (event.type == sf::Event::KeyPressed && (showCarddSelectionMessage || showMessageEscape)) {
                showCarddSelectionMessage = false;
                // Left и Right - переключение между кнопками "Да" и "Нет"
                if (event.key.code == sf::Keyboard::Left) {
                    selectedButtonnIndex = (selectedButtonnIndex - 1 + 2) % 2;
                } 
                else if (event.key.code == sf::Keyboard::Right) {
                    selectedButtonnIndex = (selectedButtonnIndex + 1) % 2;
                } 
                else if (event.key.code == sf::Keyboard::Enter) {
                    // Проверка, выбрана ли кнопка "Да" 
                    if (showMessageEscape && selectedButtonnIndex == 0) {
                        returnToMenu = true;
                        break;
                    } 
                    // Проверка, выбрана ли кнопка "Нет"
                    else if(showMessageEscape && selectedButtonnIndex == 1) {
                        showMessageEscape = false;
                    }
                }
            } 
            if (event.type == sf::Event::KeyPressed && !showMessageEscape && !showSaveLoadMenu) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left) {
                    // Переход к предыдущей карте с круговым циклом
                    if (selectedCarddIndex > 0) {
                        --selectedCarddIndex;
                    } else {
                        selectedCarddIndex = Playerrs[CurrentPlayerrIndex].hand.size() - 1;
                    }
                } if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right) {
                    // Переход к следующей карте с круговым циклом
                    if (selectedCarddIndex < Playerrs[CurrentPlayerrIndex].hand.size() - 1) {
                        ++selectedCarddIndex;
                    } else {
                        selectedCarddIndex = 0;
                    }
                } if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    if (attacking) {
                        // Если выбранной картой можно атаковать
                        if (attack(table, Playerrs[CurrentPlayerrIndex].hand[selectedCarddIndex])) {
                            attackCardds.push_back(Playerrs[CurrentPlayerrIndex].hand[selectedCarddIndex]);
                            table.push_back(Playerrs[CurrentPlayerrIndex].hand[selectedCarddIndex]);
                            attackCardd = Playerrs[CurrentPlayerrIndex].hand[selectedCarddIndex];
                            // Удаление выбранной карты из руки текущего игрока
                            Playerrs[CurrentPlayerrIndex].hand.erase(Playerrs[CurrentPlayerrIndex].hand.begin() + selectedCarddIndex); 
                            // Обновление индекса после удаления
                            if (selectedCarddIndex >= Playerrs[CurrentPlayerrIndex].hand.size() &&  !Playerrs[CurrentPlayerrIndex].hand.empty()) {
                                selectedCarddIndex = Playerrs[CurrentPlayerrIndex].hand.size() - 1;
                            }
                            moveTransitionInMove = true;
                            showCarddSelectionMessage = false;
                        } else {
                            // Если выбранной картой нельзя атаковать
                            sf::Clock clock; // Создание таймера
                            while(clock.getElapsedTime() < sf::seconds(1.5)) {
                                // Обработка событий
                                windowss.pollEvent(event);
                                sf::Vector2u windowSize = windowss.getSize();
                                float textX_ispair = windowSize.x / 2.f - messageInvalid.getLocalBounds().width / 2.f;
                                float textY_ispair = (800 + 150) / 2.f - messageInvalid.getLocalBounds().height / 2.f;
                                messageInvalid.setPosition(textX_ispair, textY_ispair);
                                // Отрисовка
                                windowss.draw(messageInvalid);
                                windowss.display();
                            }
                            showCarddSelectionMessage = false;
                        }
                    }  else if (defending) {
                        // Если выбранной картой можно защищаться
                        if (defense(attackCardd, Playerrs[CurrentPlayerrIndex].hand[selectedCarddIndex], trump)) {
                            defendingCardds.push_back(Playerrs[CurrentPlayerrIndex].hand[selectedCarddIndex]);
                            table.push_back(Playerrs[CurrentPlayerrIndex].hand[selectedCarddIndex]);
                            // Удаление карты из руки текущего игрока
                            Playerrs[CurrentPlayerrIndex].hand.erase(Playerrs[CurrentPlayerrIndex].hand.begin() + selectedCarddIndex);
                            // Обновление индекса после удаления
                            if (selectedCarddIndex >= Playerrs[CurrentPlayerrIndex].hand.size() &&  !Playerrs[CurrentPlayerrIndex].hand.empty()) {
                                selectedCarddIndex = Playerrs[CurrentPlayerrIndex].hand.size() - 1;
                            }
                            moveTransitionInMove = true;
                            showCarddSelectionMessage = false;
                        } else {
                            // Если выбранной картой нельзя защититься
                            sf::Clock clock; // Создание таймера
                            while(clock.getElapsedTime() < sf::seconds(1.5)) {
                                // Обработка событий
                                windowss.pollEvent(event);
                                sf::Vector2u windowSize = windowss.getSize();
                                float textX_ispair = windowSize.x / 2.f - messageInvalid.getLocalBounds().width / 2.f;
                                float textY_ispair = (800 + 150) / 2.f - messageInvalid.getLocalBounds().height / 2.f;
                                messageInvalid.setPosition(textX_ispair, textY_ispair);
                                // Отрисовка
                                windowss.draw(messageInvalid);
                                windowss.display();
                            }
                            showCarddSelectionMessage = false;
                        }
                    }
                }
            }
            // P - пас
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P && canTakeCardd && attacking) {
                table.clear();
                attackCardds.clear();
                defendingCardds.clear();
                showCarddSelectionMessage = false;
                moveTransition = true;
            }
            // T - взять карты
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::T && canTakeCardd && defending) {
                for (Cardd& Cardd : table) {
                    Playerrs[CurrentPlayerrIndex].hand.push_back(Cardd);
                }
                showCarddSelectionMessage = false;
                table.clear();
                attackCardds.clear();
                defendingCardds.clear();
                defending = false;
                attacking = true;
                moveTransition = true;
            }
            if(returnToMenu) {
                return 0; 
                break;
            }
            
            activePlayers = 0;
            People_win = 0;
            for (auto& player : Playerrs) {
                if (!player.hand.empty()) {
                    activePlayers++;
                }
                else
                {
                    player.hasWon = true; // Игрок выиграл
                    // if(dynamic_cast<HumanPlayer*>(player.get()) != nullptr){People_win++;}
                    People_win++;
                }
                if (Playerrs[CurrentPlayerrIndex].hasWon)
                {
                    do {
                        // Передача хода следующему игроку
                        CurrentPlayerrIndex = (CurrentPlayerrIndex + 1) % Playerrs.size();
                    } while (Playerrs[CurrentPlayerrIndex].hand.empty()); // Проверка, есть ли у игрока карты

                    // Обновление статуса игроков
                    for (int i = 0; i < Playerrs.size(); ++i) {
                        Playerrs[i].isAttacker = (i == CurrentPlayerrIndex);
                    }
                }
            }
            if (decks.empty() && (activePlayers == 1 || (number_of_players_people - People_win == 0)))
            {
                EndGame = true;
            }
            // Проверка на конец игры
            if (EndGame) {
                //окончание игры 
                if((number_of_players_people - People_win == 0))
                {
                    YouWin = true;
                }
                else if(activePlayers == 1)
                {
                    GameOver = true;
                }

                showCarddSelectionMessage = false;
                showMessageEscape = false;
            }
            // Переход хода следующему игроку
            if(moveTransition) {
                Playerrs[CurrentPlayerrIndex].isAttacker = false;
                Playerrs[CurrentPlayerrIndex].isDefender = false;
                middle = FindNextPlayerrWithCardds(Playerrs, CurrentPlayerrIndex);
                CurrentPlayerrIndex = middle;
                Playerrs[CurrentPlayerrIndex].isAttacker = true;
                Playerrs[CurrentPlayerrIndex].isDefender = false;
                neighborIndex = FindNextPlayerrWithCardds(Playerrs, CurrentPlayerrIndex);
                Playerrs[neighborIndex].isDefender = true;
                moveTransition = false;
                first = 0;
                while(first != 6)
                {
                    first++;
                    dealingCards (decks, Playerrs);
                }
                
                // Wait for "Enter" key press
                bool keyPressed = false;
                while (!keyPressed)
                {
                    message_who_move_transition.setString(L"Теперь атакует игрок: " + std::to_string(middle));
                    windowss.draw(Black_who);
                    windowss.pollEvent(event);
                    sf::Vector2u windowSize = windowss.getSize();
                    float textX_ispair = windowSize.x / 2.f - message_who.getLocalBounds().width / 2.f;
                    float textY_ispair = (800 + 150) / 2.f - message_who.getLocalBounds().height / 2.f;
                    message_who.setPosition(textX_ispair, textY_ispair);
                    windowss.draw(message_who);
                    message_who_move_transition.setPosition(textX_ispair - 50, textY_ispair + 100);
                    windowss.draw(message_who_move_transition);
                    windowss.display();
                    // Process all events
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
                    {
                        keyPressed = true; break;
                    }
                }
                // Обновление окна
                windowss.display();
                        
            }
            // Переход хода между атакующим и защищающимся
            if(moveTransitionInMove) {
                if (attacking) {
                    attacking = false;
                    defending = true;
                    Playerrs[CurrentPlayerrIndex].isAttacker = false;
                    middle = CurrentPlayerrIndex;
                    CurrentPlayerrIndex = neighborIndex;
                    neighborIndex = middle;
                    Playerrs[CurrentPlayerrIndex].isDefender = true;
                    moveTransitionInMove = false;
                } 
                else if (defending) {
                    attacking = true;
                    defending = false;
                    Playerrs[CurrentPlayerrIndex].isDefender = false;
                    neighborIndex = CurrentPlayerrIndex;
                    CurrentPlayerrIndex = middle;
                    Playerrs[CurrentPlayerrIndex].isAttacker = true;
                    moveTransitionInMove = false;
                }
                // Wait for "Enter" key press
                bool keyPressed = false;
                while (!keyPressed)
                {
                    if (attacking) {
                        message_who_move_transition.setString(L"Теперь атакует игрок: " + std::to_string(CurrentPlayerrIndex));

                    }
                    else if (defending) {
                        message_who_move_transition.setString(L"Теперь защищается игрок: " + std::to_string(CurrentPlayerrIndex));                    
                    }
                    windowss.draw(Black_who);
                    windowss.pollEvent(event);
                    sf::Vector2u windowSize = windowss.getSize();
                    float textX_ispair = windowSize.x / 2.f - message_who.getLocalBounds().width / 2.f;
                    float textY_ispair = (800 + 150) / 2.f - message_who.getLocalBounds().height / 2.f;
                    message_who.setPosition(textX_ispair, textY_ispair);
                    windowss.draw(message_who);
                    message_who_move_transition.setPosition(textX_ispair - 50, textY_ispair + 100);
                    windowss.draw(message_who_move_transition);
                    windowss.display();
                    // Process all events
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
                    {
                        keyPressed = true; break;
                    }
                }
                // Обновление окна
                windowss.display();
            }
        }
        windowss.clear(Color(50,200,150));
        windowss.draw(background);
        // Отрисовка карт игроков
        Texture back_Cardi_texture;
        if (!back_Cardi_texture.loadFromFile("resources/back.png")) { return 1; }
        RectangleShape back_Cardi_sprite(Vector2f(77,110));
        back_Cardi_sprite.setTexture(&back_Cardi_texture);
        int number_of_players_people = 4;
        if (number_of_players_people == 4) {drawFor4People(Playerrs, selectedCarddIndex, neighborIndex, windowss, messageWinner, back_Cardi_sprite,  attacking,  defending,  font_1);}
        // Отрисовка колоды карт
        Texture decksTexture;
        if (!decksTexture.loadFromFile("resources/back.png")) { 
            std::cerr << "Ошибка загрузки: resources/back.png" << std::endl;
            return -1; 
        }
        RectangleShape decksSprite(Vector2f(77,110));
        decksSprite.setTexture(&decksTexture);
        if (decks.size() > 1) {
            decksSprite.setPosition(10, 10);
            windowss.draw(decksSprite);
        }
        // Отрисовка козыря
        Texture trumpTexture;
        std::string trumpFilename = "resources/" + std::to_string(trump.value) + trump.suit + ".png";
        if (!trumpTexture.loadFromFile(trumpFilename)) {
            std::cerr << "Ошибка загрузки: " << trumpFilename << std::endl;
            return -1;
        }
        RectangleShape trumpSprite(Vector2f(77,110));
        trumpSprite.setTexture(&trumpTexture);
        if (!decks.empty()) {
            trumpSprite.setPosition(87, 10);
            windowss.draw(trumpSprite);
        }
        // Рисование сообщения о выборе карт, если флаг установлен 
        sf::Vector2u windowSize = windowss.getSize();
        if (showCarddSelectionMessage) {
            if (attacking) {
                messageReset.setString(L"Выберите карту для атаки");
            } else if (defending) {
                messageReset.setString(L"Выберите карту для защиты");
            }
            float textX = windowSize.x / 2.f - messageReset.getLocalBounds().width / 2.f;
            float textY = (800 + 150) / 2.f - messageReset.getLocalBounds().height / 2.f;
            messageReset.setPosition(textX, textY);
            windowss.draw(messageReset);
        }
        // Рисование сообщения о неверном выборе карты
        if (showInvalidMessage) {
            float textX = windowSize.x / 2.f - messageInvalid.getLocalBounds().width / 2.f;
            float textY = (800 + 150) / 2.f - messageInvalid.getLocalBounds().height / 2.f;
            messageInvalid.setPosition(textX, textY);
            windowss.draw(messageInvalid);
        }
        // Рисование сообщения о нажатие Escape, если флаг установлен 
        float textX_message_escape = windowSize.x / 2.f - message_escape.getLocalBounds().width / 2.f;
        float textY_message_escape = (800 + 150) / 2.f - message_escape.getLocalBounds().height / 2.f;
        message_escape.setPosition(textX_message_escape, textY_message_escape);
        if (showMessageEscape) {
            windowss.draw(message_escape);
            // Выделение выбранной кнопки
            float buttonWidth = ButtonnYes.getLocalBounds().width;
            float buttonHeight = ButtonnYes.getLocalBounds().height;

            float buttonX = windowSize.x / 2.f - buttonWidth / 2.f; 
            float buttonY = (800 + 150) / 2.f - buttonHeight / 2.f + 50;
            ButtonnYes.setPosition(buttonX, buttonY);

            ButtonnNo.setPosition(buttonX + buttonWidth + 50, buttonY);
            if (selectedButtonnIndex == 0) {
                ButtonnYes.setFillColor(sf::Color::Red);
                ButtonnNo.setFillColor(sf::Color::White);
            } else {
                ButtonnYes.setFillColor(sf::Color::White);
                ButtonnNo.setFillColor(sf::Color::Red);
            }
            // Рисование кнопок
            windowss.draw(ButtonnYes);
            windowss.draw(ButtonnNo);
        }
        // Отрисовка атакующих карт
        for (int cardind = 0; cardind < attackCardds.size(); ++cardind) {
            Cardd& cardd = attackCardds[cardind];// Атакующий игрок видит значения своих карт
            cardd.sprite.setTexture(cardd.texture);
            // Определение позиции карты в зависимости от её индекса 
            cardd.sprite.setPosition(Vector2f(720 + (77 + 70) * cardind, 455));
            cardd.sprite.setColor(Color::White);
            // Если карта выбрана, она рисуется в желтый цвет
            windowss.draw(cardd.sprite);

        }
        // Отрисовка отбивающихся карт
        for (int cardind = 0; cardind < defendingCardds.size(); ++cardind) {
            Cardd& cardd = defendingCardds[cardind];// Атакующий игрок видит значения своих карт
            cardd.sprite.setTexture(cardd.texture);
            // Определение позиции карты в зависимости от её индекса 
            cardd.sprite.setPosition(Vector2f(720 + (77 + 70) * cardind, 495));
            cardd.sprite.setColor(Color::White);
            // Если карта выбрана, она рисуется в желтый цвет
            windowss.draw(cardd.sprite);
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
            if (selectedButtonnIndex_2 == 0) {
                menuButtonn.setSelected(true);
                restartButtonn.setSelected(false);
            } else {
                menuButtonn.setSelected(false);
                restartButtonn.setSelected(true);
            }
            // Отрисовка кнопок
            menuButtonn.draw(windowss);
            restartButtonn.draw(windowss);
        }
        if (showSaveLoadMenu) {
            windowss.draw(saveText);
            windowss.draw(loadText);
            windowss.draw(buttonYes);
            windowss.draw(buttonNo);
        }
        windowss.display();
    }
    return 0;
}