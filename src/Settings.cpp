#include "Settings.h"

#include <SimpleIni.h>

namespace
{
	constexpr const char* INI_PATH = R"(.\Data\SKSE\Plugins\DialogueMovementEnabler.ini)";

	void IniSection(CSimpleIniA& a_ini, const char* a_section, const char* a_comment = nullptr)
	{
		a_ini.SetValue(a_section, nullptr, nullptr, a_comment);
		SKSE::log::info("[{}]", a_section);
	}

	bool IniGetBool(CSimpleIniA& a_ini, const char* a_section, const char* a_key, bool a_default, const char* a_comment = nullptr)
	{
		bool val = a_ini.GetBoolValue(a_section, a_key, a_default);
		a_ini.SetBoolValue(a_section, a_key, val, a_comment, true);
		SKSE::log::info("  {}: {}", a_key, val);
		return val;
	}

	float IniGetFloat(CSimpleIniA& a_ini, const char* a_section, const char* a_key, float a_default, const char* a_comment = nullptr)
	{
		float val = static_cast<float>(a_ini.GetDoubleValue(a_section, a_key, a_default));
		a_ini.SetDoubleValue(a_section, a_key, val, a_comment, true);
		SKSE::log::info("  {}: {}", a_key, val);
		return val;
	}
}

namespace DME
{
	Settings* Settings::GetSingleton()
	{
		static Settings singleton;
		return &singleton;
	}

