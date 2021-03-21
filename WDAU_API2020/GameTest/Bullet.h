#pragma once

class Bullet
{
private:
    CSimpleSprite* m_pSprite;
    Enemy* m_pTarget;
    int m_power;
    float x;
    float y;
    float m_timeLimit;

public:
    Bullet(CSimpleSprite* pSprite, Enemy* pTarget, int power, float sX, float sY);
    virtual ~Bullet();

    virtual void Update(float deltaTime);
    virtual void Draw();

    float GetX() { return x; }
    float GetY() { return y; }
    float GetTimeLimit() { return m_timeLimit; }
    int GetPower() { return m_power; }
    Enemy* GetTarget() { return m_pTarget; }
    CSimpleSprite* GetSprite() { return m_pSprite; }
};

