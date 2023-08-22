#pragma once

namespace universe 
{
    class AppBase {
    public: 
        virtual ~AppBase() {}

        virtual void update(double curTime, double elapsedTime) {};

        virtual void render() = 0;

        virtual void present() = 0;

        virtual void windowResize(int width, int height) = 0; 
    };
}