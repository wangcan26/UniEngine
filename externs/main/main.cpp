#include <iostream>
#include <vector>
#include <memory>
#include "app/uni_sample_app.h"
#include "NanoLog.hpp"
#include "fg/FrameGraph.hpp"
#include <sstream>
#include <fstream>

using NativeSampleApp = universe::GLFWAppBase; 

struct FrameGraphTexture {
  struct Desc {
    uint32_t width;
    uint32_t height;
  };

  FrameGraphTexture() = default;
  explicit FrameGraphTexture(int32_t _id) : id{_id} {}
  FrameGraphTexture(FrameGraphTexture &&) noexcept = default;

  void create(const Desc &, void *) {
    static auto lastId = 0;
    id = ++lastId;
  }
  void destroy(const Desc &, void *) {}

  //void preRead(const Desc &, uint32_t flags, void *) {}
  //void preWrite() {}

  static const char *toString(const Desc &desc) { return "<I>texture</I>"; }

  int32_t id{-1};
};

int main()
{
    nanolog::initialize(nanolog::GuaranteedLogger(), "/Users/bytedance/Workspace/UniverseEngine/UniEngine/build/", "nanolog", 1);
    nanolog::set_log_level(nanolog::LogLevel::INFO);
    LOG_INFO << "Hello UniEngine";

    //test FrameGraph
    FrameGraph fg;

    struct PassData {
        FrameGraphResource target;
    };
    fg.addCallbackPass<PassData>("SimplePass", [&](FrameGraph::Builder &builder, PassData &data){
        std::string_view foo{"Foo"};
        data.target = builder.create<FrameGraphTexture>(foo, {1280, 720});
        data.target = builder.write(data.target);
    }, [=](const PassData &data, FrameGraphPassResources &resources, void*) {

    });

    fg.compile();
    fg.execute();

    std::ofstream{"fg.dot"} << fg;
    
    std::unique_ptr<NativeSampleApp> sampleApp = std::make_unique<universe::SampleApp>();
    universe::GLFWAppBase::CreateInfo createInfo {.title = "hello triangle", .minorVersion = 1, .majorVersion = 4};
    sampleApp->initialize(createInfo);

    while(sampleApp->isRunning())
    {
        sampleApp->update();
        sampleApp->render();
        sampleApp->present();
    }

    sampleApp->deinitialize();

    return 0;
}