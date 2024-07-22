#include<Windows.h>
#include "gameMenu.h"
#include <SFML/Graphics.hpp> 
#include <SFML/Audio.hpp> 
#include "Animator.h"
#include "solitar.h"
using namespace sf;
/// @brief Инициализирует текст с заданными параметрами.
/// @param mtext Объект текста, который нужно инициализировать.
/// @param xpos Позиция текста по оси X.
/// @param ypos Позиция текста по оси Y.
/// @param str Текст, который нужно отобразить.
/// @param size_font Размер шрифта.
/// @param menu_text_color Цвет текста.
/// @param bord Толщина границы.
/// @param border_color Цвет границы.
void Init_Textix(Text& mtext, float xpos, float ypos, String str, int size_font=60,Color menu_text_color=Color::White, int bord=0, Color border_color = Color::Black )
{
    mtext.setCharacterSize(size_font);
    mtext.setPosition(xpos, ypos);
    mtext.setString(str);
    mtext.setFillColor(menu_text_color);
    mtext.setOutlineThickness(bord);
    mtext.setOutlineColor(border_color);

}
/// @brief Об Игре
/// @param About игровое окно
int About_Game_solitaire(sf::RenderWindow& About)
{
    RectangleShape background_ab(Vector2f(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height));
    Texture texture_ab;
    if (!texture_ab.loadFromFile("resources/rules_solit.png")) exit(3);
    background_ab.setTexture(&texture_ab);

    // Шрифт для названия экрана
    Font font;
    if (!font.loadFromFile("resources/troika.otf")) exit(6);
    // Текст с названием экрана
    Text Titul;
    Titul.setFont(font);
    Init_Textix(Titul, 500, 50, L"Описание игры", 120, Color(237, 147, 0), 3);

    while (About.isOpen())
    {
        Event event_play;
        while (About.pollEvent(event_play))
        {
            if (event_play.type == Event::Closed) return 0;
            if (event_play.type == Event::KeyPressed)
            {
                if (event_play.key.code == Keyboard::Escape) return 0;
            }
        }
        About.clear();
        About.draw(background_ab);
        // About.draw(Titul);
        About.display();
    }
    return 0;
}

int Menu_solitaire(sf::RenderWindow& window) 
{
    setlocale(LC_ALL, "Russian");
    window.setMouseCursorVisible(false); //отключаем видимость курсора

    // Размер экрана
    float widht = sf::VideoMode::getDesktopMode().width;
    float height = sf::VideoMode::getDesktopMode().height;
    // Звуковые эффекты
    SoundBuffer buffer;
    SoundBuffer buf_return;

    if (!buffer.loadFromFile("resources/audiomenu2.wav")) return 22;
    if (!buf_return.loadFromFile("resources/audiomenu5.wav")) return 22;
    Sound sound, sound_return;
    sound.setBuffer(buffer);
    sound_return.setBuffer(buf_return);

    //Название пунктов меню
    std::vector<String> name_menu{L"Играть",L"Правила игры",L"Вернуться в меню"};

    //Объект меню
    game::GameMenu mymenu(window, 950, 370, 100, 170, name_menu);
    // Установка цвета отображения меню
    mymenu.setColorTextMenu(Color(237, 147, 0), Color::Red, Color::Black);
    mymenu.AlignMenu(2);

    // Плавное появление из черного 
    sf::RectangleShape backgroundBlack(sf::Vector2f(widht, height)); // Заменяем Sprite на RectangleShape
    backgroundBlack.setFillColor(sf::Color(0, 0, 0, 255)); // Начальный цвет - черный
    float alpha = 0; // Начальная прозрачность

    // Устанавливаем фон экрана меню
    RectangleShape background(Vector2f(widht, height));

    sf::Texture texture_window;
    if (!texture_window.loadFromFile("resources/pik_dam.png")) {
        std::cerr << "Ошибка загрузки текстуры окна." << std::endl;
        return 4;
    }
    background.setTexture(&texture_window);
    
    //Шрифт
    Font font;
    if (!font.loadFromFile("resources/troika.otf")) return 5;
    // Текст
    Text Titul;
    Titul.setFont(font);
    Init_Textix(Titul, 680, 50, L"Пасьянс", 150, Color(237,147,0), 3);

    // Анимация костра
    Vector2i spriteSize(300, 313);

    Sprite sprite;
    sprite.setPosition(440, 780);
    Animator animator(sprite);

    // auto& idleAnimation = animator.CreateAnimation("Idle", "C:/Users/Sopha/Desktop/proj/image/f.png", seconds(1), true);

    // idleAnimation.AddFrames(Vector2i(0, 0), spriteSize, 5, 4);


    Clock clock;
    SetLayeredWindowAttributes(window.getSystemHandle(), 100, 0, LWA_COLORKEY);
    bool gameJustEnded = false;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyReleased)
            {
                // События выбра пунктов меню
                if (event.key.code == Keyboard::Up) {mymenu.MoveUp(); }       // вверх
                if (event.key.code == Keyboard::Down) {mymenu.MoveDown(); }  // вниз
                if (event.key.code == Keyboard::Return)                                     // выбор
                {
                    if (mymenu.getSelectedMenuNumber() == 0 && !gameJustEnded)
                    {
                        main_solitare(window);
                        gameJustEnded = true; //////////////
                    }
                    else
                    {
                        gameJustEnded = false; // Сброс флага для следующего запуска игры
                        // Обработка других пунктов меню
                    }
                    sound_return.play();
                        // Переходим на выбранный пункт меню
                        switch (mymenu.getSelectedMenuNumber())
                        {
                            case 1: About_Game_solitaire(window);  break;
                            case 2:return 0; break;
                        }
                }
            }
        }
    
        // Обновление анимации
        Time deltaTime = clock.restart();
        animator.Update(deltaTime);

        // Увеличиваем прозрачность
        alpha += 0.5f; // Скорость появления
        if (alpha > 255) alpha = 255; // Предел прозрачности

        backgroundBlack.setFillColor(sf::Color(0, 0, 0, 255 - alpha)); // Устанавливаем прозрачность
        window.draw(backgroundBlack); // Рисуем прямоугольник

        // Область отрисовки объектов      
        window.clear();
        window.draw(background);
        window.draw(Titul);
        mymenu.draw();
        window.draw(sprite);
        window.draw(backgroundBlack);
        window.display();
    }
    return 0;
}