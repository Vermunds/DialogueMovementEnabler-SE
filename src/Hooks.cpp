#include "Hooks.h"
#include "AutoCloseManager.h"
#include "Settings.h"

namespace DME
{
	using GetDialogueLookAngle_t = float (*)(RE::PlayerCharacter*);
	static REL::Relocation<GetDialogueLookAngle_t> _GetDialogueLookAngle;

	using InitDialogueLookAt_t = void (*)(RE::PlayerCharacter*);
	static REL::Relocation<InitDialogueLookAt_t> _InitDialogueLookAt;

	using IsGamepadEnabled_t = bool (*)(RE::BSInputDeviceManager*);
	static REL::Relocation<IsGamepadEnabled_t> _IsGamepadEnabled;

	using StartDialogue_t = bool (*)(RE::MenuTopicManager*, RE::TESObjectREFR*, bool, RE::TESTopicInfo*, bool);
	static REL::Relocation<StartDialogue_t> _StartDialogue;

	static bool s_isNPCInitiatedDialogue = false;

	static float GetDialogueLookAngle_Hook(RE::PlayerCharacter* a_player)
	{
		Settings* settings = Settings::GetSingleton();

		if (settings->unlockCamera || settings->freeLook)
		{
			return std::numeric_limits<float>::max();
		}

		return _GetDialogueLookAngle(a_player);
	}

	static bool Actor_SetDialogueWithPlayer_ForceGreet_Hook(RE::MenuTopicManager* a_topicManager, RE::TESObjectREFR* a_speaker, bool a_unk1, RE::TESTopicInfo* a_topicInfo, bool a_unk2)
	{
		s_isNPCInitiatedDialogue = true;
		bool result = _StartDialogue(a_topicManager, a_speaker, a_unk1, a_topicInfo, a_unk2);  // InitDialogueLookAt_Hook is called from here
		s_isNPCInitiatedDialogue = false;
		return result;
	}

	static void InitDialogueLookAt_Hook(RE::PlayerCharacter* a_player)
	{
		Settings* settings = Settings::GetSingleton();

		if (s_isNPCInitiatedDialogue ? settings->disableTurnToSpeakerNPCInitiated : settings->disableTurnToSpeakerPlayerInitiated)
		{
			return;
		}

		_InitDialogueLookAt(a_player);
	}

	static bool PlayerControls_MenuOpenCloseEvent_Handle_Hook(RE::BSInputDeviceManager* a_inputDeviceManager)
	{
		// Decides whether the cursor turns the camera at the screen edges while the dialogue menu is open, which the game only does without a gamepad
		if (Settings::GetSingleton()->freeLook)
		{
			return true;
		}

		return _IsGamepadEnabled(a_inputDeviceManager);
	}

	class MenuControlsEx : public RE::MenuControls
	{
	public:
		using ProcessEvent_t = decltype(static_cast<RE::BSEventNotifyControl (RE::MenuControls::*)(RE::InputEvent* const*, RE::BSTEventSource<RE::InputEvent*>*)>(&RE::MenuControls::ProcessEvent));
		inline static REL::Relocation<ProcessEvent_t> _ProcessEvent;

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

