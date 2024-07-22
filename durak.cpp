#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <set>
#include <random>
#include <fstream>
#include <sstream>
#include "durak.h"
using namespace sf;

/// @brief Функция для проверки на возможность атаки
/// @param table все карты, которые лежат на столе
/// @param attackCardd карта, которой хотим атаковать
/// @param defendingPlayerHandSize количество карт в руке защищающегося
/// @param attackCardds атакующие карты, которые лежат на столе
/// @return true - если можно атаковать, false - если нельзя
bool attack(std::vector<Cardd>& table, Cardd& attackCardd, int defendingPlayerHandSize, std::vector<Cardd>& attackCardds) {
    if (table.empty()) {
        return true; // Можно атаковать любой картой, если стол пустой
    } else if (!table.empty() && defendingPlayerHandSize >= 1 && attackCardds.size() < 6) {
        for (int i = 0; i < table.size(); i++) {
            if (attackCardd.value == table[i].value) {
                return true; // Можно атаковать картой с таким же значением, что и на столе
            }
        }
        return false; // Нельзя атаковать картой с другим значением, если стол не пустой
    }
}

/// @brief Функция для проверки на возможность защиты
/// @param attackCardd карта, которая атакует
/// @param defenseCardd карта, которой хотим защититься
/// @param trump козырная карта
/// @return true - если можно защититься, false - если нельзя
bool defense(Cardd& attackCardd, Cardd& defenseCardd, Cardd& trump) {
    if ((defenseCardd.value > attackCardd.value && defenseCardd.suit == attackCardd.suit) ||
        (defenseCardd.suit == trump.suit && attackCardd.suit != trump.suit)) {
        return true;
    }
    return false;
}

/// @brief Функция для атаки ботом
/// @param Bot бот
/// @param trump козырная карта
/// @param table все карты, которые лежат на столе
/// @param attackCardds атакующие карты, которые лежат на столе
/// @param attackCardd карта, которая атакует
/// @param handn количество карт у защищающегося
/// @return true - если бот может атаковать, false - если нет
bool attackBot(Playerr& Bot, Cardd trump, std::vector<Cardd>& table, std::vector<Cardd>& attackCardds, Cardd& attackCardd, int handn) {
    bool attackingBot = false;
    std::vector<Cardd> sortedHand = Bot.hand;
    std::sort(sortedHand.begin(), sortedHand.end());
    for (int i = 0; i < sortedHand.size(); i++) {
        if (attack(table, sortedHand[i], handn, attackCardds)) {
            attackCardds.push_back(sortedHand[i]);
            table.push_back(sortedHand[i]);
            attackCardd = sortedHand[i];
            // Удалите карту из Bot.hand
            Bot.hand.erase(std::remove(Bot.hand.begin(), Bot.hand.end(), sortedHand[i]), Bot.hand.end()); 
            if (i >= sortedHand.size() &&  !sortedHand.empty()) {
                i = sortedHand.size() - 1;
            }
            attackingBot = true;
            break;
        }
    }
    return attackingBot;
}

/// @brief Функция для защиты ботом
/// @param Bot бот
/// @param trump козырная карта
/// @param attackCardd карта, которая атакует
/// @param defendingCardds защищающиеся карты, которые лежат на столе
/// @param table все карты, которые лежат на столе
/// @return true - если бот может защититься, false - если нет
bool defenseBot(Playerr& Bot, Cardd trump, Cardd& attackCardd, std::vector<Cardd>& defendingCardds, std::vector<Cardd>& table) {
    bool defendingBot = false;
    std::vector<Cardd> sortedHand = Bot.hand;
    std::sort(sortedHand.begin(), sortedHand.end());
    for (int i = 0; i < sortedHand.size(); i++) {
        if (defense(attackCardd, sortedHand[i], trump)) {
            defendingCardds.push_back(sortedHand[i]);
            table.push_back(sortedHand[i]);
            Bot.hand.erase(std::remove(Bot.hand.begin(), Bot.hand.end(), sortedHand[i]), Bot.hand.end());
            if (i >= sortedHand.size() &&  !sortedHand.empty()) {
                i = sortedHand.size() - 1;
            }
            defendingBot = true;
            break;
        }
    }
    return defendingBot;
}

