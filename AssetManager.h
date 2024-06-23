#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <C:/Users/Sopha/Downloads/SFML-2.6.1-windows-gcc-13.1.0-mingw-64-bit/SFML-2.6.1/include/SFML/Graphics.hpp>
#include<map>

class AssetManager
{
public:
	AssetManager();

	static sf::Texture& GetTexture(std::string const& filename);

private:

	std::map<std::string, sf::Texture> m_Textures;

	// AssetManager ÿâëÿåòñÿ îäíîýëåìåíòíûì, ïîýòîìó îäíîâðåìåííî ìîæíî ðàáîòàòü ñ îäíèì ýëåìåíòîì
	//sInstacne ñîäåðæèò ñòàòè÷åñêèé óêàçàòåëü íà åäèíñòâåííûé ýêçåìïëÿð äèñïåò÷åðà
	static AssetManager* sInstance;
};

#endif