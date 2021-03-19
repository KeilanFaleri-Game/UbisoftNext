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
//------------------------------------------------------------------------
// Eample data....
//------------------------------------------------------------------------
//CSimpleSprite *testSprite2;

Enemy* testEnemy;
std::vector<TargetPoint> TargetPoints;

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

	TargetPoints.push_back(TargetPoint(0, 300));
	TargetPoints.push_back(TargetPoint(300, 300));
	TargetPoints.push_back(TargetPoint(450, 600));
	TargetPoints.push_back(TargetPoint(600, 150));
	TargetPoints.push_back(TargetPoint(1000, 300));

	testEnemy = new Enemy(App::CreateSprite(".\\TestData\\Test.bmp", 8, 4), 
		TargetPoints[0].GetX(), TargetPoints[0].GetY(), 
		TargetPoints[1].GetX(), TargetPoints[1].GetY(), 
		2.0f);
	testEnemy->GetSprite()->SetFrame(1);
	testEnemy->GetSprite()->SetScale(2.0f);
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{

	testEnemy->Update(deltaTime);

	float x, y;
	testEnemy->GetSprite()->GetPosition(x, y);

	if (x < TargetPoints[testEnemy->GetPointIndex()].GetX() + 1 &&
		x + 1 > TargetPoints[testEnemy->GetPointIndex()].GetX() &&
		y < TargetPoints[testEnemy->GetPointIndex()].GetY() + 1 &&
		y + 1 > TargetPoints[testEnemy->GetPointIndex()].GetY())
	{
		testEnemy->NextPoint();
		testEnemy->SetTargetLocation(TargetPoints[testEnemy->GetPointIndex()].GetX(), TargetPoints[testEnemy->GetPointIndex()].GetY());
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

	////------------------------------------------------------------------------
	//// Example Line Drawing.
	////------------------------------------------------------------------------
	//static float a = 0.0f;
	//float r = 1.0f;
	//float g = 1.0f;
	//float b = 1.0f;
	//a += 0.1f;
	//for (int i = 0; i < 20; i++)
	//{
	//	float sx = 200 + sinf(a + i * 0.1f)*60.0f;
	//	float sy = 200 + cosf(a + i * 0.1f)*60.0f;
	//	float ex = 700 - sinf(a + i * 0.1f)*60.0f;
	//	float ey = 700 - cosf(a + i * 0.1f)*60.0f;
	//	g = (float)i / 20.0f;
	//	b = (float)i / 20.0f;
	//	App::DrawLine(sx, sy, ex, ey,r,g,b);
	//}

    for (int i = 0; i < TargetPoints.size()-1; i++)
    {
		App::DrawLine(TargetPoints[i].GetX(), TargetPoints[i].GetY(),TargetPoints[i+1].GetX(), TargetPoints[i+1].GetY());   
    }

	testEnemy->Draw();

	App::Print(100, 100, "Sample Text");

}
//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{	
	delete testEnemy;
}