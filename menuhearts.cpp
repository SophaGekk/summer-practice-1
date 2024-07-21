#include<Windows.h>
#include "gameMenu.h"
#include <SFML/Graphics.hpp> 
#include <SFML/Audio.hpp> 
#include "Animator.h"
#include "menuhearts.h"

#include "Hearts.h"

using namespace sf;


void Init_Text_H(Text& mtext, float xpos, float ypos, String str, int size_font, Color menu_text_color, int bord, Color border_color) {
    mtext.setCharacterSize(size_font);
    mtext.setString(str);
    mtext.setFillColor(menu_text_color);
    mtext.setOutlineThickness(bord);
    mtext.setOutlineColor(border_color);

    // Получите размер окна
    float windowsWidth =1920;

    // Вычислите центральную позицию по горизонтали 
    float textWidth = mtext.getLocalBounds().width;
    xpos = (windowsWidth - textWidth) / 2.0f;

    mtext.setPosition(xpos, ypos);

}

// int Game_Start_Hearts(sf::RenderWindow& Play)
// {
//     RectangleShape background_play(Vector2f(1920, 1080));
//     Play.setMouseCursorVisible(true); //включаем видимость курсора
//     Texture texture_play;
//     if (!texture_play.loadFromFile("resources/table.png")) exit(1);
//     background_play.setTexture(&texture_play);

//     while (Play.isOpen())
//     {
//         Event event_play;
//         while (Play.pollEvent(event_play))
//         {
//             if (event_play.type == Event::KeyPressed)
//             {
//                 main_hearts(Play, number_of_players);
//                 if (event_play.key.code == Keyboard::Escape) { return 0; }
//             }
//         }
//         Play.clear();
//         Play.draw(background_play);
//         Play.display();
//     }
//     return 0;
// }

//Настройки
int Options_Hearts(sf::RenderWindow& Optionis)
{
    sf::RectangleShape background_opt(sf::Vector2f(1920, 1080));
    sf::Texture texture_opt;
    if (!texture_opt.loadFromFile("resources/table.png")) exit(2);
    background_opt.setTexture(&texture_opt);

    std::vector<std::wstring> playerOptions = {L"1 игрок", L"2 игрока", L"3 игрока"};
    int currentOption = 0;
    bool optionSelected = false;

    sf::Font font;
    if (!font.loadFromFile("resources/arial.ttf")) exit(3);
    sf::Text optionText;
    optionText.setFont(font);
    optionText.setCharacterSize(50);
    optionText.setFillColor(sf::Color::White);
    optionText.setPosition(860, 550);
    optionText.setString(playerOptions[currentOption]);

    sf::RectangleShape textBackground(sf::Vector2f(400, 100));
    textBackground.setFillColor(sf::Color(0, 0, 0, 150)); // Полупрозрачный черный фон
    textBackground.setPosition(750, 540);
    sf::Text messege;
    messege.setFont(font);
    messege.setCharacterSize(60);
    messege.setFillColor(sf::Color::White);
    messege.setPosition(100.f, 100.f);

    // Текст вопроса
    messege.setString(L"Выберите количество игроков - людей: ");
    messege.setPosition(450, 350);

    sf::Text messege_enter;
    messege_enter.setFont(font);
    messege_enter.setCharacterSize(40);
    messege_enter.setFillColor(sf::Color::White);
    messege_enter.setPosition(100.f, 100.f);

    // Текст вопроса
    messege_enter.setString(L"Нажмите Enter чтобы подтвердить выбор и Escape чтобы выйти");
    messege_enter.setPosition(380, 450);
    while (Optionis.isOpen())
    {
        sf::Event event_opt;
        while (Optionis.pollEvent(event_opt))
        {
            if (event_opt.type == sf::Event::Closed) return 0;
            if (event_opt.type == sf::Event::KeyPressed)
            {
                if (event_opt.key.code == sf::Keyboard::Escape) return currentOption + 1;
                if (!optionSelected)
                {
                    if (event_opt.key.code == sf::Keyboard::Right)
                    {
                        currentOption = (currentOption + 1) % playerOptions.size();
                        optionText.setString(playerOptions[currentOption]);
                    }
                    if (event_opt.key.code == sf::Keyboard::Left)
                    {
                        currentOption = (currentOption - 1 + playerOptions.size()) % playerOptions.size();
                        optionText.setString(playerOptions[currentOption]);
                    }
                    if (event_opt.key.code == sf::Keyboard::Enter)
                    {
                        optionSelected = true;
                    }
                }
            }
        }
        Optionis.clear();
        Optionis.draw(background_opt);
        Optionis.draw(textBackground);
        Optionis.draw(optionText);
        Optionis.draw(messege);
        Optionis.draw(messege_enter);
        Optionis.display();
    }
    return currentOption + 1;
}

// Об Игре
int About_Game_Hearts(sf::RenderWindow& About)
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
    Init_Text_H(Titul, 500, 50, L"Описание игры", 120, Color(237, 147, 0), 3);

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

int number_of_players = 1;
int Menu_Hearts(sf::RenderWindow& windows) 
{
    windows.setMouseCursorVisible(false); //отключаем видимость курсора

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
    game::GameMenu mymenu(windows, 950, 350, 100, 120, name_menu);
    // Установка цвета отображения меню
    mymenu.setColorTextMenu(Color(237, 147, 0), Color::Red, Color::Black);
    mymenu.AlignMenu(2);

    // Плавное появление из черного 
    sf::RectangleShape backgroundBlack(sf::Vector2f(widht, height)); // Заменяем Sprite на RectangleShape
    backgroundBlack.setFillColor(sf::Color(0, 0, 0, 255)); // Начальный цвет - черный
    float alpha = 0; // Начальная прозрачность

    // Устанавливаем фон экрана меню
    RectangleShape background(Vector2f(widht, height));

    sf::Texture texture_windows;
    if (!texture_windows.loadFromFile("resources/pik_dam.png")) {
        std::cerr << "Ошибка загрузки текстуры окна." << std::endl;
        return 4;
    }
    background.setTexture(&texture_windows);
    
    //Шрифт
    Font font;
    if (!font.loadFromFile("resources/troika.otf")) return 5;
    // Текст
    Text Titul;
    Titul.setFont(font);
    Init_Text_H(Titul, 480, 50, L"Черви", 150, Color(237,147,0), 3);

    // Анимация костра
    Vector2i spriteSize(300, 313);

    Sprite sprite;
    sprite.setPosition(440, 780);
    Animator animator(sprite);

    // auto& idleAnimation = animator.CreateAnimation("Idle", "C:/Users/Sopha/Desktop/proj/image/f.png", seconds(1), true);

    // idleAnimation.AddFrames(Vector2i(0, 0), spriteSize, 5, 4);


    Clock clock;
    SetLayeredWindowAttributes(windows.getSystemHandle(), 100, 0, LWA_COLORKEY);
    bool gameJustEnded = false;
    while (windows.isOpen())
    {
        Event event;
        while (windows.pollEvent(event))
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
                        main_hearts(windows, number_of_players);gameJustEnded = true;
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
                            // case 0:Game_Start_Hearts(windows);  break;
                            case 1:number_of_players = Options_Hearts(windows);     break;
                            case 2:About_Game_Hearts(windows);  break;
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
        windows.draw(backgroundBlack); // Рисуем прямоугольник

        // Область отрисовки объектов      
        windows.clear();
        windows.draw(background);
        windows.draw(Titul);
        mymenu.draw();
        windows.draw(sprite);
        windows.draw(backgroundBlack);
        windows.display();
    }
    return 0;
}