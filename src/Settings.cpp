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
		using ControlType = Settings::ControlType;

		Settings* settings = Settings::GetSingleton();

		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(R"(.\Data\SKSE\Plugins\DialogueMovementEnabler.ini)");

		// General
		settings->unlockCamera = ini.GetBoolValue("GENERAL", "bUnlockCamera", true);
		ini.SetBoolValue("GENERAL", "bUnlockCamera", settings->unlockCamera, "#  Unlocks camera rotation so you can look around in any direction.", true);

		settings->freeLook = ini.GetBoolValue("GENERAL", "bFreeLook", true);
		ini.SetBoolValue("GENERAL", "bFreeLook", settings->freeLook, "#  Disables cursor and allows you to look around with the mouse.", true);

		// Controls - kb/m
		settings->allowMovement[ControlType::kKeyboardMouse] = ini.GetBoolValue("CONTROLS_KEYBOARD", "bAllowMovement", true);
		settings->allowRun[ControlType::kKeyboardMouse] = ini.GetBoolValue("CONTROLS_KEYBOARD", "bAllowRun", true);
		settings->allowToggleRun[ControlType::kKeyboardMouse] = ini.GetBoolValue("CONTROLS_KEYBOARD", "bAllowToggleRun", true);
		settings->allowJump[ControlType::kKeyboardMouse] = ini.GetBoolValue("CONTROLS_KEYBOARD", "bAllowJump", true);
		settings->allowSprint[ControlType::kKeyboardMouse] = ini.GetBoolValue("CONTROLS_KEYBOARD", "bAllowSprint", true);
		settings->allowTogglePOV[ControlType::kKeyboardMouse] = ini.GetBoolValue("CONTROLS_KEYBOARD", "bAllowTogglePOV", true);
		settings->allowSneak[ControlType::kKeyboardMouse] = ini.GetBoolValue("CONTROLS_KEYBOARD", "bAllowSneak", true);
		settings->allowReadyWeapon[ControlType::kKeyboardMouse] = ini.GetBoolValue("CONTROLS_KEYBOARD", "bAllowReadyWeapon", true);
		settings->allowLeftAttack[ControlType::kKeyboardMouse] = ini.GetBoolValue("CONTROLS_KEYBOARD", "bAllowLeftAttack", false);
		settings->allowRightAttack[ControlType::kKeyboardMouse] = ini.GetBoolValue("CONTROLS_KEYBOARD", "bAllowRightAttack", false);
		settings->allowHotkeys[ControlType::kKeyboardMouse] = ini.GetBoolValue("CONTROLS_KEYBOARD", "bAllowHotkeys", true);
		settings->allowShout[ControlType::kKeyboardMouse] = ini.GetBoolValue("CONTROLS_KEYBOARD", "bAllowShout", true);

		ini.SetBoolValue("CONTROLS_KEYBOARD", "bAllowMovement", settings->allowMovement[ControlType::kKeyboardMouse], nullptr, true);		
		ini.SetBoolValue("CONTROLS_KEYBOARD", "bAllowRun", settings->allowRun[ControlType::kKeyboardMouse], nullptr, true);		
		ini.SetBoolValue("CONTROLS_KEYBOARD", "bAllowToggleRun", settings->allowToggleRun[ControlType::kKeyboardMouse], nullptr, true);		
		ini.SetBoolValue("CONTROLS_KEYBOARD", "bAllowJump", settings->allowJump[ControlType::kKeyboardMouse], nullptr, true);		
		ini.SetBoolValue("CONTROLS_KEYBOARD", "bAllowSprint", settings->allowSprint[ControlType::kKeyboardMouse], nullptr, true);		
		ini.SetBoolValue("CONTROLS_KEYBOARD", "bAllowTogglePOV", settings->allowTogglePOV[ControlType::kKeyboardMouse], nullptr, true);		
		ini.SetBoolValue("CONTROLS_KEYBOARD", "bAllowSneak", settings->allowSneak[ControlType::kKeyboardMouse], nullptr, true);		
		ini.SetBoolValue("CONTROLS_KEYBOARD", "bAllowReadyWeapon", settings->allowReadyWeapon[ControlType::kKeyboardMouse], nullptr, true);
		ini.SetBoolValue("CONTROLS_KEYBOARD", "bAllowLeftAttack", settings->allowLeftAttack[ControlType::kKeyboardMouse], nullptr, true);
		ini.SetBoolValue("CONTROLS_KEYBOARD", "bAllowRightAttack", settings->allowRightAttack[ControlType::kKeyboardMouse], nullptr, true);
		ini.SetBoolValue("CONTROLS_KEYBOARD", "bAllowHotkeys", settings->allowHotkeys[ControlType::kKeyboardMouse], nullptr, true);
		ini.SetBoolValue("CONTROLS_KEYBOARD", "bAllowShout", settings->allowShout[ControlType::kKeyboardMouse], nullptr, true);

		// Controls - controller
		settings->allowMovement[ControlType::kController] = ini.GetBoolValue("CONTROLS_CONTROLLER", "bAllowMovement", true);
		settings->allowRun[ControlType::kController] = ini.GetBoolValue("CONTROLS_CONTROLLER", "bAllowRun", true);
		settings->allowToggleRun[ControlType::kController] = ini.GetBoolValue("CONTROLS_CONTROLLER", "bAllowToggleRun", true);
		settings->allowJump[ControlType::kController] = ini.GetBoolValue("CONTROLS_CONTROLLER", "bAllowJump", true);
		settings->allowSprint[ControlType::kController] = ini.GetBoolValue("CONTROLS_CONTROLLER", "bAllowSprint", true);
		settings->allowTogglePOV[ControlType::kController] = ini.GetBoolValue("CONTROLS_CONTROLLER", "bAllowTogglePOV", true);
		settings->allowSneak[ControlType::kController] = ini.GetBoolValue("CONTROLS_CONTROLLER", "bAllowSneak", true);
		settings->allowReadyWeapon[ControlType::kController] = ini.GetBoolValue("CONTROLS_CONTROLLER", "bAllowReadyWeapon", true);
		settings->allowLeftAttack[ControlType::kController] = ini.GetBoolValue("CONTROLS_CONTROLLER", "bAllowLeftAttack", true);
		settings->allowRightAttack[ControlType::kController] = ini.GetBoolValue("CONTROLS_CONTROLLER", "bAllowRightAttack", true);
		settings->allowHotkeys[ControlType::kController] = ini.GetBoolValue("CONTROLS_CONTROLLER", "bAllowHotkeys", true);
		settings->allowShout[ControlType::kController] = ini.GetBoolValue("CONTROLS_CONTROLLER", "bAllowShout", true);

		ini.SetBoolValue("CONTROLS_CONTROLLER", "bAllowMovement", settings->allowMovement[ControlType::kController], nullptr, true);
		ini.SetBoolValue("CONTROLS_CONTROLLER", "bAllowRun", settings->allowRun[ControlType::kController], nullptr, true);
		ini.SetBoolValue("CONTROLS_CONTROLLER", "bAllowToggleRun", settings->allowToggleRun[ControlType::kController], nullptr, true);
		ini.SetBoolValue("CONTROLS_CONTROLLER", "bAllowJump", settings->allowJump[ControlType::kController], nullptr, true);
		ini.SetBoolValue("CONTROLS_CONTROLLER", "bAllowSprint", settings->allowSprint[ControlType::kController], nullptr, true);
		ini.SetBoolValue("CONTROLS_CONTROLLER", "bAllowTogglePOV", settings->allowTogglePOV[ControlType::kController], nullptr, true);
		ini.SetBoolValue("CONTROLS_CONTROLLER", "bAllowSneak", settings->allowSneak[ControlType::kController], nullptr, true);
		ini.SetBoolValue("CONTROLS_CONTROLLER", "bAllowReadyWeapon", settings->allowReadyWeapon[ControlType::kController], nullptr, true);
		ini.SetBoolValue("CONTROLS_CONTROLLER", "bAllowLeftAttack", settings->allowLeftAttack[ControlType::kController], nullptr, true);
		ini.SetBoolValue("CONTROLS_CONTROLLER", "bAllowRightAttack", settings->allowRightAttack[ControlType::kController], nullptr, true);
		ini.SetBoolValue("CONTROLS_CONTROLLER", "bAllowHotkeys", settings->allowHotkeys[ControlType::kController], nullptr, true);
		ini.SetBoolValue("CONTROLS_CONTROLLER", "bAllowShout", settings->allowShout[ControlType::kController], nullptr, true);


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
