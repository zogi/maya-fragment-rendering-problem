#include <maya/MPxLocatorNode.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MVector.h>
#include <maya/MColor.h>
#include <maya/M3dView.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

#include <maya/MDrawContext.h>
#include <maya/MDrawRegistry.h>
#include <maya/MPxGeometryOverride.h>
#include <maya/MShaderManager.h>
#include <maya/MFragmentManager.h>

#include <string>


namespace {

// === Cube vertex data ========================================================

const unsigned int cube_vertex_count = 36;
const unsigned int cube_index_count = 36;
const float cube_positions[][3] =
    { { -0.5, -0.5, -0.5 }
    , { -0.5, 0.5, -0.5 }
    , { -0.5, -0.5, 0.5 }
    , { -0.5, 0.5, 0.5 }
    , { 0.5, -0.5, -0.5 }
    , { 0.5, 0.5, -0.5 }
    , { 0.5, -0.5, 0.5 }
    , { 0.5, 0.5, 0.5 }
    , { -0.5, -0.5, -0.5 }
    , { -0.5, -0.5, 0.5 }
    , { 0.5, -0.5, -0.5 }
    , { 0.5, -0.5, 0.5 }
    , { -0.5, 0.5, -0.5 }
    , { -0.5, 0.5, 0.5 }
    , { 0.5, 0.5, -0.5 }
    , { 0.5, 0.5, 0.5 }
    , { -0.5, -0.5, -0.5 }
    , { 0.5, -0.5, -0.5 }
    , { -0.5, 0.5, -0.5 }
    , { 0.5, 0.5, -0.5 }
    , { -0.5, -0.5, 0.5 }
    , { 0.5, -0.5, 0.5 }
    , { -0.5, 0.5, 0.5 }
    , { 0.5, 0.5, 0.5 }
    };
const float cube_normals[][3] =
    { { -1, 0, 0 }
    , { -1, 0, 0 }
    , { -1, 0, 0 }
    , { -1, 0, 0 }
    , { 1, 0, 0 }
    , { 1, 0, 0 }
    , { 1, 0, 0 }
    , { 1, 0, 0 }
    , { 0, -1, 0 }
    , { 0, -1, 0 }
    , { 0, -1, 0 }
    , { 0, -1, 0 }
    , { 0, 1, 0 }
    , { 0, 1, 0 }
    , { 0, 1, 0 }
    , { 0, 1, 0 }
    , { 0, 0, -1 }
    , { 0, 0, -1 }
    , { 0, 0, -1 }
    , { 0, 0, -1 }
    , { 0, 0, 1 }
    , { 0, 0, 1 }
    , { 0, 0, 1 }
    , { 0, 0, 1 }
    };
const float cube_uvs[][2] =
    { { 0, 0 }
    , { 1, 0 }
    , { 0, 1 }
    , { 1, 1 }
    , { 0, 0 }
    , { 1, 0 }
    , { 0, 1 }
    , { 1, 1 }
    , { 0, 0 }
    , { 1, 0 }
    , { 0, 1 }
    , { 1, 1 }
    , { 0, 0 }
    , { 1, 0 }
    , { 0, 1 }
    , { 1, 1 }
    , { 0, 0 }
    , { 1, 0 }
    , { 0, 1 }
    , { 1, 1 }
    , { 0, 0 }
    , { 1, 0 }
    , { 0, 1 }
    , { 1, 1 }
    };
const float cube_tangents[][3] =
    { { 0, -1, 0 }
    , { 0, -1, 0 }
    , { 0, -1, 0 }
    , { 0, -1, 0 }
    , { 0, 1, 0 }
    , { 0, 1, 0 }
    , { 0, 1, 0 }
    , { 0, 1, 0 }
    , { 0, 0, -1 }
    , { 0, 0, -1 }
    , { 0, 0, -1 }
    , { 0, 0, -1 }
    , { 0, 0, 1 }
    , { 0, 0, 1 }
    , { 0, 0, 1 }
    , { 0, 0, 1 }
    , { -1, 0, 0 }
    , { -1, 0, 0 }
    , { -1, 0, 0 }
    , { -1, 0, 0 }
    , { 1, 0, 0 }
    , { 1, 0, 0 }
    , { 1, 0, 0 }
    , { 1, 0, 0 }
    };
const float cube_bitangents[][3] =
    { { 0, 0, -1 }
    , { 0, 0, -1 }
    , { 0, 0, -1 }
    , { 0, 0, -1 }
    , { 0, 0, 1 }
    , { 0, 0, 1 }
    , { 0, 0, 1 }
    , { 0, 0, 1 }
    , { -1, 0, 0 }
    , { -1, 0, 0 }
    , { -1, 0, 0 }
    , { -1, 0, 0 }
    , { 1, 0, 0 }
    , { 1, 0, 0 }
    , { 1, 0, 0 }
    , { 1, 0, 0 }
    , { 0, -1, 0 }
    , { 0, -1, 0 }
    , { 0, -1, 0 }
    , { 0, -1, 0 }
    , { 0, 1, 0 }
    , { 0, 1, 0 }
    , { 0, 1, 0 }
    , { 0, 1, 0 }
    };
const int cube_indices[] =
    { 0, 1, 2, 2, 1, 3
    , 4, 5, 6, 6, 5, 7
    , 8, 9, 10, 10, 9, 11
    , 12, 13, 14, 14, 13, 15
    , 16, 17, 18, 18, 17, 19
    , 20, 21, 22, 22, 21, 23
    };


// === mayaTextureOutputTooutColor fragment ====================================

const char* plumbing_fragment_name = "mayaTextureOutputTooutColor";
const char* plumbing_fragment_definition = R"(<!--
===========================================================================
Copyright 2016 Autodesk, Inc. All rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.
===========================================================================
-->
<fragment  uiName="mayaTextureOutputTooutColor" name="mayaTextureOutputTooutColor" type="plumbing" class="ShadeFragment" version="1.0" feature_level="0" >
    <description>
<![CDATA[
Extract the outColor member from the mayaTextureOutput structure.]]>
</description>
    <properties>
        <struct  name="input" struct_name="mayaTextureOutput" />
    </properties>
    <values>
    </values>
    <outputs>
        <float3  name="outColor" />
    </outputs>
    <implementation  >
    <implementation  render="OGSRenderer" language="Cg" lang_version="2.100000" >
        <function_name val="mayaTextureOutputTooutColor" />
        <source>
            <![CDATA[
float3 mayaTextureOutputTooutColor(mayaTextureOutput input) 
{ 
	return input.outColor; 
} 
            ]]>
        </source>
    </implementation>
    <implementation  render="OGSRenderer" language="HLSL" lang_version="11.000000" >
        <function_name val="mayaTextureOutputTooutColor" />
        <source>
            <![CDATA[
float3 mayaTextureOutputTooutColor(mayaTextureOutput input) 
{ 
	return input.outColor; 
} 
            ]]>
        </source>
    </implementation>
    <implementation  render="OGSRenderer" language="GLSL" lang_version="3.000000" >
        <function_name val="mayaTextureOutputTooutColor" />
        <source>
            <![CDATA[
vec3 mayaTextureOutputTooutColor(mayaTextureOutput input_is_glsl_kw) 
{ 
	return input_is_glsl_kw.outColor; 
} 
            ]]>
        </source>
    </implementation>
    <implementation  render="OGSRenderer" language="HLSL" lang_version="10.000000" >
        <function_name val="mayaTextureOutputTooutColor" />
        <source>
            <![CDATA[
float3 mayaTextureOutputTooutColor(mayaTextureOutput input) 
{ 
	return input.outColor; 
} 
            ]]>
        </source>
    </implementation>
    </implementation>
</fragment>
)";

} // unnamed namespace