		RE::BSEventNotifyControl ProcessEvent_Hook(RE::InputEvent** a_event, RE::BSTEventSource<RE::InputEvent*>* a_source)
		{
			RE::UI* ui = RE::UI::GetSingleton();
			RE::PlayerControls* pc = RE::PlayerControls::GetSingleton();
			RE::UserEvents* userEvents = RE::UserEvents::GetSingleton();
			RE::BSInputDeviceManager* inputDeviceManager = RE::BSInputDeviceManager::GetSingleton();

			// SkyrimSouls compatibility
			using func_t = bool (*)();
			REL::Relocation<func_t> func(REL::ID{ 56833 });
			bool isInMenuMode = func();

			bool movementControlsEnabled;

			RE::ControlMap* controlMap = RE::ControlMap::GetSingleton();
			movementControlsEnabled = pc->movementHandler->IsInputEventHandlingEnabled() && controlMap->IsMovementControlsEnabled();

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
							idEvent->userEvent = IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->forward) ? userEvents->forward : idEvent->userEvent;
							idEvent->userEvent = IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->back) ? userEvents->back : idEvent->userEvent;
							idEvent->userEvent = IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->strafeLeft) ? userEvents->strafeLeft : idEvent->userEvent;
							idEvent->userEvent = IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->strafeRight) ? userEvents->strafeRight : idEvent->userEvent;

							// Controllers
							idEvent->userEvent = idEvent->userEvent == userEvents->leftStick ? userEvents->move : idEvent->userEvent;
						}

						//Run
						if (settings->allowRun[controlType] && IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->run))
						{
							idEvent->userEvent = movementControlsEnabled ? userEvents->run : "";
						}

						//Toggle Walk/Run
						if (settings->allowToggleRun[controlType] && IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->toggleRun))
						{
							idEvent->userEvent = movementControlsEnabled ? userEvents->toggleRun : "";
						}

						//Jump
						if (settings->allowJump[controlType] && IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->jump))
						{
							idEvent->userEvent = movementControlsEnabled ? userEvents->jump : "";
						}

						//Sprint
						if (settings->allowSprint[controlType] && IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->sprint))
						{
							idEvent->userEvent = movementControlsEnabled ? userEvents->sprint : "";
						}

						//Toggle POV
						if (settings->allowTogglePOV[controlType] && IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->togglePOV))
						{
							idEvent->userEvent = movementControlsEnabled ? userEvents->togglePOV : "";
						}

						//Sneak
						if (settings->allowSneak[controlType] && IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->sneak))
						{
							idEvent->userEvent = movementControlsEnabled ? userEvents->sneak : "";
						}

						//Ready weapon
						if (settings->allowReadyWeapon[controlType] && IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->readyWeapon))
						{
							idEvent->userEvent = movementControlsEnabled ? userEvents->readyWeapon : "";
						}

						//Left attack
						if (settings->allowLeftAttack[controlType] && IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->leftAttack))
						{
							idEvent->userEvent = movementControlsEnabled ? userEvents->leftAttack : "";
						}

						//Right attack
						if (settings->allowRightAttack[controlType] && IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->rightAttack))
						{
							idEvent->userEvent = movementControlsEnabled ? userEvents->rightAttack : "";
						}

						//Shout
						if (settings->allowShout[controlType] && IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->shout))
						{
							idEvent->userEvent = movementControlsEnabled ? userEvents->shout : "";
						}

						//Hotkeys
						if (settings->allowHotkeys[controlType])
						{
							idEvent->userEvent = IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->hotkey1) ? userEvents->hotkey1 : idEvent->userEvent;
							idEvent->userEvent = IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->hotkey2) ? userEvents->hotkey2 : idEvent->userEvent;
							idEvent->userEvent = IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->hotkey3) ? userEvents->hotkey3 : idEvent->userEvent;
							idEvent->userEvent = IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->hotkey4) ? userEvents->hotkey4 : idEvent->userEvent;
							idEvent->userEvent = IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->hotkey5) ? userEvents->hotkey5 : idEvent->userEvent;
							idEvent->userEvent = IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->hotkey6) ? userEvents->hotkey6 : idEvent->userEvent;
							idEvent->userEvent = IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->hotkey7) ? userEvents->hotkey7 : idEvent->userEvent;
							idEvent->userEvent = IsMappedToSameButton(idEvent->idCode, idEvent->device.get(), userEvents->hotkey8) ? userEvents->hotkey8 : idEvent->userEvent;
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
					// Disable cursor when we're in free look
					if (Settings::GetSingleton()->freeLook)
					{
						this->menuFlags.reset(RE::IMenu::Flag::kUsesCursor, RE::IMenu::Flag::kUpdateUsesCursor, RE::IMenu::Flag::kDontHideCursorWhenTopmost);
					}

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
					else
					{
						break;
					}

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
					else
					{
						AutoCloseManager::GetSingleton()->InitAutoClose(nullptr);
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
		REL::Trampoline& trampoline = REL::GetTrampoline();

		_GetDialogueLookAngle = trampoline.write_call<5>(REL::ID{ 42338 }.address() + 0x450, &GetDialogueLookAngle_Hook);
		_StartDialogue = trampoline.write_call<5>(REL::ID{ 37200 }.address() + 0x23D, &Actor_SetDialogueWithPlayer_ForceGreet_Hook);
		_InitDialogueLookAt = trampoline.write_call<5>(REL::ID{ 35282 }.address() + 0x6B4, &InitDialogueLookAt_Hook);
		_IsGamepadEnabled = trampoline.write_call<5>(REL::ID{ 42339 }.address() + 0x46, &PlayerControls_MenuOpenCloseEvent_Handle_Hook);

		REL::Relocation<std::uintptr_t> vTable_mc(RE::VTABLE_MenuControls[0]);
		MenuControlsEx::_ProcessEvent = vTable_mc.write_vfunc(0x1, &MenuControlsEx::ProcessEvent_Hook);

		REL::Relocation<std::uintptr_t> vTable_dm(RE::VTABLE_DialogueMenu[0]);
		DialogueMenuEx::_ProcessMessage = vTable_dm.write_vfunc(0x4, &DialogueMenuEx::ProcessMessage_Hook);
		DialogueMenuEx::_AdvanceMovie = vTable_dm.write_vfunc(0x5, &DialogueMenuEx::AdvanceMovie_Hook);
	}
}
