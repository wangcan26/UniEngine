#pragma once 
#include "app/uni_sample_base.h"

namespace universe
{
    class SampleTriangle : public SampleBase 
    {
    public:
        SampleTriangle();
        ~SampleTriangle();

        void initialize(SampleInitInfo& info) override;

        void deinitialize() override;

        void update(double curTime, double elapsedTime) override;

        void render() override;

    private:
        Diligent::RefCntAutoPtr<Diligent::IPipelineState> mPSO;

    };
}