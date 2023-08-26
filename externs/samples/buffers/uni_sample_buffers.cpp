#include "uni_sample_buffers.h"
#include "MapHelper.hpp"
#include "NanoLog.hpp"
#include "ApplePlatformDefinitions.h"

namespace universe
{
    static const char* kVertexSource = R"(
    cbuffer Constants
    {
        float4x4 g_WorldViewProj;
    };

    struct VSInput 
    {
        float3 Pos   : ATTRIB0;
        float4 Color : ATTRIB1;
    };

    struct PSInput 
    {
        float4 Pos   : SV_POSITION;
        float4 Color : COLOR0;
    };

    void main(in VSInput VSIn,
                out PSInput PSIn)
    {
        PSIn.Pos   = mul( float4(VSIn.Pos, 1.0), g_WorldViewProj);
        PSIn.Color = VSIn.Color;
    }
    )";

    static const char* kFragmentSource = R"(
    struct PSInput 
    {
        float4 Pos   : SV_POSITION;
        float4 Color : COLOR0;
    };

    struct PSOutput 
    {
        float4 Color : SV_TARGET;
    };

    void main(in PSInput PSIn,
                out PSOutput PSOut)
    {
        PSOut.Color = PSIn.Color;
    }
    )";


    SampleBase* createSample()
    {
        return new SampleBuffers();
    }

    SampleBuffers::SampleBuffers()
    {
        LOG_INFO << "SampleBuffers contructor";
    }

    SampleBuffers::~SampleBuffers()
    {
        LOG_INFO << "SampleBuffers destructor";
    }

    void SampleBuffers::initialize(SampleInitInfo& info)
    {
        SampleBase::initialize(info);
        LOG_INFO << "SampleBuffers initialize";
        createPiplineState();
        createVertexBuffer();
        createIndexBuffer();
    }

    void SampleBuffers::deinitialize()
    {
        LOG_INFO << "SampleBuffers deinitialize";
    }

    void SampleBuffers::update(double curTime, double elapsedTime)
    {
        SampleBase::update(curTime, elapsedTime);

        //Apply rotation
        Diligent::float4x4 cubeModelTrans = Diligent::float4x4::RotationY(static_cast<float>(curTime) * 1.0f) 
            * Diligent::float4x4::RotationX(-Diligent::PI_F * 0.1f);
        
        //Camera view
        Diligent::float4x4 view = Diligent::float4x4::Translation(0.f, 0.f, 5.f);

        //world matrix
        auto worldTrans = getSurfaceMatrix(Diligent::float3{0, 0, 1});

        //projection matrix
        auto proj = getAdjustedProjectionMatrix(Diligent::PI_F /4.0f, 0.1f, 100.f);

        //compute mvp matrix
        mMVPMatrix = cubeModelTrans * view * worldTrans * proj;
    
    }

    void SampleBuffers::render()
    {
        auto* pRTV = mSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV = mSwapChain->GetDepthBufferDSV();
        //Clear the back buffer
        const float clearColor[] = {1.0f, 0.350f, 0.350f, 1.0f};
        mImmediateCtx->ClearRenderTarget(pRTV, clearColor, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        mImmediateCtx->ClearDepthStencil(pDSV, Diligent::CLEAR_DEPTH_FLAG, 1.f, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        //Map(write) cpu data (in)to IBuffer
        {
            Diligent::MapHelper<Diligent::float4x4> cbConstants(mImmediateCtx, mVSConstants, Diligent::MAP_WRITE, Diligent::MAP_FLAG_DISCARD);
            *cbConstants = mMVPMatrix.Transpose();
        }

        //Bind vertex and index buffers
        const Diligent::Uint64 offset = 0;
        Diligent::IBuffer* pBuffers[] = {mVertexBuffer};
        mImmediateCtx->SetVertexBuffers(0, 1, pBuffers, &offset, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION, Diligent::SET_VERTEX_BUFFERS_FLAG_RESET);
        mImmediateCtx->SetIndexBuffer(mIndexBuffer, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        //set the pipeline state
        mImmediateCtx->SetPipelineState(mPSO);
        //apply bindings
        mImmediateCtx->CommitShaderResources(mSRB, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        Diligent::DrawIndexedAttribs drawAttrs;
        drawAttrs.IndexType = Diligent::VT_UINT32;
        drawAttrs.NumIndices = 36;
        drawAttrs.Flags = Diligent::DRAW_FLAG_VERIFY_ALL;
        mImmediateCtx->DrawIndexed(drawAttrs);
    }

    void SampleBuffers::createPiplineState()
    {
        Diligent::GraphicsPipelineStateCreateInfo  psoCI;
        psoCI.PSODesc.Name = "Cube PSO";
        psoCI.PSODesc.PipelineType = Diligent::PIPELINE_TYPE_GRAPHICS;
        psoCI.GraphicsPipeline.NumRenderTargets = 1;
        psoCI.GraphicsPipeline.RTVFormats[0] = mSwapChain->GetDesc().ColorBufferFormat;
        psoCI.GraphicsPipeline.DSVFormat = mSwapChain->GetDesc().DepthBufferFormat;
        psoCI.GraphicsPipeline.PrimitiveTopology = Diligent::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        psoCI.GraphicsPipeline.RasterizerDesc.CullMode = Diligent::CULL_MODE_BACK;
        psoCI.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;

        Diligent::ShaderCreateInfo shaderCI;
        shaderCI.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE_HLSL;
        shaderCI.Desc.UseCombinedTextureSamplers = true;
        //create a vertex shader
        Diligent::RefCntAutoPtr<Diligent::IShader> vs;
        {
            shaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_VERTEX;
            shaderCI.EntryPoint = "main";
            shaderCI.Desc.Name = "Buffers vertex shader";
            shaderCI.Source = kVertexSource;
            mRenderDevice->CreateShader(shaderCI, &vs);

            Diligent::BufferDesc cbDesc;
            cbDesc.Name = "VS constants cb";
            cbDesc.Size = sizeof(Diligent::float4x4);
            cbDesc.Usage = Diligent::USAGE_DYNAMIC;
            cbDesc.BindFlags = Diligent::BIND_UNIFORM_BUFFER;
            cbDesc.CPUAccessFlags = Diligent::CPU_ACCESS_WRITE;
            mRenderDevice->CreateBuffer(cbDesc, nullptr, &mVSConstants);

        }

        //create a pixel shader
        Diligent::RefCntAutoPtr<Diligent::IShader> ps;
        {
            shaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_PIXEL;
            shaderCI.EntryPoint = "main";
            shaderCI.Desc.Name = "Buffers pixel shader";
            shaderCI.Source = kFragmentSource;
            mRenderDevice->CreateShader(shaderCI, &ps);
        }

        //Define vertex shader input Layout 
        Diligent::LayoutElement layoutElems[] = 
        {
            Diligent::LayoutElement{0, 0, 3, Diligent::VT_FLOAT32, false},
            Diligent::LayoutElement{1, 0, 4, Diligent::VT_FLOAT32, false}
        };
        psoCI.GraphicsPipeline.InputLayout.LayoutElements = layoutElems;
        psoCI.GraphicsPipeline.InputLayout.NumElements = _countof(layoutElems);

        psoCI.pVS = vs;
        psoCI.pPS = ps;
        psoCI.PSODesc.ResourceLayout.DefaultVariableType = Diligent::SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
        mRenderDevice->CreateGraphicsPipelineState(psoCI, &mPSO);
        mPSO->GetStaticVariableByName(Diligent::SHADER_TYPE_VERTEX, "Constants")->Set(mVSConstants);
        mPSO->CreateShaderResourceBinding(&mSRB, true);

    }

    void SampleBuffers::createVertexBuffer()
    {
        using namespace Diligent;
        struct Vertex
        {
            float3 pos;
            float4 color;
        };

        Vertex cubeVertices[8] = 
        {
            {float3(-1,-1,-1), float4(1,0,0,1)},
            {float3(-1,+1,-1), float4(0,1,0,1)},
            {float3(+1,+1,-1), float4(0,0,1,1)},
            {float3(+1,-1,-1), float4(1,1,1,1)},

            {float3(-1,-1,+1), float4(1,1,0,1)},
            {float3(-1,+1,+1), float4(0,1,1,1)},
            {float3(+1,+1,+1), float4(1,0,1,1)},
            {float3(+1,-1,+1), float4(0.2f,0.2f,0.2f,1)},
        };

        BufferDesc vertBufferDesc;
        vertBufferDesc.Name = "Cube vertex buffer";
        vertBufferDesc.Usage = USAGE_IMMUTABLE;
        vertBufferDesc.BindFlags = BIND_VERTEX_BUFFER;
        vertBufferDesc.Size = sizeof(cubeVertices);
        BufferData vbData;
        vbData.pData = cubeVertices;
        vbData.DataSize = sizeof(cubeVertices);
        mRenderDevice->CreateBuffer(vertBufferDesc, &vbData, &mVertexBuffer);
    }

    void SampleBuffers::createIndexBuffer()
    {
        using namespace Diligent;
        Uint32 indices[] = 
        {
            2, 0, 1, 2, 3, 0,
            4, 6, 5, 4, 7, 6,
            0, 7, 4, 0, 3, 7,
            1, 0, 4, 1, 4, 5, 
            1, 5, 2, 5, 6, 2, 
            3, 6, 7, 3, 2, 6
        };

        BufferDesc indexBufferDesc;
        indexBufferDesc.Name = "Cube index buffer";
        indexBufferDesc.Usage = USAGE_IMMUTABLE;
        indexBufferDesc.BindFlags = BIND_INDEX_BUFFER;
        indexBufferDesc.Size = sizeof(indices);
        BufferData indexBufferData;
        indexBufferData.pData = indices;
        indexBufferData.DataSize = sizeof(indices);
        mRenderDevice->CreateBuffer(indexBufferDesc, &indexBufferData, &mIndexBuffer);
    }

     Diligent::float4x4 SampleBuffers::getSurfaceMatrix(const Diligent::float3& cameraViewAxis) const
     {
        const auto& scDesc = mSwapChain->GetDesc();
        switch(scDesc.PreTransform)
        {
            case Diligent::SURFACE_TRANSFORM_ROTATE_90:
                return Diligent::float4x4::RotationArbitrary(cameraViewAxis, -Diligent::PI_F/2.f);
            case Diligent::SURFACE_TRANSFORM_ROTATE_180:
                return Diligent::float4x4::RotationArbitrary(cameraViewAxis, -Diligent::PI_F);
            case Diligent::SURFACE_TRANSFORM_ROTATE_270:
                return Diligent::float4x4::RotationArbitrary(cameraViewAxis, -Diligent::PI_F* 3.f/2.f);
            case Diligent::SURFACE_TRANSFORM_OPTIMAL:
                return Diligent::float4x4::Identity();
            case Diligent::SURFACE_TRANSFORM_HORIZONTAL_MIRROR:
            case Diligent::SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90:
            case Diligent::SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180:
            case Diligent::SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270:
                return Diligent::float4x4::Identity();
            default:
                return  Diligent::float4x4::Identity();
        }
     }

     Diligent::float4x4 SampleBuffers::getAdjustedProjectionMatrix(float fov, float near, float far) const
     {
        const auto& scDesc = mSwapChain->GetDesc();

        float aspectRatio = static_cast<float>(scDesc.Width)/static_cast<float>(scDesc.Height);
        float xScale, yScale;
        if(scDesc.PreTransform == Diligent::SURFACE_TRANSFORM_ROTATE_90 ||
            scDesc.PreTransform == Diligent::SURFACE_TRANSFORM_ROTATE_270 ||
            scDesc.PreTransform == Diligent::SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90 ||
            scDesc.PreTransform == Diligent::SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270)
        {
            xScale = 1.f / std::tan(fov/2.f);
            yScale = xScale * aspectRatio;
        }else 
        {
            yScale = 1.f / std::tan(fov/2.f);
            xScale = yScale / aspectRatio;
        }

        Diligent::float4x4 proj;
        proj._11 = xScale;
        proj._22 = yScale;
        proj.SetNearFarClipPlanes(near, far, mRenderDevice->GetDeviceInfo().IsGLDevice());
        return proj;
     }
}