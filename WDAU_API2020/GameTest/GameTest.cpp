//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>
#include <vector>
#include <time.h>
#include <string>
//------------------------------------------------------------------------
#include "app\app.h"
//------------------------------------------------------------------------
#include "Enemy.h"
#include "TargetPoint.h"
#include "BaseStats.h"
#include "SpawnTimer.h"
#include "Tower.h"
#include "RoundManager.h"
//------------------------------------------------------------------------
// Eample data....
//------------------------------------------------------------------------

std::vector<Enemy*> pActiveEnemies;
std::vector<Enemy*> pEnemiesPool;
std::vector<Tower*> pTowers;
std::vector<Tower*> pTemplateTowers;
RoundManager* pRoundManager;
BaseStats* pPlayer;
std::vector<TargetPoint> TargetPoints;
SpawnTimer* pEnemyTimer;
bool bCanBuy;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{

    //BlueCircle		= App::CreateSprite(".\\TestData\\Shapes.bmp", 1, 1);
    //BlueTriangle	= App::CreateSprite(".\\TestData\\Shapes.bmp", 1, 2);
    //BlueSquare		= App::CreateSprite(".\\TestData\\Shapes.bmp", 2, 1);
    //RedCircle		= App::CreateSprite(".\\TestData\\Shapes.bmp", 2, 2);
    //RedSquare		= App::CreateSprite(".\\TestData\\Shapes.bmp", 3, 1);
    //RedTriangle		= App::CreateSprite(".\\TestData\\Shapes.bmp", 3, 2);

    srand(time(NULL));


    pPlayer = new BaseStats(100, 1000);

    pEnemyTimer = new SpawnTimer(2000);

    pTemplateTowers.push_back(new Tower(App::CreateSprite(".\\TestData\\Shapes.bmp", 3, 2), -100, -100, 1000, 5, 200, 100));
    pTemplateTowers.push_back(new Tower(App::CreateSprite(".\\TestData\\Shapes.bmp", 8, 4), -100, -100, 400, 2, 100, 250));
    pTemplateTowers.push_back(new Tower(App::CreateSprite(".\\TestData\\Shapes.bmp", 3, 2), -100, -100, 2000, 20, 300, 1000));

    bCanBuy = true;

    pRoundManager = new RoundManager(0.5f);
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
    pPlayer->Update(deltaTime);

    //------------------------------------------------
    //Manages rounds and enemies
    //------------------------------------------------
    if (pEnemiesPool.empty() && pActiveEnemies.empty())
    {
        pRoundManager->NextRound();
        //--------------------------------------------
        // randomly makes enemy pathing
        //--------------------------------------------
        TargetPoints.clear();
        TargetPoints.push_back(TargetPoint(-100, rand() % 650 + 100));
        for (int i = 0; i < 5; i++)
        {
            TargetPoints.push_back(TargetPoint(rand() % 170 * i + 200, rand() % 650 + 100));
        }
        TargetPoints.push_back(TargetPoint(1024, rand() % 650 + 100));

        //--------------------------------------------
        // Creates enemies
        //--------------------------------------------
        for (int i = 0; i < 20; i++)
        {
            pEnemiesPool.push_back(new Enemy(App::CreateSprite(".\\TestData\\Shapes.bmp", 6, 1),
                TargetPoints[0].GetX(), TargetPoints[0].GetY(),
                TargetPoints[1].GetX(), TargetPoints[1].GetY(),
                0.5f + pRoundManager->GetRound() * pRoundManager->GetDifficulty(), 
                5 * pRoundManager->GetRound() * pRoundManager->GetDifficulty(), 
                20 * pRoundManager->GetRound() * pRoundManager->GetDifficulty(),
                10 * pRoundManager->GetRound()));
            pEnemiesPool[i]->GetSprite()->SetFrame(rand() % 3);
            pEnemiesPool[i]->GetSprite()->SetScale(0.5f);
        }
    }


    //------------------------------------------------
    //Spawns Enemies from pool
    //------------------------------------------------
    if (pEnemyTimer->GetLimit() <= pEnemyTimer->GetTimer())
    {
        if (!pEnemiesPool.empty())
        {
            pActiveEnemies.push_back(pEnemiesPool[pEnemiesPool.size() - 1]);
            pActiveEnemies.back()->SetIsActive(true);
            pEnemiesPool.pop_back();
            pEnemyTimer->ResetTimer();
        }
    }
    else
    {
        pEnemyTimer->AddTime(deltaTime);
    }

    //------------------------------------------------------------------------
    //Enemy Pathing
    //------------------------------------------------------------------------
    for (int i = 0; i < pActiveEnemies.size(); i++)
    {
        pActiveEnemies[i]->Update(deltaTime);

        float x, y;
        pActiveEnemies[i]->GetSprite()->GetPosition(x, y);

        if (x < TargetPoints[pActiveEnemies[i]->GetPointIndex()].GetX() + 5 &&
            x + 5 > TargetPoints[pActiveEnemies[i]->GetPointIndex()].GetX() &&
            y < TargetPoints[pActiveEnemies[i]->GetPointIndex()].GetY() + 5 &&
            y + 5 > TargetPoints[pActiveEnemies[i]->GetPointIndex()].GetY())
        {
            pActiveEnemies[i]->NextPoint();
            if (TargetPoints.size() == pActiveEnemies[i]->GetPointIndex())
            {
                pPlayer->RemoveHealth(pActiveEnemies[i]->GetPower());
                pActiveEnemies.erase(pActiveEnemies.begin() + i);
            }
            pActiveEnemies[i]->SetTargetLocation(TargetPoints[pActiveEnemies[i]->GetPointIndex()].GetX(), TargetPoints[pActiveEnemies[i]->GetPointIndex()].GetY());
        }
    }

    //------------------------------------------------
    //Tower AI
    //------------------------------------------------
    for (auto tower : pTowers)
    {
        tower->Update(deltaTime);

        for (int i = 0; i < pActiveEnemies.size(); i++)
        {
            if (tower->IsEnemyInRange(pActiveEnemies[i]->GetX(), pActiveEnemies[i]->GetY()))
            {
                //Limits how often towers can shoot
                if (tower->GetSpawnTimer()->GetLimit() <= tower->GetSpawnTimer()->GetTimer())
                {
                    tower->Shoot(pActiveEnemies[i]);
                    tower->GetSpawnTimer()->ResetTimer();
                }
                else
                {
                    tower->GetSpawnTimer()->AddTime(deltaTime);
                }
                if (pActiveEnemies[i]->GetHealth() <= 0)
                {
                    App::PlaySound(".\\TestData\\cartoon121.wav");
                    pPlayer->UseMoney(pActiveEnemies[i]->GetMoney());
                    pActiveEnemies.erase(pActiveEnemies.begin() + i);
                }
            }
        }
    }

    //------------------------------------------------
    //Shop Settings
    //------------------------------------------------
    float mX, mY;
    if (App::IsKeyPressed('Q') && pPlayer->GetMoney() >= pTemplateTowers[0]->GetCost() && bCanBuy)
    {
        App::GetMousePos(mX, mY);
        pTowers.push_back(new Tower(App::CreateSprite(".\\TestData\\Shapes.bmp", 6, 1), pTemplateTowers[0], mX, mY));
        pTowers.back()->GetSprite()->SetFrame(3);
        pTowers.back()->GetSprite()->SetScale(0.5f);
        pPlayer->UseMoney(-pTowers.back()->GetCost());
        bCanBuy = false;
    }
    if (App::IsKeyPressed('W') && pPlayer->GetMoney() >= pTemplateTowers[1]->GetCost() && bCanBuy)
    {
        App::GetMousePos(mX, mY);
        pTowers.push_back(new Tower(App::CreateSprite(".\\TestData\\Shapes.bmp", 6, 1), pTemplateTowers[1], mX, mY));
        pTowers.back()->GetSprite()->SetFrame(4);
        pTowers.back()->GetSprite()->SetScale(0.75f);
        pPlayer->UseMoney(-pTowers.back()->GetCost());
        bCanBuy = false;
    }
    if (App::IsKeyPressed('E') && pPlayer->GetMoney() >= pTemplateTowers[2]->GetCost() && bCanBuy)
    {
        App::GetMousePos(mX, mY);
        pTowers.push_back(new Tower(App::CreateSprite(".\\TestData\\Shapes.bmp", 6, 1), pTemplateTowers[2], mX, mY));
        pTowers.back()->GetSprite()->SetFrame(5);
        pTowers.back()->GetSprite()->SetScale(1);
        pPlayer->UseMoney(-pTowers.back()->GetCost());
        bCanBuy = false;
    }
    if (!App::IsKeyPressed('Q') && !App::IsKeyPressed('W') && !App::IsKeyPressed('E') && bCanBuy == false)
    {
        bCanBuy = true;
    }

}


