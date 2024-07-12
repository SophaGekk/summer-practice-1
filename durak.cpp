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
class Card_durak {
public:
    int value; // Значение карты (6-14)
    std::string suit; // Масть карты (HEARTS, DIAMONDS, CLUBS, SPADES)
    Texture texture; // Текстура карты
    Sprite sprite; // Спрайт карты
};
// Класс для представления игрока
class Player_durak {
public:
    std::string name; // Имя игрока
    std::vector<Card_durak> hand; // Рука игрока
    bool isAttacker = false; // Является ли игрок атакующим
    bool isDefender = false; // Является ли игрок защищающимся
    bool hasWon = false;
    // Конструктор игрока
    Player_durak() : isAttacker(false), isDefender(false), hasWon(false), name("") {}
};
class Button_durak {
private:
    sf::RectangleShape Button_durakShape;
    sf::Text Button_durakText;
public:
    Button_durak(const std::string& text, const sf::Vector2f& size, const int charSize, const sf::Color& bgColor, const sf::Color& textColor, sf::Font& font) {
        // Установка формы кнопки
        Button_durakShape.setSize(size);
        Button_durakShape.setFillColor(bgColor);
        // Установка текста кнопки
        Button_durakText.setFont(font);
        Button_durakText.setString(text);
        Button_durakText.setCharacterSize(charSize);
        Button_durakText.setFillColor(textColor);
        // Центрирование текста на кнопке
        sf::FloatRect textRect = Button_durakText.getLocalBounds();
        Button_durakText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        Button_durakText.setPosition(sf::Vector2f(size.x / 2.0f, size.y / 2.0f));
    }
    // Функция для установки позиции кнопки
    void setPosition(const sf::Vector2f& pos) {
        Button_durakShape.setPosition(pos);
        Button_durakText.setPosition(pos.x + Button_durakShape.getSize().x / 2.0f, pos.y + Button_durakShape.getSize().y / 2.0f);
    }
    // Функция для отрисовки кнопки
    void draw(sf::RenderWindow& window) {
        window.draw(Button_durakShape);
        window.draw(Button_durakText);
    }
    // Для состояния выделения
    bool isSelected;
    // Функция для установки состояния выделения
    void setSelected(bool select) {
        isSelected = select;
        if (isSelected) {
            Button_durakShape.setFillColor(sf::Color::Black); // Выделенная кнопка будет черного цвета
        } else {
            Button_durakShape.setFillColor(sf::Color::Transparent); // Обычный цвет кнопки
        }
    }
};
// Функция для поиска текущего игрока, чей ход необходимо выполнить
int findCurrentPlayer_duraks(const std::vector<Player_durak>& Player_duraks) {
    for (int i = 0; i < Player_duraks.size(); ++i) {
        if (!Player_duraks[i].hand.empty() && !Player_duraks[i].hasWon) {
            if (Player_duraks[i].isAttacker)
            {
                return i; // Возвращает индекс текущего атакующего игрока
            }
        }
    }
    return -1; // Все игроки выиграли
}
int findNextPlayer_durakWithCard_duraks(const std::vector<Player_durak>& Player_duraks, int currentIndex) {
    int nextIndex = (currentIndex + 1) % Player_duraks.size();
    while (Player_duraks[nextIndex].hand.empty() && nextIndex != currentIndex) {
        // Пропускаем игроков без карт
        nextIndex = (nextIndex + 1) % Player_duraks.size();
    }
    return nextIndex;
}
// Функция для проверки на атаку
bool attack(std::vector<Card_durak>& table, Card_durak& attackCard_durak) {
    if (!table.empty()) {
        for (int i = 0; i < table.size(); i++) {
            if (attackCard_durak.value == table[i].value) {
                return true;
            }
        }
    } 
    return false;
}
// Функция для проверки на защиту
bool defense(Card_durak& attackCard_durak, Card_durak& defenseCard_durak, Card_durak& trump) {
    if ((defenseCard_durak.value > attackCard_durak.value && defenseCard_durak.suit == attackCard_durak.suit) ||
        (defenseCard_durak.suit == trump.suit && attackCard_durak.suit != trump.suit)) {
        return true;
    }
    return false;
}
// Функция для определения, кто первый ходит
int whoFirst (std::vector<Player_durak>& Player_duraks, Card_durak& trump) {
    // Словарь для хранения минимального козыря и значения для каждого игрока
    std::map<int, std::pair<int, int>> Player_durakData;
    // Проход по всем игрокам
    for (int i = 0; i < Player_duraks.size(); i++) {
        int minTrump = 14;  // Максимальное значение козыря
        int minValue = 14; // Максимальное значение карты
        for (Card_durak& Card_durak : Player_duraks[i].hand) {
            if (Card_durak.suit == trump.suit) {
                minTrump = std::min(minTrump, Card_durak.value);
            }
            minValue = std::min(minValue, Card_durak.value);
        }
        // Сохранение данных в словарь
        Player_durakData[i] = std::make_pair(minTrump, minValue);
    }
    // Определение первого игрока
    int firstPlayer_durakIndex = 0;
    int minTrump = Player_durakData[0].first;
    int minValue = Player_durakData[0].second;
    for (int i = 1; i < Player_duraks.size(); i++) {
        if (Player_durakData[i].first < minTrump) {
            minTrump = Player_durakData[i].first;
            firstPlayer_durakIndex = i;
        } else if (Player_durakData[i].first == minTrump && Player_durakData[i].second < minValue) {
            minValue = Player_durakData[i].second;
            firstPlayer_durakIndex = i;
        }
    }
    return firstPlayer_durakIndex;
}
// Функция, которая проверяет, закончилась ли игра
bool isOver (std::vector<Card_durak>& deck, std::vector<Player_durak>& Player_duraks) {
    int count = 0;
    for (Player_durak& Player_durak: Player_duraks) {
        if (Player_durak.hand.empty()) {
            count++;
        }
    }
    if (deck.empty() && (count <= 3)) {
        return true;
    }
    return false;
}
// Глобальная переменная для индекса текущего игрока
int currentPlayer_durakIndexs = 0;
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
    std::vector<Card_durak> deck(36);
    sf::Texture Card_duraksTexture;
     for (int value = 6; value < 14; value++) {
        for (const std::string& suit : suits) {
            Card_durak Card_duraks;
            Card_duraks.value = value;
            Card_duraks.suit = suit;
            std::string deckFilename = "resources/" + std::to_string(value) + suit + ".png";
            if (!Card_duraksTexture.loadFromFile(deckFilename)) { 
                std::cerr << "Ошибка загрузки текстуры карт: " << deckFilename << std::endl; 
                return -1;
            } else {
                Card_duraks.texture = Card_duraksTexture; 
                Card_duraks.sprite.setTexture(Card_duraks.texture);
                deck.push_back(Card_duraks);
            }
        }
    }
    // Создание спрайта колоды
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
    Card_durak trump = deck.back();
    deck.pop_back();
    // Возврат козыря в начало колоды
    deck.insert(deck.begin(), trump);
    // Создание спрайта козыря
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
    std::vector<Player_durak> Player_duraks(4);
    Player_duraks[0].name = "Vasya";
    Player_duraks[1].name = "Masha";
    Player_duraks[2].name = "Stepan";
    Player_duraks[3].name = "Katya";
    // Player_duraks[0].isAttacker = false;
    // Player_duraks[0].isDefender = false;
    // Player_duraks[1].isAttacker = false;
    // Player_duraks[1].isDefender = false;
    // Player_duraks[2].isAttacker = false;
    // Player_duraks[2].isDefender = false;
    // Player_duraks[3].isAttacker = false;
    // Player_duraks[3].isDefender = false;
    // Текст для отображения имен игроков
    sf::Font font;
    if (!font.loadFromFile("resources/ffont.ttf"))
    {
        std::cerr << "Ошибка загрузки шрифта: resources/ffont.ttf" << std::endl;
        return -1;
    }
    sf::Text Player_durakTexts[4];
    for (size_t i = 0; i < 4; ++i) {
        Player_durakTexts[i].setFont(font);
        Player_durakTexts[i].setCharacterSize(24);
        Player_durakTexts[i].setFillColor(sf::Color::White);
        Player_durakTexts[i].setString(Player_duraks[i].name);
    }
    // Позиционирование имен игроков
    Player_durakTexts[0].setPosition(200, 700);
    Player_durakTexts[1].setPosition(1600, 200);
    Player_durakTexts[2].setPosition(200, 100);
    Player_durakTexts[3].setPosition(100, 600);
    // Раздача карт
    int Card_duraksPerPlayer_durak = 6;
    for (int i = 0; i < Card_duraksPerPlayer_durak; i++)
    {
        for (int j = 0; j < Player_duraks.size(); ++j)
        {
            if (!deck.empty()) {
                Player_duraks[j].hand.push_back(deck.back());
                deck.pop_back();
            } else {
                std::cerr << "Колода пуста. Невозможно продолжить раздачу карт." << std::endl;
                break; // Выход из цикла, если колода пуста
            }
        }
    }
    // int selectedCard_durakIndexs = 0;
    // int selectedCard_durakIndexsRight = 0;
    // // Сообщение о выходе
    // sf::Text message_escape;
    // message_escape.setFont(font);
    // message_escape.setCharacterSize(24);
    // message_escape.setFillColor(sf::Color::White);
    // message_escape.setPosition(100.f, 100.f);
    // message_escape.setString(L"Do you really want to get out?");
    // // Флаг для отображения текста выхода из игры
    // bool showMessageEscape = false;
    // // Флаг для отображения текста выхода в меню
    // bool returnToMenu = false;
    // // Сообщение
    // sf::Text message;
    // message.setFont(font);
    // message.setCharacterSize(30);
    // message.setFillColor(sf::Color::White);
    // message.setPosition(100.f, 100.f);
    // // Кнопка "Да"
    // sf::Text Button_durakYes;
    // Button_durakYes.setFont(font);
    // Button_durakYes.setString("Yes");
    // Button_durakYes.setCharacterSize(24);
    // Button_durakYes.setFillColor(sf::Color::White);
    // Button_durakYes.setPosition(200.f, 200.f);
    // // Кнопка "Нет"
    // sf::Text Button_durakNo;
    // Button_durakNo.setFont(font);
    // Button_durakNo.setString("No");
    // Button_durakNo.setCharacterSize(24);
    // Button_durakNo.setFillColor(sf::Color::White);
    // Button_durakNo.setPosition(400.f, 200.f);
    // // Индекс выбранной кнопки (0 - Да, 1 - Нет)
    // int selectedButton_durakIndex = 0;
    // // Флаги для управления игрой
    // bool showCard_durakSelectionMessage = true; // Флаг для отображения текста выбора карт
    // bool showMessage = false; // Флаг для отображения сообщения
    // bool showInvalidMove = false; // Флаг для отображения сообщения о неправильном выборе карты для атаки/защиты
    // bool attacking = true; // Можно ли атаковать
    // bool defending = false; // Можно ли защищаться
    // bool EndGame = false; //окончание игры
    // bool GameOver = false; //проигрыш
    // bool YouWin = false; //выигрыш
    // int neighborIndex = 1;
    // int activePlayer_duraks = 0;
    int selectedCard_durakIndexs = 0;
    int selectedCard_durakIndexsRight = 0;
    std::vector<int> selected_Card_durak(0);
    // sf::Font font;
    if (!font.loadFromFile("resources/ffont.ttf")) {
        std::cerr << "Ошибка загрузки шрифта!" << std::endl;
    }
    bool messege_win_Player_durak = false;
    sf::Text messege_winPlayer_durak;
    messege_winPlayer_durak.setFont(font);
    messege_winPlayer_durak.setCharacterSize(24);
    messege_winPlayer_durak.setFillColor(sf::Color::White);
    messege_winPlayer_durak.setPosition(100.f, 100.f);
    // Текст вопроса
    messege_winPlayer_durak.setString(L"The Player_durak Won ");
    sf::Text message_ispair;
    message_ispair.setFont(font);
    message_ispair.setCharacterSize(24);
    message_ispair.setFillColor(sf::Color::White);
    message_ispair.setPosition(100.f, 100.f);
    // Текст вопроса
    message_ispair.setString(L"Oh, you will not be able to disCard_durak these Card_duraks...");
    sf::Text message;
    message.setFont(font);
    message.setCharacterSize(30);
    message.setFillColor(sf::Color::White);
    message.setPosition(100.f, 100.f);
    // Текст вопроса
     message.setString("Is there anything to reset?");
    // Создание кнопок "Да" и "Нет"
    sf::Text Button_durakYes;
    Button_durakYes.setFont(font);
    Button_durakYes.setString("Yes");
    Button_durakYes.setCharacterSize(24);
    Button_durakYes.setFillColor(sf::Color::White);
    Button_durakYes.setPosition(200.f, 200.f);
    sf::Text Button_durakNo;
    Button_durakNo.setFont(font);
    Button_durakNo.setString("No");
    Button_durakNo.setCharacterSize(24);
    Button_durakNo.setFillColor(sf::Color::White);
    Button_durakNo.setPosition(400.f, 200.f);
    // Флаг для отображения текста выбора карт
    bool showCard_durakSelectionMessage = true;
    // Индекс выбранной кнопки (0 - Да, 1 - Нет)
    int selectedButton_durakIndex = 0;
    sf::Text message_escape;
    message_escape.setFont(font);
    message_escape.setCharacterSize(24);
    message_escape.setFillColor(sf::Color::White);
    message_escape.setPosition(100.f, 100.f);
    // Текст вопроса
    message_escape.setString(L"Do you really want to get out?");
    // Флаг для отображения текста выхода из игры
    bool showMessageEscape = false;
    // Флаг для отображения текста выхода в меню
    bool returnToMenu = false;
    // Переменная для отслеживания количества игроков, выбравших "нет"
    int Player_duraksChoseNo = 0; 
    // Флаг для отображения текста ожидания игроков
    bool showMessageWait = false;
    // Здесь код для отображения сообщения "Ожидаем других игроков: количество"
    sf::Text wait_message;
    wait_message.setFont(font);
    wait_message.setCharacterSize(30);
    wait_message.setFillColor(sf::Color::White);
    wait_message.setPosition(100.f, 100.f);
    bool Taking_Card_duraks = false; // Флаг для проверки, взял ли игрок карту
    bool canSelectCard_duraks = true; // Можно ли выбирать карты
    bool canTakeCard_durak = false; // Можно ли брать карту у противника
    bool Card_durakTaken = false; // Флаг, указывающий на то, что карта была взята
    bool move_transition = false;
    bool crdpair =false;
    bool EndGame = false; //окончание игры
    bool GameOver = false; //проигрыш
    bool YouWin = false; //выигрыш
    int neighborIndex = 1;
    int activePlayer_duraks = 0;
    bool attacking = false;
    bool defending = false;
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
    Button_durak menuButton_durak("Return to menu", sf::Vector2f(400, 50), 30, sf::Color::Transparent, sf::Color::White, font);
    Button_durak restartButton_durak("Restart Game", sf::Vector2f(400, 50), 30, sf::Color::Transparent, sf::Color::White, font);
    // Установка позиции кнопок
    menuButton_durak.setPosition(sf::Vector2f(770, 640));
    restartButton_durak.setPosition(sf::Vector2f(770, 740));
    int selectedButton_durakIndex_2 = 0; // Индекс выбранной кнопки
    std::vector<Button_durak> Button_duraks = {menuButton_durak, restartButton_durak}; // Вектор всех кнопок
    // Game Loop
    int countMove = 1;
    std::vector<Card_durak> table(36); 
    std::vector<Card_durak> attackCard_duraks(36);
    selectedCard_durakIndexs = 0;
    while (windowss.isOpen()) {
        // Определение текущего игрока
        if (countMove == 1) {
            currentPlayer_durakIndexs = whoFirst(Player_duraks,trump);
        } else {
            currentPlayer_durakIndexs = findCurrentPlayer_duraks(Player_duraks);
        }
        Player_duraks[currentPlayer_durakIndexs].isAttacker = true;
        // Определение соседа справа
        neighborIndex = findNextPlayer_durakWithCard_duraks(Player_duraks, currentPlayer_durakIndexs);
        Player_duraks[neighborIndex].isDefender = true;
        // Раздача карт
        for (auto& Player_durak : Player_duraks) {
            if (Player_durak.hand.size() < 6) {
                Player_durak.hand.push_back(deck.back());
                deck.pop_back();
            }
        }
        // // Проверка на конец игры
        // if (deck.empty()) {
        //     for (int i = 0; i < Player_duraks.size(); i++) {
        //         if (Player_duraks[i].hand.empty()) {
        //             std::cout << Player_duraks[i].name << " win!" << std::endl;
        //         }
        //     }
        //     Player_duraks.erase(std::remove_if(Player_duraks.begin(), Player_duraks.end(), [](const Player_durak& p) { 
        //         return p.hand.empty(); 
        //     }), Player_duraks.end()); 
        //     if (Player_duraks.size() == 1) {
        //         std::cout << Player_duraks[0].name << " - durak!" << std::endl;
        //     }
        // }
        // Обработка событий
        sf::Event event;
        while (windowss.pollEvent(event)) {
            // Закрытие окна
            if (event.type == sf::Event::Closed) windowss.close();
            // Escape - выход
            if (event.type == sf::Event::KeyPressed && event.key.code == Keyboard::Escape) {
                showMessageEscape = true;
                showCard_durakSelectionMessage = false;
            }
            // Конец игры
            if (event.type == sf::Event::KeyPressed && EndGame) {
                if (event.key.code == sf::Keyboard::Up) {
                    // Переключение на предыдущую кнопку
                    selectedButton_durakIndex_2 = (selectedButton_durakIndex_2 - 1 + 2) % 2;
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    // Переключение на следующую кнопку
                    selectedButton_durakIndex_2 = (selectedButton_durakIndex_2 + 1) % 2;
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    // Проверка, выбрана ли кнопка "Menu"
                    if (selectedButton_durakIndex_2 == 0) {
                        // returnToMenu = true;
                        // return 0;
                        return 0; break;
                    }
                    // Проверка, выбрана ли кнопка "Restart"
                    else if(selectedButton_durakIndex_2 == 1) {
                        // Здесь код для перезапуска игры
                        // Сброс всех игровых переменных и состояний
                        // Пересоздание колоды карт и перераспределение карт игрокам
                        // Сброс флагов состояния игры
                        EndGame = false;
                        GameOver = false;
                        YouWin = false;
                        // Перезапуск основного игрового цикла
                        main_durakgame(windowss);  return 0; break;
                    }
                }
            }
            // Обработка кнопок "Да", "Нет"
            if (event.type == sf::Event::KeyPressed && (showCard_durakSelectionMessage || showMessageEscape)) {
                // Проверка нажатия клавиш для переключения между кнопками "Да" и "Нет"
                if (event.key.code == sf::Keyboard::Left) {
                    // Переключение на предыдущую кнопку
                    selectedButton_durakIndex = (selectedButton_durakIndex - 1 + 2) % 2;
                } else if (event.key.code == sf::Keyboard::Right) {
                    // Переключение на следующую кнопку
                    selectedButton_durakIndex = (selectedButton_durakIndex + 1) % 2;
                } else if (event.key.code == sf::Keyboard::Enter) {
                    // Проверка, выбрана ли кнопка "Да" 
                    if (showCard_durakSelectionMessage && selectedButton_durakIndex == 0) {
                        showCard_durakSelectionMessage = false; // Сброс флага после выбора кнопки "Да"
                        selected_Card_durak.clear(); // Очистка выбора карт
                    } else if(showMessageEscape && selectedButton_durakIndex == 0) {
                        returnToMenu = true;
                        break;
                    } else if(showMessageEscape && selectedButton_durakIndex == 1) {
                        showMessageEscape = false;
                        showCard_durakSelectionMessage = true;
                        selected_Card_durak.clear(); // Очистка выбора карт
                    // Проверка, выбрана ли кнопка "Нет"
                    } else if (showCard_durakSelectionMessage && selectedButton_durakIndex == 1 && Card_durakTaken) {
                        showCard_durakSelectionMessage = false; // Сброс флага после выбора кнопки "Нет"
                        selected_Card_durak.clear(); // Очистка выбора карт
                        move_transition = true;
                    // Если выбрано "Нет"
                    } else if(showCard_durakSelectionMessage && selectedButton_durakIndex == 1 && !canTakeCard_durak )  {
                        // Player_duraksChoseNo++; так надо
                        Player_duraksChoseNo = Player_duraks.size(); // Для тестирования устанавливаем, что все игроки выбрали "нет"
                        canSelectCard_duraks = false; // Блокируем выбор карт
                        showMessageWait = true; // Активация текста ожидания
                        showCard_durakSelectionMessage = false;
                        if (Player_duraksChoseNo == Player_duraks.size()) { // Если все игроки выбрали "нет"
                            showMessageWait = false; // Деактивация текста ожидания
                            canTakeCard_durak = true; // Позволяем взятие карты у противника
                        }
                    }
                }
            }  
            // Left - выбор карты справа налево
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left && (canTakeCard_durak && !Card_durakTaken)) {
                if (selectedCard_durakIndexs > 0) {
                    --selectedCard_durakIndexs;
                } else {
                    selectedCard_durakIndexs = Player_duraks[currentPlayer_durakIndexs].hand.size() - 1;
                }
            }
            // Right - выбор карты слева направо 
            else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right && (canTakeCard_durak && !Card_durakTaken)) {
                if (selectedCard_durakIndexs < Player_duraks[currentPlayer_durakIndexs].hand.size() - 1) {
                    ++selectedCard_durakIndexs;
                } else {
                    selectedCard_durakIndexs = 0;
                }
            }
            // Enter - выбор карты для атаки
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && (canTakeCard_durak && !Card_durakTaken) && attacking) {
                if (attack(table, Player_duraks[currentPlayer_durakIndexs].hand[selectedCard_durakIndexs])) {
                    attackCard_duraks.push_back(Player_duraks[currentPlayer_durakIndexs].hand[selectedCard_durakIndexs]);
                    table.push_back(Player_duraks[currentPlayer_durakIndexs].hand[selectedCard_durakIndexs]);
                    Player_duraks[currentPlayer_durakIndexs].hand.erase(Player_duraks[currentPlayer_durakIndexs].hand.begin() + selectedCard_durakIndexs);
                    Player_duraks[currentPlayer_durakIndexs].isAttacker = false;
                    Player_duraks[neighborIndex].isDefender = true;
                    attacking = false;
                    defending = true;
                    showCard_durakSelectionMessage = true;
                } else {
                    showCard_durakSelectionMessage = true;
                }
            } 
            // Enter - выбор карты для защиты
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && (canTakeCard_durak && !Card_durakTaken) && defending) {
                if (defense(attackCard_duraks.back(), Player_duraks[currentPlayer_durakIndexs].hand[selectedCard_durakIndexs], trump)) {
                    table.push_back(Player_duraks[currentPlayer_durakIndexs].hand[selectedCard_durakIndexs]);
                    Player_duraks[currentPlayer_durakIndexs].hand.erase(Player_duraks[currentPlayer_durakIndexs].hand.begin() + selectedCard_durakIndexs);
                    Player_duraks[currentPlayer_durakIndexs].isAttacker = true;
                    Player_duraks[neighborIndex].isDefender = false;
                    attacking = true;
                    defending = false;
                    showCard_durakSelectionMessage = true;
                    attackCard_duraks.clear();
                } else {
                    showCard_durakSelectionMessage = true;
                }
            }
        //     // Обработка нажатия кнопок
        //     if (event.type == sf::Event::KeyPressed) {
        //         // Escape - выход
        //         if (event.key.code == sf::Keyboard::Escape) windowss.close();
        //         // Left - выбор карты справа налево
                // if (event.key.code == sf::Keyboard::Left) {
                //     if (selectedCard_durakIndexs > 0) {
                //         --selectedCard_durakIndexs;
                //     } else {
                //         selectedCard_durakIndexs = Player_duraks[currentPlayer_durakIndexs].hand.size() - 1;
                //     }
                // }
                // // Left - выбор карты слева направо 
                // if (event.key.code == sf::Keyboard::Right) {
                //     if (selectedCard_durakIndexs < Player_duraks[currentPlayer_durakIndexs].hand.size() - 1) {
                //         ++selectedCard_durakIndexs;
                //     } else {
                //         selectedCard_durakIndexs = 0;
                //     }
                // }
        //         // P - пас
        //         if (event.key.code == sf::Keyboard::P && attacking) {
        //             Player_duraks[currentPlayer_durakIndexs].isAttacker = false;
        //             Player_duraks[neighborIndex].isAttacker = true;
        //             Player_duraks[neighborIndex].isDefender = false;
        //             attacking = true; 
        //             defending = false;
        //             countMove++;
        //         }
        //         // T - взять карты
        //         if (event.key.code == sf::Keyboard::T && defending) {
        //             for (Card_durak& Card_durak : table) {
        //                 Player_duraks[currentPlayer_durakIndexs].hand.push_back(Card_durak);
        //             }
        //             Player_duraks[currentPlayer_durakIndexs].isDefender = false;
        //             Player_duraks[neighborIndex].isAttacker = true; 
        //             attacking = true;
        //             defending = false;
        //             table.clear();
        //             attackCard_duraks.clear();
        //             countMove++;
        //         }
                // // Enter - выбор карты для атаки
                // if (event.key.code == sf::Keyboard::Enter && attacking) {
                //     if (attack(table, Player_duraks[currentPlayer_durakIndexs].hand[selectedCard_durakIndexs])) {
                //         attackCard_duraks.push_back(Player_duraks[currentPlayer_durakIndexs].hand[selectedCard_durakIndexs]);
                //         table.push_back(Player_duraks[currentPlayer_durakIndexs].hand[selectedCard_durakIndexs]);
                //         Player_duraks[currentPlayer_durakIndexs].hand.erase(Player_duraks[currentPlayer_durakIndexs].hand.begin() + selectedCard_durakIndexs);
                //         Player_duraks[currentPlayer_durakIndexs].isAttacker = false;
                //         Player_duraks[neighborIndex].isDefender = true;
                //         attacking = false;
                //         defending = true;
                //         showCard_durakSelectionMessage = true;
                //     } else {
                //         showInvalidMove = true;
                //         showCard_durakSelectionMessage = true;
                //     }
                // // Enter - выбор карты для защиты
                // } else if (event.key.code == sf::Keyboard::Enter && defending) {
                //     if (defense(attackCard_duraks.back(), Player_duraks[currentPlayer_durakIndexs].hand[selectedCard_durakIndexs], trump)) {
                //         table.push_back(Player_duraks[currentPlayer_durakIndexs].hand[selectedCard_durakIndexs]);
                //         Player_duraks[currentPlayer_durakIndexs].hand.erase(Player_duraks[currentPlayer_durakIndexs].hand.begin() + selectedCard_durakIndexs);
                //         Player_duraks[currentPlayer_durakIndexs].isAttacker = true;
                //         Player_duraks[neighborIndex].isDefender = false;
                //         attacking = true;
                //         defending = false;
                //         showCard_durakSelectionMessage = true;
                //         attackCard_duraks.clear();
                //     } else {
                //         showInvalidMove = true;
                //         showCard_durakSelectionMessage = true;
                //     }
                // }
        //     }
        // }
        if(returnToMenu)
        {
            return 0; break;
        }
        if (EndGame) {
            //окончание игры 
            if(Player_duraks[0].hand.empty())
            {
                YouWin = true;
            }
            else
            {
                GameOver = true;
            }
            showCard_durakSelectionMessage = false;
            showMessageEscape = false;
        }
        if(move_transition)
        {
            do {
                // Передача хода следующему игроку
                currentPlayer_durakIndexs = (currentPlayer_durakIndexs + 1) % Player_duraks.size();
            } while (Player_duraks[currentPlayer_durakIndexs].hand.empty()); // Проверка, есть ли у игрока карты

            Card_durakTaken = false; // Сброс флага взятия карты

            // Обновление статуса игроков
            for (int i = 0; i < Player_duraks.size(); ++i) {
                Player_duraks[i].isAttacker = (i == currentPlayer_durakIndexs);
            }
        }
        windowss.clear(Color(50,200,150));
        windowss.draw(background);
        // Отрисовка имен игроков
        for (int i = 0; i < 4; i++) {
            if (Player_duraks[i].isAttacker) {
                Player_durakTexts[i].setFillColor(sf::Color::Yellow); // Подсветка атакующего
            } else {
                Player_durakTexts[i].setFillColor(sf::Color::White);
            }
            windowss.draw(Player_durakTexts[i]);
        }
        // Отрисовка карт игроков
        Texture back_Card_durak_texture;
        if (!back_Card_durak_texture.loadFromFile("resources/back.png")) { return 1; }
        RectangleShape back_Card_durak_sprite(Vector2f(77,110));
        back_Card_durak_sprite.setTexture(&back_Card_durak_texture);
        // Отрисовка карт для каждого игрока
        for (int Player_durakIndex = 0; Player_durakIndex < Player_duraks.size(); ++Player_durakIndex) {
            // Определение позиции и поворота карт в зависимости от индекса игрока
            float rotation;
            if (Player_durakIndex == 0) { // Игрок снизу
                rotation = 0;
            } else if (Player_durakIndex == 1) { // Игрок справа
                rotation = 90;
            } else if (Player_durakIndex == 2) { // Игрок сверху
                rotation = 0;
            } else if (Player_durakIndex == 3) { // Игрок слева
                rotation = -90;
            }
            int id = 1; // Переменная для определения позиции карты в ряду
            // Если игрок выиграл, отображаем сообщение о победе
            if (Player_duraks[Player_durakIndex].hasWon) {
                // Определение позиции для сообщения в зависимости от индекса игрока
                sf::Vector2f messagePosition;
                if (Player_durakIndex == 0) { // Игрок снизу
                    messagePosition = sf::Vector2f(900.f, 800.f);
                } else if (Player_durakIndex == 1) { // Игрок справа
                    messagePosition = sf::Vector2f(1720.f, 540.f);
                } else if (Player_durakIndex == 2) { // Игрок сверху
                    messagePosition = sf::Vector2f(900.f, 150.f);
                } else if (Player_durakIndex == 3) { // Игрок слева
                    messagePosition = sf::Vector2f(100.f, 540.f);
                }
                messege_winPlayer_durak.setPosition(messagePosition);
                windowss.draw(messege_winPlayer_durak);
                continue; // Пропускаем отрисовку карт для выигравшего игрока
            }
            // Отрисовка карт текущего игрока
            for (size_t Card_durakIndex = 0; Card_durakIndex < Player_duraks[Player_durakIndex].hand.size(); ++Card_durakIndex) {
                Card_durak& Card_durak = Player_duraks[Player_durakIndex].hand[Card_durakIndex];
                if (Player_durakIndex == 0)//Player_durak[Player_durakIndex].isAttacker
                {
                    // Атакующий игрок видит значения своих карт
                    Card_durak.sprite.setTexture(Card_durak.texture);
                    // Определение позиции карты в зависимости от её индекса 
                    Card_durak.sprite.setPosition(Vector2f(200 + (1300 / Player_duraks[Player_durakIndex].hand.size()) * id, 800));
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (std::find(selected_Card_durak.begin(), selected_Card_durak.end(), Card_durakIndex) != selected_Card_durak.end())
                    {
                        Card_durak.sprite.setColor(Color::Magenta);
                    }
                    else if (Card_durakIndex == selectedCard_durakIndexs)
                    {
                        Card_durak.sprite.setColor(Color::Yellow);
                    }
                    else
                    {
                        Card_durak.sprite.setColor(Color::White);
                    }

                    windowss.draw(Card_durak.sprite);
                    ++id; 
                    
                } else {
                    // У остальных игроков карты рубашкой вверх
                    back_Card_durak_sprite.setRotation(rotation);
                    if (Player_durakIndex == 1) {
                        back_Card_durak_sprite.setPosition(Vector2f(1820, 123 + (800 / Player_duraks[Player_durakIndex].hand.size()) * id));
                    } else if (Player_durakIndex == 2) {
                        back_Card_durak_sprite.setPosition(Vector2f(200 + (1300 / Player_duraks[Player_durakIndex].hand.size()) * id, 150));
                    } else if (Player_durakIndex == 3) {
                        back_Card_durak_sprite.setPosition(Vector2f(100, 200 + (800 / Player_duraks[Player_durakIndex].hand.size()) * id));
                    }
                    //neighborIndex
                    // Если карта выбрана, она рисуется в желтый цвет
                    if (Player_durakIndex == neighborIndex && Card_durakIndex == selectedCard_durakIndexsRight)
                    {
                        back_Card_durak_sprite.setFillColor(Color::Yellow);
                    }
                    else
                    {
                        back_Card_durak_sprite.setFillColor(Color::White);
                    }

                    windowss.draw(back_Card_durak_sprite);
                    id++;
                }
            }
        }
        // // Отрисовка карт игроков
        // Texture back_Card_durak_texture;
        // if (!back_Card_durak_texture.loadFromFile("resources/back.png")) {
        //     std::cerr << "Ошибка загрузки текстуры рубашки карт: resources/back.png" << std::endl;
        //     return -1;
        // }
        // RectangleShape back_Card_durak_sprite(Vector2f(77,110));
        // back_Card_durak_sprite.setTexture(&back_Card_durak_texture);
        // // Отрисовка карт для каждого игрока
        // for (int Player_durakIndex = 0; Player_durakIndex < Player_duraks.size(); ++Player_durakIndex) {
        //     // Определение позиции и поворота карт в зависимости от индекса игрока
        //     float rotation;
        //     if (Player_durakIndex == 0) { // Игрок снизу
        //         rotation = 0;
        //     } else if (Player_durakIndex == 1) { // Игрок справа
        //         rotation = 90;
        //     } else if (Player_durakIndex == 2) { // Игрок сверху
        //         rotation = 0;
        //     } else if (Player_durakIndex == 3) { // Игрок слева
        //         rotation = -90;
        //     }
        //     int id = 1; // Переменная для определения позиции карты в ряду
        //     // Отрисовка карт текущего игрока
        //     for (int Card_durakIndex = 0; Card_durakIndex < Player_duraks[Player_durakIndex].hand.size(); Card_durakIndex++) {
        //         Card_durak& Card_durak = Player_duraks[Player_durakIndex].hand[Card_durakIndex];
        //         if (Player_duraks[Player_durakIndex].isAttacker) {
        //             // Атакующий игрок видит значения своих карт
        //             Card_durak.sprite.setTexture(Card_durak.texture);
        //             // Определение позиции карты в зависимости от её индекса 
        //             Card_durak.sprite.setPosition(Vector2f(200 + (1300 / Player_duraks[Player_durakIndex].hand.size()) * id, 800));
        //             // Если карта выбрана, она рисуется в желтый цвет
        //             if (Card_durakIndex != selectedCard_durakIndexs)
        //             {
        //                 Card_durak.sprite.setColor(Color::Magenta);
        //             } else if (Card_durakIndex == selectedCard_durakIndexs)
        //             {
        //                 Card_durak.sprite.setColor(Color::Yellow);
        //             } else {
        //                 Card_durak.sprite.setColor(Color::White);
        //             }
        //             windowss.draw(Card_durak.sprite);
        //             ++id; 
        //         } else {
        //             back_Card_durak_sprite.setRotation(rotation);
        //             if (Player_duraks[Player_durakIndex].isDefender) {
        //                 back_Card_durak_sprite.setPosition(Vector2f(200 + (1300 / Player_duraks[Player_durakIndex].hand.size()) * id, 150));
        //             }
        //             windowss.draw(back_Card_durak_sprite);
        //             id++;
        //         }
        //     }
        }
        // Рисование сообщения о выборе карт, если флаг установлен 
        sf::Vector2u windowSize = windowss.getSize();
        if (showCard_durakSelectionMessage) {
            if (attacking) {
                message.setString(L"Select Card_durak for attack");
            } else if (defending) {
                message.setString(L"Select Card_durak for defend");
            }
            float textX = windowSize.x / 2.f - message.getLocalBounds().width / 2.f;
            float textY = (800 + 150) / 2.f - message.getLocalBounds().height / 2.f;
            message.setPosition(textX, textY);
            float Button_durakWidth = Button_durakYes.getLocalBounds().width;
            float Button_durakHeight = Button_durakYes.getLocalBounds().height;
            float Button_durakX = windowSize.x / 2.f - Button_durakWidth / 2.f; 
            float Button_durakY = (800 + 150) / 2.f - Button_durakHeight / 2.f + 50;
            Button_durakYes.setPosition(Button_durakX, Button_durakY);
            Button_durakNo.setPosition(Button_durakX + Button_durakWidth + 50, Button_durakY);
            // Выделение выбранной кнопки
            if (selectedButton_durakIndex == 0)
            {
                Button_durakYes.setFillColor(sf::Color::Red);
                Button_durakNo.setFillColor(sf::Color::White);
            } else {
                Button_durakYes.setFillColor(sf::Color::White);
                Button_durakNo.setFillColor(sf::Color::Red);
            }
            // Рисование кнопок
            windowss.draw(message);
            windowss.draw(Button_durakYes);
            windowss.draw(Button_durakNo);
        }
        // Рисование сообщения о нажатие Escape, если флаг установлен 
        float textX_message_escape = windowSize.x / 2.f - message_escape.getLocalBounds().width / 2.f;
        float textY_message_escape = (800 + 150) / 2.f - message_escape.getLocalBounds().height / 2.f;
        message_escape.setPosition(textX_message_escape, textY_message_escape);
        if (showMessageEscape) {
            windowss.draw(message_escape);
            // Выделение выбранной кнопки
            if (selectedButton_durakIndex == 0) {
                Button_durakYes.setFillColor(sf::Color::Red);
                Button_durakNo.setFillColor(sf::Color::White);
            } else {
                Button_durakYes.setFillColor(sf::Color::White);
                Button_durakNo.setFillColor(sf::Color::Red);
            }
            // Рисование кнопок
            windowss.draw(Button_durakYes);
            windowss.draw(Button_durakNo);
        }
        // Отображение текста ожидания, если флаг активен
        // if (showMessageWait) {
        //     wait_message.setString("Ожидаем других игроков: " + std::to_string(Player_durak.size() - Player_duraksChoseNo));
        //     // Рисование сообщения о выборе карт, если флаг установлен 
        //     float textX_wait_message = windowSize.x / 2.f - wait_message.getLocalBounds().width / 2.f;
        //     float textY_wait_message = (800 + 150) / 2.f - wait_message.getLocalBounds().height / 2.f;
        //     wait_message.setPosition(textX_wait_message, textY_wait_message);
        // }
        // // Рисование сообщения о неправильном выборе, если флаг установлен
        // if (showInvalidMove) {
        //     message.setString(L"Invalid move, please try again");
        //     // Рисование сообщения о выборе карт, если флаг установлен 
        //     float textX = windowSize.x / 2.f - message.getLocalBounds().width / 2.f;
        //     float textY = (800 + 150) / 2.f - message.getLocalBounds().height / 2.f;
        //     message.setPosition(textX, textY);
        //     windowss.draw(message);
        // }
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
            if (selectedButton_durakIndex_2 == 0) {
                menuButton_durak.setSelected(true);
                restartButton_durak.setSelected(false);
            } else {
                menuButton_durak.setSelected(false);
                restartButton_durak.setSelected(true);
            }
            // Отрисовка кнопок
            menuButton_durak.draw(windowss);
            restartButton_durak.draw(windowss);
        }
        windowss.display();
    }
    return 0;
}
// #include <stdio.h>
// #include <SFML/Graphics.hpp>
// #include <iostream>
// #include <vector>
// #include <ctime>
// #include <algorithm>
// #include <set>
// #include <random>
// using namespace sf;
// // Класс для представления карты
// class Card_durakss {
// public:
//     int value; // Значение карты (6-14)
//     std::string suit; // Масть карты (HEARTS, DIAMONDS, CLUBS, SPADES)
//     Texture texture; // Текстура карты
//     Sprite sprite; // Спрайт карты
// };
// // Класс для представления игрока
// class Player_duraks {
// public:
//     std::string name; // Имя игрока
//     std::vector<Card_durakss> hand; // Рука игрока
//     bool isAttacker; // Является ли игрок атакующим
//     bool isDefender; // Является ли игрок защищающимся
//     // Конструктор игрока
//     Player_duraks() : isAttacker(false), isDefender(false), name("") {}
// };
// // Функция для определения индекса текущего атакующего игрока
// int findCurrentPlayer_durakss(std::vector<Player_duraks>& Player_durakss)
// {
//     for (int i = 0; i < Player_durakss.size(); i++)
//     {
//         if (Player_durakss[i].isAttacker)
//         {
//             return i; // Возвращает индекс текущего атакующего игрока
//         }
//     }
//     return -1; // Если атакующий игрок не найден
// }
// // Функция для проверки на атаку
// bool attack(std::vector<Card_durakss>& table, Card_durakss& attackCard_durakss) {
//     if (!table.empty()) {
//         for (int i = 0; i < table.size(); i++) {
//             if (attackCard_durakss.value == table[i].value) {
//                 return true;
//             }
//         }
//     } 
//     return false;
// }
// // Функция для проверки на защиту
// bool defense(Card_durakss& attackCard_durakss, Card_durakss& defenseCard_durakss, Card_durakss& trump) {
//     if ((defenseCard_durakss.value > attackCard_durakss.value && defenseCard_durakss.suit == attackCard_durakss.suit) ||
//         (defenseCard_durakss.suit == trump.suit && attackCard_durakss.suit != trump.suit)) {
//         return true;
//     }
//     return false;
// }
// // Функция для определения, кто первый ходит
// int whoFirst (std::vector<Player_duraks>& Player_durakss, Card_durakss& trump) {
//     // Словарь для хранения минимального козыря и значения для каждого игрока
//     std::map<int, std::pair<int, int>> Player_duraksData;
//     // Проход по всем игрокам
//     for (int i = 0; i < Player_durakss.size(); i++) {
//         int minTrump = 14;  // Максимальное значение козыря
//         int minValue = 14; // Максимальное значение карты
//         for (Card_durakss& Card_durakss : Player_durakss[i].hand) {
//             if (Card_durakss.suit == trump.suit) {
//                 minTrump = std::min(minTrump, Card_durakss.value);
//             }
//             minValue = std::min(minValue, Card_durakss.value);
//         }
//         // Сохранение данных в словарь
//         Player_duraksData[i] = std::make_pair(minTrump, minValue);
//     }
//     // Определение первого игрока
//     int firstPlayer_duraksIndex = 0;
//     int minTrump = Player_duraksData[0].first;
//     int minValue = Player_duraksData[0].second;
//     for (int i = 1; i < Player_durakss.size(); i++) {
//         if (Player_duraksData[i].first < minTrump) {
//             minTrump = Player_duraksData[i].first;
//             firstPlayer_duraksIndex = i;
//         } else if (Player_duraksData[i].first == minTrump && Player_duraksData[i].second < minValue) {
//             minValue = Player_duraksData[i].second;
//             firstPlayer_duraksIndex = i;
//         }
//     }
//     return firstPlayer_duraksIndex;
// }
// // Функция, которая проверяет, закончилась ли игра
// bool isOver (std::vector<Card_durakss>& deck, std::vector<Player_duraks>& Player_durakss) {
//     int count = 0;
//     for (Player_duraks& Player_duraks: Player_durakss) {
//         if (Player_duraks.hand.empty()) {
//             count++;
//         }
//     }
//     if (deck.empty() && (count <= 3)) {
//         return true;
//     }
//     return false;
// }
// // Глобальная переменная для индекса текущего игрока
// int currentPlayer_duraksIndexs = 0;
// int main_durakgame(sf::RenderWindow& windowss) {
//     srand(time(0));
//     // Загрузка текстуры стола
//     Texture tableTexture;
//     if (!tableTexture.loadFromFile("resources/table.png")) {
//         std::cerr << "Ошибка загрузки текстуры стола: " << "resources/table.png" << std::endl;
//         return -1;
//     }
//     // Создание спрайта стола
//     RectangleShape background(Vector2f(1920,1080));
//     background.setTexture(&tableTexture);
//     background.setPosition(Vector2f(0,0));
//     // Масти карт
//     std::vector<std::string> suits = {"HEARTS", "DIAMONDS", "CLUBS", "SPADES"};
//     // Создание колоды карт 
//     std::vector<Card_durakss> deck(36);
//     sf::Texture Card_duraksssTexture;
//      for (int value = 6; value < 14; value++) {
//         for (const std::string& suit : suits) {
//             Card_durakss Card_duraksss;
//             Card_duraksss.value = value;
//             Card_duraksss.suit = suit;
//             std::string deckFilename = "resources/" + std::to_string(value) + suit + ".png";
//             if (!Card_duraksssTexture.loadFromFile(deckFilename)) { 
//                 std::cerr << "Ошибка загрузки текстуры карт: " << deckFilename << std::endl; 
//                 return -1;
//             } else {
//                 Card_duraksss.texture = Card_duraksssTexture; 
//                 Card_duraksss.sprite.setTexture(Card_duraksss.texture);
//                 deck.push_back(Card_duraksss);
//             }
//         }
//     }
//     // Отрисовка колоды
//     sf::Sprite deckSprite;
//     Texture deckTexture;
//     if (!deckTexture.loadFromFile("resources/back.png")) {
//         std::cerr << "Ошибка загрузки текстуры колоды: resources/back.png" << std::endl;
//         return -1;
//     }
//     deckSprite.setTexture(deckTexture);
//     deckSprite.setPosition(100, 400); 
//     // Перемешивание колоды
//     std::random_device rd;
//     std::mt19937 g(rd());
//     std::shuffle(deck.begin(), deck.end(), g);
//     // Выбор козыря
//     Card_durakss trump = deck.back();
//     deck.pop_back();
//     // Возврат козыря в начало колоды
//     deck.insert(deck.begin(), trump);
//     // Отрисовка козыря
//     sf::Sprite trumpSprite;
//     Texture trumpTexture;
//     std::string trumpFilename = "resources/" + std::to_string(trump.value) + trump.suit + ".png";
//     if (!trumpTexture.loadFromFile(trumpFilename)) {
//         std::cerr << "Ошибка загрузки текстуры козыря: " << trumpFilename << std::endl;
//         return -1;
//     }
//     trumpSprite.setTexture(trumpTexture);
//     trumpSprite.setPosition(100, 400); 
//     // Создание игроков
//     std::vector<Player_duraks> Player_durakss(4);
//     Player_durakss[0].name = "Vasya";
//     Player_durakss[1].name = "Masha";
//     Player_durakss[2].name = "Stepan";
//     Player_durakss[3].name = "Katya";
//     // Player_durakss[0].isAttacker = false;
//     // Player_durakss[0].isDefender = false;
//     // Player_durakss[1].isAttacker = false;
//     // Player_durakss[1].isDefender = false;
//     // Player_durakss[2].isAttacker = false;
//     // Player_durakss[2].isDefender = false;
//     // Player_durakss[3].isAttacker = false;
//     // Player_durakss[3].isDefender = false;
//     // Текст для отображения имен игроков
//     sf::Font font;
//     if (!font.loadFromFile("resources/ffont.ttf"))
//     {
//         std::cerr << "Ошибка загрузки шрифта: resources/ffont.ttf" << std::endl;
//         return -1;
//     }
//     sf::Text Player_duraksTexts[4];
//     for (size_t i = 0; i < 4; ++i) {
//         Player_duraksTexts[i].setFont(font);
//         Player_duraksTexts[i].setCharacterSize(24);
//         Player_duraksTexts[i].setFillColor(sf::Color::White);
//         Player_duraksTexts[i].setString(Player_durakss[i].name);
//     }
//     // Позиционирование имен игроков
//     Player_duraksTexts[0].setPosition(200, 700);
//     Player_duraksTexts[1].setPosition(1600, 200);
//     Player_duraksTexts[2].setPosition(200, 100);
//     Player_duraksTexts[3].setPosition(100, 600);
//     // Раздача карт
//     int Card_duraksssPerPlayer_duraks = 6;
//     for (int i = 0; i < Card_duraksssPerPlayer_duraks; i++)
//     {
//         for (int j = 0; j < Player_durakss.size(); ++j)
//         {
//             if (!deck.empty()) {
//                 Player_durakss[j].hand.push_back(deck.back());
//                 deck.pop_back();
//             }
//         }
//     }
//     // Сообщение о выходе
//     sf::Text message_escape;
//     message_escape.setFont(font);
//     message_escape.setCharacterSize(24);
//     message_escape.setFillColor(sf::Color::White);
//     message_escape.setPosition(100.f, 100.f);
//     message_escape.setString(L"Do you really want to get out?");
//     // Флаг для отображения текста выхода из игры
//     bool showMessageEscape = false;

