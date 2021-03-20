//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>
#include <vector>
//------------------------------------------------------------------------
#include "app\app.h"
//------------------------------------------------------------------------
#include "Enemy.h"
#include "TargetPoint.h"
#include "BaseStats.h"
#include "SpawnTimer.h"
#include "Tower.h"
//------------------------------------------------------------------------
// Eample data....
//------------------------------------------------------------------------
//CSimpleSprite *testSprite2;

std::vector<Enemy*> pActiveEnemies;
std::vector<Enemy*> pEnemiesPool;
std::vector<Tower*> pTowers;
BaseStats* pPlayer;
std::vector<TargetPoint> TargetPoints;
SpawnTimer* pEnemyTimer;
SpawnTimer* pShotTimer;

//------------------------------------------------------------------------

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

	TargetPoints.push_back(TargetPoint(-100, 300));
	TargetPoints.push_back(TargetPoint(300, 300));
	TargetPoints.push_back(TargetPoint(300, 600));
	TargetPoints.push_back(TargetPoint(600, 150));
	TargetPoints.push_back(TargetPoint(1000, 300));

	pPlayer = new BaseStats(100);

	pEnemyTimer = new SpawnTimer(2000);
	pShotTimer = new SpawnTimer(1000);

	for (int i = 0; i < 20; i++)
	{
		pEnemiesPool.push_back(new Enemy(App::CreateSprite(".\\TestData\\Test.bmp", 8, 4),
			TargetPoints[0].GetX(), TargetPoints[0].GetY(),
			TargetPoints[1].GetX(), TargetPoints[1].GetY(),
			2.0f,5,5));
		pEnemiesPool[i]->GetSprite()->SetFrame(1);
		pEnemiesPool[i]->GetSprite()->SetScale(2.0f);
	}
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{

	if (pPlayer->GetHealth() <= 0)
	{
		pPlayer->SetIsAlive(false);
	}

    if (pEnemyTimer->GetLimit() <= pEnemyTimer->GetTimer())
    {
		if (pEnemiesPool.size() >= 0)
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
	//Enemy controls
	//------------------------------------------------------------------------
	for (int i = 0; i < pActiveEnemies.size(); i++)
	{
		pActiveEnemies[i]->Update(deltaTime);

		float x, y;
		pActiveEnemies[i]->GetSprite()->GetPosition(x, y);

		if (x < TargetPoints[pActiveEnemies[i]->GetPointIndex()].GetX() + 1 &&
			x + 1 > TargetPoints[pActiveEnemies[i]->GetPointIndex()].GetX() &&
			y < TargetPoints[pActiveEnemies[i]->GetPointIndex()].GetY() + 1 &&
			y + 1 > TargetPoints[pActiveEnemies[i]->GetPointIndex()].GetY())
		{
			pActiveEnemies[i]->NextPoint();
			if (TargetPoints.size() == pActiveEnemies[i]->GetPointIndex())
			{
				pPlayer->RemoveHealth(pActiveEnemies[i]->GetPower());

				pActiveEnemies[i]->ResetPoint();
				pActiveEnemies[i]->ResetHealth();
				pActiveEnemies[i]->SetPos(TargetPoints[pActiveEnemies[i]->GetPointIndex() - 1].GetX(), TargetPoints[pActiveEnemies[i]->GetPointIndex() - 1].GetY());
				pActiveEnemies[i]->SetIsActive(false);
				pEnemiesPool.push_back(pActiveEnemies[i]);
				pActiveEnemies.erase(pActiveEnemies.begin() + i);
			}
			pActiveEnemies[i]->SetTargetLocation(TargetPoints[pActiveEnemies[i]->GetPointIndex()].GetX(), TargetPoints[pActiveEnemies[i]->GetPointIndex()].GetY());
		}
		for (auto tower : pTowers)
		{
			if (tower->GetX() < TargetPoints[pActiveEnemies[i]->GetPointIndex()].GetX() + 1 &&
				tower->GetX() + tower->GetRange() > TargetPoints[pActiveEnemies[i]->GetPointIndex()].GetX() &&
				tower->GetY() < TargetPoints[pActiveEnemies[i]->GetPointIndex()].GetY() + 1 &&
				tower->GetY() + tower->GetRange() > TargetPoints[pActiveEnemies[i]->GetPointIndex()].GetY())
			{
				if (pShotTimer->GetLimit() <= pShotTimer->GetTimer())
				{
					pActiveEnemies[i]->RemoveHealth(tower->GetPower());
				}
				else
				{
					pShotTimer->AddTime(deltaTime);
				}
				if (pActiveEnemies[i]->GetHealth() <= 0)
				{
					pPlayer->UseMoney(pActiveEnemies[i]->GetPower());
					pActiveEnemies[i]->ResetPoint();
					pActiveEnemies[i]->ResetHealth();
					pActiveEnemies[i]->SetPos(TargetPoints[pActiveEnemies[i]->GetPointIndex() - 1].GetX(), TargetPoints[pActiveEnemies[i]->GetPointIndex() - 1].GetY());
					pActiveEnemies[i]->SetIsActive(false);
					pEnemiesPool.push_back(pActiveEnemies[i]);
					pActiveEnemies.erase(pActiveEnemies.begin() + i);
				}
			}
		}
	}

	float mX, mY;
	if (pPlayer->GetMoney() >= 100)
	{
		if (App::IsKeyPressed('Q'))
		{
			App::GetMousePos(mX, mY);
			pTowers.push_back(new Tower(App::CreateSprite(".\\TestData\\Test.bmp", 8, 4), mX, mY, 1000, 5, 300));
			pPlayer->UseMoney(-pTowers.back()->GetCost());
		}
	}
	//------------------------------------------------------------------------
	// Sample Sound.
	//------------------------------------------------------------------------
	//if (App::GetController().CheckButton(XINPUT_GAMEPAD_B, true))
	//{
	//	App::PlaySound(".\\TestData\\Test.wav");
	//}

}


void Render()
{	
    for (int i = 0; i < TargetPoints.size()-1; i++)
    {
		App::DrawLine(TargetPoints[i].GetX(), TargetPoints[i].GetY(),TargetPoints[i+1].GetX(), TargetPoints[i+1].GetY());   
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

	//char money[10];
	//sprintf(money, "%f", pPlayer->GetMoney());
	//int mon = pPlayer->GetMoney();
	//App::Print(850, 700, "Money:");
    //App::Print(920, 700, (char*)mon);
	
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
}