/// @brief Функция для сохранения игры
/// @param players вся информация об игроках
/// @param currentPlayerIndex индекс текущего игрока
/// @param neighborIndex индекс соседнего игрока
/// @param move номер раунда
/// @param attackCards атакующие карты, которые лежат на столе
/// @param defendingCards защищающиеся карты, которые лежат на столе
/// @param table все карты, которые лежат на столе
/// @param deck колода карт
/// @param trump козырная карта
/// @param attacking флаг о процессе атаки
/// @param defending флаг о процессе защиты
void savegame(std::vector<Playerr>& players, int& currentPlayerIndex, int& neighborIndex, int& move, const std::vector<Cardd>& attackCards, const std::vector<Cardd>& defendingCards, const std::vector<Cardd>& table, const std::vector<Cardd>& deck, Cardd& trump, bool& attacking, bool& defending) {
    std::ofstream saveFile("resources/saveDurakGame1.txt");
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
    saveFile <<attackCards.size() << "\n";
    saveFile <<defendingCards.size() << "\n";
    saveFile <<table.size() << "\n";
    saveFile <<deck.size() << " ";
    // Сохраняем атакующие карты
    for (int i = 0; i < attackCards.size(); ++i) {
        saveFile << attackCards[i].value << " " << attackCards[i].suit << " ";
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
    saveFile <<attacking << "\n";
    saveFile <<defending << "\n";
    //isAttacker и hasWon
    if(attacking) {
        saveFile <<players[currentPlayerIndex].isAttacker << "\n";
        saveFile <<players[neighborIndex].isDefender << "\n";
    } else if (defending) {
        saveFile <<players[neighborIndex].isAttacker << "\n";
        saveFile <<players[currentPlayerIndex].isDefender << "\n";
    }
    for (int i = 0; i < players.size(); ++i) {
        saveFile <<players[i].hasWon << "\n";
    }
}

/// @brief Функция загрузки сохраненной игры
/// @param players вся информация об игроках
/// @param currentPlayerIndex индекс текущего игрока
/// @param neighborIndex индекс соседнего игрока
/// @param move номер раунда
/// @param attackCards атакующие карты, которые лежат на столе
/// @param defendingCards защищающиеся карты, которые лежат на столе
/// @param table все карты, которые лежат на столе
/// @param deck колода карт
/// @param trump козырная карта
/// @param attacking флаг о процессе атаки
/// @param defending флаг о процессе защиты
void loadgame(std::vector<Playerr>& players, int& currentPlayerIndex, int& neighborIndex, int& move, std::vector<Cardd>& attackCards, std::vector<Cardd>& defendingCards, std::vector<Cardd>& table, std::vector<Cardd>& deck, Cardd& trump, bool& attacking, bool& defending) {
    std::ifstream loadFile("resources/saveDurakGame1.txt");
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
    int attackCards_size, defendingCards_size,table_size, deck_size;
    loadFile >> attackCards_size;
    attackCards.resize(attackCards_size);
    loadFile >>defendingCards_size;
    defendingCards.resize(defendingCards_size);
    loadFile >>table_size;
    table.resize(table_size);
    loadFile >>deck_size;
    deck.resize(deck_size);
    // Загружаем карты массива attackCards
    if (std::getline(loadFile, line)) { // Считываем по одной строке
        std::istringstream iss(line);
        int val;
        std::string suit;
        // Считываем данные только для первой строки, если attackCards.size() != 0
        for (int i = 0; i < attackCards.size(); ++i) { 
            iss >> val >> suit;
            attackCards[i].value = val;
            attackCards[i].suit = suit;
            if (suit != "") {
                std::string filename = "resources/" + std::to_string(val) + suit + ".png";
                if (!CardsTexture.loadFromFile(filename)) {
                    std::cerr << "Ошибка загрузки: " << filename << std::endl;
                } else  {
                    attackCards[i].texture = CardsTexture;
                    attackCards[i].sprite.setTexture(attackCards[i].texture);
                    attackCards[i].sprite.setScale(0.6f, 0.6f); 
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
                    defendingCards[i].sprite.setScale(0.6f, 0.6f); 

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
                } else {
                    table[i].texture = CardsTexture;
                    table[i].sprite.setTexture(table[i].texture);
                    table[i].sprite.setScale(0.6f, 0.6f); 
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
                    deck[i].sprite.setScale(0.6f, 0.6f);                    
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
    loadFile >> attacking;
    loadFile >> defending;
    if(attacking) {
        loadFile >>players[currentPlayerIndex].isAttacker;
        loadFile >>players[neighborIndex].isDefender;
        
    } else if (defending) {
        loadFile >>players[neighborIndex].isAttacker;
        loadFile >>players[currentPlayerIndex].isDefender;
    }
    for (int i = 0; i < players.size(); ++i) {
        loadFile >>players[i].hasWon;
    }
    loadFile.close();
}

/// @brief Функция для поиска текущего атакующего игрока
/// @param Playerrs вектор с игроками
/// @return индекс текущего атакующего игрока
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

/// @brief Функция для поиска текущего защищаегося игрока
/// @param Playerrs вектор с игроками
/// @return индекс текущего защищаегося игрока
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

/// @brief Функция для поиска следующего игрока с картами после текущего
/// @param Playerrs вектор с игроками
/// @param currentIndex индекс текущего игрока
/// @return индекс следующего игрока
int FindNextPlayerrWithCardds(const std::vector<Playerr>& Playerrs, int currentIndex) {
    int nextIndex = (currentIndex + 1) % Playerrs.size();
    while (Playerrs[nextIndex].hand.empty() && nextIndex != currentIndex) {
        // Пропускаем игроков без карт
        nextIndex = (nextIndex + 1) % Playerrs.size();
    }
    return nextIndex;
}

/// @brief Функция для определения, кто первый ходит
/// @param Playerrs вектор с игроками
/// @param trump козырная карта
/// @return индекс текущего игрока
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

/// @brief Функция для раздачи карт
/// @param decks колода карт
/// @param players вектор с игроками
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
int main_durakgame(sf::RenderWindow& windowss, int number_of_players_people, int PlayerChose_P) {
    int number_of_players = 4;
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
    sf::Texture tableTexture;
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
    sf::RectangleShape background(sf::Vector2f(1920,1080));
    background.setTexture(&tableTexture);
    background.setPosition(sf::Vector2f(0,0));
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
    if(number_of_players-number_of_players_people == 1){Playerrs[3].isBot = true;}
    else if(number_of_players-number_of_players_people == 2){Playerrs[3].isBot = true; Playerrs[2].isBot = true;}
    else if(number_of_players-number_of_players_people == 3){Playerrs[3].isBot = true; Playerrs[2].isBot = true;Playerrs[1].isBot = true;}
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
    sf::RectangleShape textBackground(sf::Vector2f(500, 200));
    textBackground.setFillColor(sf::Color(0, 0, 0, 150)); // Полупрозрачный черный фон
    textBackground.setPosition(750, 540);
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
    Buttonn menuButtonn(L"Вернуться в меню", sf::Vector2f(400, 50), 30, sf::Color::Transparent, sf::Color::White, font_1);
    Buttonn restartButtonn(L"Перезапустить игру", sf::Vector2f(400, 50), 30, sf::Color::Transparent, sf::Color::White, font_1);
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
    int first = 0;
    while(first != 6) {
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
    bool moveTransitionInMove_P = false;
    int chose = 0;
    bool bot_p = false;
    while (windowss.isOpen()) {
        // Обработка событий
        sf::Event event;
        while (windowss.pollEvent(event)) {
            // Escape - выход
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                showMessageEscape = true;
            }
             // S - сохранить игру
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S) {
                showSaveLoadMenu = true;
                saveText.setFillColor(sf::Color::Red);
                loadText.setFillColor(sf::Color::White);
                selectedSaveButtonIndex = 0; // Выбираем "Save Game" по умолчанию
            }
            // L - загрузить игру
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::L) {
                showSaveLoadMenu = true;
                loadText.setFillColor(sf::Color::Red);
                saveText.setFillColor(sf::Color::White);
                selectedSaveButtonIndex = 1; // Выбираем "Load Game" по умолчанию
            }
            // Обработка событий для меню сохранения/загрузки
            if (showSaveLoadMenu) {
                if (selectedSaveButtonIndex == 0) {
                    if (event.key.code == sf::Keyboard::Left) {
                        selectedsaveyesnobuttonindex = 0;
                    } else if (event.key.code == sf::Keyboard::Right) {
                        selectedsaveyesnobuttonindex = 1;
                    } else if (event.key.code == sf::Keyboard::Enter) {
                        if (selectedsaveyesnobuttonindex == 0) {
                            savegame(Playerrs, CurrentPlayerrIndex, neighborIndex, move, attackCardds, defendingCardds, table, decks, trump, attacking, defending);
                            showSaveLoadMenu = false;
                            break;
                        } else {
                            // Скрываем текстовое сообщение
                            showSaveLoadMenu = false;
                            // Сбрасываем цвет кнопок
                            buttonYes.setFillColor(sf::Color::White);
                            buttonNo.setFillColor(sf::Color::White);
                        }
                    }
                    buttonYes.setFillColor(selectedsaveyesnobuttonindex == 0 ? sf::Color::Red : sf::Color::White);
                    buttonNo.setFillColor(selectedsaveyesnobuttonindex == 1 ? sf::Color::Red : sf::Color::White);
                } else if (selectedSaveButtonIndex == 1) {
                    if (event.key.code == sf::Keyboard::Left) {
                        selectedsaveyesnobuttonindex = 0;
                    } else if (event.key.code == sf::Keyboard::Right) {
                        selectedsaveyesnobuttonindex = 1;
                    } else if (event.key.code == sf::Keyboard::Enter) {
                        if (selectedsaveyesnobuttonindex == 0) {
                            loadgame(Playerrs, CurrentPlayerrIndex, neighborIndex, move, attackCardds, defendingCardds, table, decks, trump, attacking, defending);
                            showSaveLoadMenu = false;
                            break;
                        } else {
                            // Скрываем текстовое сообщение
                            showSaveLoadMenu = false;
                            // Сбрасываем цвет кнопок
                            buttonYes.setFillColor(sf::Color::White);
                            buttonNo.setFillColor(sf::Color::White);
                        }
                    }
                    buttonYes.setFillColor(selectedsaveyesnobuttonindex == 0 ? sf::Color::Red : sf::Color::White);
                    buttonNo.setFillColor(selectedsaveyesnobuttonindex == 1 ? sf::Color::Red : sf::Color::White);
                }
            }
            // Конец игры
            if (event.type == sf::Event::KeyPressed && EndGame) {
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
                        main_durakgame(windowss, number_of_players_people, PlayerChose_P);  
                        return 0; 
                        break;
                    }
                }
            }
            if (!Playerrs[CurrentPlayerrIndex].isBot) {
                // Обработка кнопок "Да", "Нет"
                if (event.type == sf::Event::KeyPressed && showMessageEscape) {
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
                    } 
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right) {
                        // Переход к следующей карте с круговым циклом
                        if (selectedCarddIndex < Playerrs[CurrentPlayerrIndex].hand.size() - 1) {
                            ++selectedCarddIndex;
                        } else {
                            selectedCarddIndex = 0;
                        }
                    } 
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                        if (attacking) {
                            // Если выбранной картой можно атаковать
                            if (attack(table, Playerrs[CurrentPlayerrIndex].hand[selectedCarddIndex], Playerrs[neighborIndex].hand.size(), attackCardds)) {
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
                            }
                        } else if (defending) {
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
                            }
                        }
                    }
                }
                // P - пас
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P && canTakeCardd && attacking) {
                    PlayerChose_P++;
                    if (PlayerChose_P == number_of_players)
                    {
                        moveTransition = true;
                        table.clear();
                        attackCardds.clear();
                        defendingCardds.clear();
                    }
                    else{
                        Playerrs[CurrentPlayerrIndex].isAttacker = false;
                        moveTransitionInMove_P = true;
                        attacking = false;
                        defending = true;
                    }
                }
                // T - взять карты
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::T && canTakeCardd && defending) {
                    for (Cardd& Cardd : table) {
                        Playerrs[CurrentPlayerrIndex].hand.push_back(Cardd);
                    }
                    table.clear();
                    attackCardds.clear();
                    defendingCardds.clear();
                    defending = false;
                    attacking = true;
                    moveTransition = true;
                }
            } 
            // Переход хода между атакующим и защищающимся
            if(moveTransitionInMove_P) {
                int defender = neighborIndex;
                if (attacking) {
                    attacking = false;
                    defending = true;
                    Playerrs[CurrentPlayerrIndex].isAttacker = false;
                    Playerrs[defender].isDefender = true;
                    moveTransitionInMove_P = false;
                } 
                else if (defending) {
                    attacking = true;
                    defending = false;
                    CurrentPlayerrIndex = FindNextPlayerrWithCardds(Playerrs, defender + chose);
                    chose++;
                    Playerrs[defender].isDefender = false;
                    Playerrs[CurrentPlayerrIndex].isAttacker = true;
                    moveTransitionInMove_P = false;
                }
                if (Playerrs[CurrentPlayerrIndex].isBot)
                {
                    bot_p = true;
                }
                
                if (!Playerrs[CurrentPlayerrIndex].isBot && (number_of_players_people-People_win != 1) && !Playerrs[defender].isBot) {
                    bool keyPressed = false;
                    while (!keyPressed) {
                        if (attacking) {
                            message_who_move_transition.setString(L"Теперь атакует игрок: " + std::to_string(CurrentPlayerrIndex));
                        } else if (defending) {
                            message_who_move_transition.setString(L"Теперь защищается игрок: " + std::to_string(defender));                    
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
                        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                            keyPressed = true; break;
                        }
                    }
                    // Обновление окна
                    windowss.display();
                }
            }
            if (Playerrs[CurrentPlayerrIndex].isBot && !bot_p) {
                
                if (attacking) {
                    if (attackBot(Playerrs[CurrentPlayerrIndex], trump, table, attackCardds, attackCardd, Playerrs[neighborIndex].hand.size())) {
                        moveTransitionInMove = true;
                    } else {
                        table.clear();
                        attackCardds.clear();
                        defendingCardds.clear();
                        moveTransition = true;
                    }
                } else if (defending) {
                    if (defenseBot(Playerrs[CurrentPlayerrIndex], trump, attackCardd, defendingCardds, table)) {
                        moveTransitionInMove = true;
                    } else {
                        for (Cardd& Cardd : table) {
                            Playerrs[CurrentPlayerrIndex].hand.push_back(Cardd);
                        }
                        table.clear();
                        attackCardds.clear();
                        defendingCardds.clear();
                        defending = false;
                        attacking = true;
                        moveTransition = true;
                    }
                }
            }
            else if (Playerrs[CurrentPlayerrIndex].isBot && bot_p) {
                if (attacking) {
                    if (attackBot(Playerrs[CurrentPlayerrIndex], trump, table, attackCardds, attackCardd, Playerrs[neighborIndex].hand.size())) {
                        moveTransitionInMove = true;
                    } 
                    else {
                        attacking = false;
                        defending = true;
                        PlayerChose_P++;
                        moveTransitionInMove_P = true;
                    }
                } 
                bot_p = false;
                // else if (defending) {
                //     if (defenseBot(Playerrs[CurrentPlayerrIndex], trump, attackCardd, defendingCardds, table)) {
                //         moveTransitionInMove_P = true;
                //     } else {
                //         for (Cardd& Cardd : table) {
                //             Playerrs[CurrentPlayerrIndex].hand.push_back(Cardd);
                //         }
                //         table.clear();
                //         attackCardds.clear();
                //         defendingCardds.clear();
                //         defending = false;
                //         attacking = true;
                //         moveTransition = true;
                //     }
                // }
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
                } else {
                    player.hasWon = true; // Игрок выиграл
                    if(!player.isBot){People_win++;}
                }
                if (Playerrs[CurrentPlayerrIndex].hasWon) {
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
            if (decks.empty() && (activePlayers == 1 || (number_of_players_people - People_win == 0))) {
                EndGame = true;
            }
            // Проверка на конец игры
            if (EndGame) {
                //окончание игры 
                if((number_of_players_people - People_win == 0)) {
                    YouWin = true;
                } else if(activePlayers == 1) {
                    GameOver = true;
                }
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
                while(first != 6) {
                    first++;
                    dealingCards (decks, Playerrs);
                }
                // Wait for "Enter" key press
                if (Playerrs[middle].isBot && (number_of_players_people-People_win != 1))
                {
                    bool keyPressed = false;
                    while (!keyPressed) {
                        message_who_move_transition.setString(L"Теперь защищается игрок: " + std::to_string(neighborIndex));
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
                        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                            keyPressed = true; break;
                        }
                        // Обновление окна
                        
                    }
                    windowss.display();
                
                }
                if (!Playerrs[middle].isBot && (number_of_players_people-People_win != 1)) {
                    bool keyPressed = false;
                    while (!keyPressed) {
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
                        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                            keyPressed = true; break;
                        }
                        // Обновление окна
                        
                    }
                    windowss.display();
                }
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
                if (!Playerrs[CurrentPlayerrIndex].isBot && (number_of_players_people-People_win != 1) && !Playerrs[neighborIndex].isBot) {
                    bool keyPressed = false;
                    while (!keyPressed) {
                        if (attacking) {
                            message_who_move_transition.setString(L"Теперь атакует игрок: " + std::to_string(CurrentPlayerrIndex));
                        } else if (defending) {
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
                        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                            keyPressed = true; break;
                        }
                    }
                    // Обновление окна
                    windowss.display();
                }
            }
        }
        windowss.clear(sf::Color(50,200,150));
        windowss.draw(background);
        // Отрисовка карт игроков
        sf::Texture back_Cardi_texture;
        if (!back_Cardi_texture.loadFromFile("resources/back.png")) { return 1; }
        sf::RectangleShape back_Cardi_sprite(sf::Vector2f(77,110));
        back_Cardi_sprite.setTexture(&back_Cardi_texture);
        if (number_of_players_people == 4) {drawFor4People(Playerrs,CurrentPlayerrIndex, selectedCarddIndex, neighborIndex, windowss, messageWinner, back_Cardi_sprite,  attacking,  defending,  font_1);}
        else if (number_of_players_people == 3) {drawFor3People(Playerrs, CurrentPlayerrIndex, selectedCarddIndex, neighborIndex, windowss, messageWinner, back_Cardi_sprite, attacking, defending, font_1);}
        else if (number_of_players_people == 2) {drawFor2People(Playerrs, CurrentPlayerrIndex, selectedCarddIndex, neighborIndex, windowss, messageWinner, back_Cardi_sprite,  attacking,  defending,  font_1);}
        else if (number_of_players_people == 1) {drawFor1People(Playerrs,CurrentPlayerrIndex, selectedCarddIndex, neighborIndex, windowss, messageWinner, back_Cardi_sprite, attacking, defending, font_1);}
       
        // Отрисовка колоды карт
        sf::Texture decksTexture;
        if (!decksTexture.loadFromFile("resources/back.png")) { 
            std::cerr << "Ошибка загрузки: resources/back.png" << std::endl;
            return -1; 
        }
        sf::RectangleShape decksSprite(sf::Vector2f(77,110));
        decksSprite.setTexture(&decksTexture);
        if (decks.size() > 1) {
            decksSprite.setPosition(10, 10);
            windowss.draw(decksSprite);
        }
        // Отрисовка козыря
        sf::Texture trumpTexture;
        std::string trumpFilename = "resources/" + std::to_string(trump.value) + trump.suit + ".png";
        if (!trumpTexture.loadFromFile(trumpFilename)) {
            std::cerr << "Ошибка загрузки: " << trumpFilename << std::endl;
            return -1;
        }
        sf::RectangleShape trumpSprite(sf::Vector2f(77,110));
        trumpSprite.setTexture(&trumpTexture);
        if (!decks.empty()) {
            trumpSprite.setPosition(87, 10);
            windowss.draw(trumpSprite);
        }
        sf::Vector2u windowSize = windowss.getSize();
        // Рисование сообщения о неверном выборе карты
        if (showInvalidMessage) {
            float textX = windowSize.x / 2.f - messageInvalid.getLocalBounds().width / 2.f;
            float textY = (800 + 150) / 2.f - messageInvalid.getLocalBounds().height / 2.f;
            messageInvalid.setPosition(textX, textY);
            windowss.draw(messageInvalid);
        }
        // Отрисовка атакующих карт
        for (int cardind = 0; cardind < attackCardds.size(); ++cardind) {
            Cardd& cardd = attackCardds[cardind];// Атакующий игрок видит значения своих карт
            cardd.sprite.setTexture(cardd.texture);
            // Определение позиции карты в зависимости от её индекса 
            cardd.sprite.setPosition(sf::Vector2f(720 + (77 + 70) * cardind, 455));
            cardd.sprite.setColor(sf::Color::White);
            // Если карта выбрана, она рисуется в желтый цвет
            windowss.draw(cardd.sprite);

        }
        // Отрисовка отбивающихся карт
        for (int cardind = 0; cardind < defendingCardds.size(); ++cardind) {
            Cardd& cardd = defendingCardds[cardind];// Атакующий игрок видит значения своих карт
            cardd.sprite.setTexture(cardd.texture);
            // Определение позиции карты в зависимости от её индекса 
            cardd.sprite.setPosition(sf::Vector2f(720 + (77 + 70) * cardind, 495));
            cardd.sprite.setColor(sf::Color::White);
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
        // Рисование сообщения о нажатие Escape, если флаг установлен 
        float textX_message_escape = windowSize.x / 2.f - message_escape.getLocalBounds().width / 2.f;
        float textY_message_escape = (800 + 150) / 2.f - message_escape.getLocalBounds().height / 2.f;
        message_escape.setPosition(textX_message_escape + 10, 40 + textY_message_escape);
        if (showMessageEscape) {
            textBackground.setPosition(textX_message_escape, textY_message_escape);
            windowss.draw(textBackground);
            windowss.draw(message_escape);
            // Выделение выбранной кнопки
            float buttonWidth = ButtonnYes.getLocalBounds().width;
            float buttonHeight = ButtonnYes.getLocalBounds().height;
            float buttonX = windowSize.x / 2.f - buttonWidth / 2.f; 
            float buttonY = (800 + 150) / 2.f - buttonHeight / 2.f + 50;
            ButtonnYes.setPosition(buttonX - 50, 40 + buttonY);
            ButtonnNo.setPosition(buttonX + buttonWidth + 50 - 50, 40 + buttonY);
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
        windowss.display();
    }
    return 0;
}