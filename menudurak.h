#pragma once

#include<Windows.h>
#include "gameMenu.h"
#include <SFML/Graphics.hpp> 
#include <SFML/Audio.hpp> 
#include "Animator.h"

using namespace sf;
void Init_Text_D(Text& mtext, float xpos, float ypos, String str, int size_font=60, Color menu_text_color=Color::White, int bord=0, Color border_color = Color::Black) ;

int Game_Start_Durak(sf::RenderWindow& Play);

//Настройки
int Options_Durak(sf::RenderWindow& Optionis);
// Об Игре
int About_Game_Durak(sf::RenderWindow& About);

int Menu_Durak(sf::RenderWindow& window);