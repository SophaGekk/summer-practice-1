#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <set>
#include <random>
#include "durak.h"
using namespace sf;
// Функция для поиска текущего игрока, чей ход необходимо выполнить
int FindCurrentPlayerr(const std::vector<Playerr>& Playerrs) {
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
int FindNextPlayerrWithCardds(const std::vector<Playerr>& Playerrs, int currentIndex) {
    int nextIndex = (currentIndex + 1) % Playerrs.size();
    while (Playerrs[nextIndex].hand.empty() && nextIndex != currentIndex) {
        // Пропускаем игроков без карт
        nextIndex = (nextIndex + 1) % Playerrs.size();
    }
    return nextIndex;
}
// Функция для проверки на атаку
bool attack(std::vector<Cardd>& table, Cardd& attackCard) {
    if (!table.empty()) {
        for (int i = 0; i < table.size(); i++) {
            if (attackCard.value == table[i].value) {
                return true;
            }
        }
    } 
    else {
        return true;
    }
    return false;
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
int CurrentPlayerrIndex = -1;
// Глобальная переменная для индекса соседа справа текущего игрока
int neighborIndex = -1;
int main_durakgame(sf::RenderWindow& windowss) {
    srand(time(0));
    // Загрузка шрифта
    sf::Font font_1;
    if (!font_1.loadFromFile("resources/ffont.ttf")) {
        std::cerr << "Ошибка загрузки шрифта: resources/ffont.ttf" << std::endl;
        return -1;
    }
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
                std::cerr << "Ошибка загрузки текстуры: " << filename << std::endl; 
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
    Playerrs[0].name = "Vasya";
    Playerrs[1].name = "Masha";
    Playerrs[2].name = "Stepan";
    Playerrs[3].name = "Katya";
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
    // Нижнее имя
    sf::Text bottomName;
    bottomName.setFont(font_1);
    bottomName.setCharacterSize(24);
    bottomName.setFillColor(sf::Color::White);
    bottomName.setString(Playerrs[0].name);
    bottomName.setPosition(1920 / 2, 1080 - 50);
    // Левое имя
    sf::Text leftName;
    leftName.setFont(font_1);
    leftName.setCharacterSize(24);
    leftName.setString(Playerrs[1].name);
    leftName.setFillColor(sf::Color::White);
    leftName.setPosition(50, 1080 / 2);
    // Верхнее имя
    sf::Text topName;
    topName.setFont(font_1);
    topName.setCharacterSize(24);
    topName.setString(Playerrs[2].name);
    topName.setFillColor(sf::Color::White);
    topName.setPosition(1920 / 2, 50);
    // Правое имя
    sf::Text rightName;
    rightName.setFont(font_1);
    rightName.setCharacterSize(24);
    rightName.setString(Playerrs[3].name);
    rightName.setFillColor(sf::Color::White);
    rightName.setPosition(1920 - 50, 1080 / 2);
    std::vector<int> selected_Cardd(0);
    // Сообщение о победе игрока
    bool messageWin = false;
    sf::Text messageWinner;
    messageWinner.setFont(font_1);
    messageWinner.setCharacterSize(24);
    messageWinner.setFillColor(sf::Color::White);
    messageWinner.setPosition(100.f, 100.f);
    messageWinner.setString(L"The Playerr Won ");
    // Сообщение о выборе карт
    sf::Text messageReset;
    messageReset.setFont(font_1);
    messageReset.setCharacterSize(30);
    messageReset.setFillColor(sf::Color::White);
    messageReset.setPosition(100.f, 100.f);
    messageReset.setString("Is there anything to reset?");
    //Кнопка "Да"
    sf::Text ButtonnYes;
    ButtonnYes.setFont(font_1);
    ButtonnYes.setString("Yes");
    ButtonnYes.setCharacterSize(24);
    ButtonnYes.setFillColor(sf::Color::White);
    ButtonnYes.setPosition(200.f, 200.f);
    //Кнопка "Нет"
    sf::Text ButtonnNo;
    ButtonnNo.setFont(font_1);
    ButtonnNo.setString("No");
    ButtonnNo.setCharacterSize(24);
    ButtonnNo.setFillColor(sf::Color::White);
    ButtonnNo.setPosition(400.f, 200.f);
    // Индекс выбранной кнопки (0 - Да, 1 - Нет)
    // Сообщение о выходе из игры
    sf::Text message_escape;
    message_escape.setFont(font_1);
    message_escape.setCharacterSize(24);
    message_escape.setFillColor(sf::Color::White);
    message_escape.setPosition(100.f, 100.f);
    message_escape.setString(L"Do you really want to get out?");
    // Сообщение о неверном выборе карты
    sf::Text messageInvalid;
    messageInvalid.setFont(font_1);
    messageInvalid.setCharacterSize(24);
    messageInvalid.setFillColor(sf::Color::White);
    messageInvalid.setPosition(100.f, 100.f);
    messageInvalid.setString(L"Wrong card, choose another one!");
    // Флаги для иправления игрой
    bool showMessageEscape = false; // Флаг для отображения текста выхода из игры
    bool showInvalidMessage = false; // Флаг для отображения текста о неправильном выборе карты
    bool showCarddSelectionMessage = false; // Флаг для отображения текста выбора карт
    bool returnToMenu = false; // Флаг для отображения текста выхода в меню
    bool canTakeCardd = false; // Можно ли выбирать карту
    bool moveTransition = false; // Переход хода
    bool EndGame = false; // Окончание игры
    bool GameOver = false; // Проигрыш
    bool YouWin = false; // Выигрыш
    bool attacking = false;
    bool defending = false;
    int selectedButtonnIndex = 0; // Индекс выбранной кнопки для "Yes" и "No"
    int selectedButtonnIndex_2 = 0; // Индекс выбранной кнопки для "Return to menu" и "Restart Game"
    // Создаем затемнение фона
    sf::RectangleShape backgroundFade;
    backgroundFade.setSize(sf::Vector2f(windowss.getSize().x, windowss.getSize().y));
    backgroundFade.setFillColor(sf::Color(0, 0, 0, 150)); // Полупрозрачный черный цвет
    // Загружаем изображения поражения и победы
    sf::Texture youWinTexture, gameOverTexture;
    if (!youWinTexture.loadFromFile("resources/YouWin.png")) {
        std::cerr << "Ошибка загрузки YouWin!" << std::endl;
    }
    if (!gameOverTexture.loadFromFile("resources/YouLose.png")) {
        std::cerr << "Ошибка загрузки YouLose!" << std::endl;
    }
    sf::Sprite endGameSprite;
    // Создаем кнопки "Return to menu" и "Restart Game"
    Buttonn menuButtonn("Return to menu", sf::Vector2f(400, 50), 30, sf::Color::Transparent, sf::Color::White, font_1);
    Buttonn restartButtonn("Restart Game", sf::Vector2f(400, 50), 30, sf::Color::Transparent, sf::Color::White, font_1);
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
    while (windowss.isOpen()) {
        dealingCards (decks, Playerrs);
        // Определение текущего игрока
        CurrentPlayerrIndex = whoFirst(Playerrs,trump);
        Playerrs[CurrentPlayerrIndex].isAttacker = true;
        // Определение соседа справа
        neighborIndex = FindNextPlayerrWithCardds(Playerrs, CurrentPlayerrIndex);
        Playerrs[neighborIndex].isDefender = true;
        attacking = true;
        canTakeCardd = true; // Можно выбрать карту для атаки
        showCarddSelectionMessage = true;
        // Обработка событий
        sf::Event event;
        while (windowss.pollEvent(event)) {
            // Escape - выход
            if (event.type == sf::Event::KeyPressed && event.key.code == Keyboard::Escape) {
                showCarddSelectionMessage = false;
                showMessageEscape = true;
            }
            // Конец игры
            else if (event.type == sf::Event::KeyPressed && EndGame) {
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
            else if (event.type == sf::Event::KeyPressed && (showCarddSelectionMessage || showMessageEscape)) {
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
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left && canTakeCardd && showCarddSelectionMessage) {
                // Переход к предыдущей карте с круговым циклом
                if (selectedCarddIndex > 0) {
                    --selectedCarddIndex;
                } else {
                    selectedCarddIndex = Playerrs[CurrentPlayerrIndex].hand.size() - 1;
                }
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right && canTakeCardd && showCarddSelectionMessage) {
                // Переход к следующей карте с круговым циклом
                if (selectedCarddIndex < Playerrs[CurrentPlayerrIndex].hand.size() - 1) {
                    ++selectedCarddIndex;
                } else {
                    selectedCarddIndex = 0;
                }
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && (attacking || defending) && !showCarddSelectionMessage && !showMessageEscape) {
                if (attacking) {
                    // Если выбранной картой можно атаковать
                    if (attack(table, Playerrs[CurrentPlayerrIndex].hand[selectedCarddIndex])) {
                        attackCardds.push_back(Playerrs[CurrentPlayerrIndex].hand[selectedCarddIndex]);
                        table.push_back(Playerrs[CurrentPlayerrIndex].hand[selectedCarddIndex]);
                        // Удаление выбранной карты из руки текущего игрока
                        Playerrs[CurrentPlayerrIndex].hand.erase(Playerrs[CurrentPlayerrIndex].hand.begin() + selectedCarddIndex); 
                        // Обновление индекса после удаления
                        if (selectedCarddIndex >= Playerrs[CurrentPlayerrIndex].hand.size() &&  !Playerrs[CurrentPlayerrIndex].hand.empty()) {
                            selectedCarddIndex = Playerrs[CurrentPlayerrIndex].hand.size() - 1;
                        }
                        attackCardd = Playerrs[CurrentPlayerrIndex].hand[selectedCarddIndex];
                        Playerrs[CurrentPlayerrIndex].isAttacker = false;
                        Playerrs[neighborIndex].isDefender = true;
                        attacking = false;
                        defending = true;
                        showCarddSelectionMessage = true;
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
                        showCarddSelectionMessage = true;
                    }
                } 
                else if (defending) {
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
                        Playerrs[CurrentPlayerrIndex].isAttacker = true;
                        Playerrs[neighborIndex].isDefender = false;
                        attacking = true;
                        defending = false;
                        showCarddSelectionMessage = true;
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
                        showCarddSelectionMessage = true;
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
                moveTransition = true;
            }
            if(returnToMenu) {
                return 0; 
                break;
            }
            // Проверка на конец игры
            if (EndGame) {
                if (decks.empty()) {
                    for (int i = 0; i < Playerrs.size(); i++) {
                        if (Playerrs[i].hand.empty()) {
                            YouWin = true;
                        }
                    }
                    Playerrs.erase(std::remove_if(Playerrs.begin(), Playerrs.end(), [](const Playerr& p) { 
                        return p.hand.empty(); 
                    }), Playerrs.end()); 
                    if (Playerrs.size() == 1) {
                        GameOver = true;
                    }
                }
                showCarddSelectionMessage = false;
                showMessageEscape = false;
            }
            // Переход хода следующему игроку
            if(moveTransition) {
                attacking = true;
                defending = false;
                for (neighborIndex; neighborIndex < Playerrs.size(); neighborIndex++) {
                    if (!Playerrs[neighborIndex].hand.empty()) {
                        Playerrs[CurrentPlayerrIndex].isAttacker = false;
                        Playerrs[CurrentPlayerrIndex].isDefender = false;
                        Playerrs[neighborIndex].isAttacker = true;
                        Playerrs[neighborIndex].isDefender = false;
                        Playerrs[FindNextPlayerrWithCardds(Playerrs, neighborIndex)].isAttacker = false;
                        Playerrs[FindNextPlayerrWithCardds(Playerrs, neighborIndex)].isDefender = true;
                    }
                }
                bottomName.setFillColor(sf::Color::White);
                leftName.setFillColor(sf::Color::White);
                topName.setFillColor(sf::Color::White);
                rightName.setFillColor(sf::Color::White);
            }
        }
        windowss.clear(Color(50,200,150));
        windowss.draw(background);
        // Отрисовка карт игроков
        Texture back_Cardd_texture;
        if (!back_Cardd_texture.loadFromFile("resources/back.png")) { return 1; }
        RectangleShape back_Cardd_sprite(Vector2f(77,110));
        back_Cardd_sprite.setTexture(&back_Cardd_texture);
        // Отрисовка карт для каждого игрока
        for (int playerIndex = 0; playerIndex < Playerrs.size(); ++playerIndex) {
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
            if (Playerrs[playerIndex].hasWon) {
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
                messageWinner.setPosition(messagePosition);
                windowss.draw(messageWinner);
                continue; // Пропускаем отрисовку карт для выигравшего игрока
            }
            back_Cardd_sprite.setRotation(rotation);
            for (size_t cardIndex = 0; cardIndex < Playerrs[playerIndex].hand.size(); ++cardIndex) {
                Cardd& cardd = Playerrs[playerIndex].hand[cardIndex];
                // Проверяем, является ли текущий игрок атакующим
                if (Playerrs[playerIndex].isAttacker && attacking) {
                    // Атакующий игрок видит значения своих карт
                    cardd.sprite.setTexture(cardd.texture); 
                    if (playerIndex == 0) {
                        cardd.sprite.setPosition(Vector2f(200 + (1300 / Playerrs[playerIndex].hand.size()) * id, 800));
                    } else if (playerIndex == 1) {
                        cardd.sprite.setPosition(Vector2f(1820, 123 + (800 / Playerrs[playerIndex].hand.size()) * id));
                    } else if (playerIndex == 2) {
                        cardd.sprite.setPosition(Vector2f(200 + (1300 / Playerrs[playerIndex].hand.size()) * id, 150));
                    } else if (playerIndex == 3) {
                        cardd.sprite.setPosition(Vector2f(100, 200 + (800 / Playerrs[playerIndex].hand.size()) * id));
                    }
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (std::find(selected_Cardd.begin(), selected_Cardd.end(), cardIndex) != selected_Cardd.end()) {
                        cardd.sprite.setColor(Color::Magenta);
                    } else if (cardIndex == selectedCarddIndex) {
                        cardd.sprite.setColor(Color::Yellow);
                    } else {
                        cardd.sprite.setColor(Color::White);
                    }
                    windowss.draw(cardd.sprite);
                    ++id;
                }
                // Проверяем, является ли текущий игрок защищающимся
                else if (Playerrs[playerIndex].isDefender && defending) {
                    // Защищающийся игрок видит значения своих карт
                    cardd.sprite.setTexture(cardd.texture);
                    if (playerIndex == 0) {
                        cardd.sprite.setPosition(Vector2f(200 + (1300 / Playerrs[playerIndex].hand.size()) * id, 800));
                    } else if (playerIndex == 1) {
                        cardd.sprite.setPosition(Vector2f(1820, 123 + (800 / Playerrs[playerIndex].hand.size()) * id));
                    } else if (playerIndex == 2) {
                        cardd.sprite.setPosition(Vector2f(200 + (1300 / Playerrs[playerIndex].hand.size()) * id, 150));
                    } else if (playerIndex == 3) {
                        cardd.sprite.setPosition(Vector2f(100, 200 + (800 / Playerrs[playerIndex].hand.size()) * id));
                    }
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (std::find(selected_Cardd.begin(), selected_Cardd.end(), cardIndex) != selected_Cardd.end()) {
                        cardd.sprite.setColor(Color::Magenta);
                    } else if (cardIndex == selectedCarddIndex) {
                        cardd.sprite.setColor(Color::Yellow);
                    } else {
                        cardd.sprite.setColor(Color::White);
                    }
                    windowss.draw(cardd.sprite);
                    ++id;
                }
                else {
                    // У остальных игроков карты рубашкой вверх
                    if (playerIndex == 0) {
                        back_Cardd_sprite.setPosition(Vector2f(200 + (1300 / Playerrs[playerIndex].hand.size()) * id, 800));
                    } else if (playerIndex == 1) {
                        back_Cardd_sprite.setPosition(Vector2f(1820, 123 + (800 / Playerrs[playerIndex].hand.size()) * id));
                    } else if (playerIndex == 2) {
                        back_Cardd_sprite.setPosition(Vector2f(200 + (1300 / Playerrs[playerIndex].hand.size()) * id, 150));
                    } else if (3) {
                        back_Cardd_sprite.setPosition(Vector2f(100, 200 + (800 / Playerrs[playerIndex].hand.size()) * id));
                    }
                    windowss.draw(back_Cardd_sprite);
                    id++;
                }
            }
        }
        // Отрисовка колоды карт
        Texture decksTexture;
        if (!decksTexture.loadFromFile("resources/back.png")) { 
            std::cerr << "Ошибка загрузки текстуры колоды: resources/back.png" << std::endl;
            return -1; 
        }
        RectangleShape decksSprite(Vector2f(77,110));
        decksSprite.setTexture(&decksTexture);
        if (decks.size() != 1) {
            decksSprite.setPosition(10, 10);
            windowss.draw(decksSprite);
        }
        // Отрисовка козыря
        Texture trumpTexture;
        std::string trumpFilename = "resources/" + std::to_string(trump.value) + trump.suit + ".png";
        if (!trumpTexture.loadFromFile(trumpFilename)) {
            std::cerr << "Ошибка загрузки текстуры козыря: " << trumpFilename << std::endl;
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
                messageReset.setString(L"Select Cardd for attack");
            } else if (defending) {
                messageReset.setString(L"Select Cardd for defend");
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
            int id = 1; // Переменная для определения позиции карты в ряду
            Cardd& cardd = attackCardds[cardind];// Атакующий игрок видит значения своих карт
            cardd.sprite.setTexture(cardd.texture);
            // Определение позиции карты в зависимости от её индекса 
            cardd.sprite.setPosition(Vector2f(920 + (130 / attackCardds.size()) * id, 595));
            // Если карта выбрана, она рисуется в желтый цвет
            windowss.draw(cardd.sprite);
            ++id; 
        }
        // Отрисовка отбивающихся карт
        for (int cardind = 0; cardind < defendingCardds.size(); ++cardind) {
            int id = 1; // Переменная для определения позиции карты в ряду
            Cardd& cardd = defendingCardds[cardind];// Атакующий игрок видит значения своих карт
            cardd.sprite.setTexture(cardd.texture);
            // Определение позиции карты в зависимости от её индекса 
            cardd.sprite.setPosition(Vector2f(920 + (130 / defendingCardds.size()) * id, 695));
            // Если карта выбрана, она рисуется в желтый цвет
            windowss.draw(cardd.sprite);
            ++id;
        }
        // Отрисовка имен атакующего(красный) и защищающегося(зеленый) игроков
        if (Playerrs[0].isDefender == true) {
            bottomName.setFillColor(sf::Color::Green);
        } else if (Playerrs[0].isAttacker == true) {
            bottomName.setFillColor(sf::Color::Red);
        } else {
            bottomName.setFillColor(sf::Color::White);
        }
        if (Playerrs[1].isDefender == true) {
            leftName.setFillColor(sf::Color::Green);
        } else if (Playerrs[1].isAttacker == true) {
            leftName.setFillColor(sf::Color::Red);
        } else {
            leftName.setFillColor(sf::Color::White);
        }
        leftName.setRotation(-90);
        if (Playerrs[2].isDefender == true) {
            topName.setFillColor(sf::Color::Green);
        } else if (Playerrs[2].isAttacker == true) {
            topName.setFillColor(sf::Color::Red);
        } else {
            topName.setFillColor(sf::Color::White);
        }
        if (Playerrs[3].isDefender == true) {
            rightName.setFillColor(sf::Color::Green);
        } else if (Playerrs[3].isAttacker == true) {
            rightName.setFillColor(sf::Color::Red);
        } else {
            rightName.setFillColor(sf::Color::White);
        }
        rightName.setRotation(90);
        windowss.draw(bottomName);
        windowss.draw(leftName);
        windowss.draw(topName);
        windowss.draw(rightName);
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
        windowss.display();
    }
    return 0;
}