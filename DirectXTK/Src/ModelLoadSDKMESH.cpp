//--------------------------------------------------------------------------------------
// File: ModelLoadSDKMESH.cpp
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "Model.h"

#include "Effects.h"
#include "VertexTypes.h"

#include "DirectXHelpers.h"
#include "PlatformHelpers.h"
#include "BinaryReader.h"

using namespace DirectX;
using namespace Microsoft::WRL;

#pragma warning(disable : 4121)

//--------------------------------------------------------------------------------------
// SDKMESH format is generated by the legacy DirectX SDK's Content Exporter and
// originally rendered by the DXUT helper class SDKMesh
//
// http://go.microsoft.com/fwlink/?LinkId=226208
//--------------------------------------------------------------------------------------
namespace DXUT
{
    // .SDKMESH files

    // SDKMESH_HEADER
    // SDKMESH_VERTEX_BUFFER_HEADER header->VertexStreamHeadersOffset
    // SDKMESH_INDEX_BUFFER_HEADER  header->IndexStreamHeadersOffset
    // SDKMESH_MESH                 header->MeshDataOffset
    // SDKMESH_SUBSET               header->SubsetDataOffset
    // SDKMESH_FRAME                header->FrameDataOffset
    // SDKMESH_MATERIAL             header->MaterialDataOffset
    // [header->NonBufferDataSize]
    // { [ header->NumVertexBuffers]
    //      VB data
    // }
    // { [ header->NumIndexBuffers]
    //      IB data
    // }


    // .SDDKANIM files

    // SDKANIMATION_FILE_HEADER
    // BYTE[] - Length of fileheader->AnimationDataSize

    // .SDKMESH uses Direct3D 9 decls, but only a subset of these is ever generated by the legacy DirectX SDK Content Exporter

    // D3DDECLUSAGE_POSITION / D3DDECLTYPE_FLOAT3
    // (D3DDECLUSAGE_BLENDWEIGHT / D3DDECLTYPE_UBYTE4N
    // D3DDECLUSAGE_BLENDINDICES / D3DDECLTYPE_UBYTE4)?
    // (D3DDECLUSAGE_NORMAL / D3DDECLTYPE_FLOAT3 or D3DDECLTYPE_FLOAT16_4)?
    // (D3DDECLUSAGE_COLOR / D3DDECLTYPE_D3DCOLOR)?
    // (D3DDECLUSAGE_TEXCOORD / D3DDECLTYPE_FLOAT1, D3DDECLTYPE_FLOAT2 or D3DDECLTYPE_FLOAT16_2, D3DDECLTYPE_FLOAT3 or D3DDECLTYPE_FLOAT16_4, D3DDECLTYPE_FLOAT4 or D3DDECLTYPE_FLOAT16_4)*
    // (D3DDECLUSAGE_TANGENT / D3DDECLTYPE_FLOAT3 or D3DDECLTYPE_FLOAT16_4)?
    // (D3DDECLUSAGE_BINORMAL / D3DDECLTYPE_FLOAT3 or D3DDECLTYPE_FLOAT16_4)?

    enum D3DDECLUSAGE
    {
        D3DDECLUSAGE_POSITION = 0,
        D3DDECLUSAGE_BLENDWEIGHT =1,
        D3DDECLUSAGE_BLENDINDICES =2,
        D3DDECLUSAGE_NORMAL =3,
        D3DDECLUSAGE_TEXCOORD = 5,
        D3DDECLUSAGE_TANGENT = 6,
        D3DDECLUSAGE_BINORMAL = 7,
        D3DDECLUSAGE_COLOR = 10,
    };

    enum D3DDECLTYPE
    {
        D3DDECLTYPE_FLOAT1    =  0,  // 1D float expanded to (value, 0., 0., 1.)
        D3DDECLTYPE_FLOAT2    =  1,  // 2D float expanded to (value, value, 0., 1.)
        D3DDECLTYPE_FLOAT3    =  2,  // 3D float expanded to (value, value, value, 1.)
        D3DDECLTYPE_FLOAT4    =  3,  // 4D float
        D3DDECLTYPE_D3DCOLOR  =  4,  // 4D packed unsigned bytes mapped to 0. to 1. range
                                     // Input is in D3DCOLOR format (ARGB) expanded to (R, G, B, A)
        D3DDECLTYPE_UBYTE4    =  5,  // 4D unsigned byte
        D3DDECLTYPE_UBYTE4N   =  8,  // Each of 4 bytes is normalized by dividing to 255.0
        D3DDECLTYPE_DEC3N     = 14,  // 3D signed 10 10 10 format normalized and expanded to (v[0]/511.0, v[1]/511.0, v[2]/511.0, 1)
        D3DDECLTYPE_FLOAT16_2 = 15,  // Two 16-bit floating point values, expanded to (value, value, 0, 1)
        D3DDECLTYPE_FLOAT16_4 = 16,  // Four 16-bit floating point values

