#include <SFML/Graphics.hpp> //http://www.sfml-dev.org/tutorials/2.4
#include <vector>
#include <iostream> //For output to the terminal
#include <sstream> //For input through arguments

#define game_W        1920
#define game_H        1080
#define tex_card_W    153
#define tex_card_H    200
#define card_W        77
#define card_H        110
#define tex_cursor_W  32
#define tex_cursor_H  32
#define cursor_W      32
#define cursor_H      32
#define margin        21  //Margin used in layout
#define card_peek     5   //Fraction of the card to show peeking from under overlapped cards
#define font_size     48
#define stack_draw    4   //How many potentially unseen stack cards should be drawn

struct Card {
    uint8_t num;
    bool facedown = true;
    float rotation;

    // Оператор сравнения < для класса Card
    bool operator<(const Card& other) const {
        return num < other.num;
    }
};
class Deck {
  public:
    sf::Vector2f pos;
    bool is_stack;
    bool allow_drop;
    std::vector<Card> cards;
    uint8_t type;
    
    Deck (sf::Vector2f, bool, bool, uint8_t);
};
class Button {
public:
    Button(const std::wstring& text, const sf::Vector2f& size, const int charSize, const sf::Color& bgColor, const sf::Color& textColor, sf::Font& font_1) {
        // Установка формы кнопки
        buttonShape.setSize(size);
        buttonShape.setFillColor(bgColor);

        // Установка текста кнопки
        buttonText.setFont(font_1);
        buttonText.setString(text);
        buttonText.setCharacterSize(charSize);
        buttonText.setFillColor(textColor);

        // Центрирование текста на кнопке
        sf::FloatRect textRect = buttonText.getLocalBounds();
        buttonText.setOrigin(textRect.left + textRect.width / 2.0f,
                             textRect.top + textRect.height / 2.0f);
        buttonText.setPosition(sf::Vector2f(size.x / 2.0f, size.y / 2.0f));
    }

    // Функция для установки позиции кнопки
    void setPosition(const sf::Vector2f& pos) {
        buttonShape.setPosition(pos);
        buttonText.setPosition(pos.x + buttonShape.getSize().x / 2.0f, pos.y + buttonShape.getSize().y / 2.0f);
    }

    // Функция для отрисовки кнопки
    void draw(sf::RenderWindow& window) {
        window.draw(buttonShape);
        window.draw(buttonText);
    }

    // для состояния выделения
    bool isSelected;
    // Функция для установки состояния выделения
    void setSelected(bool select) {
        isSelected = select;
        if (isSelected) {
            buttonShape.setFillColor(sf::Color::Black); // Выделенная кнопка будет черного цвета
        } else {
            buttonShape.setFillColor(sf::Color::Transparent); // Обычный цвет кнопки
        }
    }

private:
    sf::RectangleShape buttonShape;
    sf::Text buttonText;
};
int main_solitare (sf::RenderWindow& window); //, int argc, char* argv[]);
