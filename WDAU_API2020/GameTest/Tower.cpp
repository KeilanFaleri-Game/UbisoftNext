#include "stdafx.h"
#include "App/app.h"
#include "Tower.h"

Tower::Tower(CSimpleSprite* pSprite, float xPos, float yPos, float attackSpeed, int power, float range, int cost)
{
    m_pTowerSprite = pSprite;
    m_attackSpeed = attackSpeed;
    m_power = power;
    m_range = range;
    m_cost = cost;
    x = xPos;
    y = yPos;
    m_pTowerSprite->SetPosition(x, y);
}

Tower::~Tower()
{
}

void Tower::Update(float deltaTime)
{
    m_pTowerSprite->SetPosition(x, y);
}

void Tower::Draw()
{
    m_pTowerSprite->Draw();
}
