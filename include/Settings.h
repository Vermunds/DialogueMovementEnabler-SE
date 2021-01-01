#pragma once

namespace DME
{
	class Settings
	{
	private:
		static Settings* singleton;
		Settings();

	public:
		static Settings* GetSingleton();

		bool unlockCamera;
		bool allowToggleRun;

		bool autoCloseMenus;
		float autoCloseDistance;
		float autoCloseTolerance;
	};

	void LoadSettings();
}
