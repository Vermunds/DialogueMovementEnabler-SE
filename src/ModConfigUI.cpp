#include "ModConfigUI.h"

#include "Hooks.h"
#include "Settings.h"
#include "Version.h"

#include <ModConfigUI/Localization.h>

namespace DME
{
	const char* Translate(const char* a_key)
	{
		return ModConfigUI::Localization::Get(a_key);
	}

	// Slider ranges for the auto-close distances
	constexpr float AUTO_CLOSE_DISTANCE_MIN = 200.0f;
	constexpr float AUTO_CLOSE_DISTANCE_MAX = 5000.0f;
	constexpr float AUTO_CLOSE_TOLERANCE_MIN = 0.0f;
	constexpr float AUTO_CLOSE_TOLERANCE_MAX = 2000.0f;

	// Every per-control toggle, in the order it appears on the controls page.
	// Both the keyboard/mouse and the controller variant of a control are described by a single entry.
	struct Control
	{
		const char* labelKey;
		bool (Settings::*value)[Settings::kTotal];
		bool defaults[Settings::kTotal];  // Indexed by Settings::ControlType, like value
		bool controllerInvalid;           // The controller has no button for this control, its checkbox is greyed out
	};

	constexpr Control CONTROLS[] = {
		{ "$DME_AllowMovement", &Settings::allowMovement, { ALLOW_MOVEMENT_KEYBOARD_DEFAULT_VALUE, ALLOW_MOVEMENT_CONTROLLER_DEFAULT_VALUE } },
		{ "$DME_AllowRun", &Settings::allowRun, { ALLOW_RUN_KEYBOARD_DEFAULT_VALUE, ALLOW_RUN_CONTROLLER_DEFAULT_VALUE }, true },
		{ "$DME_AllowToggleRun", &Settings::allowToggleRun, { ALLOW_TOGGLE_RUN_KEYBOARD_DEFAULT_VALUE, ALLOW_TOGGLE_RUN_CONTROLLER_DEFAULT_VALUE }, true },
		{ "$DME_AllowJump", &Settings::allowJump, { ALLOW_JUMP_KEYBOARD_DEFAULT_VALUE, ALLOW_JUMP_CONTROLLER_DEFAULT_VALUE } },
		{ "$DME_AllowSprint", &Settings::allowSprint, { ALLOW_SPRINT_KEYBOARD_DEFAULT_VALUE, ALLOW_SPRINT_CONTROLLER_DEFAULT_VALUE } },
		{ "$DME_AllowTogglePOV", &Settings::allowTogglePOV, { ALLOW_TOGGLE_POV_KEYBOARD_DEFAULT_VALUE, ALLOW_TOGGLE_POV_CONTROLLER_DEFAULT_VALUE } },
		{ "$DME_AllowSneak", &Settings::allowSneak, { ALLOW_SNEAK_KEYBOARD_DEFAULT_VALUE, ALLOW_SNEAK_CONTROLLER_DEFAULT_VALUE } },
		{ "$DME_AllowReadyWeapon", &Settings::allowReadyWeapon, { ALLOW_READY_WEAPON_KEYBOARD_DEFAULT_VALUE, ALLOW_READY_WEAPON_CONTROLLER_DEFAULT_VALUE } },
		{ "$DME_AllowLeftAttack", &Settings::allowLeftAttack, { ALLOW_LEFT_ATTACK_KEYBOARD_DEFAULT_VALUE, ALLOW_LEFT_ATTACK_CONTROLLER_DEFAULT_VALUE } },
		{ "$DME_AllowRightAttack", &Settings::allowRightAttack, { ALLOW_RIGHT_ATTACK_KEYBOARD_DEFAULT_VALUE, ALLOW_RIGHT_ATTACK_CONTROLLER_DEFAULT_VALUE } },
		{ "$DME_AllowHotkeys", &Settings::allowHotkeys, { ALLOW_HOTKEYS_KEYBOARD_DEFAULT_VALUE, ALLOW_HOTKEYS_CONTROLLER_DEFAULT_VALUE } },
		{ "$DME_AllowShout", &Settings::allowShout, { ALLOW_SHOUT_KEYBOARD_DEFAULT_VALUE, ALLOW_SHOUT_CONTROLLER_DEFAULT_VALUE } }
	};

