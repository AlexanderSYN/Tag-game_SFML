#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include "Game.h"

int main()
{
	Game game;

	unsigned int width = 600;
	unsigned int height = 600;
	
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ width, height }), "SFML");

	sf::Text text(game.font, "F2 - New Game / Esc - Exit / Arrow Keys - Move Tile", 20);
	text.setFillColor(sf::Color::Cyan);
	text.setPosition({ 5.f, 5.f });

	game.setPosition({ 50.f, 50.f });

	int move_counter = 0;

	while (window->isOpen())
	{
		while (const std::optional event = window->pollEvent()) {
			if (event->is<sf::Event::Closed>()) window->close();
			
			if (event->is<sf::Event::KeyPressed>())
			{
				const auto* keyEvent = event->getIf<sf::Event::KeyPressed>();
				if (keyEvent && keyEvent->code == sf::Keyboard::Key::Escape) 
					window->close();
				if (keyEvent && keyEvent->code == sf::Keyboard::Key::Left) 
					game.Move(Direction::Left);
				if (keyEvent && keyEvent->code == sf::Keyboard::Key::Right) game.Move(Direction::Right);
				if (keyEvent && keyEvent->code == sf::Keyboard::Key::Up) game.Move(Direction::Up);
				if (keyEvent && keyEvent->code == sf::Keyboard::Key::Down) game.Move(Direction::Down);
				// Новая игра
				if (keyEvent && keyEvent->code == sf::Keyboard::Key::F2)
				{
					game.Init();
					move_counter = 100;
				}
			}
			if (move_counter-- > 0)
				game.Move(static_cast<Direction>(rand() % 4));

			
			window->clear();
			window->draw(game);
			window->draw(text);
			window->display();
	
			
		}

	}

	delete window;
	return 0;
}
