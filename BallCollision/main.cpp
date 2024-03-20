#include "SFML/Graphics.hpp"
#include "MiddleAverageFilter.h"
#include <list>
#include "constants.h"
using namespace consts;
import Ball;
import LevelGame;

Math::MiddleAverageFilter<float, 100> fpscounter;

void draw_fps(std::shared_ptr<sf::RenderWindow> window, float fps)
{
    window->setTitle(std::to_string(fps));
}

int main()
{
    srand(time(NULL));
    int ballsCount = MIN_BALLS + rand() % (MAX_BALLS - MIN_BALLS);
    std::vector<std::shared_ptr<Ball>> balls (ballsCount);
    for (int i = 0; i < ballsCount; i++)
    {
        float radius = 5 + rand() % 10;
        sf::Vector2f point(rand() % WINDOW_X, rand() % WINDOW_Y);
        sf::Vector2f dir((-5 + (rand() % 10)) / 5.f, (-5 + (rand() % 10)) / 5.f);
        float speed = 30 + rand() % 30;
        balls[i] = std::make_shared<Ball>(point, Ball::normalize(dir), radius, speed);
    }
    
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(WINDOW_X, WINDOW_Y), "ball collision demo");
    auto game = std::make_shared<LevelGame>(window, balls);

    window->setFramerateLimit(0);

    sf::Clock clock;
    float lastime = clock.restart().asSeconds();

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window->close();
            }
        }

        float current_time = clock.getElapsedTime().asSeconds();
        float deltaTime = current_time - lastime;
        fpscounter.push(1.0f / (current_time - lastime));
        lastime = current_time;

        game->update(deltaTime);
		draw_fps(window, fpscounter.getAverage());
    }
    return 0;
}
