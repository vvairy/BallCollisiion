#include <vector>
#include <memory>
#include <array>
#include <ranges>
#include <algorithm>
#include "constants.h"

export module GridManager;

import Ball;

export class GridManager
{
public:
    GridManager(std::vector<std::shared_ptr<Ball>>& balls) : balls(balls)
    {
        sectors.resize(consts::GRID_WIDTH * consts::GRID_HEIGHT);
    }

    void update(float& dt)
    {
        updateSectors();
        for (int i = 0; i < sectors.size(); ++i)
        {
            for (auto& ball : sectors[i])
                ball->move(dt);
            checkCollisionsInSector(i);
        }
    }

private:
    std::vector<std::shared_ptr<Ball>>& balls;
    std::vector<std::vector<std::shared_ptr<Ball>>> sectors;

    int getSectorIndex(const std::shared_ptr<Ball>& ball) const
    {
        int x = ball->p.x / consts::GRID_SIZE;
        int y = ball->p.y / consts::GRID_SIZE;
        return x + y * consts::GRID_WIDTH;
    }

    void checkCollisionsInSector(int sectorIndex)
    {
        if (sectorIndex < 0 || sectorIndex >= sectors.size()) return;

        auto& sectorBalls = sectors[sectorIndex];
        checkCollisionsInSameSector(sectorBalls);
        checkCollisionsWithNeighbors(sectorIndex, sectorBalls);
    }

    void updateSectors()
    {
        for (auto& sector : sectors)
            sector.clear();

        for (auto& ball : balls)
        {
            int index = getSectorIndex(ball);
            if (index >= 0 && index < sectors.size())
                sectors[index].push_back(ball);
        }
    }

    void checkCollisionsInSameSector(const std::vector<std::shared_ptr<Ball>>& sectorBalls)
    {
        for (const auto& ball : sectorBalls)
            for (const auto& otherBall : sectorBalls | std::views::drop(&ball - &sectorBalls.front() + 1)) //AB <-> BA
                ball->check_collision(otherBall);
    }

    void checkCollisionsWithNeighbors(int idx, const std::vector<std::shared_ptr<Ball>>& sectorBalls)
    {
        std::array<int, 4> neighbors = { idx - 1, idx + 1, idx - consts::GRID_WIDTH, idx - consts::GRID_WIDTH };

        for (const auto& ball : sectorBalls)
            for (int neighbor : neighbors)
                if (neighbor >= 0 && neighbor < sectors.size())
                    for (const auto& otherBall : sectors[neighbor])
                        ball->check_collision(otherBall);
    }
};