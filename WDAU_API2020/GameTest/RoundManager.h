#pragma once
class RoundManager
{
private:
    int m_round;
    float m_difficulty;
public:
    RoundManager(float difficulty) { m_round = 0; m_difficulty = difficulty; }

    void NextRound() { m_round++; }

    int GetRound() { return m_round; }
    float GetDifficulty() { return m_difficulty; }
};

