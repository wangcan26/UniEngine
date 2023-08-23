#pragma once

#include "uni_glfw_app_base.h"

namespace universe
{
    class SampleApp : public GLFWAppBase 
    {
    public:
        SampleApp();

        ~SampleApp();

        void initialize(CreateInfo& createInfo) override;

        void deinitialize() override;

        void update(double curTime, double elapsedTime) override;

        void render() override;

        void present() override;

        void windowResize(int width, int height) override; 
    private:
    };
}