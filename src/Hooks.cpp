#include "Hooks.h"
#include "Settings.h"
#include "AutoCloseManager.h"

namespace DME
{
	bool IsMappedToSameButton(std::uint32_t a_keyMask, RE::INPUT_DEVICE a_deviceType, RE::BSFixedString a_controlName, RE::UserEvents::INPUT_CONTEXT_ID a_context = RE::UserEvents::INPUT_CONTEXT_ID::kGameplay)
	{
		RE::ControlMap* controlMap = RE::ControlMap::GetSingleton();

		if (a_deviceType == RE::INPUT_DEVICE::kKeyboard)
		{
			std::uint32_t keyMask = controlMap->GetMappedKey(a_controlName, RE::INPUT_DEVICE::kKeyboard, a_context);
			return a_keyMask == keyMask;
		}
		else if (a_deviceType == RE::INPUT_DEVICE::kMouse)
		{
			std::uint32_t keyMask = controlMap->GetMappedKey(a_controlName, RE::INPUT_DEVICE::kMouse, a_context);
			return a_keyMask == keyMask;
		}
		return false;
	}

	class MenuControlsEx : public RE::MenuControls
	{
	public:
		using ProcessEvent_t = decltype(static_cast<RE::BSEventNotifyControl(RE::MenuControls::*)(RE::InputEvent* const*, RE::BSTEventSource<RE::InputEvent*>*)>(&RE::MenuControls::ProcessEvent));
		inline static REL::Relocation<ProcessEvent_t> _ProcessEvent;

		RE::BSEventNotifyControl ProcessEvent_Hook(RE::InputEvent** a_event, RE::BSTEventSource<RE::InputEvent*>* a_source)
		{
			RE::UI* ui = RE::UI::GetSingleton();
			RE::UserEvents* userEvents = RE::UserEvents::GetSingleton();

			// SkyrimSouls compatibility
			using func_t = bool(*)();
			REL::Relocation<func_t> func(REL::ID{ 56476 });
			bool isInMenuMode = func();

			if (a_event && *a_event && !this->remapMode && !isInMenuMode && ui->IsMenuOpen(RE::DialogueMenu::MENU_NAME))
			{
				for (RE::InputEvent* evn = *a_event; evn; evn = evn->next)
				{
					if (evn && evn->HasIDCode())
					{
						RE::IDEvent* idEvent = static_cast<RE::IDEvent*>(evn);

						//Forward
						if (idEvent->userEvent == userEvents->up && IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->forward))
						{
							idEvent->userEvent = userEvents->forward;
						}
						//Back
						if (idEvent->userEvent == userEvents->down && IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->back))
						{
							idEvent->userEvent = userEvents->back;
						}
						//Left
						if (idEvent->userEvent == userEvents->left && IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->strafeLeft))
						{
							idEvent->userEvent = userEvents->strafeLeft;
						}
						//Right
						if (idEvent->userEvent == userEvents->right && IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->strafeRight))
						{
							idEvent->userEvent = userEvents->strafeRight;
						}

						//Controllers
						if (idEvent->userEvent == userEvents->leftStick)
						{
							idEvent->userEvent = userEvents->move;
						}
					}
				}
			}

			return _ProcessEvent(this, a_event, a_source);
		}
	};

	class DialogueMenuEx : public RE::DialogueMenu
	{
	public:
		using ProcessMessage_t = decltype(&RE::DialogueMenu::ProcessMessage);
		inline static REL::Relocation<ProcessMessage_t> _ProcessMessage;

		using AdvanceMovie_t = decltype(&RE::DialogueMenu::AdvanceMovie);
		inline static REL::Relocation<AdvanceMovie_t> _AdvanceMovie;

		RE::UI_MESSAGE_RESULTS ProcessMessage_Hook(RE::UIMessage& a_message)
		{
			switch (a_message.type.get())
			{
				case RE::UI_MESSAGE_TYPE::kShow:
				{
					RE::MenuTopicManager* topicManager = RE::MenuTopicManager::GetSingleton();

					RE::TESObjectREFR* target = nullptr;

					if (topicManager->speaker && topicManager->speaker.get())
					{
						target = topicManager->speaker.get().get();
					}
					else if (topicManager->lastSpeaker && topicManager->lastSpeaker.get())
					{
						target = topicManager->lastSpeaker.get().get();
					}

					AutoCloseManager::GetSingleton()->InitAutoClose(target);
				}
			}

			return _ProcessMessage(this, a_message);
		}

		void AdvanceMovie_Hook(float a_interval, std::uint32_t a_currentTime)
		{
			AutoCloseManager::GetSingleton()->CheckAutoClose();
			return _AdvanceMovie(this, a_interval, a_currentTime);
		}
	};

	void InstallHooks()
	{
		REL::Relocation<std::uintptr_t> vTable_mc(REL::ID{ 269528 });
		MenuControlsEx::_ProcessEvent = vTable_mc.write_vfunc(0x1, &MenuControlsEx::ProcessEvent_Hook);

		//Hook ProcessMessage
		REL::Relocation<std::uintptr_t> vTable_dm(REL::ID{ 268589 });
		DialogueMenuEx::_ProcessMessage = vTable_dm.write_vfunc(0x4, &DialogueMenuEx::ProcessMessage_Hook);
		DialogueMenuEx::_AdvanceMovie = vTable_dm.write_vfunc(0x5, &DialogueMenuEx::AdvanceMovie_Hook);
		
		if (Settings::GetSingleton()->unlockCamera)
		{
			std::uint8_t buf[] = { 0xE9, 0xBE, 0x00, 0x00, 0x00, 0x90 }; //jmp + nop
			REL::safe_write(REL::ID{ 41292 }.address() + 0x25, buf);
		}
	}
}
