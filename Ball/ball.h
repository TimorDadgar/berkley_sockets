#include <cstdint> 
#ifndef BALL_HPP
#define BALL_HPP

class BallState
{
    public:
        BallState(): mPosX(0), mPosY(0), mBounceCount(0) {}
        int32_t mPosX;
        int32_t mPosY;
        int32_t mBounceCount;
    private:

};

#endif