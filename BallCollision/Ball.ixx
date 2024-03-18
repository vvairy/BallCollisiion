#include "SFML/Graphics.hpp"
#include "constants.h"
using namespace consts;
export module Ball;

export class Ball
{
public:
    Ball(sf::Vector2f p, sf::Vector2f dir, float r, float speed) :
        gball(r),
        p(p),
        dir(dir),
        r(r),
        speed(speed)
    {}

    void window_collision()
    {
        if (p.x < 0 + r)
        {
            dir.x *= -1;
            p.x = r;
        }
        if (p.x > WINDOW_X - 2 * r)
        {
            dir.x *= -1;
            p.x = WINDOW_X - 2 * r;
        }
        if (p.y < 0 + r)
        {
            dir.y *= -1;
            p.y = r;
        }
        if (p.y > WINDOW_Y - 2 * r)
        {
            dir.y *= -1;
            p.y = WINDOW_Y - 2 * r;
        }
    }

    void move(float& dt)
    {
        p += dir * speed * dt;
        gball.setPosition(p);
    }

    void check_collision(const std::shared_ptr<Ball>& other)
    {
        window_collision();
        sf::Vector2f delta = other->p - p;
        float distance = getLenght(delta);
        if (distance < (r + other->r)) {

            /*
            if (gball.getFillColor() == sf::Color::Red)
                gball.setFillColor(sf::Color::Blue);
            else
                gball.setFillColor(sf::Color::Red);

            if (other->gball.getFillColor() == sf::Color::Red)
                other->gball.setFillColor(sf::Color::Blue);
            else
                other->gball.setFillColor(sf::Color::Red);*/

            sf::Vector2f normal = delta / distance;
            sf::Vector2f tangent = { -normal.y, normal.x };

            float dotProductTangent1 = dotProduct(dir, tangent);
            float dotProductTangent2 = dotProduct(other->dir, tangent);

            float mass1 = r;
            float mass2 = other->r;
            float newNormSpeed1 = (dotProduct(dir, normal) * (mass1 - mass2) + 2 * mass2 * dotProduct(other->dir, normal)) / (mass1 + mass2);
            float newNormSpeed2 = (dotProduct(other->dir, normal) * (mass2 - mass1) + 2 * mass1 * dotProduct(dir, normal)) / (mass1 + mass2);

            //set new dirs
                   dir = normal * newNormSpeed1 + tangent * dotProductTangent1;
            other->dir = normal * newNormSpeed2 + tangent * dotProductTangent2;

            // prevent balls from sticking
            float penetrationDepth = (r + other->r) - distance;
            sf::Vector2f correction = normal * penetrationDepth / (mass1 + mass2) * 2.0f;

                   p -= correction * r;
            other->p += correction * other->r;
        }
    }

    static sf::Vector2f normalize(sf::Vector2f& vec)
    {
        return vec /= getLenght(vec);
    }

    sf::CircleShape gball;
    sf::Vector2f p;
private:
    sf::Vector2f dir;
    float r = 0;
    float speed = 0;

    static float dotProduct(const sf::Vector2f& vec1, const sf::Vector2f& vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y;
    }

    static float getLenght(const sf::Vector2f& vec2)
    {
        return sqrt(vec2.x * vec2.x + vec2.y * vec2.y);
    }
};