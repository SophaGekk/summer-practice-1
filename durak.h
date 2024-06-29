#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
// const int CARD_WIDTH = 71; 
// const int CARD_HEIGHT = 96; 
// const int WINDOW_WIDTH = 800; 
// const int WINDOW_HEIGHT = 600; 
// const int NUM_PLAYERS = 2;
// const int CARDS_PER_PLAYER = 36/NUM_PLAYERS;
// Класс карты
class Card {
private:
    int value; // Значение карты (6-14)
    std::string suit; // Масть карты
    sf::Texture* texture; // Текстура карты
public:
    // Конструктор по умолчанию (создание "пустой" карты)
    Card();
    // Конструктор с параметрами
    Card(int value, const std::string& suit, sf::Texture* texture);
    // Метод для отрисовки карты
    void draw(sf::RenderWindow& window, sf::Vector2f position);
    // Получение значения карты
    int getValue();
    // Получение масти карты
    const std::string& getSuit();
    // Получение текстуры карты
    sf::Texture* getTexture();
    // Определение оператора сравнения
    bool operator==(const Card& other);
};
// Класс игрока
class Player {
private:
    std::string name; // Имя игрока
    std::vector<Card> hand; // Рука игрока
    sf::Vector2f handPosition; // Позиция руки игрока на экране
public:
    // Конструктор по умолчанию
    Player();
    // Конструктор с параметрами
    Player(const std::string& name, sf::Vector2f handPosition);
    // Метод для взятия карт из колоды
    void takeCards(std::vector<Card>& deck, int numCards);
    // Метод для атаки
    Card attack(std::vector<Card>& table, sf::Vector2f mousePosition, Player& targetPlayer);
    // Метод для защиты
    Card defend(std::vector<Card>& attack_cards, sf::Vector2f mousePosition, Card trump);
    // Получение имени игрока
    const std::string& getName();
    // Получение руки игрока
    const std::vector<Card>& getHand();
    // Получение позиции руки игрока на экране
    sf::Vector2f getHandPosition();
    // Удаление карты из руки
    void removeCard(Card card);
};
// Функция для определения первого хода
int determineFirstTurn(std::vector<Player>& players, Card trump);
int main_durak(sf::RenderWindow& window);