#include "SFML/Graphics.hpp"
export module LevelGame;
import GridManager;
class Ball; //imported in GridManager module

export class LevelGame
{
public:
	LevelGame(std::shared_ptr<sf::RenderWindow> window_ptr,
		std::vector<std::shared_ptr<Ball>>& balls)
		: window(window_ptr),
		  gridManager(balls),
		  balls(balls)
	{}

	void update(float& dt)
	{

		window->clear();
		gridManager.update(dt);
		for (const auto& ball : balls)
		{
			ball->move(dt);
			ball->gball.setPosition(ball->p - sf::Vector2f(ball->r, ball->r));
			window->draw(ball->gball);
		}
		window->display();
	}

private:
	GridManager gridManager;
	std::vector<std::shared_ptr<Ball>> balls;
	std::shared_ptr<sf::RenderWindow> window;
};