// === Node definition =========================================================

class FragmentDisplay : public MPxLocatorNode {
public:
    FragmentDisplay() {}
    virtual ~FragmentDisplay() {}

    virtual MStatus compute(const MPlug& plug, MDataBlock& data)
    {
        return MS::kUnknownParameter;
    }

    virtual bool isBounded() const
    {
        return true;
    }

    virtual MBoundingBox boundingBox() const
    {
        return MBoundingBox(MPoint(0.5f, 0.5f, 0.5f), MPoint(-0.5f, -0.5f, -0.5f));
    }

    static void* creator()
    {
        return new FragmentDisplay();
    }

    static MStatus initialize()
    {
        return MStatus::kSuccess;
    }

public:
    static MTypeId id;
    static MString drawDbClassification;
    static MString drawRegistrantId;
    static MString typeName;
};

MTypeId FragmentDisplay::id(0xdeadbeef);
MString FragmentDisplay::drawDbClassification("drawdb/geometry/FragmentDisplay");
MString FragmentDisplay::drawRegistrantId("fragmentDisplayPlugin");
MString FragmentDisplay::typeName("FragmentDisplay");


// === GeometryOverride ========================================================

class FragmentDisplayGeometryOverride : public MHWRender::MPxGeometryOverride {
public:
    static MHWRender::MPxGeometryOverride* Creator(const MObject& obj)
    {
        return new FragmentDisplayGeometryOverride(obj);
    }

