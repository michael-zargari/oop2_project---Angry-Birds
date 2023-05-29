#pragma once

#include "Macro.h"
#include "GameResources.h"



class Menu
{
public:
	Menu();
	~Menu() = default;

	void setSound(const menuCommand& cmd);
	void setHelp(const bool &x);
	void drawMenu(sf::RenderWindow& window);
	menuCommand handleClick(const sf::Vector2f &mouse_loc);

private:
	std::vector<sf::RectangleShape> m_buttons;
	sf::RectangleShape m_backGround;
	menuCommand m_menu_state;
	sf::Sound m_music;
	bool m_helpScreen;

	/* init funcs */
	void initButtons();
	

};