//     // Сообщение
//     sf::Text message;
//     message.setFont(font);
//     message.setCharacterSize(30);
//     message.setFillColor(sf::Color::White);
//     message.setPosition(100.f, 100.f);
//     // Кнопка "Да"
//     sf::Text Button_durakYes;
//     Button_durakYes.setFont(font);
//     Button_durakYes.setString("Yes");
//     Button_durakYes.setCharacterSize(24);
//     Button_durakYes.setFillColor(sf::Color::White);
//     Button_durakYes.setPosition(200.f, 200.f);
//     // Кнопка "Нет"
//     sf::Text Button_durakNo;
//     Button_durakNo.setFont(font);
//     Button_durakNo.setString("No");
//     Button_durakNo.setCharacterSize(24);
//     Button_durakNo.setFillColor(sf::Color::White);
//     Button_durakNo.setPosition(400.f, 200.f);
//     // Индекс выбранной кнопки (0 - Да, 1 - Нет)
//     int selectedButton_durakIndex = 0;
//     // Флаги для управления игрой
//     bool showCard_durakssSelectionMessage = true; // Флаг для отображения текста выбора карт
//     bool showMessage = false; // Флаг для отображения сообщения
//     bool showInvalidMove = false; // Флаг для отображения сообщения о неправильном выборе карты для атаки/защиты
//     bool attacking = true; // Можно ли атаковать
//     bool defending = false; // Можно ли защищаться
//     // Game Loop
//     int countMove = 1;
//     std::vector<Card_durakss> table(36); 
//     std::vector<Card_durakss> attackCard_duraksss(36);
//     int selectedCard_durakssIndex = 0;
//     sf::Event event;
//     while (windowss.isOpen()) {
//         // Раздача карт
//         for (auto& Player_duraks : Player_durakss) {
//             if (Player_duraks.hand.size() < 6) {
//                 Player_duraks.hand.push_back(deck.back());
//                 deck.pop_back();
//             }
//         }
//         // Определение текущего игрока
//         if (countMove == 1) {
//             currentPlayer_duraksIndexs = whoFirst(Player_durakss,trump);
//         } else {
//             currentPlayer_duraksIndexs = findCurrentPlayer_durakss(Player_durakss);
//         }
//         Player_durakss[currentPlayer_duraksIndexs].isAttacker = true;
//         // Определение соседа справа
//         int neighborIndex = (currentPlayer_duraksIndexs + 1) % Player_durakss.size();
//         Player_durakss[neighborIndex].isDefender = true;
//         // Проверка на конец игры
//         if (deck.empty()) {
//             for (int i = 0; i < Player_durakss.size(); i++) {
//                 if (Player_durakss[i].hand.empty()) {
//                     std::cout << Player_durakss[i].name << " win!" << std::endl;
//                 }
//             }
//             Player_durakss.erase(std::remove_if(Player_durakss.begin(), Player_durakss.end(), [](const Player_duraks& p) { 
//                 return p.hand.empty(); 
//             }), Player_durakss.end()); 
//             if (Player_durakss.size() == 1) {
//                 std::cout << Player_durakss[0].name << " - durak!" << std::endl;
//             }
//         }
//         // Обработка событий
//         sf::Event event;
//         while (windowss.pollEvent(event)) {
            
