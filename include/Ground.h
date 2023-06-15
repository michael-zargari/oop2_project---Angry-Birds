#pragma once
#include "StaticObjects.h"

class Ground : public StaticObjects
{
public:
	Ground(b2World& world, const sf::Vector2f& position, const sf::Vector2f& size = sf::Vector2f{WINDOW_WIDTH, WINDOW_HEIGHT});
	 ~Ground() = default;

	void objectUpdate()override {}
	void drawObject(sf::RenderWindow& window) override;
	sf::Vector2f getPosition() const override{ return m_ground.getPosition(); };
	void rotate(const int& x) override { ; };
private:
	sf::RectangleShape m_ground;

	//init functions
	void initPhysicBody(b2World& world, const sf::Vector2f& position, const sf::Vector2f& size);
	void initGraphicBody(const sf::Vector2f& size = {WINDOW_WIDTH, WINDOW_HEIGHT})override;
	
};

