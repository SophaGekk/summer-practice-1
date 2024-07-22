#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp> 
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <set>
#include <random>
#include <fstream> // For file operations
#include <sstream> // For string manipulation
#include "pikdam.h"
#include <memory>
/// @brief функция для сравнения двух карт и исключения дамы пики
/// @param Cardi1 первая карта
/// @param Cardi2 вторая карта
/// @return true or false
bool pair_of_Cardis(const Cardi& Cardi1, const Cardi& Cardi2) {
    if((Cardi1.suit == "SPADES" && Cardi1.value == 12) || (Cardi2.suit == "SPADES" && Cardi2.value == 12))
    {
        return false;
    
    }
    return Cardi1.value == Cardi2.value;
}
/// @brief Функция для поиска текущего игрока, чей ход необходимо выполнить
/// @param players вектор игроков
/// @return индекс текущего атакующего игрока
int findCurrentPlayer(const  std::vector<std::unique_ptr<Player_>>& players) {
    for (int i = 0; i < players.size(); ++i) {
        if (!players[i]->hand.empty() && !players[i]->hasWon) {
            if (players[i]->isAttacker)
            {
                return i; // Возвращает индекс текущего атакующего игрока
            }
        }
    }
    return -1; // Все игроки выиграли
}
/// @brief Функция для поиска текущего игрока, чей ход необходимо выполнить
/// @param players вектор игроков
/// @param currentIndex индекс текущего атакующего игрока
/// @return индекс соседа атакующего игрока
int findNextPlayerWithCardis(const std::vector<std::unique_ptr<Player_>>& players, int currentIndex) {
    int nextIndex = (currentIndex + 1) % players.size();
    while (players[nextIndex]->hand.empty() && nextIndex != currentIndex) {
        // Пропускаем игроков без карт
        nextIndex = (nextIndex + 1) % players.size();
    }
    return nextIndex;
}
/// @brief Функция для поиска текущего игрока человека, чей ход необходимо выполнить
/// @param players вектор игроков
/// @param currentIndex индекс текущего атакующего игрока
/// @param number_of_players_people количество игроков - людей
/// @return индекс соседа атакующего игрока
int who_move_transition(const std::vector<std::unique_ptr<Player_>>& players, int& currentIndex, int& number_of_players_people){
    int nextIndex = (currentIndex + 1) % number_of_players_people;
    while (players[nextIndex]->hand.empty() && nextIndex != currentIndex) {
        // Пропускаем игроков без карт
        nextIndex = (nextIndex + 1) % number_of_players_people;
    }
    return nextIndex;
}

