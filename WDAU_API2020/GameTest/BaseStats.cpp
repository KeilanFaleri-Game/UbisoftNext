#include "stdafx.h"
#include "BaseStats.h"

BaseStats::BaseStats(int health, int money)
{
    m_health = health;
    m_money = money;
    m_isAlive = true;
}

BaseStats::~BaseStats()
{
}

void BaseStats::Update(float deltaTime)
{
}
