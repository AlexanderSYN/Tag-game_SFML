////////////////////////////////////////////////////////////////
//// game from habr -> https://habr.com/ru/articles/449596/ ////
/////////////// rewrites on SFML 3.0.2 /////////////////////////
////////////////////////////////////////////////////////////////

#include "Game.h"

Game::Game()
{
	font.openFromFile("fonts/calibri.ttf");
	Init();
}

void Game::Init() 
{
	// Filling in the array of cubes
	for (int i = 0; i < ARRAY_SIZE - 1; i++) 
		elements[i] = i + 1;

	// An empty cell is in the last element of the array
	empty_index = ARRAY_SIZE - 1;
	elements[empty_index] = 0;
	solved = true;
}

bool Game::Check()
{
	// Checking the consistency of the puzzle
	for (unsigned int i = 0; i < ARRAY_SIZE; i++)
		if (elements[i] > 0 && elements[i] != i + 1) return false;

	return true;
}

void Game::Move(Direction direction)
{
	// Calculating the row and column of an empty die
	int col = empty_index % SIZE;
	int row = empty_index / SIZE;

	// Checking for the possibility of moving and calculating the index of the movable die
	int move_index = -1;
	if (direction == Direction::Left && col < (SIZE - 1)) move_index = empty_index + 1;
	else if (direction == Direction::Right && col > 0) move_index = empty_index - 1;
	else if (direction == Direction::Up && row < (SIZE - 1)) move_index = empty_index + SIZE;
	else if (direction == Direction::Down && row > 0) move_index = empty_index - SIZE;

	// Moving the die to an empty place
	if (empty_index >= 0 && move_index >= 0)
	{
		int tmp = elements[empty_index];
		elements[empty_index] = elements[move_index];
		elements[move_index] = tmp;
		empty_index = move_index;
	}

	solved = Check();
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	sf::Color color = sf::Color(200, 100, 200);

	// Рисуем рамку игрового поля
	sf::RectangleShape shape(sf::Vector2f(FIELD_SIZE, FIELD_SIZE));
	shape.setOutlineThickness(2.f);
	shape.setOutlineColor(color);
	shape.setFillColor(sf::Color::Transparent);
	target.draw(shape, states);

	// Подготавливаем рамку для отрисовки всех плашек
	shape.setSize(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
	shape.setOutlineThickness(2.f);
	shape.setOutlineColor(color);
	shape.setFillColor(sf::Color::Transparent);

	// Подготавливаем текстовую заготовку для отрисовки номеров плашек
	sf::Text text(font, "", 52);

	for (unsigned int i = 0; i < ARRAY_SIZE; i++)
	{
		shape.setOutlineColor(color);
		text.setFillColor(color);
		text.setString(std::to_string(elements[i]));
		if (solved)
		{
			// Решенную головоломку выделяем другим цветом
			shape.setOutlineColor(sf::Color::Cyan);
			text.setFillColor(sf::Color::Cyan);
		}
		else if (elements[i] == i + 1)
		{
			// Номера плашек на своих местах выделяем цветом
			text.setFillColor(sf::Color::Green);
		}

		// Рисуем все плашки, кроме пустой
		if (elements[i] > 0)
		{
			// Вычисление позиции плашки для отрисовки
			sf::Vector2f position(i % SIZE * CELL_SIZE + 10.f, i / SIZE * CELL_SIZE + 10.f);
			shape.setPosition(position);
			// Позицию текста подбирал вручную
			text.setPosition({ position.x + 30.f + (elements[i] < 10 ? 15.f : 0.f), position.y + 25.f});
			// Отрисовываем рамку плашки
			target.draw(shape, states);
			// Отрисовываем номер плашки
			target.draw(text, states);
		}
	}
}