/// @brief Функция сохарнения игры
/// @param players вектор игроков
/// @param number_of_players_people количество игроков - людей
/// @param currentIndex индекс текущего атакующего игрока
/// @param playersChoseNo количество игрков скинувших карты(выбравших нет)
/// @param move_transition переход хода
/// @param crdpair Флаг, для скидывания карт
/// @param CardiTaken Флаг, указывающий на то, что карта была взята
/// @param canTakeCardi Можно ли брать карту у противника
/// @param showCardiSelectionMessage сообщение о выборе карт
void savegame(const std::vector<std::unique_ptr<Player_>>& players, int& number_of_players_people, int& currentIndex, int& playersChoseNo, bool& move_transition, bool& crdpair, bool& CardiTaken, bool& canTakeCardi, bool& showCardiSelectionMessage) {
    std::ofstream saveFile("resources/pikgame_save.txt");
    if (!saveFile.is_open()) {
        std::cerr << "Error opening file for saving!" << std::endl;
        return;
    }

    // Сохраняем карты каждого игрока
    for (const auto& player : players) {
        for (const auto& card : player->hand) {
            saveFile << card.value << " " << card.suit << " ";
        }
        saveFile << "\n";
    }
    //>isAttacker и hasWon
    for (int i = 0; i < players.size(); ++i) {
        players[i]->isAttacker = (i == currentIndex);
        saveFile <<players[i]->isAttacker << "\n";
        saveFile <<players[i]->hasWon << "\n";
    }
    // Сохраняем текущего игрока и количество игроков людей
    saveFile << currentIndex << "\n";
    saveFile << number_of_players_people << "\n";
    //количество игроков скинувших карты
    saveFile << playersChoseNo << "\n";
    //сохрянем булевые переменные для отслеживания состояния игры
    saveFile << showCardiSelectionMessage << "\n";
    saveFile << canTakeCardi << "\n";
    saveFile << CardiTaken << "\n";
    saveFile << crdpair << "\n";
    saveFile << move_transition << "\n";

    saveFile.close();
}
/// @brief Функция загрузки сохранения игры
/// @param players вектор игроков
/// @param number_of_players_people количество игроков - людей
/// @param currentIndex индекс текущего атакующего игрока
/// @param playersChoseNo количество игрков скинувших карты(выбравших нет)
/// @param move_transition переход хода
/// @param crdpair Флаг, для скидывания карт
/// @param CardiTaken Флаг, указывающий на то, что карта была взята
/// @param canTakeCardi Можно ли брать карту у противника
/// @param showCardiSelectionMessage сообщение о выборе карт
void loadgame(const std::vector<std::unique_ptr<Player_>>& players, int& number_of_players_people, int& currentIndex, int& playersChoseNo, bool& move_transition, bool& crdpair, bool& CardiTaken, bool& canTakeCardi, bool& showCardiSelectionMessage) {
    std::ifstream loadFile("resources/pikgame_save.txt");
    if (!loadFile.is_open()) {
        std::cerr << "Error opening file for loading!" << std::endl;
        return;
    }
    std::vector<std::string> suits = {"HEARTS", "DIAMONDS", "SPADES", "CLUBS"};

    // Загружаем карты каждого игрока
    for (auto& player : players) {
        player->hand.clear();
        std::string line;
        sf::Texture CardsTexture;
        // Загружаем карты каждого игрока
        if (std::getline(loadFile, line))
        {
            std::istringstream iss(line);
            int value;
            std::string suit;
            while (iss >> value >> suit) {
                Cardi card;
                card.value = value;
                card.suit = suit;
                // Загрузка текстуры и установка спрайта
                std::string filename = "resources/" + std::to_string(value) + suit + ".png";
                if (!CardsTexture.loadFromFile(filename)) {
                    std::cerr << "Ошибка загрузки текстуры: " << filename << std::endl;
                } else {
                    card.texture = CardsTexture;
                    card.sprite.setTexture(card.texture);
                    card.sprite.setScale(0.6f, 0.6f); // Уменьшение размера спрайта 
                }
                player->hand.push_back(card);
            
            }
        }
    }
    for (int i = 0; i < players.size(); ++i) {
        players[i]->isAttacker = (i == currentIndex);
        loadFile >>players[i]->isAttacker;
        loadFile >>players[i]->hasWon;
    }
    // Загружаем текущего игрока
    loadFile >> currentIndex;

    // Загружаем количество игроков людей
    loadFile >> number_of_players_people;

    //количество игроков скинувших карты
    loadFile >> playersChoseNo;

    //Загружаем булевые переменные для отслеживания состояния игры
    loadFile >> showCardiSelectionMessage;
    loadFile >> canTakeCardi;
    loadFile >> CardiTaken;
    loadFile >> crdpair;
    loadFile >> move_transition;

    loadFile.close();
}
/// @brief Основная функция игры - здесь храниться логика
/// @param windowss игровое окно
/// @param number_of_players_people количество игроков - людей
int main_pikgame(sf::RenderWindow& windowss, int& number_of_players_people) {
    int currentPlayerIndex = 0;
    int number_of_players = 4;
    int People_win = 0;
    srand(time(0));
    bool PlayerHuman = false;
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
    std::vector<Cardi> deck;
    sf::Texture CardiTexture;
    
    std::vector<std::unique_ptr<Player_>> Player;

    // Создание игроков-людей и ботов
    for (int i = 0; i < number_of_players_people; ++i) {
        Player.push_back(std::make_unique<HumanPlayer>());
    }
    for (int i = 0; i < number_of_players - number_of_players_people; ++i) {
        Player.push_back(std::make_unique<Bot>());
    }
    for (int value = 6; value <= 14; ++value) {
        for (const std::string& suit : suits) {
            // Исключаем даму крести
            if (value == 12 && suit == "CLUBS") {
                continue; // Пропускаем добавление карты "Дама Крести"
            }
            Cardi Cardi;
            Cardi.value = value;
            Cardi.suit = suit;
            std::string filename = "resources/" + std::to_string(value) + suit + ".png";
            if (!CardiTexture.loadFromFile(filename)) { 
                std::cerr << "Ошибка загрузки текстуры: " << filename << std::endl; 
            } else {
                Cardi.texture = CardiTexture; 
                Cardi.sprite.setTexture(Cardi.texture);
                Cardi.sprite.setScale(0.6f, 0.6f); // Уменьшение размера спрайта 
                deck.push_back(Cardi);
            }
        }
    }

    // Перемешивание колоды
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);

    Player[0]->isAttacker = true;
    Player[0]->isDefender = false;
    for (int i = 1; i < Player.size(); ++i) {
        Player[i]->isAttacker = false;
        Player[i]->isDefender = false;
    }
    //раздача карт на руки
    for (int i = 0; i < 9; ++i)
    {
        for (auto& player : Player) {
            if (deck.empty()) {
                std::cerr << "Колода пуста. Невозможно продолжить раздачу карт." << std::endl;
                break; // Выход из цикла, если колода пуста
            }
            player->hand.push_back(deck.back());
            deck.pop_back();
        }
    }
    for (auto& player : Player) {
        if (auto bot = dynamic_cast<Bot*>(player.get())) {
            bot->disCardiPairs();
        }
    }
    
    int selectedCardiIndex = 0;
    int selectedCardiIndexRight = 0;
    
    // Звуковые эффекты
    SoundBuffer buffer;
    SoundBuffer buf_return;

    if (!buffer.loadFromFile("resources/audiomenu2.wav")) return 22;
    if (!buf_return.loadFromFile("resources/audiomenu5.wav")) return 22;
    Sound sound, sound_return;
    sound.setBuffer(buffer);
    sound_return.setBuffer(buf_return);
    
    std::vector<int> selected_Cardi(0);
    sf::Font font_1;
    if (!font_1.loadFromFile("resources/arial.ttf"))
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
    messege_winplayer.setString(L"Игрок выиграл");

    sf::Text message_ispair;
    message_ispair.setFont(font_1);
    message_ispair.setCharacterSize(24);
    message_ispair.setFillColor(sf::Color::White);
    message_ispair.setPosition(100.f, 100.f);

    // Текст вопроса
    message_ispair.setString(L"Увы, эти карты сбросить не получиться...");
    // message_ispair.setString(L"Oh, you will not be able to disCardi these Cardis...");

    sf::Text message;
    message.setFont(font_1);
    message.setCharacterSize(30);
    message.setFillColor(sf::Color::White);
    message.setPosition(100.f, 100.f);

    // Текст вопроса
    message.setString(L"Есть ли что сбросить?");
    //  message.setString("Is there anything to reset?");
    // Создание кнопок "Да" и "Нет"
    sf::Text buttonYes;
    buttonYes.setFont(font_1);
    buttonYes.setString(L"Да");
    // buttonYes.setString("Yes");
    buttonYes.setCharacterSize(24);
    buttonYes.setFillColor(sf::Color::White);
    buttonYes.setPosition(200.f, 200.f);

    sf::Text buttonNo;
    buttonNo.setFont(font_1);
    buttonNo.setString(L"Нет");
    // buttonNo.setString("No");
    buttonNo.setCharacterSize(24);
    buttonNo.setFillColor(sf::Color::White);
    buttonNo.setPosition(400.f, 200.f);

    // Флаг для отображения текста выбора карт
    bool showCardiSelectionMessage = true;
    // Индекс выбранной кнопки (0 - Да, 1 - Нет)
    int selectedButtonIndex = 0;
    sf::Text message_escape;
    message_escape.setFont(font_1);
    message_escape.setCharacterSize(24);
    message_escape.setFillColor(sf::Color::White);
    message_escape.setPosition(100.f, 100.f);

    // Текст вопроса
    message_escape.setString(L"Вы действительно хотите выйти?");
    // message_escape.setString(L"Do you really want to get out?");

    // Флаг для отображения текста выхода из игры
    bool showMessageEscape = false;
    // Флаг для отображения текста выхода в меню
    bool returnToMenu = false;

    // Переменная для отслеживания количества игроков, выбравших "нет"
    int playersChoseNo = number_of_players - number_of_players_people; 

    bool Taking_Cardis = false; // Флаг для проверки, взял ли игрок карту
    bool canSelectCardis = true; // Можно ли выбирать карты
    bool canTakeCardi = false; // Можно ли брать карту у противника
    bool CardiTaken = false; // Флаг, указывающий на то, что карта была взята

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
    if (!youWinTexture.loadFromFile("resources/YouWin.png")) {
        std::cerr << "Ошибка загрузки YouWin!" << std::endl;
    }
    if (!gameOverTexture.loadFromFile("resources/YouLose.png")) {
        std::cerr << "Ошибка загрузки YouLose!" << std::endl;
    }
    sf::Sprite endGameSprite;
    // Создаем кнопки 
    Button menuButton(L"Вернуться в меню", sf::Vector2f(400, 50), 30, sf::Color::Transparent, sf::Color::White, font_1);
    Button restartButton(L"Перезапустить игру", sf::Vector2f(400, 50), 30, sf::Color::Transparent, sf::Color::White, font_1);
    // Установка позиции кнопок
    menuButton.setPosition(sf::Vector2f(770, 640));
    restartButton.setPosition(sf::Vector2f(770, 740));
    int selectedButtonIndex_2 = 0; // Индекс выбранной кнопки
    sf::Clock gameTimer; // Создание таймера

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

    bool showSaveLoadMenu = false;
    int selectedSaveButtonIndex = 0;
    // Флаг для отображения меню
    int selectedsaveyesnobuttonindex = 0;
    // сделаем сохранения
    // Текст для элементов меню
    sf::Text saveText("Save Game", font_1, 24);
    saveText.setFillColor(sf::Color::White);
    saveText.setPosition(800.f, 450.f);

    sf::Text loadText("Load Game", font_1, 24);
    loadText.setFillColor(sf::Color::White);
    loadText.setPosition(1020.f, 450.f);
    
    bool showMessage = false;
    // Game Loop
    while (windowss.isOpen()) 
    {
        currentPlayerIndex = findCurrentPlayer(Player);
        neighborIndex = findNextPlayerWithCardis(Player, currentPlayerIndex); // Индекс соседа справа
        sf::Event event;
        while (windowss.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed && event.key.code == Keyboard::Escape)
            {
                showMessageEscape = true;
                showCardiSelectionMessage = false;
                sound_return.play();
            }

            // Обработка нажатия на кнопку "Save Game"
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::S) {
                showSaveLoadMenu = true;
                saveText.setFillColor(sf::Color::Red);
                loadText.setFillColor(sf::Color::White);
                selectedSaveButtonIndex = 0; // Выбираем "Save Game" по умолчанию
                showMessageEscape = false;
                if (showCardiSelectionMessage)
                {
                    showCardiSelectionMessage = false;
                    showMessage = true;
                }
                sound_return.play();
            }
            // Обработка нажатия на кнопку "Load Game"
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::L) {
                showSaveLoadMenu = true;
                loadText.setFillColor(sf::Color::Red);
                saveText.setFillColor(sf::Color::White);
                selectedSaveButtonIndex = 1; // Выбираем "Load Game" по умолчанию
                showMessageEscape = false;
                if (showCardiSelectionMessage)
                {
                    showCardiSelectionMessage = false;
                    showMessage = true;
                }
                sound_return.play();
            }

            // Обработка событий для меню сохранения/загрузки
            if (showSaveLoadMenu) {
                if (selectedSaveButtonIndex == 0)
                {
                    if (event.key.code == Keyboard::Left) {
                        // selectedsaveyesnobuttonindex = (selectedsaveyesnobuttonindex - 1 + 2) % 2;
                        selectedsaveyesnobuttonindex = 0;
                            
                    } else if (event.key.code == Keyboard::Right) {
                        // selectedsaveyesnobuttonindex = (selectedsaveyesnobuttonindex + 1) % 2;
                        selectedsaveyesnobuttonindex = 1;
                        
                    } else if (event.key.code == Keyboard::Enter) {
                        if (showMessage)
                        {
                            showCardiSelectionMessage = true;
                            showMessage = false;
                        }
                        if (selectedsaveyesnobuttonindex == 0) {
                            savegame(Player, number_of_players_people, currentPlayerIndex, playersChoseNo, move_transition, crdpair, CardiTaken, canTakeCardi, showCardiSelectionMessage);
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
                else if (selectedSaveButtonIndex == 1)
                {
                    if (event.key.code == Keyboard::Left) {
                            selectedsaveyesnobuttonindex = 0;
                            
                        } else if (event.key.code == Keyboard::Right) {
                            selectedsaveyesnobuttonindex = 1;
                            
                        } else if (event.key.code == Keyboard::Enter) {
                            if (showMessage)
                            {
                                showCardiSelectionMessage = true;
                                showMessage = false;
                            }
                            if (selectedsaveyesnobuttonindex == 0) {
                                loadgame(Player, number_of_players_people, currentPlayerIndex, playersChoseNo, move_transition, crdpair, CardiTaken, canTakeCardi, showCardiSelectionMessage);
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
                        returnToMenu = true;
                    }
                    else if(selectedButtonIndex_2 == 1)// Проверка, выбрана ли кнопка "Restart"
                    {
                        EndGame = false;
                        GameOver = false;
                        YouWin = false;
                        // Перезапуск основного игрового цикла
                        main_pikgame(windowss, number_of_players_people);  return 0; break;
                    }
                }
                sound_return.play();
            }
            if(playersChoseNo != number_of_players)
            {
                if (event.type == sf::Event::KeyPressed && (showCardiSelectionMessage || showMessageEscape))
                {// Проверка нажатия клавиш для переключения между кнопками "Да" и "Нет"
                    if (event.key.code == sf::Keyboard::Left)
                    {
                        // Переключение на предыдущую кнопку
                        selectedButtonIndex = (selectedButtonIndex - 1 + 2) % 2;
                        sound.play();
                    }
                    else if (event.key.code == sf::Keyboard::Right)
                    {
                        // Переключение на следующую кнопку
                        selectedButtonIndex = (selectedButtonIndex + 1) % 2;
                        sound.play();
                    }
                    else if (event.key.code == sf::Keyboard::Enter)
                    {
                        if (showCardiSelectionMessage && selectedButtonIndex == 0) // Проверка, выбрана ли кнопка "Да"
                        {
                            showCardiSelectionMessage = false; // Сброс флага после выбора кнопки "Да"
                            selected_Cardi.clear(); // Очистка выбора карт
                        }
                        else if(showMessageEscape && selectedButtonIndex == 0)
                        {
                            returnToMenu = true;
                            break;
                        }
                        else if(showMessageEscape && selectedButtonIndex == 1)
                        {
                            showMessageEscape = false;
                            showCardiSelectionMessage = true;
                            selected_Cardi.clear(); // Очистка выбора карт

                        }
                        else if (showCardiSelectionMessage && selectedButtonIndex == 1) // Проверка, выбрана ли кнопка "Нет"
                        {
                            showCardiSelectionMessage = false; // Сброс флага после выбора кнопки "Нет"
                            selected_Cardi.clear(); // Очистка выбора карт
                            move_transition = true;
                            playersChoseNo++; //так надо
                            canSelectCardis = false; // Блокируем выбор карт
                            showCardiSelectionMessage = false;
                        }
                    }
                }              
                else if (currentPlayerIndex != -1 && dynamic_cast<HumanPlayer*>(Player[currentPlayerIndex].get()) != nullptr)
                { 
                    switch (event.type)
                    {
                    case sf::Event::KeyPressed:
                        if (event.key.code == sf::Keyboard::Left )
                        {
                            // Переход к предыдущей карте с круговым циклом
                            if (selectedCardiIndex > 0)
                            {
                                --selectedCardiIndex;
                            }
                            else
                            {
                                selectedCardiIndex = Player[currentPlayerIndex]->hand.size() - 1;
                            }
                            sound.play();
                        }
                        else if (event.key.code == sf::Keyboard::Right)
                        {
                            // Переход к следующей карте с круговым циклом
                            if (selectedCardiIndex < Player[currentPlayerIndex]->hand.size() - 1)
                            {
                                ++selectedCardiIndex;
                            }
                            else
                            {
                                selectedCardiIndex = 0;
                            }
                            sound.play();
                        }
                        else if (event.key.code == sf::Keyboard::Enter)
                        {
                            if (!showCardiSelectionMessage && !showMessageEscape)
                            {// Проверяем, выбрана ли уже карта
                                auto it = std::find(selected_Cardi.begin(), selected_Cardi.end(), selectedCardiIndex);

                                // Проверяем, выбрана ли уже карта
                                if (it == selected_Cardi.end())
                                {
                                    // Если не выбрана, добавляем в вектор и меняем её цвет на красный
                                    selected_Cardi.push_back(selectedCardiIndex);
                                    
                                    // Проверка выбрано ли две карты
                                    if (selected_Cardi.size() == 2)
                                    {
                                        // Вызов функции для проверки на пару карт
                                        bool is_pair = pair_of_Cardis(Player[currentPlayerIndex]->hand[selected_Cardi[0]], Player[currentPlayerIndex]->hand[selected_Cardi[1]]);
                                        if (currentPlayerIndex != -1)
                                        {
                                            // Вывод результата
                                            if (is_pair)
                                            {// Действия, если выбранные карты являются парой
                                                // Сортировка индексов выбранных карт по убыванию
                                                std::sort(selected_Cardi.rbegin(), selected_Cardi.rend());
                                                // Удаление парных карт из руки текущего игрока
                                                for (int index : selected_Cardi)
                                                {
                                                    Player[currentPlayerIndex]->hand.erase(Player[currentPlayerIndex]->hand.begin() + index);
                                                }
                                                // Возврат к выбору кнопок
                                                showCardiSelectionMessage = true;
                                                selectedCardiIndex = 0;
                                            }
                                            else
                                            {// Действия, если выбранные карты не являются парой
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
                                                showCardiSelectionMessage = true;
                                            }
                                            // Очистка списка выбранных карт
                                            selected_Cardi.clear();
                                        }
                                    }
                                }
                                else
                                { // Если уже выбрана, удаляем из вектора и меняем её цвет на обычный
                                    selected_Cardi.erase(it);
                                    Player[currentPlayerIndex]->hand[selectedCardiIndex].sprite.setColor(Color::White);
                                }
                            }
                        }
                        break;

                    default:
                        break;
                    }
                }
                if(move_transition)
                {
                    if (currentPlayerIndex != -1 && dynamic_cast<HumanPlayer*>(Player[currentPlayerIndex].get()) != nullptr && number_of_players_people != 1)
                    {
                        // Wait for "Enter" key press
                        bool keyPressed = false;
                        while (!keyPressed)
                        {
                            message_who_move_transition.setString(L"Теперь ходит игрок: " + std::to_string(who_move_transition(Player, currentPlayerIndex, number_of_players_people)));
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
                    do {
                        // Передача хода следующему игроку
                        currentPlayerIndex = (currentPlayerIndex + 1) % Player.size();
                    } while (Player[currentPlayerIndex]->hand.empty()); // Проверка, есть ли у игрока карты

                    CardiTaken = false; // Сброс флага взятия карты

                    // Обновление статуса игроков
                    for (int i = 0; i < Player.size(); ++i) {
                        Player[i]->isAttacker = (i == currentPlayerIndex);
                    }
                    move_transition =false;
                    showCardiSelectionMessage = true;
                    selectedButtonIndex = 0;
                    if(playersChoseNo == number_of_players)
                    {
                        showCardiSelectionMessage = false;
                        canTakeCardi = true;
                    }
                    
                }
            }
            else
            {
            if (currentPlayerIndex != -1 && dynamic_cast<HumanPlayer*>(Player[currentPlayerIndex].get()) != nullptr)
            {
                if (event.type == sf::Event::KeyPressed && (showCardiSelectionMessage || showMessageEscape))
                {
                    // Проверка нажатия клавиш для переключения между кнопками "Да" и "Нет"

                        if (event.key.code == sf::Keyboard::Left)
                        {
                            // Переключение на предыдущую кнопку
                            selectedButtonIndex = (selectedButtonIndex - 1 + 2) % 2;
                            sound.play();
                        }
                        else if (event.key.code == sf::Keyboard::Right)
                        {
                            // Переключение на следующую кнопку
                            selectedButtonIndex = (selectedButtonIndex + 1) % 2;
                            sound.play();
                        }
                        else if (event.key.code == sf::Keyboard::Enter)
                        {
                            if (showCardiSelectionMessage && selectedButtonIndex == 0) // Проверка, выбрана ли кнопка "Да"
                            {
                                showCardiSelectionMessage = false; // Сброс флага после выбора кнопки "Да"
                                selected_Cardi.clear(); // Очистка выбора карт
                            }
                            else if(showMessageEscape && selectedButtonIndex == 0)
                            {
                                returnToMenu = true;
                                break;
                            }
                            else if(showMessageEscape && selectedButtonIndex == 1)
                            {
                                showMessageEscape = false;
                                showCardiSelectionMessage = true;
                                selected_Cardi.clear(); // Очистка выбора карт

                            }
                            else if (showCardiSelectionMessage && selectedButtonIndex == 1 && CardiTaken) // Проверка, выбрана ли кнопка "Нет"
                            {
                                showCardiSelectionMessage = false; // Сброс флага после выбора кнопки "Нет"
                                selected_Cardi.clear(); // Очистка выбора карт
                                move_transition = true;
                            }
                    }
                }  
                // Переход к предыдущей карте с круговым циклом
                else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left && (canTakeCardi && !CardiTaken))
                {                        
                    if (selectedCardiIndexRight  > 0)
                    {
                        --selectedCardiIndexRight ;
                    }
                    else
                    {
                        selectedCardiIndexRight  = Player[neighborIndex]->hand.size() - 1;
                    }
                    sound.play();
                }
                else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right && (canTakeCardi && !CardiTaken))
                {
                    // Переход к предыдущей карте с круговым циклом
                    if (selectedCardiIndexRight  < Player[neighborIndex]->hand.size() - 1)
                    {
                        ++selectedCardiIndexRight ;
                    }
                    else
                    {
                        selectedCardiIndexRight  = 0;
                    }
                    sound.play();
                }
                else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && (canTakeCardi && !CardiTaken)) 
                {
                    // Если не выбрана, добавляем в вектор и меняем её цвет на красный
                    selected_Cardi.push_back(selectedCardiIndexRight);
                    // Взятие выбранной карты
                    Cardi selectedCardi = Player[neighborIndex]->hand[selectedCardiIndexRight ];
                    Player[neighborIndex]->hand.erase(Player[neighborIndex]->hand.begin() + selectedCardiIndexRight );
                    Player[currentPlayerIndex]->hand.push_back(selectedCardi);
                    CardiTaken = true; // Установка флага взятия карты
                    if(Player[currentPlayerIndex]->isAttacker)////////
                    {
                        showCardiSelectionMessage = true;  
                    } 
                    else
                    {
                            showCardiSelectionMessage = false;  
                    } 
                    selectedCardiIndexRight = 0; // Обновление индекса после удаления
                    selected_Cardi.clear(); // Очистка выбора карт
                    crdpair = true;
                }
                else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left && crdpair)
                {
                    // Переход к предыдущей карте с круговым циклом
                    if (selectedCardiIndex > 0)
                    {
                        --selectedCardiIndex;
                    }
                    else
                    {
                        selectedCardiIndex = Player[currentPlayerIndex]->hand.size() - 1;
                    }
                    sound.play();
                }
                else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right && crdpair)
                {
                    // Переход к следующей карте с круговым циклом
                    if (selectedCardiIndex < Player[currentPlayerIndex]->hand.size() - 1)
                    {
                        ++selectedCardiIndex;
                    }
                    else
                    {
                        selectedCardiIndex = 0;
                    }
                    sound.play();
                }
                else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && crdpair)
                {
                    if (showCardiSelectionMessage && selectedButtonIndex == 0) // Проверка, выбрана ли кнопка "Да"
                    {
                        showCardiSelectionMessage = false; // Сброс флага после выбора кнопки "Да"
                        selected_Cardi.clear(); // Очистка выбора карт
                    }
                    else if (showCardiSelectionMessage && selectedButtonIndex == 1 && CardiTaken) // Проверка, выбрана ли кнопка "Нет"
                    {
                        showCardiSelectionMessage = false; // Сброс флага после выбора кнопки "Нет"
                        selected_Cardi.clear(); // Очистка выбора карт
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
                        // showCardiSelectionMessage = true;
                        selected_Cardi.clear(); // Очистка выбора карт
                    }
                    else if (!showCardiSelectionMessage && !showMessageEscape)
                    {//(логика выбора и пролистывания карт)
                        // Проверяем, выбрана ли уже карта
                        auto it = std::find(selected_Cardi.begin(), selected_Cardi.end(), selectedCardiIndex);

                        // Проверяем, выбрана ли уже карта
                        if (it == selected_Cardi.end())
                        {
                            // Если не выбрана, добавляем в вектор и меняем её цвет на красный
                            selected_Cardi.push_back(selectedCardiIndex);
                            
                            // Проверка выбрано ли две карты
                            if (selected_Cardi.size() == 2)
                            {
                                // Вызов функции для проверки на пару карт
                                bool is_pair = pair_of_Cardis(Player[currentPlayerIndex]->hand[selected_Cardi[0]], Player[currentPlayerIndex]->hand[selected_Cardi[1]]);
                                if (currentPlayerIndex != -1)
                                {
                                    // Вывод результата
                                    if (is_pair)
                                    {
                                        // Действия, если выбранные карты являются парой
                                        // Сортировка индексов выбранных карт по убыванию
                                        std::sort(selected_Cardi.rbegin(), selected_Cardi.rend());

                                        // Удаление парных карт из руки текущего игрока
                                        for (int index : selected_Cardi)
                                        {
                                            Player[currentPlayerIndex]->hand.erase(Player[currentPlayerIndex]->hand.begin() + index);
                                        }
                                        // Обновление индекса после удаления
                                        if (selectedCardiIndex >= Player[currentPlayerIndex]->hand.size() &&  !Player[currentPlayerIndex]->hand.empty())
                                        {
                                            selectedCardiIndex = Player[currentPlayerIndex]->hand.size() - 1;
                                        }
                                        // Возврат к выбору кнопок
                                        showCardiSelectionMessage = true;
                                        selectedCardiIndex = 0;

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
                                        showCardiSelectionMessage = true;
                                    }
                                    
                                    // Очистка списка выбранных карт
                                    selected_Cardi.clear();
                                }
                            }
                        }
                        else
                        {
                            // Если уже выбрана, удаляем из вектора и меняем её цвет на обычный
                            selected_Cardi.erase(it);
                            Player[currentPlayerIndex]->hand[selectedCardiIndex].sprite.setColor(Color::White);
                        }
                    }
                }
            }
            }
        }
        if(returnToMenu)
        {
            Player.clear();
            return 0; break;
        }
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
            showCardiSelectionMessage = false;
            showMessageEscape = false;
        }
        if(move_transition)
        {
            if (currentPlayerIndex != -1 && dynamic_cast<HumanPlayer*>(Player[currentPlayerIndex].get()) != nullptr && number_of_players_people != 1 && number_of_players_people - People_win != 1)
            {
                // Wait for "Enter" key press
                bool keyPressed = false;
                while (!keyPressed)
                {
                    message_who_move_transition.setString(L"Теперь ходит игрок: " + std::to_string(who_move_transition(Player, currentPlayerIndex, number_of_players_people)));
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
            do {
                // Передача хода следующему игроку
                currentPlayerIndex = (currentPlayerIndex + 1) % Player.size();
            } while (Player[currentPlayerIndex]->hand.empty()); // Проверка, есть ли у игрока карты

            CardiTaken = false; // Сброс флага взятия карты

            // Обновление статуса игроков
            for (int i = 0; i < Player.size(); ++i) {
                Player[i]->isAttacker = (i == currentPlayerIndex);
            }
            move_transition = false;
        }
        // Если текущий игрок - бот, выполняем его действия
        if (currentPlayerIndex != -1 && dynamic_cast<Bot*>(Player[currentPlayerIndex].get()) != nullptr ) {
            Bot* currentBot =  dynamic_cast<Bot*>(Player[currentPlayerIndex].get());;
            neighborIndex = findNextPlayerWithCardis(Player, currentPlayerIndex);
            currentBot->takeCardiFromPlayer(*Player[neighborIndex].get());
            currentBot->disCardiPairs();
            move_transition = true; // Передаем ход следующему игроку
            if(dynamic_cast<HumanPlayer*>(Player[neighborIndex].get()) != nullptr)
            {
                move_transition = false;
                canTakeCardi = true;
                CardiTaken = false;
                do {
                    // Передача хода следующему игроку
                    currentPlayerIndex = (currentPlayerIndex + 1) % Player.size();
                } while (Player[currentPlayerIndex]->hand.empty()); // Проверка, есть ли у игрока карты

                // Обновление статуса игроков
                for (int i = 0; i < Player.size(); ++i) {
                    Player[i]->isAttacker = (i == currentPlayerIndex);
                }
            }
        }
        activePlayers = 0;
        People_win = 0;
        for (const auto& player : Player) {
            if (!player->hand.empty()) {
                activePlayers++;
            }
            else
            {
                player->hasWon = true; // Игрок выиграл
                if(dynamic_cast<HumanPlayer*>(player.get()) != nullptr){People_win++;}
            }
            if (Player[currentPlayerIndex]->hasWon)
            {
                do {
                    // Передача хода следующему игроку
                    currentPlayerIndex = (currentPlayerIndex + 1) % Player.size();
                } while (Player[currentPlayerIndex]->hand.empty()); // Проверка, есть ли у игрока карты

                // Обновление статуса игроков
                for (int i = 0; i < Player.size(); ++i) {
                    Player[i]->isAttacker = (i == currentPlayerIndex);
                }
            }
        }
        
        if (activePlayers == 1 || (number_of_players_people - People_win == 0)) 
        {
            // Только один игрок остался с картами, игра окончена
            EndGame = true;
        }
        
        windowss.clear(Color(50,200,150));
        windowss.draw(background);
        
        // Отрисовка меню сохранения/загрузки
        if (showSaveLoadMenu) {
            windowss.draw(saveText);
            windowss.draw(loadText);
            windowss.draw(buttonYes);
            windowss.draw(buttonNo);
        }
        // Отрисовка карт игроков

        Texture back_Cardi_texture;
        if (!back_Cardi_texture.loadFromFile("resources/back.png")) { return 1; }
        RectangleShape back_Cardi_sprite(Vector2f(77,110));
        back_Cardi_sprite.setTexture(&back_Cardi_texture);
        if (number_of_players_people == 1) {draw_for1(Player, selectedCardiIndex, selected_Cardi, selectedCardiIndexRight, neighborIndex, windowss, messege_winplayer, back_Cardi_sprite);}
        if (number_of_players_people == 2) {draw_for2(Player, selectedCardiIndex, selected_Cardi, selectedCardiIndexRight, neighborIndex, windowss, messege_winplayer, back_Cardi_sprite);}
        if (number_of_players_people == 3) {draw_for3(Player, selectedCardiIndex, selected_Cardi, selectedCardiIndexRight, neighborIndex, windowss, messege_winplayer, back_Cardi_sprite);}
        if (number_of_players_people == 4) {draw_for4(Player, selectedCardiIndex, selected_Cardi, selectedCardiIndexRight, neighborIndex, windowss, messege_winplayer, back_Cardi_sprite);}

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

        if (showCardiSelectionMessage)//showMessageEscape
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
        // Например, если вы хотите сохранять состояние игры каждую секунду:
        // if (gameTimer.getElapsedTime().asSeconds() >= 1.0f) {
        //     savegame(Player, number_of_players_people, currentPlayerIndex, playersChoseNo, move_transition, crdpair, CardiTaken, canTakeCardi, showCardiSelectionMessage);
        //     gameTimer.restart();
        // }
        windowss.display();
    }
    return 0;
}
