#pragma once
class BaseStats
{
private:
    int m_health;
    int m_money;
    bool m_isAlive;

public:
    BaseStats(int health, int money = 500);
    virtual ~BaseStats();

    virtual void Update(float deltaTime);

    void RemoveHealth(int damage) { m_health -= damage; }
    int GetHealth() { return m_health; }

    int GetMoney() { return m_money; }
    void UseMoney(int money) { m_money += money; }

    void SetIsAlive(bool alive) { m_isAlive = alive; }
    bool GetIsAlive() { return m_isAlive; }

};

