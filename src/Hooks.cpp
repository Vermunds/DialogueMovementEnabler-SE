#include "Hooks.h"
#include "AutoCloseManager.h"
#include "Settings.h"

namespace DME
{
	constexpr RE::UserEvents::INPUT_CONTEXT_ID GAMEPLAY_CONTEXT = RE::UserEvents::INPUT_CONTEXT_ID::kGameplay;

	class MenuControlsEx : public RE::MenuControls
	{
	public:
		using ProcessEvent_t = decltype(static_cast<RE::BSEventNotifyControl (RE::MenuControls::*)(RE::InputEvent* const*, RE::BSTEventSource<RE::InputEvent*>*)>(&RE::MenuControls::ProcessEvent));
		inline static REL::Relocation<ProcessEvent_t> _ProcessEvent;

		RE::BSEventNotifyControl ProcessEvent_Hook(RE::InputEvent** a_event, RE::BSTEventSource<RE::InputEvent*>* a_source)
		{
			RE::UI* ui = RE::UI::GetSingleton();
			RE::PlayerControls* pc = RE::PlayerControls::GetSingleton();
			RE::ControlMap* controlMap = RE::ControlMap::GetSingleton();
			RE::UserEvents* userEvents = RE::UserEvents::GetSingleton();
			RE::BSInputDeviceManager* inputDeviceManager = RE::BSInputDeviceManager::GetSingleton();

			// SkyrimSouls compatibility
			using func_t = bool (*)();
			REL::Relocation<func_t> func(REL::ID{ 56833 });
			bool isInMenuMode = func();

			bool movementControlsEnabled = pc->movementHandler->IsInputEventHandlingEnabled() && controlMap->IsMovementControlsEnabled();

			if (a_event && *a_event && !this->remapMode && !isInMenuMode && ui->IsMenuOpen(RE::DialogueMenu::MENU_NAME))
			{
				for (RE::InputEvent* evn = *a_event; evn; evn = evn->next)
				{
					if (evn && evn->HasIDCode())
					{
						RE::IDEvent* idEvent = static_cast<RE::IDEvent*>(evn);
						Settings* settings = Settings::GetSingleton();

						Settings::ControlType controlType = inputDeviceManager->IsGamepadEnabled() ? Settings::ControlType::kController : Settings::ControlType::kKeyboardMouse;

						// Movement
						if (settings->allowMovement[controlType])
						{
							// WASD
							idEvent->userEvent = controlMap->GetMappedKey(userEvents->forward, idEvent->device.get(), GAMEPLAY_CONTEXT) == idEvent->idCode ? userEvents->forward : idEvent->userEvent;
							idEvent->userEvent = controlMap->GetMappedKey(userEvents->back, idEvent->device.get(), GAMEPLAY_CONTEXT) == idEvent->idCode ? userEvents->back : idEvent->userEvent;
							idEvent->userEvent = controlMap->GetMappedKey(userEvents->strafeLeft, idEvent->device.get(), GAMEPLAY_CONTEXT) == idEvent->idCode ? userEvents->strafeLeft : idEvent->userEvent;
							idEvent->userEvent = controlMap->GetMappedKey(userEvents->strafeRight, idEvent->device.get(), GAMEPLAY_CONTEXT) == idEvent->idCode ? userEvents->strafeRight : idEvent->userEvent;

							// Controllers
							idEvent->userEvent = idEvent->userEvent == userEvents->leftStick ? userEvents->move : idEvent->userEvent;
						}

						//Run
						if (settings->allowRun[controlType] && controlMap->GetMappedKey(userEvents->run, idEvent->device.get(), GAMEPLAY_CONTEXT) == idEvent->idCode)
						{
							idEvent->userEvent = movementControlsEnabled ? userEvents->run : "";
						}

						//Toggle Walk/Run
						if (settings->allowToggleRun[controlType] && controlMap->GetMappedKey(userEvents->toggleRun, idEvent->device.get(), GAMEPLAY_CONTEXT) == idEvent->idCode)
						{
							idEvent->userEvent = movementControlsEnabled ? userEvents->toggleRun : "";
						}

						//Jump
						if (settings->allowJump[controlType] && controlMap->GetMappedKey(userEvents->jump, idEvent->device.get(), GAMEPLAY_CONTEXT) == idEvent->idCode)
						{
							idEvent->userEvent = movementControlsEnabled ? userEvents->jump : "";
						}

						//Sprint
						if (settings->allowSprint[controlType] && controlMap->GetMappedKey(userEvents->sprint, idEvent->device.get(), GAMEPLAY_CONTEXT) == idEvent->idCode)
						{
							idEvent->userEvent = movementControlsEnabled ? userEvents->sprint : "";
						}

						//Toggle POV
						if (settings->allowTogglePOV[controlType] && controlMap->GetMappedKey(userEvents->togglePOV, idEvent->device.get(), GAMEPLAY_CONTEXT) == idEvent->idCode)
						{
							idEvent->userEvent = movementControlsEnabled ? userEvents->togglePOV : "";
						}

						//Sneak
						if (settings->allowSneak[controlType] && controlMap->GetMappedKey(userEvents->sneak, idEvent->device.get(), GAMEPLAY_CONTEXT) == idEvent->idCode)
						{
							idEvent->userEvent = movementControlsEnabled ? userEvents->sneak : "";
						}

						//Ready weapon
						if (settings->allowReadyWeapon[controlType] && controlMap->GetMappedKey(userEvents->readyWeapon, idEvent->device.get(), GAMEPLAY_CONTEXT) == idEvent->idCode)
						{
							idEvent->userEvent = movementControlsEnabled ? userEvents->readyWeapon : "";
						}

						//Left attack
						if (settings->allowLeftAttack[controlType] && controlMap->GetMappedKey(userEvents->leftAttack, idEvent->device.get(), GAMEPLAY_CONTEXT) == idEvent->idCode)
						{
							idEvent->userEvent = movementControlsEnabled ? userEvents->leftAttack : "";
						}

						//Right attack
						if (settings->allowRightAttack[controlType] && controlMap->GetMappedKey(userEvents->rightAttack, idEvent->device.get(), GAMEPLAY_CONTEXT) == idEvent->idCode)
						{
							idEvent->userEvent = movementControlsEnabled ? userEvents->rightAttack : "";
						}

						//Shout
						if (settings->allowShout[controlType] && controlMap->GetMappedKey(userEvents->shout, idEvent->device.get(), GAMEPLAY_CONTEXT) == idEvent->idCode)
						{
							idEvent->userEvent = movementControlsEnabled ? userEvents->shout : "";
						}

						//Hotkeys
						if (settings->allowHotkeys[controlType])
						{
							idEvent->userEvent = controlMap->GetMappedKey(userEvents->hotkey1, idEvent->device.get(), GAMEPLAY_CONTEXT) == idEvent->idCode ? userEvents->hotkey1 : idEvent->userEvent;
							idEvent->userEvent = controlMap->GetMappedKey(userEvents->hotkey2, idEvent->device.get(), GAMEPLAY_CONTEXT) == idEvent->idCode ? userEvents->hotkey2 : idEvent->userEvent;
							idEvent->userEvent = controlMap->GetMappedKey(userEvents->hotkey3, idEvent->device.get(), GAMEPLAY_CONTEXT) == idEvent->idCode ? userEvents->hotkey3 : idEvent->userEvent;
							idEvent->userEvent = controlMap->GetMappedKey(userEvents->hotkey4, idEvent->device.get(), GAMEPLAY_CONTEXT) == idEvent->idCode ? userEvents->hotkey4 : idEvent->userEvent;
							idEvent->userEvent = controlMap->GetMappedKey(userEvents->hotkey5, idEvent->device.get(), GAMEPLAY_CONTEXT) == idEvent->idCode ? userEvents->hotkey5 : idEvent->userEvent;
							idEvent->userEvent = controlMap->GetMappedKey(userEvents->hotkey6, idEvent->device.get(), GAMEPLAY_CONTEXT) == idEvent->idCode ? userEvents->hotkey6 : idEvent->userEvent;
							idEvent->userEvent = controlMap->GetMappedKey(userEvents->hotkey7, idEvent->device.get(), GAMEPLAY_CONTEXT) == idEvent->idCode ? userEvents->hotkey7 : idEvent->userEvent;
							idEvent->userEvent = controlMap->GetMappedKey(userEvents->hotkey8, idEvent->device.get(), GAMEPLAY_CONTEXT) == idEvent->idCode ? userEvents->hotkey8 : idEvent->userEvent;
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

					if (target->formType == RE::FormType::ActorCharacter)
					{
						RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

						if (player->GetParentCell()->IsInteriorCell() && target->GetParentCell()->IsInteriorCell())
						{
							if (player->GetParentCell() == target->GetParentCell())
							{
								AutoCloseManager::GetSingleton()->InitAutoClose(target);
							}
						}
						else if (player->GetParentCell()->IsExteriorCell() && target->GetParentCell()->IsExteriorCell())
						{
							if (player->GetWorldspace() == target->GetWorldspace())
							{
								AutoCloseManager::GetSingleton()->InitAutoClose(target);
							}
						}
					}
				}
				break;
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
		Settings* settings = Settings::GetSingleton();

		REL::Relocation<std::uintptr_t> vTable_mc(REL::ID{ 215773 });
		MenuControlsEx::_ProcessEvent = vTable_mc.write_vfunc(0x1, &MenuControlsEx::ProcessEvent_Hook);

		//Hook ProcessMessage
		REL::Relocation<std::uintptr_t> vTable_dm(REL::ID{ 215255 });
		DialogueMenuEx::_ProcessMessage = vTable_dm.write_vfunc(0x4, &DialogueMenuEx::ProcessMessage_Hook);
		DialogueMenuEx::_AdvanceMovie = vTable_dm.write_vfunc(0x5, &DialogueMenuEx::AdvanceMovie_Hook);

		if (settings->unlockCamera)
		{
			std::uint8_t buf[] = { 0xE9, 0xB1, 0x00, 0x00, 0x00, 0x90 };  //jmp + nop
			REL::safe_write(REL::ID{ 42338 }.address() + 0x5AF, std::span<uint8_t>(buf));
		}
	}
}
