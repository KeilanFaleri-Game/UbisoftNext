#include "stdafx.h"
#include <vector>
#include "App/app.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Tower.h"
#include "SpawnTimer.h"

Tower::Tower(CSimpleSprite* pSprite, float xPos, float yPos, float attackSpeed, int power, float range, int cost)
{
    m_pTowerSprite = pSprite;
    m_attackSpeed = attackSpeed;
    m_pShotTimer = new SpawnTimer(attackSpeed);
    m_power = power;
    m_range = range;
    m_cost = cost;
    x = xPos;
    y = yPos;
    m_pTowerSprite->SetPosition(x, y);
}

Tower::Tower(CSimpleSprite* pSprite, Tower* tower, float xPos, float yPos)
{
    m_pTowerSprite = pSprite;
    m_attackSpeed = tower->GetAtkSpeed();
    m_pShotTimer = new SpawnTimer(m_attackSpeed);
    m_power = tower->GetPower();
    m_range = tower->GetRange();
    m_cost = tower->GetCost();
    x = xPos;
    y = yPos;
    m_pTowerSprite->SetPosition(x, y);
}

Tower::~Tower()
{
    for (auto bullet : m_pBullet)
    {
        delete  bullet;
    }
    delete m_pTowerSprite;
    delete m_pShotTimer;
}

void Tower::Shoot(Enemy* target)
{
    //Creates a bullet
    m_pBullet.push_back(new Bullet(App::CreateSprite(".\\TestData\\Shapes.bmp", 6, 1), target, GetPower(), GetX(), GetY()));
    m_pBullet.back()->GetSprite()->SetFrame(m_pTowerSprite->GetFrame());
}

void Tower::Update(float deltaTime)
{
    m_pTowerSprite->SetPosition(x, y);

    //--------------------------------------------
    //Bullet Collision
    //--------------------------------------------
    for (int i = 0; i < m_pBullet.size(); i++)
    {
        m_pBullet[i]->Update(deltaTime);

        if (m_pBullet[i]->GetX() < m_pBullet[i]->GetTarget()->GetX() + 5 &&
            m_pBullet[i]->GetX() + 5 > m_pBullet[i]->GetTarget()->GetX() &&
            m_pBullet[i]->GetY() < m_pBullet[i]->GetTarget()->GetY() + 5 &&
            m_pBullet[i]->GetY() + 5 > m_pBullet[i]->GetTarget()->GetY())
        {
            m_pBullet[i]->GetTarget()->RemoveHealth(m_pBullet[i]->GetPower());
            m_pBullet.erase(m_pBullet.begin() + i);
        }
        else if (m_pBullet[i]->GetTimeLimit() > 1000)
        {
            m_pBullet.erase(m_pBullet.begin() + i);
        }
    }
}

void Tower::Draw()
{
    m_pTowerSprite->Draw();

    for (auto bullet : m_pBullet)
    {
        bullet->Draw();
    }
}

bool Tower::IsEnemyInRange(float xEnemy, float yEnemy)
{
    return ((x - xEnemy) * (x - xEnemy) + (y - yEnemy) * (y - yEnemy)) < m_range * m_range;
}
