
#include "stdafx.h"
#include "Enemy.h"
#include "App/app.h"

Enemy::Enemy(CSimpleSprite* pSprite, float startX, float startY, float targetX, float targetY, float speed, int power, int health, int money)
{
    //Setting Base Values
    m_pEnemySprite = pSprite;
    m_speed = speed;
    m_power = power;
    m_health = health;
    m_maxHealth = health;
    m_money = money;

    //Setting Target Location
    x = startX;
    y = startY;
    m_pEnemySprite->SetPosition(x, y);
    SetTargetLocation(targetX, targetY);
    SetAngle((m_yTarget - y) / (m_xTarget - x));
    m_targetPointIndex = 1;
}

Enemy::~Enemy()
{
    delete m_pEnemySprite;
}


void Enemy::Update(float deltaTime)
{
    //--------------------------------------------
    //enemy movement
    //--------------------------------------------
    if (m_isActive)
    {
        angle = atan2f(m_yTarget - y, m_xTarget - x);
        x += cosf(angle) * m_speed;
        y += sinf(angle) * m_speed;

        m_pEnemySprite->SetPosition(x, y);

        m_pEnemySprite->Update(deltaTime);
    }
}

void Enemy::Draw()
{
    if (m_isActive)
    {
        m_pEnemySprite->Draw();
    }
}
