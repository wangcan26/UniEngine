#pragma once 
#include "app/uni_sample_base.h"
#include "BasicMath.hpp"

namespace universe
{
    class SampleBuffers : public SampleBase 
    {
    public:
        SampleBuffers();
        ~SampleBuffers();

        void initialize(SampleInitInfo& info) override;

        void deinitialize() override;

        void update(double curTime, double elapsedTime) override;

        void render() override;

    private:
        void createPiplineState();
        void createVertexBuffer();
        void createIndexBuffer();

        Diligent::float4x4 getSurfaceMatrix(const Diligent::float3& cameraViewAxis) const;
        Diligent::float4x4 getAdjustedProjectionMatrix(float fov, float near, float far) const;
    private:
        Diligent::RefCntAutoPtr<Diligent::IPipelineState>         mPSO;
        Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> mSRB;
        Diligent::RefCntAutoPtr<Diligent::IBuffer>                mVertexBuffer;
        Diligent::RefCntAutoPtr<Diligent::IBuffer>                mIndexBuffer;
        Diligent::RefCntAutoPtr<Diligent::IBuffer>                mVSConstants;
        Diligent::float4x4                                        mMVPMatrix;
    };
}