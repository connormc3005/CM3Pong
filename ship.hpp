#pragma once <
#include <SFML/Graphics.hpp>

class Ship : public sf::Sprite {
	public
		Ship();
	Ship(const Ships& s);
	Ship(sf::IntRect ir);
	virtual ~Ship() = 0;
	virtual void Update(const float& dt);
protected:
	sf::IntRect _sprite;
};
