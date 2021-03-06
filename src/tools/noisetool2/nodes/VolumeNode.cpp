#include "VolumeNode.h"
#include "image/Image.h"
#include "core/GLM.h"
#include "core/Log.h"
#include "video/Camera.h"
#include "NNode.h"
#include "voxel/polyvox/RawVolume.h"
#include "voxel/MaterialColor.h"

VolumeNode::VolumeNode() {
	_camera.setRotationType(video::CameraRotationType::Target);
	_camera.setMode(video::CameraMode::Perspective);
}

VolumeNode::~VolumeNode() {
	_frameBuffer.shutdown();
	_rawVolumeRenderer.shutdown();
}

void VolumeNode::onEdited() {
	NNode* noise = dynamic_cast<NNode*>(nge->getInputNodeForNodeAndSlot(this, 0));
	if (noise == nullptr) {
		Log::info("No input node set");
		return;
	}

	const voxel::Region region(0, 0, 0, volumeWidth - 1, volumeHeight - 1, volumeDepth - 1);
	delete _rawVolumeRenderer.setVolume(0, new voxel::RawVolume(region));

	// TODO: thread me

	const float threshold = getThreshold();
	voxel::RawVolume* v = _rawVolumeRenderer.volume();
	const voxel::Voxel& voxel = voxel::createColorVoxel(voxel::VoxelType::Grass, 0);
	voxelCnt = 0;
	for (int x = 0; x < volumeWidth; ++x) {
		for (int y = 0; y < volumeHeight; ++y) {
			for (int z = 0; z < volumeDepth; ++z) {
				const float n = noise->getNoise(x, y);
				if (n > threshold) {
					v->setVoxel(x, y, z, voxel);
					++voxelCnt;
				}
			}
		}
	}

	_rawVolumeRenderer.extractAll();

	_camera.setAngles(0.0f, 0.0f, 0.0f);
	const glm::ivec3& center = region.getCentre();
	_camera.setTarget(center);
	_camera.setPosition(glm::vec3(-center.x, region.getHeightInVoxels() + center.y, -center.z));
	_camera.lookAt(center);
}

void VolumeNode::getDefaultTitleBarColors(ImU32& defaultTitleTextColorOut, ImU32& defaultTitleBgColorOut, float& defaultTitleBgColorGradientOut) const {
	defaultTitleTextColorOut = IM_COL32(230, 180, 180, 255);
	defaultTitleBgColorOut = IM_COL32(40, 55, 55, 200);
	defaultTitleBgColorGradientOut = 0.025f;
}

bool VolumeNode::render(float nodeWidth) {
	const bool retVal = NodeBase::render(nodeWidth);
	int vertices = 0;
	int indices = 0;
	voxel::RawVolume* v = _rawVolumeRenderer.volume(0);
	if (v != nullptr) {
		_camera.update(0L);

		_frameBuffer.bind();
		_rawVolumeRenderer.render(_camera);
		_frameBuffer.unbind();

		const voxel::Mesh* mesh = _rawVolumeRenderer.mesh(0);
		vertices = mesh->getNoOfVertices();
		indices = mesh->getNoOfIndices();
	}

	// the fbo is flipped in memory, we have to deal with it here
	// TODO: opengl specific
	const glm::ivec2& dim = _frameBuffer.dimension();
	ImGui::Image((ImTextureID) (intptr_t) _frameBuffer.texture(), dim, ImVec2(0.0f, 1.0f), ImVec2(1.0, 0.0f));
	if (ImGui::IsItemHovered()) {
		const ImGuiIO& io = ImGui::GetIO();
		const glm::vec3 delta(io.MouseDelta.y, io.MouseDelta.x, 0.0f);
		_camera.rotate(delta * _rotationSpeed->floatVal());
		const float targetDistance = glm::clamp(_camera.targetDistance() - io.MouseWheel, 0.0f, _camera.farPlane());
		_camera.setTargetDistance(targetDistance);
	}

	ImGui::Text("Voxels: %i, vertices: %i, indices: %i", voxelCnt, vertices, indices);
	return retVal;
}

bool VolumeNode::onInit() {
	glm::ivec2 dimension(300, 300);
	if (!_frameBuffer.init(dimension)) {
		Log::error("Failed to initialize the frame buffer");
		return false;
	}
	if (!_rawVolumeRenderer.init()) {
		Log::error("Failed to initialize the raw volume renderer");
		return false;
	}
	if (!_rawVolumeRenderer.onResize(glm::ivec2(), dimension)) {
		Log::error("Failed to initialize the raw volume renderer");
		return false;
	}

	_frameBuffer.bind();
	video::clear(video::ClearFlag::Color);
	_frameBuffer.unbind();

	_camera.init(glm::ivec2(), _frameBuffer.dimension());
	_rotationSpeed = core::Var::getSafe(cfg::ClientMouseRotationSpeed);

	return true;
}

float VolumeNode::getThreshold() {
	float defaultNoiseThreshold = 0.5f;
	NNode* noise = dynamic_cast<NNode*>(nge->getInputNodeForNodeAndSlot(this, 1));
	if (noise == nullptr) {
		return defaultNoiseThreshold;
	}
	return noise->getNoise(0, 0);
}

VolumeNode* VolumeNode::Create(const ImVec2& pos, ImGui::NodeGraphEditor& nge) {
	VolumeNode* node = imguiAlloc<VolumeNode>();
	if (!node->setup(nge, pos, "noise;threshold", nullptr, NodeType::Volume)) {
		return nullptr;
	}
	node->fields.addField(&node->volumeWidth, 1, "Width", "Volume width", 0, 32, 512);
	node->fields.addField(&node->volumeHeight, 1, "Height", "Volume height", 0, 32, 128);
	node->fields.addField(&node->volumeDepth, 1, "Depth", "Volume depth", 0, 32, 512);
	return node;
}
