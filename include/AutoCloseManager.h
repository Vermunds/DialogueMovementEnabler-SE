#pragma once

#include <map>

namespace DME
{
	class AutoCloseManager
	{
	public:
		void CheckAutoClose();
		void InitAutoClose(RE::TESObjectREFR* a_ref);

		static AutoCloseManager* GetSingleton();

	private:
		AutoCloseManager(){};
		~AutoCloseManager(){};
		AutoCloseManager(const AutoCloseManager&) = delete;
		AutoCloseManager& operator=(const AutoCloseManager&) = delete;
		
		struct AutoCloseData
		{
			RE::TESObjectREFR* target = nullptr;
			float initialDistance = 0.0f;
			float minDistance = 0.0f;
		};
		AutoCloseData* _data;

		float GetDistance(RE::NiPoint3 a_playerPos, float a_playerHeight, RE::NiPoint3 a_refPos);
	};
}
