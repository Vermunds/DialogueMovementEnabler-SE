#include "AutoCloseManager.h"
#include "Settings.h"

namespace DME
{
	void AutoCloseManager::CheckAutoClose()
	{
		Settings* settings = Settings::GetSingleton();
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

		float maxDistance = settings->autoCloseDistance;
		float tolerance = settings->autoCloseTolerance;

		RE::TESObjectREFRPtr target = _data.target.get();

		if (target && settings->autoCloseMenus)
		{
			float currentDistance = GetBBDistance(player, target.get());
			bool tooFarOnOpen = _data.initialDistance > maxDistance;

			if (!tooFarOnOpen && currentDistance > maxDistance)
			{
				//Normal case
				CloseMenu("Target moved out of range.");
			}
			else if (tooFarOnOpen)
			{
				//Target was opened when it was too far
				if (currentDistance > maxDistance && currentDistance > (_data.minDistance + tolerance))  //Close only if the distance is increasing
				{
					CloseMenu("Target is too far and the distance has increased further");
					return;
				}
			}

			_data.minDistance = (currentDistance < _data.minDistance) ? currentDistance : _data.minDistance;
		}
	}

	void AutoCloseManager::InitAutoClose(RE::TESObjectREFR* a_ref)
	{
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		_data = AutoCloseData{};

		if (a_ref)
		{
			_data.target = a_ref->GetHandle();
		}
		_data.initialDistance = a_ref ? GetBBDistance(player, a_ref) : 0.0f;
		_data.minDistance = _data.initialDistance;

		_data.PrintDebugInfo();
	}

	AutoCloseManager* AutoCloseManager::GetSingleton()
	{
		static AutoCloseManager singleton;
		return &singleton;
	}

	void AutoCloseManager::CloseMenu(const std::string& a_reason)
	{
		SKSE::log::info("Closing {}: {}", RE::DialogueMenu::MENU_NAME, a_reason);

		RE::UIMessageQueue* uiMessageQueue = RE::UIMessageQueue::GetSingleton();
		uiMessageQueue->AddMessage(RE::DialogueMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
	}

	float AutoCloseManager::GetBBDistance(const RE::TESObjectREFR* a_refA, const RE::TESObjectREFR* a_refB)
	{
		if (!a_refA || !a_refB)
		{
			return 0.0f;
		}

		RE::NiPoint3 posA = a_refA->GetPosition();
		RE::NiPoint3 posB = a_refB->GetPosition();

		RE::NiPoint3 aMinA = a_refA->GetBoundMin() + posA;
		RE::NiPoint3 aMaxA = a_refA->GetBoundMax() + posA;
		RE::NiPoint3 aMinB = a_refB->GetBoundMin() + posB;
		RE::NiPoint3 aMaxB = a_refB->GetBoundMax() + posB;

		auto axisDist = [](float minA, float maxA, float minB, float maxB) {
			if (maxA < minB)
			{
				return minB - maxA;  // A is left/below/behind B
			}
			if (maxB < maxA && maxB < minA)
			{
				return minA - maxB;  // B is left/below/behind A
			}
			return 0.0f;  // Overlapping on this axis
		};

		float dx = axisDist(aMinA.x, aMaxA.x, aMinB.x, aMaxB.x);
		float dy = axisDist(aMinA.y, aMaxA.y, aMinB.y, aMaxB.y);
		float dz = axisDist(aMinA.z, aMaxA.z, aMinB.z, aMaxB.z);

		return std::sqrt(dx * dx + dy * dy + dz * dz);
	}

	std::string AutoCloseManager::GetRefDebugString(const RE::TESObjectREFR* a_ref)
	{
		if (!a_ref)
		{
			return "NULL";
		}

		return std::format("{} \"{}\" [{}:{:08X}]", a_ref->GetFormEditorID(), a_ref->GetName(), RE::FormTypeToString(a_ref->formType.get()), a_ref->formID);
	}

	void AutoCloseManager::AutoCloseData::PrintDebugInfo() const
	{
		RE::TESObjectREFRPtr ref = target.get();

		SKSE::log::info("Auto-close data for {} | Target: {} | Initial distance: {}", RE::DialogueMenu::MENU_NAME, GetRefDebugString(ref.get()), initialDistance);
	}
}