        D3DDECLTYPE_UNUSED    = 17,  // When the type field in a decl is unused.
    };

    #pragma pack(push,4)

    struct D3DVERTEXELEMENT9
    {
        WORD    Stream;     // Stream index
        WORD    Offset;     // Offset in the stream in bytes
        BYTE    Type;       // Data type
        BYTE    Method;     // Processing method
        BYTE    Usage;      // Semantics
        BYTE    UsageIndex; // Semantic index
    };

    #pragma pack(pop)

    //--------------------------------------------------------------------------------------
    // Hard Defines for the various structures
    //--------------------------------------------------------------------------------------
    const uint32_t SDKMESH_FILE_VERSION = 101;
    const uint32_t MAX_VERTEX_ELEMENTS = 32;
    const uint32_t MAX_VERTEX_STREAMS = 16;
    const uint32_t MAX_FRAME_NAME = 100;
    const uint32_t MAX_MESH_NAME = 100;
    const uint32_t MAX_SUBSET_NAME = 100;
    const uint32_t MAX_MATERIAL_NAME = 100;
    const uint32_t MAX_TEXTURE_NAME = MAX_PATH;
    const uint32_t MAX_MATERIAL_PATH = MAX_PATH;
    const uint32_t INVALID_FRAME = uint32_t(-1);
    const uint32_t INVALID_MESH =  uint32_t(-1);
    const uint32_t INVALID_MATERIAL = uint32_t(-1);
    const uint32_t INVALID_SUBSET = uint32_t(-1);
    const uint32_t INVALID_ANIMATION_DATA = uint32_t(-1);
    const uint32_t INVALID_SAMPLER_SLOT = uint32_t(-1);
    const uint32_t ERROR_RESOURCE_VALUE = 1;

    template<typename TYPE> bool IsErrorResource( TYPE data )
    {
        if( ( TYPE )ERROR_RESOURCE_VALUE == data )
            return true;
        return false;
    }

    //--------------------------------------------------------------------------------------
    // Enumerated Types.  These will have mirrors in both D3D9 and D3D11
    //--------------------------------------------------------------------------------------
    enum SDKMESH_PRIMITIVE_TYPE
    {
        PT_TRIANGLE_LIST = 0,
        PT_TRIANGLE_STRIP,
        PT_LINE_LIST,
        PT_LINE_STRIP,
        PT_POINT_LIST,
        PT_TRIANGLE_LIST_ADJ,
        PT_TRIANGLE_STRIP_ADJ,
        PT_LINE_LIST_ADJ,
        PT_LINE_STRIP_ADJ,
        PT_QUAD_PATCH_LIST,
        PT_TRIANGLE_PATCH_LIST,
    };

    enum SDKMESH_INDEX_TYPE
    {
        IT_16BIT = 0,
        IT_32BIT,
    };

    enum FRAME_TRANSFORM_TYPE
    {
        FTT_RELATIVE = 0,
        FTT_ABSOLUTE,		//This is not currently used but is here to support absolute transformations in the future
    };

    //--------------------------------------------------------------------------------------
    // Structures.
    //--------------------------------------------------------------------------------------
    #pragma pack(push,8)

    struct SDKMESH_HEADER
    {
        //Basic Info and sizes
        UINT Version;
        BYTE IsBigEndian;
        UINT64 HeaderSize;
        UINT64 NonBufferDataSize;
        UINT64 BufferDataSize;

        //Stats
        UINT NumVertexBuffers;
        UINT NumIndexBuffers;
        UINT NumMeshes;
        UINT NumTotalSubsets;
        UINT NumFrames;
        UINT NumMaterials;

        //Offsets to Data
        UINT64 VertexStreamHeadersOffset;
        UINT64 IndexStreamHeadersOffset;
        UINT64 MeshDataOffset;
        UINT64 SubsetDataOffset;
        UINT64 FrameDataOffset;
        UINT64 MaterialDataOffset;
    };

