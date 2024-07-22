#ifndef DURAK_H
#define DURAK_H
#include <SFML/Graphics.hpp>
#include <vector>

/// @brief Класс карт
class Cardd {
public:
    int value; // Значение карты (6-14)
    std::string suit; // Масть карты (HEARTS, DIAMONDS, CLUBS, SPADES)
    sf::Texture texture; // Текстура карты
    sf::Sprite sprite; // Спрайт карты
    // Операторы сравнения
    bool operator<(const Cardd& other) const {
        return value < other.value;
    }
    bool operator==(const Cardd& other) const { 
        return value == other.value && suit == other.suit; 
    }
};

/// @brief Класс игрока
class Playerr {
public:
    std::vector<Cardd> hand; // Рука игрока
    bool isAttacker = false; // Является ли игрок атакующим
    bool isDefender = false; // Является ли игрок защищающимся
    bool hasWon = false;
    bool isBot = false;
    Playerr() : isAttacker(false), isDefender(false), hasWon(false) {} // Конструктор игрока
    virtual ~Playerr() {} // Виртуальный деструктор
};


/// @brief класс кнопки
class Buttonn {
private:
    sf::RectangleShape ButtonnShape;
    sf::Text ButtonnText;
public:
    Buttonn(const std::wstring& text, const sf::Vector2f& size, const int charSize, const sf::Color& bgColor, const sf::Color& textColor, sf::Font& font_1) {
        // Установка формы кнопки
        ButtonnShape.setSize(size);
        ButtonnShape.setFillColor(bgColor);
        // Установка текста кнопки
        ButtonnText.setFont(font_1);
        ButtonnText.setString(text);
        ButtonnText.setCharacterSize(charSize);
        ButtonnText.setFillColor(textColor);
        // Центрирование текста на кнопке
        sf::FloatRect textRect = ButtonnText.getLocalBounds();
        ButtonnText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        ButtonnText.setPosition(sf::Vector2f(size.x / 2.0f, size.y / 2.0f));
    }
    /// @brief Метод для установки позиции кнопки
    /// @param pos позиция кнопки
    void setPosition(const sf::Vector2f& pos) {
        ButtonnShape.setPosition(pos);
        ButtonnText.setPosition(pos.x + ButtonnShape.getSize().x / 2.0f, pos.y + ButtonnShape.getSize().y / 2.0f);
    }
    /// @brief Метод для отрисовки кнопки
    /// @param window окно игры
    void draw(sf::RenderWindow& window) {
        window.draw(ButtonnShape);
        window.draw(ButtonnText);
    }
    bool isSelected; // Состояние выделения
    /// @brief Метод для установки состояния выделения
    /// @param select выделена ли кнопка
    void setSelected(bool select) {
        isSelected = select;
        if (isSelected) {
            ButtonnShape.setFillColor(sf::Color::Black); // Выделенная кнопка будет черного цвета
        } else {
            ButtonnShape.setFillColor(sf::Color::Transparent); // Обычный цвет кнопки
        }
    }
};

void drawFor1People(std::vector<Playerr>& players, int& CurrentPlayerrIndex, int& selectedCardiIndex, int& neighborIndex, sf::RenderWindow& windowss, sf::Text& messageWinner, sf::RectangleShape& back_Cardi_sprite, bool attacking, bool defending, sf::Font& font_1);
void drawFor2People(std::vector<Playerr>& players, int& CurrentPlayerrIndex,int& selectedCardiIndex, int& neighborIndex, sf::RenderWindow& windowss, sf::Text& messageWinner, sf::RectangleShape& back_Cardi_sprite, bool attacking, bool defending, sf::Font& font_1);
void drawFor3People(std::vector<Playerr>& players, int& CurrentPlayerrIndex,int& selectedCardiIndex, int& neighborIndex, sf::RenderWindow& windowss, sf::Text& messageWinner, sf::RectangleShape& back_Cardi_sprite, bool attacking, bool defending, sf::Font& font_1);
void drawFor4People (std::vector<Playerr>& players, int& CurrentPlayerrIndex,int& selectedCardiIndex, int& neighborIndex, sf::RenderWindow& windowss, sf::Text& messageWinner, sf::RectangleShape& back_Cardi_sprite, bool attacking, bool defending, sf::Font& font_1);
int main_durakgame(sf::RenderWindow& windowss, int number_of_players_people, int PlayerChose_P);

#endif 