	void LoadSettings()
	{
		using ControlType = Settings::ControlType;

		Settings* settings = Settings::GetSingleton();

		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(INI_PATH);

		SKSE::log::info("Loading settings from: {}", std::filesystem::absolute(INI_PATH).string());

		IniSection(ini, "GENERAL");
		settings->unlockCamera = IniGetBool(ini, "GENERAL", "bUnlockCamera", UNLOCK_CAMERA_DEFAULT_VALUE, "#  Unlocks camera rotation so you can look around in any direction.");

		IniSection(ini, "CONTROLS_KEYBOARD");
		settings->allowMovement[ControlType::kKeyboardMouse] = IniGetBool(ini, "CONTROLS_KEYBOARD", "bAllowMovement", ALLOW_MOVEMENT_KEYBOARD_DEFAULT_VALUE);
		settings->allowRun[ControlType::kKeyboardMouse] = IniGetBool(ini, "CONTROLS_KEYBOARD", "bAllowRun", ALLOW_RUN_KEYBOARD_DEFAULT_VALUE);
		settings->allowToggleRun[ControlType::kKeyboardMouse] = IniGetBool(ini, "CONTROLS_KEYBOARD", "bAllowToggleRun", ALLOW_TOGGLE_RUN_KEYBOARD_DEFAULT_VALUE);
		settings->allowJump[ControlType::kKeyboardMouse] = IniGetBool(ini, "CONTROLS_KEYBOARD", "bAllowJump", ALLOW_JUMP_KEYBOARD_DEFAULT_VALUE);
		settings->allowSprint[ControlType::kKeyboardMouse] = IniGetBool(ini, "CONTROLS_KEYBOARD", "bAllowSprint", ALLOW_SPRINT_KEYBOARD_DEFAULT_VALUE);
		settings->allowTogglePOV[ControlType::kKeyboardMouse] = IniGetBool(ini, "CONTROLS_KEYBOARD", "bAllowTogglePOV", ALLOW_TOGGLE_POV_KEYBOARD_DEFAULT_VALUE);
		settings->allowSneak[ControlType::kKeyboardMouse] = IniGetBool(ini, "CONTROLS_KEYBOARD", "bAllowSneak", ALLOW_SNEAK_KEYBOARD_DEFAULT_VALUE);
		settings->allowReadyWeapon[ControlType::kKeyboardMouse] = IniGetBool(ini, "CONTROLS_KEYBOARD", "bAllowReadyWeapon", ALLOW_READY_WEAPON_KEYBOARD_DEFAULT_VALUE);
		settings->allowLeftAttack[ControlType::kKeyboardMouse] = IniGetBool(ini, "CONTROLS_KEYBOARD", "bAllowLeftAttack", ALLOW_LEFT_ATTACK_KEYBOARD_DEFAULT_VALUE);
		settings->allowRightAttack[ControlType::kKeyboardMouse] = IniGetBool(ini, "CONTROLS_KEYBOARD", "bAllowRightAttack", ALLOW_RIGHT_ATTACK_KEYBOARD_DEFAULT_VALUE);
		settings->allowHotkeys[ControlType::kKeyboardMouse] = IniGetBool(ini, "CONTROLS_KEYBOARD", "bAllowHotkeys", ALLOW_HOTKEYS_KEYBOARD_DEFAULT_VALUE);
		settings->allowShout[ControlType::kKeyboardMouse] = IniGetBool(ini, "CONTROLS_KEYBOARD", "bAllowShout", ALLOW_SHOUT_KEYBOARD_DEFAULT_VALUE);

		IniSection(ini, "CONTROLS_CONTROLLER");
		settings->allowMovement[ControlType::kController] = IniGetBool(ini, "CONTROLS_CONTROLLER", "bAllowMovement", ALLOW_MOVEMENT_CONTROLLER_DEFAULT_VALUE);
		settings->allowRun[ControlType::kController] = IniGetBool(ini, "CONTROLS_CONTROLLER", "bAllowRun", ALLOW_RUN_CONTROLLER_DEFAULT_VALUE);
		settings->allowToggleRun[ControlType::kController] = IniGetBool(ini, "CONTROLS_CONTROLLER", "bAllowToggleRun", ALLOW_TOGGLE_RUN_CONTROLLER_DEFAULT_VALUE);
		settings->allowJump[ControlType::kController] = IniGetBool(ini, "CONTROLS_CONTROLLER", "bAllowJump", ALLOW_JUMP_CONTROLLER_DEFAULT_VALUE);
		settings->allowSprint[ControlType::kController] = IniGetBool(ini, "CONTROLS_CONTROLLER", "bAllowSprint", ALLOW_SPRINT_CONTROLLER_DEFAULT_VALUE);
		settings->allowTogglePOV[ControlType::kController] = IniGetBool(ini, "CONTROLS_CONTROLLER", "bAllowTogglePOV", ALLOW_TOGGLE_POV_CONTROLLER_DEFAULT_VALUE);
		settings->allowSneak[ControlType::kController] = IniGetBool(ini, "CONTROLS_CONTROLLER", "bAllowSneak", ALLOW_SNEAK_CONTROLLER_DEFAULT_VALUE);
		settings->allowReadyWeapon[ControlType::kController] = IniGetBool(ini, "CONTROLS_CONTROLLER", "bAllowReadyWeapon", ALLOW_READY_WEAPON_CONTROLLER_DEFAULT_VALUE);
		settings->allowLeftAttack[ControlType::kController] = IniGetBool(ini, "CONTROLS_CONTROLLER", "bAllowLeftAttack", ALLOW_LEFT_ATTACK_CONTROLLER_DEFAULT_VALUE);
		settings->allowRightAttack[ControlType::kController] = IniGetBool(ini, "CONTROLS_CONTROLLER", "bAllowRightAttack", ALLOW_RIGHT_ATTACK_CONTROLLER_DEFAULT_VALUE);
		settings->allowHotkeys[ControlType::kController] = IniGetBool(ini, "CONTROLS_CONTROLLER", "bAllowHotkeys", ALLOW_HOTKEYS_CONTROLLER_DEFAULT_VALUE);
		settings->allowShout[ControlType::kController] = IniGetBool(ini, "CONTROLS_CONTROLLER", "bAllowShout", ALLOW_SHOUT_CONTROLLER_DEFAULT_VALUE);

		IniSection(ini, "AUTOCLOSE");
		settings->autoCloseMenus = IniGetBool(ini, "AUTOCLOSE", "bAutoCloseMenus", AUTO_CLOSE_MENUS_DEFAULT_VALUE, "#  Enable or disable auto-closing of the dialogue menu if the distance to the speaker is too large.");
		settings->autoCloseDistance = IniGetFloat(ini, "AUTOCLOSE", "fAutoCloseDistance", AUTO_CLOSE_DISTANCE_DEFAULT_VALUE, "#  The maximum distance (in in-game units) before the menu will automatically close.\n#  Too small values can cause the menus to close immediately.");
		settings->autoCloseTolerance = IniGetFloat(ini, "AUTOCLOSE", "fAutoCloseTolerance", AUTO_CLOSE_TOLERANCE_DEFAULT_VALUE, "#  The maximum distance (in in-game units) where the menu NEVER auto-closes (in relation to the players initial position when the menu was opened).\n#  This is used as a failsafe when the initial distance is larger than the maximum allowed to prevent the menu from closing immediately.");

		SKSE::log::info("Settings loaded.");

		ini.SaveFile(INI_PATH);
	}

