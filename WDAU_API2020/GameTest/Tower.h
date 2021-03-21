#pragma once

class CSimpleSprite;
class Bullet;
class SpawnTimer;

class Tower
{
private:
    std::vector<Bullet*> m_pBullet;
    CSimpleSprite* m_pTowerSprite;
    SpawnTimer* m_pShotTimer;
    float m_attackSpeed;
    int m_power;
    float m_range;
    int m_cost;

    float x;
    float y;
public:
    Tower(CSimpleSprite* pSprite, float xPos, float yPos, float attackSpeed = 1000, int power = 5, float range = 50, int cost = 100);
    Tower(CSimpleSprite* pSprite, Tower* tower, float xPos, float yPos);
    virtual ~Tower();

    void Shoot(Enemy* target);

    virtual void Update(float deltaTime);
    virtual void Draw();

    bool IsEnemyInRange(float xEnemy, float yEnemy);

    SpawnTimer* GetSpawnTimer() { return m_pShotTimer; }
    CSimpleSprite* GetSprite() { return m_pTowerSprite; }
    float GetAtkSpeed() { return m_attackSpeed; }
    int GetPower() { return m_power; }
    int GetRange() { return m_range; }
    int GetCost() { return m_cost; }
    float GetX() { return x; }
    float GetY() { return y; }
};

