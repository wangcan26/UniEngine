#include "uni_sample_app.h"
#include "NanoLog.hpp"

namespace universe
{
    SampleApp::SampleApp()
    {
        LOG_INFO << "SampleApp constructor";
    }

    SampleApp::~SampleApp()
    {
        LOG_INFO << "SampleApp deconstructor";
    }

    void SampleApp::initialize(CreateInfo& createInfo)
    {
        GLFWAppBase::initialize(createInfo);
    }

    void SampleApp::deinitialize()
    {
        GLFWAppBase::deinitialize();
    }

    void SampleApp::update(double curTime, double elapsedTime)
    {

    }

    void SampleApp::render()
    {

    }

    void SampleApp::present()
    {

    }

    void SampleApp::windowResize(int width, int height)
    {

    }
} 
