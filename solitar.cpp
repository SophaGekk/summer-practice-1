#include <SFML/Graphics.hpp> 
#include <vector>
#include <iostream> //For output to the terminal
#include <sstream> //For input through arguments
#include "solitar.h"

//Properties of a deck
const bool _allow_drop = true, _disallow_drop = false;
const bool _stack = true, _pile = false;
const uint8_t _type_stock = 0, _type_draw = 1, _type_fountain = 2, _type_pile = 3, _type_drag = 5;

bool operator==(const Card& l, const Card& r) { return (l.num == r.num); }
bool operator!=(const Card& l, const Card& r) { return (l.num != r.num); }
sf::Vector2f operator-(const sf::Vector2f& l, sf::Vector2i& r) { return sf::Vector2f(l.x - r.x, l.y - r.y); }
sf::Vector2f operator+(const sf::Vector2i& l, sf::Vector2f& r) { return sf::Vector2f(l.x + r.x, l.y + r.y); }


Deck::Deck (sf::Vector2f pos_, bool is_stack_, bool allow_drop_, uint8_t type_)
{
    pos = pos_;
    is_stack = is_stack_;
    allow_drop = allow_drop_;
    type = type_;
}

void pollInput (sf::RenderWindow& window, bool& mouse_down, bool& dragging, bool& showMessageEsc, bool& returnToMenu,int& selectedButtonIndex, int& selectedButtonIndex_2, bool& end_game, bool&YouWin)
{
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                mouse_down = true;
                break;
            case sf::Event::MouseButtonReleased:
                mouse_down = false;
                dragging = false;
                break;
            default: break;
        }
                    
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {showMessageEsc = true;}           
        else if (event.type == sf::Event::KeyPressed && (showMessageEsc))
        {
            // Проверка нажатия клавиш для переключения между кнопками "Да" и "Нет"

                if (event.key.code == sf::Keyboard::Left)
                {
                    // Переключение на предыдущую кнопку
                    selectedButtonIndex = (selectedButtonIndex - 1 + 2) % 2;
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    // Переключение на следующую кнопку
                    selectedButtonIndex = (selectedButtonIndex + 1) % 2;
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    if(showMessageEsc && selectedButtonIndex == 0)
                    {
                        returnToMenu = true;
                        break;
                    }
                    else if(showMessageEsc && selectedButtonIndex == 1)
                    {
                        showMessageEsc = false;
                    }
            }
        }
        
            if (event.type == sf::Event::KeyPressed && end_game)
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    // Переключение на предыдущую кнопку
                    selectedButtonIndex_2 = (selectedButtonIndex_2 - 1 + 2) % 2;
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    // Переключение на следующую кнопку
                    selectedButtonIndex_2 = (selectedButtonIndex_2 + 1) % 2;
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    if (selectedButtonIndex_2 == 0) // Проверка, выбрана ли кнопка "Menu"
                    {
                        returnToMenu = true;
                    }
                    else if(selectedButtonIndex_2 == 1)// Проверка, выбрана ли кнопка "Restart"
                    {
                        // Здесь код для перезапуска игры
                        // Сброс всех игровых переменных и состояний
                        // Пересоздание колоды карт и перераспределение карт игрокам
                        // Сброс флагов состояния игры
                        end_game = false;
                        YouWin = false;
                        // Перезапуск основного игрового цикла
                        main_solitare(window); returnToMenu = true;
                    }

                }
            }
    }
}

