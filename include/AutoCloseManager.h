#pragma once

namespace DME
{
	class AutoCloseManager
	{
	public:
		void CheckAutoClose();
		void InitAutoClose(RE::TESObjectREFR* a_ref);

		static AutoCloseManager* GetSingleton();

	private:
		AutoCloseManager() {};
		~AutoCloseManager() {};
		AutoCloseManager(const AutoCloseManager&) = delete;
		AutoCloseManager& operator=(const AutoCloseManager&) = delete;

		struct AutoCloseData
		{
			RE::ObjectRefHandle target;
			float initialDistance = 0.0f;
			float minDistance = 0.0f;

			void PrintDebugInfo() const;
		};
		AutoCloseData _data;

		void CloseMenu(const std::string& a_reason);

		static float GetBBDistance(const RE::TESObjectREFR* a_refA, const RE::TESObjectREFR* a_refB);
		static std::string GetRefDebugString(const RE::TESObjectREFR* a_ref);
	};
}