//             // Обработка событий для текстового сообщения
//             if (showMessageEscape) {
//                 if (event.type == Event::KeyPressed) {
//                     if (event.key.code == Keyboard::Left) {
//                         selectedButton_durakIndex = 0;
//                         // Изменяем цвет выделенной кнопки
//                         Button_durakYes.setFillColor(Color::Red);
//                         Button_durakNo.setFillColor(Color::White);
//                     } else if (event.key.code == Keyboard::Right) {
//                         selectedButton_durakIndex = 1;
//                         // Изменяем цвет выделенной кнопки
//                         Button_durakYes.setFillColor(Color::White);
//                         Button_durakNo.setFillColor(Color::Red);
//                     } else if (event.key.code == Keyboard::Enter) {
//                         if (selectedButton_durakIndex == 0) {
//                             // Выходим из игры
//                             // windowss.close();
//                             return 0;  
//                             break;
//                         } else {
//                             // Скрываем текстовое сообщение
//                             showMessageEscape = false;
//                             // Сбрасываем цвет кнопок
//                             Button_durakYes.setFillColor(Color::White);
//                             Button_durakNo.setFillColor(Color::White);
//                         }
//                     }
//                 }
//             }
//             // Закрытие окна
//             if (event.type == sf::Event::Closed) windowss.close();
//             // Обработка нажатия кнопок
//             if (event.type == sf::Event::KeyPressed) {
//                 // Escape - выход
//                 if (event.key.code == sf::Keyboard::Escape) 
//                 {
//                                     // Показывает текстовое сообщение с запросом "Вы точно хотите выйти?"
//                 showMessageEscape = true;
//                 selectedButton_durakIndex = 0;
//                 }
//                 // Left - выбор карты справа налево
//                 if (event.key.code == sf::Keyboard::Left) {
//                     if (selectedCard_durakssIndex > 0) {
//                         --selectedCard_durakssIndex;
//                     } else {
//                         selectedCard_durakssIndex = Player_durakss[currentPlayer_duraksIndexs].hand.size() - 1;
//                     }
//                 }
//                 // Left - выбор карты слева направо 
//                 if (event.key.code == sf::Keyboard::Right) {
//                     if (selectedCard_durakssIndex < Player_durakss[currentPlayer_duraksIndexs].hand.size() - 1) {
//                         ++selectedCard_durakssIndex;
//                     } else {
//                         selectedCard_durakssIndex = 0;
//                     }
//                 }
//                 // P - пас
//                 if (event.key.code == sf::Keyboard::P && attacking) {
//                     Player_durakss[currentPlayer_duraksIndexs].isAttacker = false;
//                     Player_durakss[neighborIndex].isAttacker = true;
//                     Player_durakss[neighborIndex].isDefender = false;
//                     attacking = true; 
//                     defending = false;
//                     countMove++;
//                 }
//                 // T - взять карты
//                 if (event.key.code == sf::Keyboard::T && defending) {
//                     for (Card_durakss& Card_durakss : table) {
//                         Player_durakss[currentPlayer_duraksIndexs].hand.push_back(Card_durakss);
//                     }
//                     Player_durakss[currentPlayer_duraksIndexs].isDefender = false;
//                     Player_durakss[neighborIndex].isAttacker = true; 
//                     attacking = true;
//                     defending = false;
//                     table.clear();
//                     attackCard_duraksss.clear();
//                     countMove++;
//                 }
//                 // Enter - выбор карты для атаки
//                 if (event.key.code == sf::Keyboard::Enter && attacking) {
//                     if (attack(table, Player_durakss[currentPlayer_duraksIndexs].hand[selectedCard_durakssIndex])) {
//                         attackCard_duraksss.push_back(Player_durakss[currentPlayer_duraksIndexs].hand[selectedCard_durakssIndex]);
//                         table.push_back(Player_durakss[currentPlayer_duraksIndexs].hand[selectedCard_durakssIndex]);
//                         Player_durakss[currentPlayer_duraksIndexs].hand.erase(Player_durakss[currentPlayer_duraksIndexs].hand.begin() + selectedCard_durakssIndex);
//                         Player_durakss[currentPlayer_duraksIndexs].isAttacker = false;
//                         Player_durakss[neighborIndex].isDefender = true;
//                         attacking = false;
//                         defending = true;
//                         showCard_durakssSelectionMessage = true;
//                     } else {
//                         showInvalidMove = true;
//                         showCard_durakssSelectionMessage = true;
//                     }
//                 // Enter - выбор карты для защиты
//                 } else if (event.key.code == sf::Keyboard::Enter && defending) {
//                     if (defense(attackCard_duraksss.back(), Player_durakss[currentPlayer_duraksIndexs].hand[selectedCard_durakssIndex], trump)) {
//                         table.push_back(Player_durakss[currentPlayer_duraksIndexs].hand[selectedCard_durakssIndex]);
//                         Player_durakss[currentPlayer_duraksIndexs].hand.erase(Player_durakss[currentPlayer_duraksIndexs].hand.begin() + selectedCard_durakssIndex);
//                         Player_durakss[currentPlayer_duraksIndexs].isAttacker = true;
//                         Player_durakss[neighborIndex].isDefender = false;
//                         attacking = true;
//                         defending = false;
//                         showCard_durakssSelectionMessage = true;
//                         attackCard_duraksss.clear();
//                     } else {
//                         showInvalidMove = true;
//                         showCard_durakssSelectionMessage = true;
//                     }
//                 }
//             }
//         }
//         windowss.clear(Color(50,200,150));
//         windowss.draw(background);
//         // Отрисовка имен игроков
//         for (int i = 0; i < 4; i++) {
//             if (Player_durakss[i].isAttacker) {
//                 Player_duraksTexts[i].setFillColor(sf::Color::Yellow); // Подсветка атакующего
//             } else {
//                 Player_duraksTexts[i].setFillColor(sf::Color::White);
//             }
//             windowss.draw(Player_duraksTexts[i]);
//         }
//         // Отрисовка карт игроков
//         Texture back_Card_durakss_texture;
//         if (!back_Card_durakss_texture.loadFromFile("resources/back.png")) {
//             std::cerr << "Ошибка загрузки текстуры рубашки карт: resources/back.png" << std::endl;
//             return -1;
//         }
//         RectangleShape back_Card_durakss_sprite(Vector2f(77,110));
//         back_Card_durakss_sprite.setTexture(&back_Card_durakss_texture);
//         // Отрисовка карт для каждого игрока
//         for (int Player_duraksIndex = 0; Player_duraksIndex < Player_durakss.size(); ++Player_duraksIndex) {
//             // Определение позиции и поворота карт в зависимости от индекса игрока
//             float rotation;
//             if (Player_duraksIndex == 0) { // Игрок снизу
//                 rotation = 0;
//             } else if (Player_duraksIndex == 1) { // Игрок справа
//                 rotation = 90;
//             } else if (Player_duraksIndex == 2) { // Игрок сверху
//                 rotation = 0;
//             } else if (Player_duraksIndex == 3) { // Игрок слева
//                 rotation = -90;
//             }
//             int id = 1; // Переменная для определения позиции карты в ряду
//             // Отрисовка карт текущего игрока
//             for (int Card_durakssIndex = 0; Card_durakssIndex < Player_durakss[Player_duraksIndex].hand.size(); Card_durakssIndex++) {
//                 Card_durakss& Card_durakss = Player_durakss[Player_duraksIndex].hand[Card_durakssIndex];
//                 if (Player_durakss[Player_duraksIndex].isAttacker) {
//                     // Атакующий игрок видит значения своих карт
//                     Card_durakss.sprite.setTexture(Card_durakss.texture);
//                     // Определение позиции карты в зависимости от её индекса 
//                     Card_durakss.sprite.setPosition(Vector2f(200 + (1300 / Player_durakss[Player_duraksIndex].hand.size()) * id, 800));
//                     // Если карта выбрана, она рисуется в желтый цвет
//                     if (Card_durakssIndex != selectedCard_durakssIndex)
//                     {
//                         Card_durakss.sprite.setColor(Color::Magenta);
//                     } else if (Card_durakssIndex == selectedCard_durakssIndex)
//                     {
//                         Card_durakss.sprite.setColor(Color::Yellow);
//                     } else {
//                         Card_durakss.sprite.setColor(Color::White);
//                     }
//                     windowss.draw(Card_durakss.sprite);
//                     ++id; 
//                 } else {
//                     back_Card_durakss_sprite.setRotation(rotation);
//                     if (Player_durakss[Player_duraksIndex].isDefender) {
//                         back_Card_durakss_sprite.setPosition(Vector2f(200 + (1300 / Player_durakss[Player_duraksIndex].hand.size()) * id, 150));
//                     }
//                     windowss.draw(back_Card_durakss_sprite);
//                     id++;
//                 }
//             }
//         }
//         // Рисование сообщения о выборе карт, если флаг установлен 
//         sf::Vector2u windowSize = windowss.getSize();
//         if (showCard_durakssSelectionMessage) {
//             if (attacking) {
//                 message.setString(L"Select Card_durakss for attack");
//             } else if (defending) {
//                 message.setString(L"Select Card_durakss for defend");
//             }
//             float textX = windowSize.x / 2.f - message.getLocalBounds().width / 2.f;
//             float textY = (800 + 150) / 2.f - message.getLocalBounds().height / 2.f;
//             message.setPosition(textX, textY);
//             float Button_durakWidth = Button_durakYes.getLocalBounds().width;
//             float Button_durakHeight = Button_durakYes.getLocalBounds().height;
//             float Button_durakX = windowSize.x / 2.f - Button_durakWidth / 2.f; 
//             float Button_durakY = (800 + 150) / 2.f - Button_durakHeight / 2.f + 50;
//             Button_durakYes.setPosition(Button_durakX, Button_durakY);
//             Button_durakNo.setPosition(Button_durakX + Button_durakWidth + 50, Button_durakY);
//             // Выделение выбранной кнопки
//             if (selectedButton_durakIndex == 0)
//             {
//                 Button_durakYes.setFillColor(sf::Color::Red);
//                 Button_durakNo.setFillColor(sf::Color::White);
//             } else {
//                 Button_durakYes.setFillColor(sf::Color::White);
//                 Button_durakNo.setFillColor(sf::Color::Red);
//             }
//             // Рисование кнопок
//             windowss.draw(message);
//             windowss.draw(Button_durakYes);
//             windowss.draw(Button_durakNo);
//         }
//         // Рисование сообщения о нажатие Escape, если флаг установлен 
//         float textX_message_escape = windowSize.x / 2.f - message_escape.getLocalBounds().width / 2.f;
//         float textY_message_escape = (800 + 150) / 2.f - message_escape.getLocalBounds().height / 2.f;
//         message_escape.setPosition(textX_message_escape, textY_message_escape);
//         // Рисование сообщения о неправильном выборе, если флаг установлен
//         if (showInvalidMove) {
//             message.setString(L"Invalid move, please try again");
//             // Рисование сообщения о выборе карт, если флаг установлен 
//             float textX = windowSize.x / 2.f - message.getLocalBounds().width / 2.f;
//             float textY = (800 + 150) / 2.f - message.getLocalBounds().height / 2.f;
//             message.setPosition(textX, textY);
//             windowss.draw(message);
//         }
//                 // Отрисовка текстового сообщения, если оно активно
//         if (showMessageEscape) {
//             windowss.draw(message_escape);
//             windowss.draw(Button_durakYes);
//             windowss.draw(Button_durakNo);
//         }
//         windowss.display();
//     }
//     return 0;
// }