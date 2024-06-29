#pragma once
#include<iostream>
#include<vector>
#include<list>
#include <C:/Users/Sopha/Desktop/proj/resources/SFML-2.6.1/include/SFML/Graphics.hpp>
#include "AssetManager.h"

class Animator
{

public:
	
	struct Animation
	{
		std::string m_Name;
		std::string m_TextureName;
		std::vector<sf::IntRect> m_Frames;
		sf::Time m_Duration;
		bool m_Looping;

		Animation(std::string const& name, std::string const& textureName,
			sf::Time const& duration, bool looping) :m_Name(name), m_TextureName(textureName),
			m_Duration(duration), m_Looping(looping)
		{}

		// Äîáàâëÿåì êàäðû ïî ãîðèçîíòàëè
		void AddFrames(sf::Vector2i const& startFrom,
			sf::Vector2i const& frameSize, unsigned int frames, unsigned int traccia)
		{
			sf::Vector2i  current = startFrom;
			for (unsigned int t = 0; t < traccia; t++) {

            
			for (unsigned int i = 0; i < frames; i++)
			{
				// äîáàâèòü ïîëîæåíèå è ðàçìåð òåêóùåãî êàäðà
				m_Frames.push_back(sf::IntRect(current.x, current.y, frameSize.x, frameSize.y));
				// äâèãàòü òåêóùèé êàäð ïî ãîðèçîíòàëè 
				current.x += frameSize.x;
			}
			current.y += frameSize.y;
			current.x = startFrom.x;
			}
		}
	};

	Animator(sf::Sprite & sprite);

	Animator::Animation& CreateAnimation(std::string const& name,
		std::string const& textureName, sf::Time const& duration, bool loop = false
	);

	void Update(sf::Time const& dt);

	// âåðíóòü èñòèíà, åñëè ïåðåêëþ÷åíèå ïðîøëî óñïåøíî
	bool SwitchAnimation(std::string const& name);

	std::string GetCurrentAnimationName() const;

private:

	// ââåäèòå èìÿ àíèìàöèè
	// âîçâðàùàåò nullptr, åñëè òàêàÿ àíèìàöèÿ íå íàéäåíà
	Animator::Animation* FindAnimation(std::string const& name);

	void SwitchAnimation(Animator::Animation* animation);

	//ññûëêà íà ñïðàéò
	sf::Sprite& m_Sprite;
	sf::Time m_CurrentTime;
	std::list<Animator::Animation> m_Animations;
	Animator::Animation* m_CurrentAnimation;

};