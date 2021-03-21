#pragma once

class CSimpleSprite;

class Enemy
{
protected:
    CSimpleSprite* m_pEnemySprite;
    float m_speed;
    int m_power;
    int m_health;
    int m_maxHealth;
    int m_targetPointIndex;
    int m_money;

    float x, y;
    float angle;

    float m_xTarget;
    float m_yTarget;

    bool m_isActive;

public:
    Enemy(CSimpleSprite* pSprite, float startX, float startY, float targetX, float targetY, float speed = 2, int power = 1, int health = 1, int money = 10);
    virtual ~Enemy();

    virtual void Update(float deltaTime);
    virtual void Draw();

    void SetTargetLocation(float xTarget, float yTarget) { m_xTarget = xTarget; m_yTarget = yTarget; }
    void NextPoint() { m_targetPointIndex++; }
    void ResetPoint() { m_targetPointIndex = 1; }
    void SetAngle(float a) { angle = a; }

    void SetPos(float pX, float pY) { x = pX; y = pY; }
    float GetX() { return x; }
    float GetY() { return y; }

    int GetPointIndex() { return  m_targetPointIndex; }
    CSimpleSprite* GetSprite() { return m_pEnemySprite; }
    int GetPower() { return m_power; }

    void RemoveHealth(int damage) { m_health -= damage; }
    int GetHealth() { return m_health; }

    int GetMoney() { return m_money; }

    void SetIsActive(bool active) { m_isActive = active; }
};
