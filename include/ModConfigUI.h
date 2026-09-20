#pragma once

#include <ModConfigUI/ModConfigUI.h>

namespace DME
{
	void InstallModConfigUI();

	void DrawSettingsPage(ModConfigUI::Renderer& a_renderer);
	void DrawControlsPage(ModConfigUI::Renderer& a_renderer);
}
