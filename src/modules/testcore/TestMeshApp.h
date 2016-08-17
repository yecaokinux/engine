/**
 * @file
 */

#pragma once

#include "testcore/TestApp.h"
#include "video/DepthBuffer.h"
#include "video/MeshPool.h"
#include "video/VertexBuffer.h"
#include "video/SunLight.h"
#include "FrontendShaders.h"

class TestMeshApp: public TestApp {
private:
	using Super = TestApp;
protected:
	video::DepthBuffer _depthBuffer;
	video::MeshPtr _mesh;
	video::MeshPool _meshPool;
	shader::MeshShader _meshShader;
	shader::ColorShader _colorShader;
	video::SunLight _sunLight;
	shader::ShadowmapShader _shadowMapShader;
	glm::vec3 _diffuseColor = glm::vec3(1.0, 1.0, 1.0);

	virtual void renderPlane();
	virtual void doRender() override;
public:
	TestMeshApp(io::FilesystemPtr filesystem, core::EventBusPtr eventBus);

	core::AppState onInit() override;
	core::AppState onCleanup() override;

	void onMouseWheel(int32_t x, int32_t y) override;
};