    virtual ~FragmentDisplayGeometryOverride() {}

    virtual MHWRender::DrawAPI supportedDrawAPIs() const;

    virtual bool hasUIDrawables() const { return false; }

    virtual void updateDG() {}
    virtual bool isIndexingDirty(const MHWRender::MRenderItem &item) { return false; }
    virtual bool isStreamDirty(const MHWRender::MVertexBufferDescriptor &desc) { return false; }
    virtual void updateRenderItems(const MDagPath &path, MHWRender::MRenderItemList& list);
    virtual void populateGeometry(const MHWRender::MGeometryRequirements &requirements, const MHWRender::MRenderItemList &renderItems, MHWRender::MGeometry &data);
    virtual void cleanUp() {};

private:
    FragmentDisplayGeometryOverride(const MObject& obj);

    MHWRender::MShaderInstance* m_shader;
    MObject m_node;
};

FragmentDisplayGeometryOverride::FragmentDisplayGeometryOverride(const MObject& obj)
    : MHWRender::MPxGeometryOverride(obj)
    , m_shader(nullptr)
    , m_node(obj)
{
}

MHWRender::DrawAPI FragmentDisplayGeometryOverride::supportedDrawAPIs() const
{
    return (MHWRender::kOpenGL | MHWRender::kOpenGLCoreProfile | MHWRender::kDirectX11);
}

static std::string g_plugin_path;

void FragmentDisplayGeometryOverride::updateRenderItems(const MDagPath& path, MHWRender::MRenderItemList& list)
{
    MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
    if (!renderer)
        return;

    const MHWRender::MShaderManager* shader_manager = renderer->getShaderManager();
    if (!shader_manager)
        return;

    MHWRender::MFragmentManager* fragment_manager = renderer->getFragmentManager();
    if (!fragment_manager)
        return;

    if (!m_shader) {
        if (!fragment_manager->hasFragment(plumbing_fragment_name)) {
            fragment_manager->addShadeFragmentFromBuffer(plumbing_fragment_definition, false);
        }

        constexpr const char* my_fragment_name = "TestFragment";
        if (fragment_manager->hasFragment(my_fragment_name)) {
            fragment_manager->removeFragment(my_fragment_name);
        }
        const std::string fragment_file = g_plugin_path + "/test_fragment.xml";
        fragment_manager->addFragmentGraphFromFile(fragment_file.c_str()).asChar();
        m_shader = shader_manager->getFragmentShader(my_fragment_name, "outSurfaceFinal", true);
    }

    MHWRender::MRenderItem* my_render_item = nullptr;

    int index = list.indexOf("my_render_item");

    if (index < 0) {
        my_render_item = MHWRender::MRenderItem::Create(
            "my_render_item",
            MHWRender::MRenderItem::MaterialSceneItem,
            MHWRender::MGeometry::kTriangles);
        my_render_item->setDrawMode(MHWRender::MGeometry::kAll);
        my_render_item->depthPriority(5);

        list.append(my_render_item);
    } else {
        my_render_item = list.itemAt(index);
    }

    if (my_render_item && m_shader) {
        my_render_item->setShader(m_shader);
        my_render_item->enable(true);
    }
}

