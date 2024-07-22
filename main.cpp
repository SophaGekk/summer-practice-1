#include<Windows.h>
#include "gameMenu.h"
#include <SFML/Graphics.hpp> 
#include <SFML/Audio.hpp> 
#include "Animator.h"

#include "menupik.h"
#include "menuhearts.h"
#include "menudurak.h"
#include "menusolitaire.h"

using namespace sf;

void InitTexts(Text& mtext, float xpos, float ypos, String str, int size_font=60,Color menu_text_color=Color::White, int bord=0, Color border_color = Color::Black )
{
    mtext.setCharacterSize(size_font);
    mtext.setString(str);
    mtext.setFillColor(menu_text_color);
    mtext.setOutlineThickness(bord);
    mtext.setOutlineColor(border_color);

    // Получите размер окна
    float windowWidth =1920;

    // Вычислите центральную позицию по горизонтали 
    float textWidth = mtext.getLocalBounds().width;
    xpos = (windowWidth - textWidth) / 2.0f;

    mtext.setPosition(xpos, ypos);

}

// Об Игре
int About_Game(sf::RenderWindow& About)
{
    RectangleShape background_ab(Vector2f(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height));
    Texture texture_ab;
    if (!texture_ab.loadFromFile("resources/management.png")) exit(3);
    background_ab.setTexture(&texture_ab);

    // Шрифт для названия экрана
    Font font;
    if (!font.loadFromFile("resources/troika.otf")) exit(6);
    // Текст с названием экрана
    Text Titul;
    Titul.setFont(font);
    Init_Text_D(Titul, 500, 50, L"Управление в играх", 120, Color(237, 147, 0), 3);

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
        About.display();
    }
    return 0;
}
int main() 
{
    // Инициализация окна SFML
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(),  L"Карточные игры", Style::Fullscreen);
    
    // Делаем окно windows прозрачным
    SetWindowLong(window.getSystemHandle(), GWL_EXSTYLE, GetWindowLong(window.getSystemHandle(), GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(window.getSystemHandle(), 0, 0, LWA_COLORKEY);

    window.setMouseCursorVisible(false); //отключаем видимость курсора

    // Размер экрана
    float widht = sf::VideoMode::getDesktopMode().width;
    float height = sf::VideoMode::getDesktopMode().height;

    //Заставка загрузки
    Texture texthome;
    texthome.loadFromFile("resources/window.png");
    RectangleShape homecls(Vector2f(640, 280));
    homecls.setTexture(&texthome);
    homecls.setPosition(Vector2f(widht / 2 - homecls.getLocalBounds().width/2, height / 2- homecls.getLocalBounds().height/2));
    window.draw(homecls);
    window.display();

    // Звуковые эффекты
    SoundBuffer buffer, buf_return;
    if (!buffer.loadFromFile("resources/audiomenu2.wav")) return 22;
    if (!buf_return.loadFromFile("resources/audiomenu5.wav")) return 22;
    Sound sound, sound_return;
    sound.setBuffer(buffer);
    sound_return.setBuffer(buf_return);

    Music music;
    if (!music.openFromFile("resources/bat.ogg")) return 25;
    music.setLoop(true);
    music.setVolume(50);
    music.play();
    
    //звук костра
    Music musicF;
    if (!musicF.openFromFile("resources/faer.ogg")) return 28;
    musicF.setLoop(true);
    musicF.setVolume(20);
    musicF.play();

    //Название пунктов меню
    std::vector<String> name_menu{L"Пиковая Дама",L"Дурак",L"Пасьянс",L"Черви", L"Управление", L"Выход"};

    //Объект меню
    game::GameMenu mymenu(window, 950, 325, 100, 120, name_menu);
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
    if (!texture_window.loadFromFile("resources/window.png")) {
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
    InitTexts(Titul, 480,48, L"Карточные игры", 150, Color(237,147,0), 3);

    // Анимация костра
    Vector2i spriteSize(300, 313);

    Sprite sprite;
    sprite.setPosition(440, 780);
    Animator animator(sprite);
    Clock clock;
    SetLayeredWindowAttributes(window.getSystemHandle(), 100, 0, LWA_COLORKEY);

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
                    // music.pause(); musicF.pause();
                    sound_return.play();
                        // Переходим на выбранный пункт меню
                        switch (mymenu.getSelectedMenuNumber())
                        {
                            case 0:Menu_pik_Dam(window);  break; //пиковая дама
                            case 1:Menu_Durak(window);     break; //дурак
                            case 2:Menu_solitaire(window);  break; //пасьянс
                            case 3:Menu_Hearts(window);  break; //черви
                            case 4:About_Game(window);break;//управление
                            case 5:window.close(); break; //выход
                        }
                    // music.play(); musicF.play();
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