int main_solitare (sf::RenderWindow& window)
{
    bool showMessageEsc = false;
    // Флаг для отображения текста выхода в меню
    bool returnToMenu = false;
    // Индекс выбранной кнопки (0 - Да, 1 - Нет)
    int selectedButtonIndex = 0;
    sf::Font fonts;
    if (!fonts.loadFromFile("resources/arial.ttf")) {
        std::cerr << "Ошибка загрузки шрифта!" << std::endl;
    }
    sf::Text message_escape;
    message_escape.setFont(fonts);
    message_escape.setCharacterSize(30);
    message_escape.setFillColor(sf::Color::White);
    message_escape.setPosition(100.f, 100.f);
        // Создание кнопок "Да" и "Нет"
    sf::Text buttonYess;
    buttonYess.setFont(fonts);
    buttonYess.setString(L"Да");
    // buttonYess.setString("Yes");
    buttonYess.setCharacterSize(30);
    buttonYess.setFillColor(sf::Color::White);
    buttonYess.setPosition(200.f, 200.f);

    sf::Text buttonNoo;
    buttonNoo.setFont(fonts);
    buttonNoo.setString(L"Нет");
    // buttonNoo.setString("No");
    buttonNoo.setCharacterSize(30);
    buttonNoo.setFillColor(sf::Color::White);
    buttonNoo.setPosition(400.f, 200.f);

    // Текст вопроса
    // message_escape.setString(L"Вы действительно хотите выйти?");
    message_escape.setString(L"Вы действительно хотите выйти?");
    // Флаг для отображения текста выхода из игры
  //Prepare environment
    
    // time_t rand_seed;
    // if (argc - 1) {
    //     std::stringstream streamer;
    //     streamer << argv[1];
    //     streamer >> rand_seed;
    // } else {
    //     rand_seed = time(NULL);
    // }
    // std::cout << "Game seed: " << rand_seed << std::endl;
    // srand(rand_seed);

  //Create window
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    
  //Create textures and sprites
    //Textures
    sf::Texture tex_cursor;
    sf::Texture tex_table;
    sf::Texture tex_card;
    sf::Font font_arial;
    tex_table.loadFromFile("resources/table.png");
    tex_cursor.loadFromFile("resources/cursors.png");
    tex_card.loadFromFile("resources/deck.png");
    font_arial.loadFromFile("resources/arial.ttf");
    //Shapes & objects
    sf::RectangleShape spr_cursor ({ cursor_W, cursor_H });
    sf::RectangleShape spr_table ({ game_W, game_H });
    sf::RectangleShape spr_card ({ card_W, card_H });
    sf::RectangleShape spr_hole ({ card_W + margin, card_H + margin});
    sf::Text spr_text;
    //Settings
    spr_table.setTexture(&tex_table);
    spr_cursor.setTexture(&tex_cursor);
    spr_card.setTexture(&tex_card);
    spr_card.setOutlineThickness(1);
    spr_card.setOutlineColor(sf::Color(0, 0, 0));
    spr_card.setScale(1.5f, 1.5f); // размер спрайта 

    spr_hole.setFillColor(sf::Color(64, 0, 0, 128));
    spr_hole.setOutlineThickness(2);
    spr_hole.setOutlineColor(sf::Color(0, 0, 0));
    spr_hole.setScale(1.3f, 1.3f); // размер спрайта 

    spr_text.setFont(font_arial);
    spr_text.setFillColor(sf::Color::White);
    spr_text.setCharacterSize(font_size);
    spr_text.setPosition(margin, game_H - font_size - margin);

  //Prepare common variables
    const uint16_t fountains_X = game_W - (card_W + margin) * 6; //x для окошек сбора карт
    const uint16_t piles_Y = card_H * 1.5 + 75; //у для колон карт
    bool mouse_down = false, dragging = false;
    sf::Clock game_clock;
    bool paused = false, end_game = false; 
    bool GameOver = false; //проигрыш
    bool YouWin = false; //выигрыш
    
    // Создаем затемнение фона
    sf::RectangleShape backgroundFade;
    backgroundFade.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    backgroundFade.setFillColor(sf::Color(0, 0, 0, 150)); // Полупрозрачный черный цвет

    // Загружаем изображения
    sf::Texture youWinTexture, gameOverTexture;
    // RectangleShape gameOverTexture(Vector2f(300,200));
    // RectangleShape youWinTexture(Vector2f(300,200));
    if (!youWinTexture.loadFromFile("resources/YouWin.png")) {
        std::cerr << "Ошибка загрузки YouWin!" << std::endl;
    }
    if (!gameOverTexture.loadFromFile("resources/YouLose.png")) {
        std::cerr << "Ошибка загрузки YouLose!" << std::endl;
    }


    sf::Sprite endGameSprite;

    // Создаем кнопки 
    Button menuButton(L"Вернуться в меню", sf::Vector2f(400, 50), 30, sf::Color::Transparent, sf::Color::White, fonts);
    Button restartButton(L"Перезапустить игру", sf::Vector2f(400, 50), 30, sf::Color::Transparent, sf::Color::White, fonts);
    // Установка позиции кнопок
    menuButton.setPosition(sf::Vector2f(770, 640));
    restartButton.setPosition(sf::Vector2f(770, 740));
    int selectedButtonIndex_2 = 0; // Индекс выбранной кнопки
    std::vector<Button> buttons = {menuButton, restartButton}; // Вектор всех кнопок
    
    uint32_t pause_start = 0, pause_adjust = 0;
    uint8_t cards_in_fountain = 0;
    int percentage = 0;
    std::string previous_text;
    std::string stop_time = "";

  //Prepare common lambdas
    
    //Lambda to calculate fountain display X
    auto fountainPos = [fountains_X] (uint8_t f) -> sf::Vector2f {
        return sf::Vector2f (fountains_X + (f * 1.4) * (card_W + margin), margin + 25);
    };
    
    //Lambda to calculate pile display X
    auto pilePos = [piles_Y] (uint8_t p) -> sf::Vector2f {
        return sf::Vector2f ((p * 2 * card_W) + (margin * (p + 20)), piles_Y + 100);
    };
    
    //Lambda to select correct card texture
    auto selectCardTexture = [&spr_card] (Card card) {
        if (card.facedown) {
            spr_card.setTextureRect({ 0, tex_card_H * 4, tex_card_W, tex_card_H });
        } else {
            uint8_t cX = card.num % 13;
            uint8_t cY = card.num / 13;
            spr_card.setTextureRect({ tex_card_W * cX, tex_card_H * cY, tex_card_W, tex_card_H });
        }
    };

//Start game logic
    
  //Setup resources
    //deckk
    std::vector<Deck*> deckk;
      //Stock
    Deck* deck_stock = new Deck (sf::Vector2f (margin + 20, margin + 25), _stack, _disallow_drop, _type_stock);
    deckk.push_back(deck_stock);
      //Draw
    Deck* deck_draw = new Deck (sf::Vector2f (margin*2 + 60 + card_W, margin + 25), _stack, _disallow_drop, _type_draw);
    deckk.push_back(deck_draw);
      //Fountains
    std::vector<Deck*> deck_fountains;
    for (uint8_t f = 0; f < 4; ++f) {
        deck_fountains.push_back(new Deck (fountainPos(f), _stack, _allow_drop, _type_fountain));
        deckk.push_back(deck_fountains.back());
    }
    std::vector<sf::RectangleShape> pileFrames;
      //Piles
    std::vector<Deck*> deck_piles;
    for (uint8_t p = 0; p < 7; ++p) {
        sf::RectangleShape frame ({ card_W *1.5, card_H *1.5});
        frame.setPosition(pilePos(p)); // Позиция рамки
        frame.setFillColor(sf::Color::Transparent); // Прозрачный фон
        frame.setOutlineColor(sf::Color::White); // Цвет рамки
        frame.setOutlineThickness(2); // Толщина рамки
        pileFrames.push_back(frame);
        deck_piles.push_back(new Deck (pilePos(p), _pile, _allow_drop, _type_pile));
        deckk.push_back(deck_piles.back());
    }
    
    //Lambda to handle dragging, moving cards into the drag deck
    Deck* deck_drag = new Deck(sf::Vector2f(0, 0), _pile, _disallow_drop, _type_drag); //Cards being dragged
    deckk.push_back(deck_drag);
    Deck* deck_drag_from; //Pile drag originated from
    Deck* deck_drop_to; //Reported pile which card can be dropped onto
    deck_drag_from = deck_drop_to = nullptr;
    sf::Vector2f drag_offset;
    auto startDrag = [&deck_drag, &deck_drag_from, &spr_card, &drag_offset] (Deck& deck_parent, Card card, auto mouse_pos) {
        deck_drag_from = &deck_parent;
      //Find the card in the vector
        for (uint8_t i = 0, ilen = deck_drag_from->cards.size(); i < ilen; ++i) {
            if (deck_drag_from->cards[i] == card) {
              //Record mouse offset
                drag_offset = spr_card.getPosition() - mouse_pos;
              //Cycle through the pile until reaching the end of the pile
                for (uint8_t c = i, clen = deck_drag_from->cards.size(); c < clen; ++c) {    
                  //Move the card into the drag vector
                    deck_drag->cards.push_back(deck_drag_from->cards[c]);
                }
              //Remove dragged cards from the pile
                for (uint8_t c = i, clen = deck_drag_from->cards.size(); c < clen; ++c) {
                    deck_drag_from->cards.pop_back();
                }
                break;
            }
        }
    };
    
    //Lambda to determine if one card can be dropped onto another
    auto canDrop = [] (Card drop, Card to, bool for_fountain = false) -> bool {
        uint8_t drag_N = drop.num;
        uint8_t card_N = to.num;
        if (for_fountain) { return (drag_N == card_N + 1); }
        if (drag_N / 13 % 2 != card_N / 13 % 2) {
            return card_N % 13 == (drag_N % 13) + 1;
        } else {
            return false;
        }
    };

    auto drawFromStock = [&deck_stock, &deck_draw] () {
    if (deck_stock->cards.size()) {
        deck_draw->cards.push_back(deck_stock->cards.back());
        deck_draw->cards.back().facedown = false;
        deck_stock->cards.pop_back();
    } else { // Refill stock
        if (deck_draw->cards.size() == 0) {
            std::cerr << "Error: Both deck_stock and deck_draw are empty!" << std::endl;
            return;
        }
        deck_stock->cards.insert(deck_stock->cards.end(), deck_draw->cards.begin(), deck_draw->cards.end());
        std::vector<Card>().swap(deck_draw->cards);
        // Reverse
        std::reverse(deck_stock->cards.begin(), deck_stock->cards.end());
    }
};
  //Deal deckk
    Card card;
    for (uint8_t i = 0; i < 52; ++i) {
        card.num = i;
        card.rotation = 0;
        deck_stock->cards.push_back(card);
    }
    //Randomise stock
    std::random_shuffle(deck_stock->cards.begin(), deck_stock->cards.end());
    //Deal piles
    for (uint8_t p = 0; p < 7; ++p) {
        for (uint8_t c = 0, clen = p + 1; c < clen; ++c) {
            Card to_move = deck_stock->cards.back();
            deck_stock->cards.pop_back();
            to_move.facedown = !(c == p);
            deck_piles[p]->cards.push_back(to_move);
        }
    }
    
    
      
    
//Begin game-loop
    while (window.isOpen()) {
        if(returnToMenu)
        {
            return 0; break;
        }
      //Sleep thread
        sf::sleep(sf::milliseconds(16));
        
      //Poll input
        bool was_dragging = dragging;
        pollInput(window, mouse_down, dragging, showMessageEsc, returnToMenu, selectedButtonIndex, selectedButtonIndex_2, end_game, YouWin);      
        
      //Check pause status
        if (!window.hasFocus() && !paused) {
            paused = true;
            pause_start = (int)game_clock.getElapsedTime().asSeconds();
            window.setMouseCursorVisible(true);
        } else if (paused && window.hasFocus()) {
            paused = false;
          //Calculate the time between now and the pause start
            pause_adjust += (int)game_clock.getElapsedTime().asSeconds() - pause_start;
            window.setMouseCursorVisible(false);
        }
        if (paused) { continue; }
    
    //Logic
    
        auto mouse_pos = sf::Mouse::getPosition(window);
        bool hovered = false;
        
      //Check if end-game, where the stock is empty, the draw is empty, and all cards are face-up on the piles
        if (!deck_stock->cards.size() && !deck_draw->cards.size() && !dragging) {
          //Check all piles are face-up
            bool all_face_up = true;
            for (uint8_t p = 0; p < 7; ++p) {
                for (uint8_t c = 0, clen = deck_piles[p]->cards.size(); c < clen; ++c) {
                    if (deck_piles[p]->cards[c].facedown) { all_face_up = false; break; }
                }
            }
            if (all_face_up) {
              //Move a card onto the fountains
                for (uint8_t f = 0; f < 4; ++f) {
                    Card top = deck_fountains[f]->cards.back();
                    for (uint8_t p = 0; p < 7; ++p) {
                        if (canDrop(deck_piles[p]->cards.back(), top, true)) {
                            deck_fountains[f]->cards.insert(deck_fountains[f]->cards.end(), deck_piles[p]->cards.begin(), deck_piles[p]->cards.end());
                            deck_piles[p]->cards.pop_back();
                            ++cards_in_fountain;
                            sf::sleep(sf::milliseconds(100));
                            f = 5; break;
                        }
                    }
                }
            }
        }
        
        
    //Drawing
      //Clear
        window.clear();
        
      //Draw table
        window.draw(spr_table);
        // Отрисовка рамок
        for (const auto& frame : pileFrames) {
            window.draw(frame);
        }
        sf::Rect<float> drag_rect (mouse_pos.x + drag_offset.x, mouse_pos.y + drag_offset.y, card_W, card_H);
        
      //Draw deckk
        int8_t highlight_remaining_deck = -1;
        if (dragging) { deck_drop_to = nullptr; }
        for (uint8_t d = 0, dlen = deckk.size(); d < dlen; ++d) {
            bool is_stock = (deckk[d]->type == _type_stock);
            bool is_fountain = (deckk[d]->type == _type_fountain);
            uint8_t clen = deckk[d]->cards.size();
            uint8_t c = 0;
          //If a stack: draw a hole, select only the last X cards be drawn
            spr_hole.setPosition(deckk[d]->pos); //Though it may not be drawn, it's still used for drop detection
            spr_hole.move(-margin/2, -margin/2);
            if (deckk[d]->is_stack) {
                window.draw(spr_hole);
                c = (clen >= stack_draw ? clen - stack_draw : 0);
            }
          //If stock: check if the mouse is over us, or trying to draw from stock
            if (is_stock) {
                bool over_us = spr_hole.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y);
                hovered |= over_us;
                if (over_us && mouse_down) {
                    mouse_down = false;
                    drawFromStock();
                }
            }
          //Draw cards
            bool being_dragged = !(deckk[d]->pos.x + deckk[d]->pos.y);
            for (; c < clen; ++c) {
                Card this_card = deckk[d]->cards[c];
                bool is_top = (c == clen - 1);
                if (deckk[d]->is_stack) {
                    spr_card.setPosition(deckk[d]->pos);
                    spr_card.setFillColor(sf::Color(255, 255, 255));
                } else {
                    spr_card.setPosition(deckk[d]->pos.x, deckk[d]->pos.y + (c*card_H / card_peek));
                    if (being_dragged) { spr_card.move(mouse_pos + drag_offset); }
                  //Calculate gradient
                    uint8_t grad = 255 - ((clen - c) * 16);
                    spr_card.setFillColor(sf::Color(grad, grad, grad));
                }
                if (is_stock) { this_card.facedown = true; }
                selectCardTexture(this_card);
              //Check mouse
                if (!this_card.facedown && (deckk[d]->is_stack ? is_top : true) && !end_game) {
                    auto bounds = spr_card.getGlobalBounds();
                    if (c != clen - 1) { bounds.height /= card_peek; }
                    bool over_us = bounds.contains(mouse_pos.x, mouse_pos.y);
                    hovered |= over_us;
                  //If we're being hovered over: highlight, maybe drag, maybe place
                    if ((over_us && !dragging) || (highlight_remaining_deck == d)) {
                        highlight_remaining_deck = d;
                        spr_card.setFillColor(sf::Color(255, 200, 200));
                      //Check if starting to drag the card, or right click to place in fountain or piles
                        if (over_us && mouse_down && !dragging) {
                            startDrag(*deckk.at(d), this_card, mouse_pos);
                            dragging = true;
                            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                              //Try moving into the fountains
                                bool fountains_success = false;
                                if (deck_drag->cards.size() == 1) {
                                    bool is_ace = !(this_card.num % 13);
                                    for (uint8_t f = 0; f < 4; ++f) {
                                        if (is_ace) { //If an ace, place it on the first empty fountain
                                            fountains_success = !deck_fountains[f]->cards.size();
                                        } else { //If a normal card, find its correct fountain
                                            if (deck_fountains[f]->cards.size()) { fountains_success = (deck_fountains[f]->cards.back().num / 13 == this_card.num / 13); }
                                        }
                                        if (fountains_success && (is_ace ? true : fountains_success = canDrop(deck_drag->cards.front(), deck_fountains[f]->cards.back(), true))) {
                                            deck_drop_to = deck_fountains[f];
                                            break;
                                        }
                                    }
                                }
                              //Try moving into the piles
                                bool success = fountains_success;
                                if (!fountains_success) {
                                  //Search the piles
                                    for (uint8_t p = 0; p < 7; ++p) {
                                        if (   !deck_piles[p]->cards.size()             //
                                             && deck_drag->cards.front().num % 13 == 12  // A king to an empty pile
                                            ||
                                                deck_piles[p]->cards.size()                                    //
                                             && !deck_piles[p]->cards.back().facedown                          //
                                             && canDrop(deck_drag->cards.front(), deck_piles[p]->cards.back()) // Normal
                                            ) {
                                            deck_drop_to = deck_piles[p];
                                            success = true;
                                            break;
                                        }
                                    }
                                }
                                if (success) {
                                    mouse_down = false;  //
                                    was_dragging = true; //
                                    dragging = false;    // Elicit a drag-n-drop onto the fountain
                                } else {
                                    deck_drop_to = nullptr;
                                }
                            }
                        }
                    }
                  //Check if hovering to drop
                    if (deckk[d]->allow_drop && dragging && is_top) {
                        over_us = bounds.intersects(drag_rect);
                        if (over_us && canDrop(deck_drag->cards.front(), this_card, is_fountain) && (is_fountain ? deck_drag->cards.size() == 1 : true)) {
                            deck_drop_to = deckk[d];
                            spr_card.setFillColor(sf::Color(128, 128, 255));
                        }
                    }
                }
              //Draw
                spr_card.setOrigin(card_W/2, card_H/2);
                if (deckk[d]->type != _type_stock) {
                    spr_card.setRotation(this_card.rotation);
                } else {
                    spr_card.setRotation(0);
                }
                spr_card.setOrigin(0, 0);
                window.draw(spr_card);
            }
          //Check if hovering over an empty deck for drop
            if (dragging && deckk[d]->allow_drop && !deckk[d]->cards.size()) {
                bool over_us = spr_hole.getGlobalBounds().intersects(drag_rect);
                bool is_king_onto_pile = (deck_drag->cards.front().num % 13 == 12);
                bool is_ace_onto_fountain = is_fountain && (deck_drag->cards[0].num % 13 == 0);
                if (over_us && (is_king_onto_pile || is_ace_onto_fountain)) {
                    if (!deck_drop_to) { deck_drop_to = deckk[d]; }
                }
            }
        }
        
      //Draw info text
        uint32_t now = ((int)game_clock.getElapsedTime().asSeconds() - pause_adjust);
        uint8_t seconds = now % 60;
        uint8_t minutes = now / 60;
        percentage = (int)((float)cards_in_fountain / 52 * 100);
        std::string time = (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);

        // Условие остановки времени
        if (cards_in_fountain >= 52) {
            if (stop_time == "") {
                stop_time = time; // Фиксируем время остановки
            }
            previous_text = stop_time + "  " + std::to_string(100) + "%";
        } else {
            previous_text = time + "  " + std::to_string(percentage) + "%";
        }

        spr_text.setString(previous_text);
        window.draw(spr_text);

      //Draw cursor
        spr_cursor.setPosition(mouse_pos.x - cursor_W/2, mouse_pos.y - cursor_H/2);
        spr_cursor.setTextureRect({ tex_cursor_W * (hovered + dragging), 0, tex_cursor_W, tex_cursor_H });
        window.draw(spr_cursor);
      
      //Check if dropping
        if (was_dragging && !dragging) {
          //Check if our new location was droppable
          if (deck_drop_to) {
          try {
              deck_drop_to->cards.insert(deck_drop_to->cards.end(), deck_drag->cards.begin(), deck_drag->cards.end());
              if (deck_drop_to->type == _type_fountain) { ++cards_in_fountain;}
              // Face up last card
              if (!deck_drag_from->cards.empty()) {
                  deck_drag_from->cards.back().facedown = false;
              }
              
          } catch (const std::exception& e) {
              std::cerr << "Exception: " << e.what() << std::endl;
          }
      } 
      else { // It was apparently not droppable
        try {
            deck_drag_from->cards.insert(deck_drag_from->cards.end(), deck_drag->cards.begin(), deck_drag->cards.end());
        } catch (const std::exception& e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
      }
                //Clear the drag vectors
            std::vector<Card>().swap(deck_drag->cards);
            deck_drag_from = deck_drop_to = nullptr;
        }
        if (percentage == 100) {
            end_game = true;
            YouWin = true;
            showMessageEsc = false;
        }
        // Рисование сообщения о нажатие Escape, если флаг установлен 
        float textX_message_escape = game_W / 2.f - message_escape.getLocalBounds().width / 2.f;
        float textY_message_escape = (800 + 400) / 2.f - message_escape.getLocalBounds().height / 2.f;
        message_escape.setPosition(textX_message_escape, textY_message_escape);
        
        float buttonWidth = buttonYess.getLocalBounds().width;
        float buttonHeight = buttonYess.getLocalBounds().height;

        float buttonX = game_W / 2.f - buttonWidth / 2.f; 
        float buttonY = (800 + 400) / 2.f - buttonHeight / 2.f + 50;
        buttonYess.setPosition(buttonX, buttonY);

        buttonNoo.setPosition(buttonX + buttonWidth + 50, buttonY);
        if (showMessageEsc)
        {
            window.draw(message_escape);
            // Выделение выбранной кнопки
            if (selectedButtonIndex == 0)
            {
                buttonYess.setFillColor(sf::Color::Red);
                buttonNoo.setFillColor(sf::Color::White);
            }
            else
            {
                buttonYess.setFillColor(sf::Color::White);
                buttonNoo.setFillColor(sf::Color::Red);
            }

            // Рисование кнопок
            window.draw(buttonYess);
            window.draw(buttonNoo);
        }
        
        if (end_game) {
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
            // endGameSprite.setScale(0.1f, 0.1f); // Уменьшение размера спрайта
            // Отрисовка затемнения фона
            window.draw(backgroundFade);

            // Отрисовка изображения
            window.draw(endGameSprite);
                        // Выделение выбранной кнопки
            if (selectedButtonIndex_2 == 0)
            {
                menuButton.setSelected(true);
                restartButton.setSelected(false);
            }
            else
            {
                menuButton.setSelected(false);
                restartButton.setSelected(true);
            }
            // Отрисовка кнопок
            menuButton.draw(window);
            restartButton.draw(window);
        }
      //Display
        window.display();
    }
    return 0;
}