    struct SDKMESH_VERTEX_BUFFER_HEADER
    {
        UINT64 NumVertices;
        UINT64 SizeBytes;
        UINT64 StrideBytes;
        D3DVERTEXELEMENT9 Decl[MAX_VERTEX_ELEMENTS];
        union
        {
            UINT64 DataOffset;
            ID3D11Buffer* pVB11;
        };
    };

    struct SDKMESH_INDEX_BUFFER_HEADER
    {
        UINT64 NumIndices;
        UINT64 SizeBytes;
        UINT IndexType;
        union
        {
            UINT64 DataOffset;
            ID3D11Buffer* pIB11;
        };
    };

    struct SDKMESH_MESH
    {
        char Name[MAX_MESH_NAME];
        BYTE NumVertexBuffers;
        UINT VertexBuffers[MAX_VERTEX_STREAMS];
        UINT IndexBuffer;
        UINT NumSubsets;
        UINT NumFrameInfluences; //aka bones

        DirectX::XMFLOAT3 BoundingBoxCenter;
        DirectX::XMFLOAT3 BoundingBoxExtents;

        union
        {
            UINT64 SubsetOffset;
            INT* pSubsets;
        };
        union
        {
            UINT64 FrameInfluenceOffset;
            UINT* pFrameInfluences;
        };
    };

    struct SDKMESH_SUBSET
    {
        char Name[MAX_SUBSET_NAME];
        UINT MaterialID;
        UINT PrimitiveType;
        UINT64 IndexStart;
        UINT64 IndexCount;
        UINT64 VertexStart;
        UINT64 VertexCount;
    };

    struct SDKMESH_FRAME
    {
        char Name[MAX_FRAME_NAME];
        UINT Mesh;
        UINT ParentFrame;
        UINT ChildFrame;
        UINT SiblingFrame;
        DirectX::XMFLOAT4X4 Matrix;
        UINT AnimationDataIndex;		//Used to index which set of keyframes transforms this frame
    };

    struct SDKMESH_MATERIAL
    {
        char    Name[MAX_MATERIAL_NAME];

        // Use MaterialInstancePath
        char    MaterialInstancePath[MAX_MATERIAL_PATH];

        // Or fall back to d3d8-type materials
        char    DiffuseTexture[MAX_TEXTURE_NAME];
        char    NormalTexture[MAX_TEXTURE_NAME];
        char    SpecularTexture[MAX_TEXTURE_NAME];

        DirectX::XMFLOAT4 Diffuse;
        DirectX::XMFLOAT4 Ambient;
        DirectX::XMFLOAT4 Specular;
        DirectX::XMFLOAT4 Emissive;
        FLOAT Power;

        union
        {
            UINT64 Force64_1;			//Force the union to 64bits
            ID3D11Texture2D* pDiffuseTexture11;
        };
        union
        {
            UINT64 Force64_2;			//Force the union to 64bits
            ID3D11Texture2D* pNormalTexture11;
        };
        union
        {
            UINT64 Force64_3;			//Force the union to 64bits
            ID3D11Texture2D* pSpecularTexture11;
        };

        union
        {
            UINT64 Force64_4;			//Force the union to 64bits
            ID3D11ShaderResourceView* pDiffuseRV11;
        };
        union
        {
            UINT64 Force64_5;		    //Force the union to 64bits
            ID3D11ShaderResourceView* pNormalRV11;
        };
        union
        {
            UINT64 Force64_6;			//Force the union to 64bits
            ID3D11ShaderResourceView* pSpecularRV11;
        };
    };

    struct SDKANIMATION_FILE_HEADER
    {
        UINT Version;
        BYTE IsBigEndian;
        UINT FrameTransformType;
        UINT NumFrames;
        UINT NumAnimationKeys;
        UINT AnimationFPS;
        UINT64 AnimationDataSize;
        UINT64 AnimationDataOffset;
    };

    struct SDKANIMATION_DATA
    {
        DirectX::XMFLOAT3 Translation;
        DirectX::XMFLOAT4 Orientation;
        DirectX::XMFLOAT3 Scaling;
    };

    struct SDKANIMATION_FRAME_DATA
    {
        char FrameName[MAX_FRAME_NAME];
        union
        {
            UINT64 DataOffset;
            SDKANIMATION_DATA* pAnimationData;
        };
    };

    #pragma pack(pop)

}; // namespace

