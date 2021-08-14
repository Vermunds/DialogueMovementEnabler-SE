#pragma once

namespace DME
{
	class Settings
	{
	private:
		static Settings* singleton;
		Settings();

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
		bool freeLook;

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
	};

	void LoadSettings();
}
