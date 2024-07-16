#pragma once

#include<Windows.h>
#include "gameMenu.h"
#include <SFML/Graphics.hpp> 
#include <SFML/Audio.hpp> 
#include "Animator.h"

using namespace sf;

void Init_Textix(Text& mtext, float xpos, float ypos, String str, int size_font=60,Color menu_text_color=Color::White, int bord=0, Color border_color = Color::Black );

//Настройки
int Options(sf::RenderWindow& window);

// Об Игре
int About_Game(sf::RenderWindow& window);

int Menu_solitaire(sf::RenderWindow& window); 