#include "stdafx.h"
#include "App/app.h"
#include "Enemy.h"
#include "Bullet.h"

Bullet::Bullet(CSimpleSprite* pSprite, Enemy* pTarget, int power, float sX, float sY)
{
    m_pSprite = pSprite;
    m_pSprite->SetScale(0.25f);
    m_pTarget = pTarget;
    m_power = power;
    x = sX;
    y = sY;
    m_timeLimit = 0;
}

Bullet::~Bullet()
{
    delete m_pSprite;
}

void Bullet::Update(float deltaTime)
{
    //------------------------------------------------
    //Bullet movement
    //------------------------------------------------
    float aX, aY;
    m_pTarget->GetSprite()->GetPosition(aX, aY);
    float angle = atan2f(aY - y, aX - x);
    m_pTarget->GetSprite()->SetAngle(angle);
    x += cosf(angle) * 5;
    y += sinf(angle) * 5;

    m_pSprite->SetPosition(x, y);

    m_pSprite->Update(deltaTime);

    m_timeLimit += deltaTime;
}

void Bullet::Draw()
{
    m_pSprite->Draw();
}
