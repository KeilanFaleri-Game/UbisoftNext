#pragma once

class CSimpleSprite;

class Enemy
{
protected:
    CSimpleSprite* m_pEnemySprite;
    float m_speed;
    int m_power;
    int m_health;
    int m_targetPointIndex;

    float x, y;
    float angle;

    float m_xTarget;
    float m_yTarget;

public:
    Enemy(CSimpleSprite* pSprite, float startX, float startY, float targetX, float targetY, float speed = 2, int power = 1, int health = 1);
    virtual ~Enemy();

    virtual void Update(float deltaTime);
    virtual void Draw();

    void SetTargetLocation(float xTarget, float yTarget) { m_xTarget = xTarget; m_yTarget = yTarget; }
    void NextPoint() { m_targetPointIndex++; }
    void SetAngle(float a) { angle = a; }

    int GetPointIndex() { return  m_targetPointIndex; }
    CSimpleSprite* GetSprite() { return m_pEnemySprite; }
};
