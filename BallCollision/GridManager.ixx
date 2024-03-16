#include <vector>
#include <memory>
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

    int getSectorIndex(std::shared_ptr<Ball>& ball) 
    {
        int x = ball->p.x / consts::GRID_SIZE;
        int y = ball->p.y / consts::GRID_SIZE;
        return x + y * consts::GRID_WIDTH;
    }

    void checkCollisionsInSector(int sectorIndex) {
        if (sectorIndex < 0 || sectorIndex >= sectors.size()) return;
        auto& sectorBalls = sectors[sectorIndex];
        for (auto it1 = sectorBalls.begin(); it1 != sectorBalls.end(); ++it1) 
        {
            for (auto it2 = std::next(it1); it2 != sectorBalls.end(); ++it2)
                (*it1)->check_collision(*it2);

            std::vector neighbors = 
            { sectorIndex - 1, sectorIndex + 1, sectorIndex - consts::GRID_WIDTH, sectorIndex + consts::GRID_WIDTH };
            for (int& neighbor : neighbors)
                if (neighbor >= 0 && neighbor < consts::GRID_WIDTH * consts::GRID_HEIGHT)
                    for (auto& otherBall : sectors[neighbor])
                        (*it1)->check_collision(otherBall);
        }
    }

    void update(float dt) 
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
    std::vector<std::vector<std::shared_ptr<Ball>>> sectors; //I tried vector of deques, lists, vectors. 
    //experiment showed vector of vectors is the fastest
};