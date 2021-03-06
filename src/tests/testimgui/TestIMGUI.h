/**
 * @file
 */

#pragma once

#include "imgui/IMGUIApp.h"
#include "imgui/IMGUI.h"

/**
 * @brief Renders the imgui demo
 */
class TestIMGUI: public imgui::IMGUIApp {
private:
	using Super = imgui::IMGUIApp;
	bool _showTestWindow = false;
	bool _showMetricsWindow = false;

public:
	TestIMGUI(const io::FilesystemPtr& filesystem, const core::EventBusPtr& eventBus, const core::TimeProviderPtr& timeProvider);

	core::AppState onInit() override;
	void onRenderUI() override;
};