void Render()
{
    if (!TargetPoints.empty())
    {
        for (int i = 0; i < TargetPoints.size() - 1; i++)
        {
            App::DrawLine(TargetPoints[i].GetX(), TargetPoints[i].GetY(), TargetPoints[i + 1].GetX(), TargetPoints[i + 1].GetY());
        }
    }

    for (auto enemy : pActiveEnemies)
    {
        enemy->Draw();
    }

    for (auto tower : pTowers)
    {
        tower->Draw();
    }

    if (!pPlayer->GetIsAlive())
    {
        App::Print(450, 670, "You Lose");
    }

    
    std::string money = std::to_string(pPlayer->GetMoney());
    char* charMoney;
    charMoney = &money[0];
    App::Print(850, 700, "Money:");
    App::Print(920, 700, charMoney);

    std::string round = std::to_string(pRoundManager->GetRound());
    char* charRound;
    charRound = &round[0];
    App::Print(850, 720, "Round:");
    App::Print(920, 720, charRound);

    std::string health = std::to_string(pPlayer->GetHealth());
    char* charHealth;
    charHealth = &health[0];
    App::Print(850, 740, "Health:");
    App::Print(920, 740, charHealth);
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
    for (auto enemy : pActiveEnemies)
    {
        delete enemy;
    }

    for (auto enemy : pEnemiesPool)
    {
        delete enemy;
    }

    for (auto tower : pTowers)
    {
        delete tower;
    }

    for (auto tower : pTemplateTowers)
    {
        delete tower;
    }

    delete pRoundManager;
    delete pPlayer;
    delete pEnemyTimer;
}