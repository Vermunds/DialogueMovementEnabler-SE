#include "Settings.h"

#include <SimpleIni.h>

namespace
{
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

		constexpr const char* iniPath = R"(.\Data\SKSE\Plugins\DialogueMovementEnabler.ini)";

		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(iniPath);

		SKSE::log::info("Loading settings from: {}", std::filesystem::absolute(iniPath).string());

		IniSection(ini, "GENERAL");
		settings->unlockCamera = IniGetBool(ini, "GENERAL", "bUnlockCamera", true, "#  Unlocks camera rotation so you can look around in any direction.");

		IniSection(ini, "CONTROLS_KEYBOARD");
		settings->allowMovement[ControlType::kKeyboardMouse] = IniGetBool(ini, "CONTROLS_KEYBOARD", "bAllowMovement", true);
		settings->allowRun[ControlType::kKeyboardMouse] = IniGetBool(ini, "CONTROLS_KEYBOARD", "bAllowRun", false);
		settings->allowToggleRun[ControlType::kKeyboardMouse] = IniGetBool(ini, "CONTROLS_KEYBOARD", "bAllowToggleRun", false);
		settings->allowJump[ControlType::kKeyboardMouse] = IniGetBool(ini, "CONTROLS_KEYBOARD", "bAllowJump", false);
		settings->allowSprint[ControlType::kKeyboardMouse] = IniGetBool(ini, "CONTROLS_KEYBOARD", "bAllowSprint", false);
		settings->allowTogglePOV[ControlType::kKeyboardMouse] = IniGetBool(ini, "CONTROLS_KEYBOARD", "bAllowTogglePOV", false);
		settings->allowSneak[ControlType::kKeyboardMouse] = IniGetBool(ini, "CONTROLS_KEYBOARD", "bAllowSneak", false);
		settings->allowReadyWeapon[ControlType::kKeyboardMouse] = IniGetBool(ini, "CONTROLS_KEYBOARD", "bAllowReadyWeapon", false);
		settings->allowLeftAttack[ControlType::kKeyboardMouse] = IniGetBool(ini, "CONTROLS_KEYBOARD", "bAllowLeftAttack", false);
		settings->allowRightAttack[ControlType::kKeyboardMouse] = IniGetBool(ini, "CONTROLS_KEYBOARD", "bAllowRightAttack", false);
		settings->allowHotkeys[ControlType::kKeyboardMouse] = IniGetBool(ini, "CONTROLS_KEYBOARD", "bAllowHotkeys", false);
		settings->allowShout[ControlType::kKeyboardMouse] = IniGetBool(ini, "CONTROLS_KEYBOARD", "bAllowShout", false);

		IniSection(ini, "CONTROLS_CONTROLLER");
		settings->allowMovement[ControlType::kController] = IniGetBool(ini, "CONTROLS_CONTROLLER", "bAllowMovement", true);
		settings->allowRun[ControlType::kController] = IniGetBool(ini, "CONTROLS_CONTROLLER", "bAllowRun", false);
		settings->allowToggleRun[ControlType::kController] = IniGetBool(ini, "CONTROLS_CONTROLLER", "bAllowToggleRun", false);
		settings->allowJump[ControlType::kController] = IniGetBool(ini, "CONTROLS_CONTROLLER", "bAllowJump", false);
		settings->allowSprint[ControlType::kController] = IniGetBool(ini, "CONTROLS_CONTROLLER", "bAllowSprint", false);
		settings->allowTogglePOV[ControlType::kController] = IniGetBool(ini, "CONTROLS_CONTROLLER", "bAllowTogglePOV", false);
		settings->allowSneak[ControlType::kController] = IniGetBool(ini, "CONTROLS_CONTROLLER", "bAllowSneak", false);
		settings->allowReadyWeapon[ControlType::kController] = IniGetBool(ini, "CONTROLS_CONTROLLER", "bAllowReadyWeapon", false);
		settings->allowLeftAttack[ControlType::kController] = IniGetBool(ini, "CONTROLS_CONTROLLER", "bAllowLeftAttack", false);
		settings->allowRightAttack[ControlType::kController] = IniGetBool(ini, "CONTROLS_CONTROLLER", "bAllowRightAttack", false);
		settings->allowHotkeys[ControlType::kController] = IniGetBool(ini, "CONTROLS_CONTROLLER", "bAllowHotkeys", false);
		settings->allowShout[ControlType::kController] = IniGetBool(ini, "CONTROLS_CONTROLLER", "bAllowShout", false);

		IniSection(ini, "AUTOCLOSE");
		settings->autoCloseMenus = IniGetBool(ini, "AUTOCLOSE", "bAutoCloseMenus", true, "#  Enable or disable auto-closing of the dialogue menu if the distance to the speaker is too large.");
		settings->autoCloseDistance = IniGetFloat(ini, "AUTOCLOSE", "fAutoCloseDistance", 1350.f, "#  The maximum distance (in in-game units) before the menu will automatically close.\n#  Too small values can cause the menus to close immediately.");
		settings->autoCloseTolerance = IniGetFloat(ini, "AUTOCLOSE", "fAutoCloseTolerance", 400.f, "#  The maximum distance (in in-game units) where the menu NEVER auto-closes (in relation to the players initial position when the menu was opened).\n#  This is used as a failsafe when the initial distance is larger than the maximum allowed to prevent the menu from closing immediately.");

		SKSE::log::info("Settings loaded.");

		ini.SaveFile(iniPath);
	}
}