	// Pages
	void DrawSettingsPage(ModConfigUI::Renderer& a_renderer)
	{
		Settings* settings = Settings::GetSingleton();

		a_renderer.SeparatorText(Translate("$DME_Section_Camera"));

		if (a_renderer.Checkbox(Translate("$DME_UnlockCamera"), &settings->unlockCamera, UNLOCK_CAMERA_DEFAULT_VALUE, Translate("$DME_UnlockCamera_Tooltip")))
		{
			SaveSettings();
		}

		a_renderer.SeparatorText(Translate("$DME_Section_AutoClose"));

		if (a_renderer.Checkbox(Translate("$DME_AutoCloseMenus"), &settings->autoCloseMenus, AUTO_CLOSE_MENUS_DEFAULT_VALUE, Translate("$DME_AutoCloseMenus_Tooltip")))
		{
			SaveSettings();
		}

		if (a_renderer.SliderFloat(Translate("$DME_AutoCloseDistance"), &settings->autoCloseDistance, AUTO_CLOSE_DISTANCE_MIN, AUTO_CLOSE_DISTANCE_MAX, "%.0f", AUTO_CLOSE_DISTANCE_DEFAULT_VALUE, Translate("$DME_AutoCloseDistance_Tooltip")))
		{
			SaveSettings();
		}

		if (a_renderer.SliderFloat(Translate("$DME_AutoCloseTolerance"), &settings->autoCloseTolerance, AUTO_CLOSE_TOLERANCE_MIN, AUTO_CLOSE_TOLERANCE_MAX, "%.0f", AUTO_CLOSE_TOLERANCE_DEFAULT_VALUE, Translate("$DME_AutoCloseTolerance_Tooltip")))
		{
			SaveSettings();
		}
	}

	// Every control is set for both devices at once, one column per device.
	void DrawControlsPage(ModConfigUI::Renderer& a_renderer)
	{
		Settings* settings = Settings::GetSingleton();

		a_renderer.TextWrappedMuted(Translate("$DME_Controls_Description"));

		a_renderer.SeparatorText(Translate("$DME_Section_Controls"));

		// The control name column, then one checkbox column per device. The name keeps the share of the
		// row a label has everywhere else in the menu, the devices split what is left of it between them.
		constexpr std::int32_t COLUMN_COUNT = Settings::kTotal + 1;
		constexpr float DEVICE_COLUMN_WIDTH = (1.0f - ModConfigUI::LABEL_COLUMN_RATIO) / static_cast<float>(Settings::kTotal);

		const char* headers[COLUMN_COUNT]{};
		headers[0] = "";
		headers[Settings::ControlType::kKeyboardMouse + 1] = Translate("$DME_Column_KeyboardMouse");
		headers[Settings::ControlType::kController + 1] = Translate("$DME_Column_Controller");

		constexpr float widths[COLUMN_COUNT] = { ModConfigUI::LABEL_COLUMN_RATIO, DEVICE_COLUMN_WIDTH, DEVICE_COLUMN_WIDTH };

		if (!a_renderer.BeginTable("DME_Controls", headers, COLUMN_COUNT, widths))
		{
			return;
		}

		for (const Control& control : CONTROLS)
		{
			a_renderer.TableNextRow();

			a_renderer.TableNextColumn();
			a_renderer.Text(Translate(control.labelKey));

			for (std::int32_t controlType = 0; controlType < Settings::kTotal; ++controlType)
			{
				a_renderer.TableNextColumn();

				// Without a button of its own on the controller, the control follows movement there. Nothing to
				// set, so the cell shows movement's value greyed out.
				bool followsMovement = control.controllerInvalid && controlType == Settings::ControlType::kController;
				bool* value = followsMovement ? &settings->allowMovement[Settings::ControlType::kController] : &(settings->*control.value)[controlType];

				// The row's cells all share the control's name, the column tells them apart.
				a_renderer.PushID(controlType);

				a_renderer.BeginDisabled(followsMovement);
				if (a_renderer.CheckboxWidget(control.labelKey, value))
				{
					SaveSettings();
				}
				a_renderer.EndDisabled();

				// The name is already in the first column, only the default is left to say.
				if (!followsMovement)
				{
					a_renderer.ItemTooltip(nullptr, ModConfigUI::GetDefaultLine(control.defaults[controlType]).c_str());
				}

				a_renderer.PopID();
			}
		}

		a_renderer.EndTable();
	}

	void InstallModConfigUI()
	{
		static constexpr ModConfigUI::ModInfo MOD_INFO{
			.pluginName = Version::NAME.data(),
			.displayName = Version::FORMATTED_NAME.data(),
			.version = Version::STRING.data(),
			.author = Version::AUTHOR.data(),
			.description = "$DME_Description",
			.nexusUrl = "https://www.nexusmods.com/skyrimspecialedition/mods/43708",
			.sourceUrl = "https://github.com/Vermunds/DialogueMovementEnabler-SE"
		};

		static constexpr ModConfigUI::Page PAGES[] = {
			{ "$DME_Page_Settings", &DrawSettingsPage },
			{ "$DME_Page_Controls", &DrawControlsPage }
		};

		ModConfigUI::Install(MOD_INFO, PAGES, &RestoreDefaults);
	}
}
