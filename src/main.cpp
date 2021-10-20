#include "version.h"
#include "Settings.h"
#include "Hooks.h"

constexpr auto MESSAGE_BOX_TYPE = 0x00001010L; // MB_OK | MB_ICONERROR | MB_SYSTEMMODAL

extern "C" {
	DLLEXPORT bool SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
	{
		assert(SKSE::log::log_directory().has_value());
		auto path = SKSE::log::log_directory().value() / std::filesystem::path("DialogueMovementEnabler.log");
		auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path.string(), true);
		auto log = std::make_shared<spdlog::logger>("global log", std::move(sink));

		log->set_level(spdlog::level::trace);
		log->flush_on(spdlog::level::trace);

		spdlog::set_default_logger(std::move(log));
		spdlog::set_pattern("%g(%#): [%^%l%$] %v", spdlog::pattern_time_type::local);

		SKSE::log::info("Dialogue Movement Enabler v" + std::string(Version::NAME) + " - (" + std::string(__TIMESTAMP__) + ")");

		a_info->infoVersion = SKSE::PluginInfo::kVersion;
		a_info->name = Version::PROJECT.data();
		a_info->version = Version::MAJOR;

		if (a_skse->IsEditor()) {
			SKSE::log::critical("Loaded in editor, marking as incompatible!");
			return false;
		}

		if (a_skse->RuntimeVersion() < SKSE::RUNTIME_1_5_39) {
			SKSE::log::critical("Unsupported runtime version " + a_skse->RuntimeVersion().string());
			SKSE::WinAPI::MessageBox(nullptr, std::string("Unsupported runtime version " + a_skse->RuntimeVersion().string()).c_str(), "Dialogue Movement Enabler - Error", MESSAGE_BOX_TYPE);
			return false;
		}

		return true;
	}

	DLLEXPORT bool SKSEPlugin_Load(SKSE::LoadInterface* a_skse)
	{
		SKSE::Init(a_skse);

		DME::LoadSettings();
		SKSE::log::info("Settings loaded.");

		DME::InstallHooks();
		SKSE::log::info("Hooks installed.");

		SKSE::log::info("Dialogue Movement Enabler loaded.");

		return true;
	}
};
