#include "Settings.h"

#include "SimpleIni.h"

namespace DME
{
	Settings* Settings::singleton = nullptr;

	Settings::Settings() {}

	Settings* Settings::GetSingleton()
	{
		if (singleton)
		{
			return singleton;
		}
		singleton = new Settings();
		return singleton;
	}

	void LoadSettings()
	{
		Settings* settings = Settings::GetSingleton();

		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(R"(.\Data\SKSE\Plugins\DialogueMovementEnabler.ini)");

		// General
		settings->unlockCamera = ini.GetBoolValue("GENERAL", "bUnlockCamera", true);
		ini.SetBoolValue("GENERAL", "bUnlockCamera", settings->unlockCamera, "#  Unlocks camera rotation so you can look around in any direction.", true);

		settings->allowToggleRun = ini.GetBoolValue("GENERAL", "bAllowToggleRun", true);
		ini.SetBoolValue("GENERAL", "bAllowToggleRun", settings->allowToggleRun, "#  Allows the toggle walk/run control (Caps Lock by default).", true);

		//Auto-close
		settings->autoCloseMenus = ini.GetBoolValue("AUTOCLOSE", "bAutoCloseMenus", true);
		settings->autoCloseDistance = static_cast<float>(ini.GetDoubleValue("AUTOCLOSE", "fAutoCloseDistance", 1350.0));
		settings->autoCloseTolerance = static_cast<float>(ini.GetDoubleValue("AUTOCLOSE", "fAutoCloseTolerance", 400.0));

		ini.SetBoolValue("AUTOCLOSE", "bAutoCloseMenus", settings->autoCloseMenus, "#  Enable or disable auto-closing of the dialogue menu if the distance to the speaker is too large.", true);
		ini.SetDoubleValue("AUTOCLOSE", "fAutoCloseDistance", settings->autoCloseDistance, "#  The maximum distance (in in-game units) before the menu will automatically close.\n#  Too small values can cause the menus to close immediately.", true);
		ini.SetDoubleValue("AUTOCLOSE", "fAutoCloseTolerance", settings->autoCloseTolerance, "#  The maximum distance (in in-game units) where the menu NEVER auto-closes (in relation to the players initial position when the menu was opened).\n#  This is used as a failsafe when the initial distance is larger than the maximum allowed to prevent the menu from closing immediately.", true);


		ini.SaveFile(R"(.\Data\SKSE\Plugins\DialogueMovementEnabler.ini)");
	}
}
