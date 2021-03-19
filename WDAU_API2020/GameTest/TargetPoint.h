#pragma once
class TargetPoint
{
private:
    float x;
    float y;
public:
    TargetPoint(float nX, float nY) { x = nX; y = nY; }

    float GetX() { return x; }
    float GetY() { return y; }
};

