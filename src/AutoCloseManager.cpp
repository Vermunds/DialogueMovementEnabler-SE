#include "AutoCloseManager.h"
#include "Settings.h"

#include <cmath>

namespace DME
{
	void AutoCloseManager::CheckAutoClose()
	{
		Settings* settings = Settings::GetSingleton();
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		RE::UIMessageQueue* uiMessageQueue = RE::UIMessageQueue::GetSingleton();

		float maxDistance = settings->autoCloseDistance;
		float tolerance = settings->autoCloseTolerance;

		if (_data.target && settings->autoCloseMenus)
		{
			float currentDistance = GetDistance(player->GetPosition(), player->GetHeight(), _data.target->GetPosition());
			bool tooFarOnOpen = (_data.initialDistance > maxDistance) ? true : false;

			if (!tooFarOnOpen && currentDistance > maxDistance)
			{
				//Normal case
				uiMessageQueue->AddMessage(RE::DialogueMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
			}
			else if (tooFarOnOpen)
			{
				//Target was opened when it was too far
				if (currentDistance > maxDistance && currentDistance > (_data.minDistance + tolerance))  //Close only if the distance is increasing
				{
					uiMessageQueue->AddMessage(RE::DialogueMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
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

		_data.target = a_ref;
		_data.initialDistance = a_ref ? GetDistance(player->GetPosition(), player->GetHeight(), a_ref->GetPosition()) : 0.0f;
		_data.minDistance = _data.initialDistance;
	}

	AutoCloseManager* AutoCloseManager::GetSingleton()
	{
		static AutoCloseManager singleton;
		return &singleton;
	}

	float AutoCloseManager::GetDistance(RE::NiPoint3 a_playerPos, float a_playerHeight, RE::NiPoint3 a_targetPos)
	{
		//Get distance from feet and head, return the smaller
		float distanceHead = static_cast<float>(std::pow(a_playerPos.x - a_targetPos.x, 2) + std::pow(a_playerPos.y - a_targetPos.y, 2) + std::pow((a_playerPos.z + a_playerHeight) - a_targetPos.z, 2));
		float distanceFeet = static_cast<float>(std::pow(a_playerPos.x - a_targetPos.x, 2) + std::pow(a_playerPos.y - a_targetPos.y, 2) + std::pow(a_playerPos.z - a_targetPos.z, 2));
		if (distanceHead < distanceFeet)
		{
			return std::sqrt(distanceHead);
		}
		return std::sqrt(distanceFeet);
	}
}
