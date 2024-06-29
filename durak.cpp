#include <C:/Users/timoh/Desktop/SFML-2.6.1/include/SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
const int CARD_WIDTH = 71; 
const int CARD_HEIGHT = 96; 
const int WINDOW_WIDTH = 800; 
const int WINDOW_HEIGHT = 600; 
const int NUM_PLAYERS = 2;
const int CARDS_PER_PLAYER = 36/NUM_PLAYERS;
// Вектор для мастей карт
std::vector<std::string> suits = {"HEARTS", "SPADES", "CLUBS", "DIAMONDS"};
// Класс карты
class Card {
private:
    int value; // Значение карты (6-14)
    std::string suit; // Масть карты
    sf::Texture* texture; // Текстура карты
public:
    // Конструктор по умолчанию (создание "пустой" карты)
    Card() : value(0), suit(""), texture(nullptr) {}
    // Конструктор с параметрами
    Card(int value, const std::string& suit, sf::Texture* texture) : value(value), suit(suit), texture(texture) {}
    // Метод для отрисовки карты
    void draw(sf::RenderWindow& window, sf::Vector2f position) {
        sf::Sprite sprite;
        sprite.setTexture(*texture);
        sprite.setPosition(position);
        window.draw(sprite);
    }
    // Получение значения карты
    int getValue() const { return value; }
    // Получение масти карты
    const std::string& getSuit() const { return suit; }
    // Получение текстуры карты
    sf::Texture* getTexture() const { return texture; }
    // Определение оператора сравнения
    bool operator==(const Card& other) const {
        return value == other.value && suit == other.suit;
    }
};
// Класс игрока
class Player {
private:
    std::string name; // Имя игрока
    std::vector<Card> hand; // Рука игрока
    sf::Vector2f handPosition; // Позиция руки игрока на экране
public:
    // Конструктор по умолчанию
    Player() : name(""), hand(), handPosition(0, 0) {}
    // Конструктор с параметрами
    Player(const std::string& name, sf::Vector2f handPosition) : name(name), hand(), handPosition(handPosition) {}
    // Метод для взятия карт из колоды
    void takeCards(std::vector<Card>& deck, int numCards) {
        for (int i = 0; i < numCards; ++i) {
            if (!deck.empty()) {
                hand.push_back(deck.back());
                deck.pop_back();
            }
        }
    }
    // Метод для атаки
    Card attack(std::vector<Card>& table, sf::Vector2f mousePosition, Player& targetPlayer) {
        // Проверка на возможность атаки
        if (!hand.empty()) {
            // Проверка, была ли нажата кнопка "пас" (предполагается, что кнопка находится в определенной позиции на экране)
            if (mousePosition.x >= 500 && mousePosition.x <= 600 && mousePosition.y >= 500 && mousePosition.y <= 550) {
                std::cout << "Пас" << std::endl;
                return Card(); // Возвращает пустую карту (пас)
            }
            // Если стол пустой, можно кинуть любую карту
            if (table.empty()) {
                for (int i = 0; i < hand.size(); ++i) {
                    // Проверка, была ли нажата карта
                    if (hand[i].getTexture() && mousePosition.x >= hand[i].getTexture()->getSize().x * i &&
                        mousePosition.x < hand[i].getTexture()->getSize().x * (i + 1) &&
                        mousePosition.y >= 0 && mousePosition.y < hand[i].getTexture()->getSize().y) {
                        // Возвращение выбранной карты
                        return hand[i];
                    }
                }
            }
            // Если на столе есть карты, нужно кинуть карту того же значения
            else {
                for (int i = 0; i < hand.size(); ++i) {
                    // Проверка, была ли нажата карта
                    if (hand[i].getTexture() && mousePosition.x >= hand[i].getTexture()->getSize().x * i &&
                        mousePosition.x < hand[i].getTexture()->getSize().x * (i + 1) &&
                        mousePosition.y >= 0 && mousePosition.y < hand[i].getTexture()->getSize().y) {
                        // Проверка, есть ли на столе карта с таким же значением
                        for (int j = 0; j < table.size(); ++j) {
                            if (hand[i].getValue() == table[j].getValue()) {
                                // Возвращение выбранной карты
                                return hand[i];
                            }
                        }
                    }
                }
            }
        } 
        else {
            // Невозможно атаковать (нет карт в руке)
            std::cout << "У вас закончились карты." << std::endl;
            return Card(); // Возвращает пустую карту
        }
        // Карта не была выбрана
        std::cout << "Не выбрана карта. Попробуйте еще раз." << std::endl;
        return Card(); // Возвращает пустую карту
    }
    // Метод для защиты
    Card defend(std::vector<Card>& attack_cards, sf::Vector2f mousePosition, Card trump) {
        // Проверка, была ли нажата кнопка "Забрать карты" (предполагается, что кнопка находится в определенной позиции на экране)
        if (mousePosition.x >= 500 && mousePosition.x <= 600 && mousePosition.y >= 500 && mousePosition.y <= 550) {
            std::cout << "Забрать карты" << std::endl;
            return Card(); // Возвращает пустую карту (забрать карты)
        }
        // Проверка, была ли нажата карта в руке
        if (!hand.empty()) {
            for (int i = 0; i < hand.size(); ++i) {
                if (hand[i].getTexture() && mousePosition.x >= hand[i].getTexture()->getSize().x * i &&
                    mousePosition.x < hand[i].getTexture()->getSize().x * (i + 1) &&
                    mousePosition.y >= 0 && mousePosition.y < hand[i].getTexture()->getSize().y) {
                    // Сохраняем выбранную карту из руки
                    Card selectedCardFromHand = hand[i];
                    // Проверка, была ли нажата карта в attack_cards
                    if (!attack_cards.empty()) {
                        for (int j = 0; j < attack_cards.size(); ++j) {
                            if (attack_cards[j].getTexture() && mousePosition.x >= attack_cards[j].getTexture()->getSize().x * j &&
                                mousePosition.x < attack_cards[j].getTexture()->getSize().x * (j + 1) &&
                                mousePosition.y >= 0 && mousePosition.y < attack_cards[j].getTexture()->getSize().y) {
                                // Проверка, можно ли защищаться (сравнение значений)
                                if (((selectedCardFromHand.getValue() > attack_cards[j].getValue()) && (selectedCardFromHand.getSuit()==attack_cards[j].getSuit())) ||
                                    ((selectedCardFromHand.getSuit()==trump.getSuit()) && attack_cards[j].getSuit()!=trump.getSuit()) ||
                                    ((selectedCardFromHand.getSuit()==trump.getSuit()) && (attack_cards[j].getSuit()==trump.getSuit()) && (selectedCardFromHand.getValue() > attack_cards[j].getValue()))) { 
                                    // Возвращение выбранной карты
                                    return selectedCardFromHand;
                                } 
                                else {
                                    // Невозможно защищаться этой картой
                                    std::cout << "Невозможно защищаться этой картой. Попробуйте еще раз." << std::endl;
                                    return Card(); // Возвращает пустую карту
                                }
                            }
                        }
                    }
                }
            }
        }
        // Карта не была выбрана
        std::cout << "Не выбрана карта. Попробуйте еще раз." << std::endl;
        return Card(); // Возвращает пустую карту
    }
    // Получение имени игрока
    const std::string& getName() const { return name; }
    // Получение руки игрока
    const std::vector<Card>& getHand() const { return hand; }
    // Получение позиции руки игрока на экране
    sf::Vector2f getHandPosition() const { return handPosition; }
    // Удаление карты из руки
    void removeCard(Card card) {
        hand.erase(std::remove(hand.begin(), hand.end(), card), hand.end());
    }
};
// Функция для определения первого хода
int determineFirstTurn(std::vector<Player>& players, Card trump) {
    // 1) Если хотя бы у одного игрока есть козырная карта в руках
    bool hasTrump = false;
    for (int i = 0; i < players.size(); ++i) {
        for (const Card& card : players[i].getHand()) {
            if (card.getSuit() == trump.getSuit()) {
                hasTrump = true;
                break;
            }
        }
        if (hasTrump) break;
    }
    if (hasTrump) {
        // Найдем игрока с наименьшим значением козыря
        int minTrumpValue = 15; // Инициализируем значение больше максимально возможного
        int minTrumpPlayerIndex = -1;
        for (int i = 0; i < players.size(); ++i) {
            for (const Card& card : players[i].getHand()) {
                if (card.getSuit() == trump.getSuit() && card.getValue() < minTrumpValue) {
                    minTrumpValue = card.getValue();
                    minTrumpPlayerIndex = i;
                }
            }
        }
        return minTrumpPlayerIndex;
    } 
    else {
        // 2) Если козырной карты нет ни у одного игрока в руках
        // Найдем игрока с наименьшим значением карты
        int minValue = 15; // Инициализируем значение больше максимально возможного
        std::vector<int> minPlayersIndices;
        for (int i = 0; i < players.size(); ++i) {
            for (const Card& card : players[i].getHand()) {
                if (card.getValue() < minValue) {
                    minValue = card.getValue();
                    minPlayersIndices.clear(); // Очищаем вектор, так как нашли новое минимальное значение
                    minPlayersIndices.push_back(i);
                } 
                else if (card.getValue() == minValue) {
                    minPlayersIndices.push_back(i);
                }
            }
        }
        // Если игроков с минимальным значением несколько, выбираем случайного
        if (minPlayersIndices.size() > 1) {
            std::random_device rd;
            std::mt19937 g(rd());
            std::uniform_int_distribution<> distrib(0, minPlayersIndices.size() - 1);
            return minPlayersIndices[distrib(g)];
        } 
        else {
            return minPlayersIndices[0];
        }
    }
}
int main() {
    // Инициализация SFML
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Подкидной Дурак");
    // Создание игроков
    Player player1("Игрок 1", sf::Vector2f(50, WINDOW_HEIGHT - CARD_HEIGHT));
    Player player2("Игрок 2", sf::Vector2f(WINDOW_WIDTH - CARD_WIDTH * 6 - 50, WINDOW_HEIGHT - CARD_HEIGHT));
    // Добавление игроков в вектор
    std::vector<Player> players;
    players.push_back(player1);
    players.push_back(player2);
    // Создание колоды карт
    std::vector<Card> deck;
    for (int value = 6; value <= 14; ++value) {
        for (const std::string& suit : suits) { 
            // Загрузка текстуры карты
            sf::Texture cardTexture;
            if (cardTexture.loadFromFile("C:/Users/timoh/Desktop/durak_2.0/resources/" + std::to_string(value) + suit + ".png")) {
                deck.push_back(Card(value, suit, &cardTexture));
            }
        }
    }
    // Перемешивание колоды
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
    // Раздача карт
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        players[i].takeCards(deck, CARDS_PER_PLAYER);
    }
    // Выбор козыря
    Card trump = deck.back();
    deck.pop_back();
    // Возврат козыря в начало колоды
    deck.insert(deck.begin(), trump);
    // Кнопки "пас" и "забрать карты"
    sf::RectangleShape passButton(sf::Vector2f(100, 50));
    passButton.setFillColor(sf::Color(0, 128, 0));
    passButton.setPosition(500, 500);
    sf::Text passButtonText("Пас", sf::Font(), 20);
    passButtonText.setFillColor(sf::Color::White);
    passButtonText.setPosition(520, 515);
    sf::RectangleShape takeButton(sf::Vector2f(100, 50));
    takeButton.setFillColor(sf::Color(0, 128, 0));
    takeButton.setPosition(500, 500);
    sf::Text takeButtonText("Забрать", sf::Font(), 20);
    takeButtonText.setFillColor(sf::Color::White);
    takeButtonText.setPosition(510, 515);
    // Основной цикл игры
    int currentPlayerIndex = determineFirstTurn(players, trump); // Определяем первого игрока
    std::vector<Card> table; // Стол
    std::vector<Card> attack_cards; // Карты атакующих
    std::vector<Card> defend_cards; // Карты защищающегося
    bool isAttacking = true; // Флаг, определяющий, атакует игрок или защищается
    bool isGameOver = false;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePosition = (sf::Vector2f)sf::Mouse::getPosition(window);
                if (isGameOver) { // Обработка клика после окончания игры
                    if (mousePosition.x >= 500 && mousePosition.x <= 600 && mousePosition.y >= 500 && mousePosition.y <= 550) {
                        // Начать новую игру
                        isGameOver = false;
                        // Перезапускаем игру
                        // ... (Инициализируем переменные игры, как в начале программы)
                    }
                } 
                else { // Обработка клика во время игры
                    if (isAttacking) { // Атака
                        // Card selectedCard = players[currentPlayerIndex].attack(table, mousePosition, players[(currentPlayerIndex + 1) % NUM_PLAYERS], trump);
                        Card selectedCard = players[currentPlayerIndex].attack(table, mousePosition, players[(currentPlayerIndex + 1) % NUM_PLAYERS]);
                        if (selectedCard.getTexture()) {
                            attack_cards.push_back(selectedCard);
                            players[currentPlayerIndex].removeCard(selectedCard);
                            // Смена хода
                            currentPlayerIndex = (currentPlayerIndex + 1) % NUM_PLAYERS;
                        }
                    } 
                    else { // Защита
                        Card selectedCard = players[(currentPlayerIndex + 1) % NUM_PLAYERS].defend(attack_cards, mousePosition, trump);
                        if (selectedCard.getTexture()) {
                            defend_cards.push_back(selectedCard);
                            players[(currentPlayerIndex + 1) % NUM_PLAYERS].removeCard(selectedCard);
                        }
                        if (attack_cards.empty()) { // Атака завершена
                            // Смена хода
                            currentPlayerIndex = (currentPlayerIndex + 1) % NUM_PLAYERS;
                            // Переключаемся на атаку
                            isAttacking = true;
                        }
                    }
                }
            }
        }
        // Логика игры
        if (!isGameOver) {
            if (deck.empty() && (players[0].getHand().empty() || players[1].getHand().empty())) {
                isGameOver = true;
                std::cout << "Игра окончена!" << std::endl;
                if (players[0].getHand().empty()) {
                    std::cout << "Победил " << players[0].getName() << std::endl;
                } else {
                    std::cout << "Победил " << players[1].getName() << std::endl;
                }
                passButtonText.setString("Новая игра");
                passButtonText.setPosition(500, 515);
            }
            if (isAttacking) {
                if (currentPlayerIndex == 0 && table.empty()) {
                    // Первый ход - игрок должен скинуть карту
                    passButtonText.setString("Пас");
                    passButtonText.setPosition(520, 515);
                } else {
                    // Можно пасовать или скидывать карту
                    passButtonText.setString("Пас");
                    passButtonText.setPosition(520, 515);
                }
            } 
            else {
                if (!attack_cards.empty()) {
                    // Можно защищаться или забрать карты
                    takeButtonText.setString("Забрать");
                    takeButtonText.setPosition(510, 515);
                    takeButton.setPosition(500, 500);
                } 
                else {
                    // Можно пасовать или скидывать карту
                    passButtonText.setString("Пас");
                    passButtonText.setPosition(520, 515);
                }
            }
        }
        // Отрисовка
        window.clear(sf::Color::White);
        // Отрисовка колоды
        if (!deck.empty()) {
            deck.back().draw(window, sf::Vector2f(WINDOW_WIDTH / 2 - CARD_WIDTH / 2, WINDOW_HEIGHT / 2 - CARD_HEIGHT / 2));
        }
        // Отрисовка козыря
        trump.draw(window, sf::Vector2f(WINDOW_WIDTH / 2 - CARD_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - CARD_HEIGHT / 2));

        // Отрисовка стола
        for (int i = 0; i < table.size(); ++i) {
            table[i].draw(window, sf::Vector2f(WINDOW_WIDTH / 2 - CARD_WIDTH * (table.size() / 2 + 0.5) + CARD_WIDTH * i, WINDOW_HEIGHT / 2 - CARD_HEIGHT / 2));
        }
        // ?????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
        // // Отрисовка карт в руке каждого игрока
        // for (int i = 0; i < NUM_PLAYERS; ++i) {
        //     for (int j = 0; j < players[i].getHand().size(); ++j) {
        //         players[i].getHand()[j].draw(window, players[i].getHandPosition() + sf::Vector2f(CARD_WIDTH * j, 0));
        //     }
        // }
        // ??????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
        // Отрисовка кнопок "пас" и "забрать карты"
        if (isAttacking) {
            window.draw(passButton);
            window.draw(passButtonText);
        } else {
            window.draw(takeButton);
            window.draw(takeButtonText);
        }
        window.display();
        // Проверка условий конца хода
        if (isAttacking && !attack_cards.empty()) { // Если атакующий сбросил карту, то переключаемся на защиту
            isAttacking = false;
            table = attack_cards;
            attack_cards.clear();
            // Проверка, была ли карта сброшена в атаку
            if (table.empty()) {
                isAttacking = true;
                currentPlayerIndex = (currentPlayerIndex + 1) % NUM_PLAYERS;
            }
        } else if (!isAttacking && !defend_cards.empty()) { // Если защищающийся сбросил карту, то переключаемся на атаку
            isAttacking = true;
            table.clear(); // Очищаем стол
            defend_cards.clear(); // Очищаем карты защищающегося
            // Проверка, была ли карта сброшена в защиту
            if (attack_cards.empty()) {
                isAttacking = true;
                currentPlayerIndex = (currentPlayerIndex + 1) % NUM_PLAYERS;
            }
        }
        // Дополнение карт игрокам
        for (int i = 0; i < NUM_PLAYERS; ++i) {
            if (players[i].getHand().size() < CARDS_PER_PLAYER) {
                players[i].takeCards(deck, CARDS_PER_PLAYER - players[i].getHand().size());
            }
        }
        // // Пауза между ходами
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}