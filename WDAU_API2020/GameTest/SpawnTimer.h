#pragma once
class SpawnTimer
{
private:
    float m_timer;
    float m_timerLimit;

public:
    SpawnTimer(float limit) { m_timerLimit = limit; m_timer = 0; }

    void ResetTimer() { m_timer = 0; }
    void AddTime(float dt) { m_timer += dt; }

    float GetTimer() { return m_timer; }
    float GetLimit() { return m_timerLimit; }
};

