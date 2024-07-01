#include<Windows.h>
#include "gameMenu.h"
#include <SFML/Graphics.hpp> 
#include <SFML/Audio.hpp> 
#include "Animator.h"
#include "menupik.h"
#include "pikdam.h"

using namespace sf;
void Init_Text(Text& mtext, float xpos, float ypos, String str, int size_font=60,Color menu_text_color=Color::White, int bord=0, Color border_color = Color::Black )
{
    mtext.setCharacterSize(size_font);
    mtext.setPosition(xpos, ypos);
    mtext.setString(str);
    mtext.setFillColor(menu_text_color);
    mtext.setOutlineThickness(bord);
    mtext.setOutlineColor(border_color);

}

int Game_Start(sf::RenderWindow& Play)
{
    // RenderWindow Play(VideoMode::getDesktopMode(), L"Уровень", Style::Fullscreen);
    

    RectangleShape background_play(Vector2f(1920, 1080));
    Play.setMouseCursorVisible(true); //включаем видимость курсора
    Texture texture_play;
    if (!texture_play.loadFromFile("resources/table.png")) exit(1);
    background_play.setTexture(&texture_play);

    while (Play.isOpen())
    {
        Event event_play;
        while (Play.pollEvent(event_play))
        {
            if (event_play.type == Event::KeyPressed)
            {
                main_pikgame(Play);
                 return 0; 
            }
        }
        Play.clear();
        Play.draw(background_play);
        Play.display();
    }
    return 0;
}

//Настройки
int Optionis(sf::RenderWindow& Optionis)
{
    // RenderWindow Optionis(VideoMode::getDesktopMode(), L"Настройки", Style::Fullscreen);

    RectangleShape background_opt(Vector2f(1920, 1080));
    Texture texture_opt;
    if (!texture_opt.loadFromFile("resources/table.png")) exit(2);

    background_opt.setTexture(&texture_opt);
    while (Optionis.isOpen())
    {
        Event event_opt;
        while (Optionis.pollEvent(event_opt))
        {
            if (event_opt.type == Event::Closed) return 0;
            if (event_opt.type == Event::KeyPressed)
            {
                if (event_opt.key.code == Keyboard::Escape) return 0;
            }
        }
        Optionis.clear();
        Optionis.draw(background_opt);
        Optionis.display();
    }
    return 0;
}

// Об Игре
int About_Game_Pik(sf::RenderWindow& About)
{
    // RenderWindow About(VideoMode::getDesktopMode(), L"Правила игры", Style::Fullscreen);

    RectangleShape background_ab(Vector2f(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height));
    Texture texture_ab;
    if (!texture_ab.loadFromFile("resources/rules_pik_dam.png")) exit(3);
    background_ab.setTexture(&texture_ab);

    // Шрифт для названия экрана
    Font font;
    if (!font.loadFromFile("resources/troika.otf")) exit(6);
    // Текст с названием экрана
    Text Titul;
    Titul.setFont(font);
    Init_Text(Titul, 500, 50, L"Описание игры", 120, Color(237, 147, 0), 3);

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

int Menu_pik_Dam(sf::RenderWindow& window) 
{
    setlocale(LC_ALL, "Russian");
    // // Инициализация окна SFML
    // sf::RenderWindow window(sf::VideoMode::getDesktopMode(),  L"Пиковая Дама", Style::Fullscreen);
    
    // // Делаем окно windows прозрачным
    // SetWindowLong(window.getSystemHandle(), GWL_EXSTYLE, GetWindowLong(window.getSystemHandle(), GWL_EXSTYLE) | WS_EX_LAYERED);
    // SetLayeredWindowAttributes(window.getSystemHandle(), 0, 0, LWA_COLORKEY);

    window.setMouseCursorVisible(false); //отключаем видимость курсора

    // Размер экрана
    float widht = sf::VideoMode::getDesktopMode().width;
    float height = sf::VideoMode::getDesktopMode().height;

    //Заставка загрузки

    // Texture texthome;
    // texthome.loadFromFile("C:/Users/Sopha/Desktop/proj/pik_dam.png");
    // RectangleShape homecls(Vector2f(640, 280));
    // homecls.setTexture(&texthome);
    // homecls.setPosition(Vector2f(widht / 2 - homecls.getLocalBounds().width/2, height / 2- homecls.getLocalBounds().height/2));
    // window.draw(homecls);
    // window.display();

    // Звуковые эффекты
    SoundBuffer buffer;
    SoundBuffer buf_return;

    if (!buffer.loadFromFile("resources/audiomenu2.wav")) return 22;
    if (!buf_return.loadFromFile("resources/audiomenu5.wav")) return 22;
    Sound sound, sound_return;
    sound.setBuffer(buffer);
    sound_return.setBuffer(buf_return);

    // Music music;
    // if (!music.openFromFile("C:/Users/Sopha/Desktop/proj/audio/horror.ogg")) return 25;
    // music.setLoop(true);
    // music.setVolume(50);
    // music.play();

    // Music musicF;
    // if (!musicF.openFromFile("C:/Users/Sopha/Desktop/proj/audio/faer.ogg")) return 28;
    // musicF.setLoop(true);
    // musicF.setVolume(50);
    // musicF.play();

    //Название пунктов меню
    std::vector<String> name_menu{L"Играть",L"Настройки",L"Правила игры",L"Вернуться в меню"};

    //Объект меню
    game::GameMenu mymenu(window, 950, 350, 100, 120, name_menu);
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
    Init_Text(Titul, 480, 50, L"Пиковая Дама", 150, Color(237,147,0), 3);

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
                        main_pikgame(window);gameJustEnded = true;
                    }
                    else
                    {
                        gameJustEnded = false; // Сброс флага для следующего запуска игры
                        // Обработка других пунктов меню
                    }
                    // music.pause(); musicF.pause();
                    sound_return.play();
                        // Переходим на выбранный пункт меню
                        switch (mymenu.getSelectedMenuNumber())
                        {
                            // case 0:main_pikgame(window);  break;
                            case 1:Optionis(window);     break;
                            case 2:About_Game_Pik(window);  break;
                            case 3:return 0; break;
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