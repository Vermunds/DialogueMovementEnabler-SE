#pragma once

namespace DME
{
	// General
	constexpr bool UNLOCK_CAMERA_DEFAULT_VALUE = true;

	// Controls (Keyboard)
	constexpr bool ALLOW_MOVEMENT_KEYBOARD_DEFAULT_VALUE = true;
	constexpr bool ALLOW_RUN_KEYBOARD_DEFAULT_VALUE = true;
	constexpr bool ALLOW_TOGGLE_RUN_KEYBOARD_DEFAULT_VALUE = true;
	constexpr bool ALLOW_JUMP_KEYBOARD_DEFAULT_VALUE = false;
	constexpr bool ALLOW_SPRINT_KEYBOARD_DEFAULT_VALUE = false;
	constexpr bool ALLOW_TOGGLE_POV_KEYBOARD_DEFAULT_VALUE = false;
	constexpr bool ALLOW_SNEAK_KEYBOARD_DEFAULT_VALUE = false;
	constexpr bool ALLOW_READY_WEAPON_KEYBOARD_DEFAULT_VALUE = false;
	constexpr bool ALLOW_LEFT_ATTACK_KEYBOARD_DEFAULT_VALUE = false;
	constexpr bool ALLOW_RIGHT_ATTACK_KEYBOARD_DEFAULT_VALUE = false;
	constexpr bool ALLOW_HOTKEYS_KEYBOARD_DEFAULT_VALUE = false;
	constexpr bool ALLOW_SHOUT_KEYBOARD_DEFAULT_VALUE = false;

	// Controls (Controller)
	constexpr bool ALLOW_MOVEMENT_CONTROLLER_DEFAULT_VALUE = true;
	constexpr bool ALLOW_RUN_CONTROLLER_DEFAULT_VALUE = true;
	constexpr bool ALLOW_TOGGLE_RUN_CONTROLLER_DEFAULT_VALUE = true;
	constexpr bool ALLOW_JUMP_CONTROLLER_DEFAULT_VALUE = false;
	constexpr bool ALLOW_SPRINT_CONTROLLER_DEFAULT_VALUE = false;
	constexpr bool ALLOW_TOGGLE_POV_CONTROLLER_DEFAULT_VALUE = false;
	constexpr bool ALLOW_SNEAK_CONTROLLER_DEFAULT_VALUE = false;
	constexpr bool ALLOW_READY_WEAPON_CONTROLLER_DEFAULT_VALUE = false;
	constexpr bool ALLOW_LEFT_ATTACK_CONTROLLER_DEFAULT_VALUE = false;
	constexpr bool ALLOW_RIGHT_ATTACK_CONTROLLER_DEFAULT_VALUE = false;
	constexpr bool ALLOW_HOTKEYS_CONTROLLER_DEFAULT_VALUE = false;
	constexpr bool ALLOW_SHOUT_CONTROLLER_DEFAULT_VALUE = false;

	// Auto-close
	constexpr bool AUTO_CLOSE_MENUS_DEFAULT_VALUE = true;
	constexpr float AUTO_CLOSE_DISTANCE_DEFAULT_VALUE = 1350.f;
	constexpr float AUTO_CLOSE_TOLERANCE_DEFAULT_VALUE = 400.f;

	class Settings
	{
	public:
		enum ControlType
		{
			kKeyboardMouse,
			kController,

			kTotal
		};

		static Settings* GetSingleton();

		// General
		bool unlockCamera;

		// Controls
		bool allowMovement[kTotal];
		bool allowJump[kTotal];
		bool allowSprint[kTotal];
		bool allowRun[kTotal];
		bool allowToggleRun[kTotal];
		bool allowSneak[kTotal];
		bool allowTogglePOV[kTotal];
		bool allowReadyWeapon[kTotal];
		bool allowLeftAttack[kTotal];
		bool allowRightAttack[kTotal];
		bool allowHotkeys[kTotal];
		bool allowShout[kTotal];

		// Auto-close
		bool autoCloseMenus;
		float autoCloseDistance;
		float autoCloseTolerance;

	private:
		Settings() {};
		~Settings() {};
		Settings(const Settings&) = delete;
		Settings& operator=(const Settings&) = delete;
	};

	void LoadSettings();
	void SaveSettings();
	void RestoreDefaults();
}