static_assert( sizeof(DXUT::D3DVERTEXELEMENT9) == 8, "Direct3D9 Decl structure size incorrect" );
static_assert( sizeof(DXUT::SDKMESH_HEADER)== 104, "SDK Mesh structure size incorrect" );
static_assert( sizeof(DXUT::SDKMESH_VERTEX_BUFFER_HEADER) == 288, "SDK Mesh structure size incorrect" );
static_assert( sizeof(DXUT::SDKMESH_INDEX_BUFFER_HEADER) == 32, "SDK Mesh structure size incorrect" );
static_assert( sizeof(DXUT::SDKMESH_MESH) == 224, "SDK Mesh structure size incorrect" );
static_assert( sizeof(DXUT::SDKMESH_SUBSET) == 144, "SDK Mesh structure size incorrect" );
static_assert( sizeof(DXUT::SDKMESH_FRAME) == 184, "SDK Mesh structure size incorrect" );
static_assert( sizeof(DXUT::SDKMESH_MATERIAL) == 1256, "SDK Mesh structure size incorrect" );
static_assert( sizeof(DXUT::SDKANIMATION_FILE_HEADER) == 40, "SDK Mesh structure size incorrect" );
static_assert( sizeof(DXUT::SDKANIMATION_DATA) == 40, "SDK Mesh structure size incorrect" );
static_assert( sizeof(DXUT::SDKANIMATION_FRAME_DATA) == 112, "SDK Mesh structure size incorrect" );


//--------------------------------------------------------------------------------------
struct MaterialRecordSDKMESH
{
    std::shared_ptr<IEffect> effect;
    bool alpha;
};


static void LoadMaterial( _In_ const DXUT::SDKMESH_MATERIAL& mh,
                          _In_ bool perVertexColor,
                          _In_ bool enableSkinning,
                          _Inout_ IEffectFactory& fxFactory, _Inout_ MaterialRecordSDKMESH& m )
{
    WCHAR matName[ DXUT::MAX_MATERIAL_NAME ];
    MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, mh.Name, -1, matName, DXUT::MAX_MATERIAL_NAME );

    WCHAR txtName[ DXUT::MAX_TEXTURE_NAME ];
    MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, mh.DiffuseTexture, -1, txtName, DXUT::MAX_TEXTURE_NAME );

    EffectFactory::EffectInfo info;
    info.name = matName;
    info.perVertexColor = perVertexColor;
    info.enableSkinning = enableSkinning;
    info.ambientColor = XMFLOAT3( mh.Ambient.x, mh.Ambient.y, mh.Ambient.z );
    info.diffuseColor = XMFLOAT3( mh.Diffuse.x, mh.Diffuse.y, mh.Diffuse.z );
    info.emissiveColor= XMFLOAT3( mh.Emissive.x, mh.Emissive.y, mh.Emissive.z );

    if ( mh.Diffuse.w != 1.f && mh.Diffuse.w != 0.f )
    {
        info.alpha = mh.Diffuse.w;
    }
    else
        info.alpha = 1.f;

    if ( mh.Power )
    {
        info.specularPower = mh.Power;
        info.specularColor = XMFLOAT3( mh.Specular.x, mh.Specular.y, mh.Specular.z );
    }

    info.texture = txtName;
           
    m.effect = fxFactory.CreateEffect( info, nullptr );
    m.alpha = (info.alpha < 1.f);
}


//--------------------------------------------------------------------------------------
// Direct3D 9 Vertex Declaration to DirectInput 11 Input Layout mapping