void FragmentDisplayGeometryOverride::populateGeometry(
    const MHWRender::MGeometryRequirements& requirements,
    const MHWRender::MRenderItemList& renderItems,
    MHWRender::MGeometry& data)
{
    MHWRender::MVertexBuffer* position_buffer = nullptr;
    MHWRender::MVertexBuffer* normal_buffer = nullptr;
    MHWRender::MVertexBuffer* uv_buffer = nullptr;
    MHWRender::MVertexBuffer* tangent_buffer = nullptr;
    MHWRender::MVertexBuffer* bitangent_buffer = nullptr;

    const MHWRender::MVertexBufferDescriptorList& vertexBufferDescriptorList = requirements.vertexRequirements();
    MHWRender::MVertexBufferDescriptor vertexBufferDescriptor;

    for (int i = 0; i < vertexBufferDescriptorList.length(); ++i) {
        if (!vertexBufferDescriptorList.getDescriptor(i, vertexBufferDescriptor)) {
            continue;
        }

        switch (vertexBufferDescriptor.semantic()) {
        case MHWRender::MGeometry::kPosition:
            position_buffer = data.createVertexBuffer(vertexBufferDescriptor);
            position_buffer->update(cube_positions, 0, cube_vertex_count, false);
            break;
        case MHWRender::MGeometry::kNormal:
            normal_buffer = data.createVertexBuffer(vertexBufferDescriptor);
            normal_buffer->update(cube_normals, 0, cube_vertex_count, false);
            break;
        case MHWRender::MGeometry::kTexture:
            uv_buffer = data.createVertexBuffer(vertexBufferDescriptor);
            uv_buffer->update(cube_uvs, 0, cube_vertex_count, false);
            break;
        case MHWRender::MGeometry::kTangent:
            tangent_buffer = data.createVertexBuffer(vertexBufferDescriptor);
            tangent_buffer->update(cube_tangents, 0, cube_vertex_count, false);
            break;
        case MHWRender::MGeometry::kBitangent:
            bitangent_buffer = data.createVertexBuffer(vertexBufferDescriptor);
            bitangent_buffer->update(cube_bitangents, 0, cube_vertex_count, false);
            break;
        default:
            break;
        }
    }

    MHWRender::MIndexBuffer* index_buffer = data.createIndexBuffer(MHWRender::MGeometry::kUnsignedInt32);
    index_buffer->update(cube_indices, 0, cube_index_count, false);

    const auto render_item_index = renderItems.indexOf("my_render_item");
    if (render_item_index < 0) {
        return;
    }
    const MHWRender::MRenderItem* item = renderItems.itemAt(render_item_index);
    item->associateWithIndexBuffer(index_buffer);
}


// === Plugin main =============================================================

MStatus PLUGIN_EXPORT initializePlugin(MObject obj)
{
    MStatus status;
    MFnPlugin plugin(obj, "", "", "Any");

    g_plugin_path = plugin.loadPath().asChar();

    status = plugin.registerNode(
                FragmentDisplay::typeName,
                FragmentDisplay::id,
                &FragmentDisplay::creator,
                &FragmentDisplay::initialize,
                MPxNode::kLocatorNode,
                &FragmentDisplay::drawDbClassification);
    if (!status) {
        status.perror("registerNode");
        return status;
    }

    status = MHWRender::MDrawRegistry::registerGeometryOverrideCreator(
        FragmentDisplay::drawDbClassification,
        FragmentDisplay::drawRegistrantId,
        FragmentDisplayGeometryOverride::Creator);
    if (!status) {
        status.perror("registerDrawOverrideCreator");
        return status;
    }

    return status;
}

MStatus PLUGIN_EXPORT uninitializePlugin(MObject obj)
{
    MStatus status;
    MFnPlugin plugin(obj);

    status = MHWRender::MDrawRegistry::deregisterGeometryOverrideCreator(
        FragmentDisplay::drawDbClassification,
        FragmentDisplay::drawRegistrantId);
    if (!status) {
        status.perror("deregisterDrawOverrideCreator");
        return status;
    }

    status = plugin.deregisterNode(FragmentDisplay::id);
    if (!status) {
        status.perror("deregisterNode");
        return status;
    }

    return status;
}