	void SaveSettings()
	{
		using ControlType = Settings::ControlType;

		Settings* settings = Settings::GetSingleton();

		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(INI_PATH);

		ini.SetBoolValue("GENERAL", "bUnlockCamera", settings->unlockCamera, nullptr, true);

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

		ini.SetBoolValue("AUTOCLOSE", "bAutoCloseMenus", settings->autoCloseMenus, nullptr, true);
		ini.SetDoubleValue("AUTOCLOSE", "fAutoCloseDistance", settings->autoCloseDistance, nullptr, true);
		ini.SetDoubleValue("AUTOCLOSE", "fAutoCloseTolerance", settings->autoCloseTolerance, nullptr, true);

		ini.SaveFile(INI_PATH);

		SKSE::log::info("Settings saved.");
	}

	void RestoreDefaults()
	{
		using ControlType = Settings::ControlType;

		Settings* settings = Settings::GetSingleton();

		settings->unlockCamera = UNLOCK_CAMERA_DEFAULT_VALUE;

		settings->allowMovement[ControlType::kKeyboardMouse] = ALLOW_MOVEMENT_KEYBOARD_DEFAULT_VALUE;
		settings->allowRun[ControlType::kKeyboardMouse] = ALLOW_RUN_KEYBOARD_DEFAULT_VALUE;
		settings->allowToggleRun[ControlType::kKeyboardMouse] = ALLOW_TOGGLE_RUN_KEYBOARD_DEFAULT_VALUE;
		settings->allowJump[ControlType::kKeyboardMouse] = ALLOW_JUMP_KEYBOARD_DEFAULT_VALUE;
		settings->allowSprint[ControlType::kKeyboardMouse] = ALLOW_SPRINT_KEYBOARD_DEFAULT_VALUE;
		settings->allowTogglePOV[ControlType::kKeyboardMouse] = ALLOW_TOGGLE_POV_KEYBOARD_DEFAULT_VALUE;
		settings->allowSneak[ControlType::kKeyboardMouse] = ALLOW_SNEAK_KEYBOARD_DEFAULT_VALUE;
		settings->allowReadyWeapon[ControlType::kKeyboardMouse] = ALLOW_READY_WEAPON_KEYBOARD_DEFAULT_VALUE;
		settings->allowLeftAttack[ControlType::kKeyboardMouse] = ALLOW_LEFT_ATTACK_KEYBOARD_DEFAULT_VALUE;
		settings->allowRightAttack[ControlType::kKeyboardMouse] = ALLOW_RIGHT_ATTACK_KEYBOARD_DEFAULT_VALUE;
		settings->allowHotkeys[ControlType::kKeyboardMouse] = ALLOW_HOTKEYS_KEYBOARD_DEFAULT_VALUE;
		settings->allowShout[ControlType::kKeyboardMouse] = ALLOW_SHOUT_KEYBOARD_DEFAULT_VALUE;

		settings->allowMovement[ControlType::kController] = ALLOW_MOVEMENT_CONTROLLER_DEFAULT_VALUE;
		settings->allowRun[ControlType::kController] = ALLOW_RUN_CONTROLLER_DEFAULT_VALUE;
		settings->allowToggleRun[ControlType::kController] = ALLOW_TOGGLE_RUN_CONTROLLER_DEFAULT_VALUE;
		settings->allowJump[ControlType::kController] = ALLOW_JUMP_CONTROLLER_DEFAULT_VALUE;
		settings->allowSprint[ControlType::kController] = ALLOW_SPRINT_CONTROLLER_DEFAULT_VALUE;
		settings->allowTogglePOV[ControlType::kController] = ALLOW_TOGGLE_POV_CONTROLLER_DEFAULT_VALUE;
		settings->allowSneak[ControlType::kController] = ALLOW_SNEAK_CONTROLLER_DEFAULT_VALUE;
		settings->allowReadyWeapon[ControlType::kController] = ALLOW_READY_WEAPON_CONTROLLER_DEFAULT_VALUE;
		settings->allowLeftAttack[ControlType::kController] = ALLOW_LEFT_ATTACK_CONTROLLER_DEFAULT_VALUE;
		settings->allowRightAttack[ControlType::kController] = ALLOW_RIGHT_ATTACK_CONTROLLER_DEFAULT_VALUE;
		settings->allowHotkeys[ControlType::kController] = ALLOW_HOTKEYS_CONTROLLER_DEFAULT_VALUE;
		settings->allowShout[ControlType::kController] = ALLOW_SHOUT_CONTROLLER_DEFAULT_VALUE;

		settings->autoCloseMenus = AUTO_CLOSE_MENUS_DEFAULT_VALUE;
		settings->autoCloseDistance = AUTO_CLOSE_DISTANCE_DEFAULT_VALUE;
		settings->autoCloseTolerance = AUTO_CLOSE_TOLERANCE_DEFAULT_VALUE;

		SaveSettings();
	}
}
