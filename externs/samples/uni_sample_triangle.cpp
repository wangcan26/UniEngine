#include "uni_sample_triangle.h"
#include "NanoLog.hpp"

namespace universe 
{
    static const char* kVertexSource = R"(
    struct PSInput
    {
        float4 Pos   : SV_POSITION;
        float3 Color : COLOR;
    };
    
    void main(in uint VertId: SV_VertexID,
                out PSInput PSIn)
    {
        float4 Pos[3];
        Pos[0] = float4(-0.5, -0.5, 0.0, 1.0);
        Pos[1] = float4(0.0, 0.5, 0.0, 1.0);
        Pos[2] = float4(0.5, -0.5, 0.0, 1.0);

        float3 Col[3];
        Col[0] = float3(1.0, 0.0, 0.0); 
        Col[1] = float3(0.0, 1.0, 0.0); 
        Col[2] = float3(0.0, 0.0, 1.0);

        PSIn.Pos = Pos[VertId];
        PSIn.Color = Col[VertId];
    }
    )";

    static const char* kFragmentSource = R"(
    struct PSInput 
    {
        float4 Pos   : SV_POSITION;
        float3 Color : COLOR;
    };

    struct PSOutput
    {
        float4 Color : SV_TARGET;
    };

    void main(in PSInput PSIn, out PSOutput PSOut)
    {
        PSOut.Color = float4(PSIn.Color.rgb, 1.0);
    }
    )";

    SampleBase* createSample()
    {
        return new SampleTriangle();
    }

    SampleTriangle::SampleTriangle()
    {
        LOG_INFO << "SampleTriangle contructor";
    }

    SampleTriangle::~SampleTriangle()
    {
        LOG_INFO << "SampleTriangle destructor";
    }

    void SampleTriangle::initialize(SampleInitInfo& info)
    {
        SampleBase::initialize(info);
        LOG_INFO << "SampleTriangle initialize";
        Diligent::GraphicsPipelineStateCreateInfo pipelineStateCreateInfo;
        pipelineStateCreateInfo.PSODesc.Name = "Simple triangle PSO";
        pipelineStateCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
        pipelineStateCreateInfo.GraphicsPipeline.RTVFormats[0] = mSwapChain->GetDesc().ColorBufferFormat;
        pipelineStateCreateInfo.GraphicsPipeline.DSVFormat = mSwapChain->GetDesc().DepthBufferFormat;
        pipelineStateCreateInfo.GraphicsPipeline.PrimitiveTopology = Diligent::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        pipelineStateCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = Diligent::CULL_MODE_NONE;
        pipelineStateCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;

        Diligent::ShaderCreateInfo shaderCreateInfo;
        shaderCreateInfo.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE_HLSL;
        shaderCreateInfo.Desc.UseCombinedTextureSamplers = true;
        //Create a vertex shader
        Diligent::RefCntAutoPtr<Diligent::IShader>  vs;
        {
            shaderCreateInfo.Desc.ShaderType = Diligent::SHADER_TYPE_VERTEX;
            shaderCreateInfo.EntryPoint = "main";
            shaderCreateInfo.Desc.Name = "Triangle vertex shader";
            shaderCreateInfo.Source = kVertexSource;
            mRenderDevice->CreateShader(shaderCreateInfo, &vs);
        }

        //create a fragment shader
        Diligent::RefCntAutoPtr<Diligent::IShader> fs;
        {
            shaderCreateInfo.Desc.ShaderType = Diligent::SHADER_TYPE_PIXEL;
            shaderCreateInfo.EntryPoint = "main";
            shaderCreateInfo.Desc.Name = "Triangle pixel shader";
            shaderCreateInfo.Source = kFragmentSource;
            mRenderDevice->CreateShader(shaderCreateInfo, &fs);
        }

        //create pipeline state
        pipelineStateCreateInfo.pVS = vs;
        pipelineStateCreateInfo.pPS = fs;
        mRenderDevice->CreateGraphicsPipelineState(pipelineStateCreateInfo, &mPSO);
    }

    void SampleTriangle::deinitialize()
    {
        LOG_INFO << "SampleTriangle deinitialize";
    }

    void SampleTriangle::update(double curTime, double elapsedTime)
    {
        SampleBase::update(curTime, elapsedTime);
    }

    void SampleTriangle::render()
    {
        //Clear the back buffer
        const float clearColor[] = {0.350f, 0.350f, 0.350f, 1.0f};
        auto* pRTV = mSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV = mSwapChain->GetDepthBufferDSV();
        mImmediateCtx->ClearRenderTarget(pRTV, clearColor, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        mImmediateCtx->ClearDepthStencil(pDSV, Diligent::CLEAR_DEPTH_FLAG, 1.f, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        mImmediateCtx->SetPipelineState(mPSO);

        Diligent::DrawAttribs drawAttrs;
        drawAttrs.NumVertices = 3;
        mImmediateCtx->Draw(drawAttrs);
    }
}