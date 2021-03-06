/**
 * @file
 */

#pragma once

#include "ui/Window.h"
#include "core/Common.h"
#include "../Client.h"

namespace frontend {

class HudWindow: public ui::Window {
private:
	Client* _client;
public:
	HudWindow(Client* client, const glm::ivec2& dimension) :
			ui::Window(client), _client(client) {
		core_assert_always(loadResourceFile("ui/window/client-hud.tb.txt"));
		SetSettings(tb::WINDOW_SETTINGS_NONE);
		SetSize(dimension.x, 20);
	}

	bool OnEvent(const tb::TBWidgetEvent &ev) override {
		if (ev.type == tb::EVENT_TYPE_CLICK && ev.target->GetID() == TBIDC("disconnect")) {
			_client->disconnect();
			return true;
		}
		return ui::Window::OnEvent(ev);
	}
};

}