static void GetInputLayoutDesc( _In_reads_(32) const DXUT::D3DVERTEXELEMENT9 decl[], std::vector<D3D11_INPUT_ELEMENT_DESC>& inputDesc,
                                bool &perVertexColor, bool& enableSkinning )
{
    static const D3D11_INPUT_ELEMENT_DESC elements[] =
    {
        { "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",       0, DXGI_FORMAT_B8G8R8A8_UNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BINORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BLENDINDICES",0, DXGI_FORMAT_R8G8B8A8_UINT,      0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BLENDWEIGHT", 0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    using namespace DXUT;

    uint32_t offset = 0;

    bool posfound = false;

    for( uint32_t index = 0; index < DXUT::MAX_VERTEX_ELEMENTS; ++index )
    {
        if ( decl[index].Usage == 0xFF )
            break;

        if ( decl[index].Offset != offset )
            break;

        if ( decl[index].Usage == D3DDECLUSAGE_POSITION && decl[index].Type == D3DDECLTYPE_FLOAT3 )
        {
            inputDesc.push_back( elements[0] );
            offset += 12;
            posfound = true;
        }
        else if ( decl[index].Usage == D3DDECLUSAGE_NORMAL )
        {
            if ( decl[index].Type == D3DDECLTYPE_FLOAT3 )
            {
                inputDesc.push_back( elements[1] );
                offset += 12;
            }
            else if ( decl[index].Type == D3DDECLTYPE_FLOAT16_4 )
            {
                D3D11_INPUT_ELEMENT_DESC desc = elements[1];
                desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
                inputDesc.push_back( desc );
                offset += 8;
            }
            else
                break;
        }
        else if ( decl[index].Usage == D3DDECLUSAGE_COLOR && decl[index].Type == D3DDECLTYPE_D3DCOLOR )
        {
            inputDesc.push_back( elements[2] );
            offset += 4;
            perVertexColor = true;
        }
        else if ( decl[index].Usage == D3DDECLUSAGE_TANGENT )
        {
            if ( decl[index].Type == D3DDECLTYPE_FLOAT3 )
            {
                inputDesc.push_back( elements[3] );
                offset += 12;
            }
            else if ( decl[index].Type == D3DDECLTYPE_FLOAT16_4 )
            {
                D3D11_INPUT_ELEMENT_DESC desc = elements[3];
                desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
                inputDesc.push_back( desc );
                offset += 8;
            }
            else
                break;
        }
        else if ( decl[index].Usage == D3DDECLUSAGE_BINORMAL )
        {
            if ( decl[index].Type == D3DDECLTYPE_FLOAT3 )
            {
                inputDesc.push_back( elements[4] );
                offset += 12;
            }
            else if ( decl[index].Type == D3DDECLTYPE_FLOAT16_4 )
            {
                D3D11_INPUT_ELEMENT_DESC desc = elements[4];
                desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
                inputDesc.push_back( desc );
                offset += 8;
            }
            else
                break;
        }
        else if ( decl[index].Usage == D3DDECLUSAGE_TEXCOORD )
        {
            D3D11_INPUT_ELEMENT_DESC desc = elements[5];
            desc.SemanticIndex = decl[index].UsageIndex;

            bool unk = false;
            switch( decl[index].Type )
            {
            case D3DDECLTYPE_FLOAT2:    offset += 8; break;
            case D3DDECLTYPE_FLOAT1:    desc.Format = DXGI_FORMAT_R32_FLOAT; offset += 4; break;
            case D3DDECLTYPE_FLOAT3:    desc.Format = DXGI_FORMAT_R32G32B32_FLOAT; offset += 12; break;
            case D3DDECLTYPE_FLOAT4:    desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; offset += 16; break;
            case D3DDECLTYPE_FLOAT16_2: desc.Format = DXGI_FORMAT_R16G16_FLOAT; offset += 4; break;
            case D3DDECLTYPE_FLOAT16_4: desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; offset += 8; break;

            default:
                unk = true;
                break;
            }

            if ( unk )
                break;

            inputDesc.push_back( desc );
        }
        else if ( decl[index].Usage == D3DDECLUSAGE_BLENDINDICES && decl[index].Type == D3DDECLTYPE_UBYTE4 )
        {
            enableSkinning = true;
            inputDesc.push_back( elements[6] );
            offset += 4;
        }
        else if ( decl[index].Usage == D3DDECLUSAGE_BLENDWEIGHT && decl[index].Type == D3DDECLTYPE_UBYTE4N )
        {
            enableSkinning = true;
            inputDesc.push_back( elements[7] );
            offset += 4;
        }
        else
            break;
    }

    if ( !posfound )
        throw std::exception("SV_Position is required");
}

// Helper for creating a D3D input layout.
static void CreateInputLayout(_In_ ID3D11Device* device, _In_ IEffect* effect, std::vector<D3D11_INPUT_ELEMENT_DESC>& inputDesc, _Out_ ID3D11InputLayout** pInputLayout)
{
    void const* shaderByteCode;
    size_t byteCodeLength;

    effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

    ThrowIfFailed(
        device->CreateInputLayout(&inputDesc.front(),
                                  static_cast<UINT>( inputDesc.size() ),
                                  shaderByteCode, byteCodeLength,
                                  pInputLayout)
    );

    SetDebugObjectName(*pInputLayout, "ModelSDKMESH");
}


//--------------------------------------------------------------------------------------
_Use_decl_annotations_
std::unique_ptr<Model> DirectX::Model::CreateFromSDKMESH( ID3D11Device* d3dDevice, const uint8_t* meshData, size_t dataSize, IEffectFactory& fxFactory, bool ccw, bool pmalpha )
{
    if ( !d3dDevice || !meshData )
        throw std::exception("Device and meshData cannot be null");

    // File Headers
    if ( dataSize < sizeof(DXUT::SDKMESH_HEADER) )
        throw std::exception("End of file");
    auto header = reinterpret_cast<const DXUT::SDKMESH_HEADER*>( meshData );

    size_t headerSize = sizeof( DXUT::SDKMESH_HEADER )
                        + header->NumVertexBuffers * sizeof(DXUT::SDKMESH_VERTEX_BUFFER_HEADER)
                        + header->NumIndexBuffers * sizeof(DXUT::SDKMESH_INDEX_BUFFER_HEADER);
    if ( header->HeaderSize != headerSize )
        throw std::exception("Not a valid SDKMESH file");

    if ( dataSize < header->HeaderSize )
        throw std::exception("End of file");

    if( header->Version != DXUT::SDKMESH_FILE_VERSION )
        throw std::exception("Not a supported SDKMESH version");
                          
    if ( header->IsBigEndian )
        throw std::exception("Loading BigEndian SDKMESH files not supported");

    if ( !header->NumMeshes )
        throw std::exception("No meshes found");

    if ( !header->NumVertexBuffers )
        throw std::exception("No vertex buffers found");

    if ( !header->NumIndexBuffers )
        throw std::exception("No index buffers found");

    // Sub-headers
    if ( dataSize < header->VertexStreamHeadersOffset
         || ( dataSize < (header->VertexStreamHeadersOffset + header->NumVertexBuffers * sizeof(DXUT::SDKMESH_VERTEX_BUFFER_HEADER) ) ) )
        throw std::exception("End of file");
    auto vbArray = reinterpret_cast<const DXUT::SDKMESH_VERTEX_BUFFER_HEADER*>( meshData + header->VertexStreamHeadersOffset );
    
    if ( dataSize < header->IndexStreamHeadersOffset
         || ( dataSize < (header->IndexStreamHeadersOffset + header->NumIndexBuffers * sizeof(DXUT::SDKMESH_INDEX_BUFFER_HEADER) ) ) )
        throw std::exception("End of file");
    auto ibArray = reinterpret_cast<const DXUT::SDKMESH_INDEX_BUFFER_HEADER*>( meshData + header->IndexStreamHeadersOffset );

    if ( dataSize < header->MeshDataOffset
         || ( dataSize < (header->MeshDataOffset + header->NumMeshes * sizeof(DXUT::SDKMESH_MESH) ) ) )
        throw std::exception("End of file");
    auto meshArray = reinterpret_cast<const DXUT::SDKMESH_MESH*>( meshData + header->MeshDataOffset );

    if ( dataSize < header->SubsetDataOffset
         || ( dataSize < (header->SubsetDataOffset + header->NumTotalSubsets * sizeof(DXUT::SDKMESH_SUBSET) ) ) )
        throw std::exception("End of file");
    auto subsetArray = reinterpret_cast<const DXUT::SDKMESH_SUBSET*>( meshData + header->SubsetDataOffset );

    if ( dataSize < header->FrameDataOffset
         || (dataSize < (header->FrameDataOffset + header->NumFrames * sizeof(DXUT::SDKMESH_FRAME) ) ) )
        throw std::exception("End of file");
    // TODO - auto frameArray = reinterpret_cast<const DXUT::SDKMESH_FRAME*>( meshData + header->FrameDataOffset );

    if ( dataSize < header->MaterialDataOffset
         || (dataSize < (header->MaterialDataOffset + header->NumMaterials * sizeof(DXUT::SDKMESH_MATERIAL) ) ) )
        throw std::exception("End of file");
    auto materialArray = reinterpret_cast<const DXUT::SDKMESH_MATERIAL*>( meshData + header->MaterialDataOffset );

    // Buffer data
    uint64_t bufferDataOffset = header->HeaderSize + header->NonBufferDataSize;
    if ( ( dataSize < bufferDataOffset )
         || ( dataSize < bufferDataOffset + header->BufferDataSize ) )
        throw std::exception("End of file");
    const uint8_t* bufferData = meshData + bufferDataOffset; 

    // Create vertex buffers
    std::vector<ComPtr<ID3D11Buffer>> vbs;
    vbs.resize( header->NumVertexBuffers );

    std::vector<std::shared_ptr<std::vector<D3D11_INPUT_ELEMENT_DESC>>> vbDecls;
    vbDecls.resize( header->NumVertexBuffers );

    std::vector<bool> perVertexColor;
    perVertexColor.resize( header->NumVertexBuffers );

    std::vector<bool> enableSkinning;
    enableSkinning.resize( header->NumVertexBuffers );

    for( UINT j=0; j < header->NumVertexBuffers; ++j )
    {
        auto& vh = vbArray[j];

        if ( dataSize < vh.DataOffset
             || ( dataSize < vh.DataOffset + vh.SizeBytes ) )
        throw std::exception("End of file");

        vbDecls[j] = std::make_shared<std::vector<D3D11_INPUT_ELEMENT_DESC>>();
        bool vertColor = false;
        bool skinning = false;
        GetInputLayoutDesc( vh.Decl, *vbDecls[j].get(), vertColor, skinning );
        perVertexColor[j] = vertColor;
        enableSkinning[j] = skinning;

        auto verts = reinterpret_cast<const uint8_t*>( bufferData + (vh.DataOffset - bufferDataOffset) );

        D3D11_BUFFER_DESC desc = {0};
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = static_cast<UINT>( vh.SizeBytes );
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA initData = {0};
        initData.pSysMem = verts;

        ThrowIfFailed(
            d3dDevice->CreateBuffer( &desc, &initData, &vbs[j] )
            );

        SetDebugObjectName( vbs[j].Get(), "ModelSDKMESH" ); 
    }

    // Create index buffers
    std::vector<ComPtr<ID3D11Buffer>> ibs;
    ibs.resize( header->NumIndexBuffers );
    
    for( UINT j=0; j < header->NumIndexBuffers; ++j )
    {
        auto& ih = ibArray[j];

        if ( dataSize < ih.DataOffset
             || ( dataSize < ih.DataOffset + ih.SizeBytes ) )
            throw std::exception("End of file");

        if ( ih.IndexType != DXUT::IT_16BIT && ih.IndexType != DXUT::IT_32BIT )
            throw std::exception("Invalid index buffer type found");

        auto indexes = reinterpret_cast<const uint8_t*>( bufferData + (ih.DataOffset - bufferDataOffset) );

        D3D11_BUFFER_DESC desc = {0};
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = static_cast<UINT>( ih.SizeBytes );
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA initData = {0};
        initData.pSysMem = indexes;

        ThrowIfFailed(
            d3dDevice->CreateBuffer( &desc, &initData, &ibs[j] )
            );

        SetDebugObjectName( ibs[j].Get(), "ModelSDKMESH" ); 
    }

    // Create meshes
    std::vector<MaterialRecordSDKMESH> materials;
    materials.resize( header->NumMaterials );

    std::unique_ptr<Model> model(new Model());
    model->meshes.reserve( header->NumMeshes );
  
    for( UINT meshIndex = 0; meshIndex < header->NumMeshes; ++meshIndex )
    {
        auto& mh = meshArray[ meshIndex ];

        if ( !mh.NumSubsets
             || !mh.NumVertexBuffers
             || mh.IndexBuffer >= header->NumIndexBuffers
             || mh.VertexBuffers[0] >= header->NumVertexBuffers )
            throw std::exception("Invalid mesh found");

        // mh.NumVertexBuffers is sometimes not what you'd expect, so we skip validating it
      
        if ( dataSize < mh.SubsetOffset
             || (dataSize < mh.SubsetOffset + mh.NumSubsets*sizeof(UINT) ) )
            throw std::exception("End of file");

        auto subsets = reinterpret_cast<const UINT*>( meshData + mh.SubsetOffset );

        if ( mh.NumFrameInfluences > 0 )
        {
            if ( dataSize < mh.FrameInfluenceOffset
                 || (dataSize < mh.FrameInfluenceOffset + mh.NumFrameInfluences*sizeof(UINT) ) )
                throw std::exception("End of file");

            // TODO - auto influences = reinterpret_cast<const UINT*>( meshData + mh.FrameInfluenceOffset );
        }

        auto mesh = std::make_shared<ModelMesh>();
        WCHAR meshName[ DXUT::MAX_MESH_NAME ];
        MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, mh.Name, -1, meshName, DXUT::MAX_MESH_NAME );
        mesh->name = meshName;
        mesh->ccw = ccw;
        mesh->pmalpha = pmalpha;

        // Extents
        mesh->boundingBox.Center = mh.BoundingBoxCenter;
        mesh->boundingBox.Extents = mh.BoundingBoxExtents;
        BoundingSphere::CreateFromBoundingBox( mesh->boundingSphere, mesh->boundingBox );
       
        // Create subsets
        mesh->meshParts.reserve( mh.NumSubsets );
        for( UINT j = 0; j < mh.NumSubsets; ++j )
        {
            auto sIndex = subsets[ j ];
            if ( sIndex >= header->NumTotalSubsets )
                throw std::exception("Invalid mesh found");

            auto& subset = subsetArray[ sIndex ];

            D3D11_PRIMITIVE_TOPOLOGY primType;
            switch( subset.PrimitiveType )
            {
            case DXUT::PT_TRIANGLE_LIST:        primType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;       break;
            case DXUT::PT_TRIANGLE_STRIP:       primType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;      break;
            case DXUT::PT_LINE_LIST:            primType = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;           break;
            case DXUT::PT_LINE_STRIP:           primType = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;          break;
            case DXUT::PT_POINT_LIST:           primType = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;          break;
            case DXUT::PT_TRIANGLE_LIST_ADJ:    primType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;   break;
            case DXUT::PT_TRIANGLE_STRIP_ADJ:   primType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;  break;
            case DXUT::PT_LINE_LIST_ADJ:        primType = D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;       break;
            case DXUT::PT_LINE_STRIP_ADJ:       primType = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;      break;

            case DXUT::PT_QUAD_PATCH_LIST:
            case DXUT::PT_TRIANGLE_PATCH_LIST:
                throw std::exception("Direct3D9 era tessellation not supported");

            default:
                throw std::exception("Unknown primitive type");
            }

            if ( subset.MaterialID >= header->NumMaterials )
                throw std::exception("Invalid mesh found");

            auto& mat = materials[ subset.MaterialID ];

            if ( !mat.effect )
            {
                LoadMaterial( materialArray[ subset.MaterialID ],
                              perVertexColor[ mh.VertexBuffers[0] ], enableSkinning[ mh.VertexBuffers[0] ],
                              fxFactory, mat );
            }

            ComPtr<ID3D11InputLayout> il;
            CreateInputLayout( d3dDevice, mat.effect.get(), *vbDecls[ mh.VertexBuffers[0] ].get(), &il );

            auto part = new ModelMeshPart();
            part->isAlpha = mat.alpha;

            part->indexCount = static_cast<uint32_t>( subset.IndexCount );
            part->startIndex = static_cast<uint32_t>( subset.IndexStart );
            part->vertexStride = static_cast<uint32_t>( vbArray[ mh.VertexBuffers[0] ].StrideBytes );
            part->indexFormat = ( ibArray[ mh.IndexBuffer ].IndexType == DXUT::IT_32BIT ) ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
            part->primitiveType = primType; 
            part->inputLayout = il;
            part->indexBuffer = ibs[ mh.IndexBuffer ];
            part->vertexBuffer = vbs[ mh.VertexBuffers[0] ];
            part->effect = mat.effect;
            part->vbDecl = vbDecls[ mh.VertexBuffers[0] ];

            mesh->meshParts.emplace_back( part );
        }

        model->meshes.emplace_back( mesh );
    }

    return model;
}


//--------------------------------------------------------------------------------------
_Use_decl_annotations_
std::unique_ptr<Model> DirectX::Model::CreateFromSDKMESH( ID3D11Device* d3dDevice, const wchar_t* szFileName, IEffectFactory& fxFactory, bool ccw, bool pmalpha )
{
    size_t dataSize = 0;
    std::unique_ptr<uint8_t[]> data;
    HRESULT hr = BinaryReader::ReadEntireFile( szFileName, data, &dataSize );
    if ( FAILED(hr) )
    {
//        DebugTrace( "CreateFromSDKMESH failed (%08X) loading '%S'\n", hr, szFileName );
        throw std::exception( "CreateFromSDKMESH" );
    }

    auto model = CreateFromSDKMESH( d3dDevice, data.get(), dataSize, fxFactory, ccw, pmalpha );

    model->name = szFileName;

    return model;
}
