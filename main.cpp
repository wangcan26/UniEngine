#include <iostream>
#include <vector>
#include <memory>
#include "app/uni_sample_app.h"
#include "NanoLog.hpp"

using NativeSampleApp = universe::GLFWAppBase;

int main()
{
    nanolog::initialize(nanolog::GuaranteedLogger(), "/Users/bytedance/Workspace/UniverseEngine/UniEngine/build/", "nanolog", 1);
    nanolog::set_log_level(nanolog::LogLevel::INFO);
    LOG_INFO << "Hello UniEngine";
    
    std::unique_ptr<NativeSampleApp> sampleApp = std::make_unique<universe::SampleApp>();
    universe::GLFWAppBase::CreateInfo createInfo {.title = "hello triangle", .minorVersion = 1, .majorVersion = 4};
    sampleApp->initialize(createInfo);

    while(sampleApp->isRunning())
    {
        sampleApp->update(0.0, 0.0);
        sampleApp->render();
        sampleApp->present();
    }

    sampleApp->deinitialize();
    return 0;
}