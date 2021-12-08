#include "version.h"
#include "Hooks.h"
#include "Settings.h"

extern "C" {
	DLLEXPORT SKSE::PluginVersionData SKSEPlugin_Version = []() {
		SKSE::PluginVersionData v{};
		v.PluginVersion(REL::Version{ Version::MAJOR, Version::MINOR, Version::PATCH, 0 });
		v.PluginName(Version::PROJECT);
		v.AuthorName("Vermunds"sv);
		v.CompatibleVersions({ SKSE::RUNTIME_1_6_318 });

		v.addressLibrary = true;
		v.sigScanning = false;
		return v;
	}();

	DLLEXPORT bool SKSEPlugin_Load(SKSE::LoadInterface* a_skse)
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

		if (a_skse->IsEditor())
		{
			SKSE::log::critical("Loaded in editor, marking as incompatible!");
			return false;
		}

		SKSE::Init(a_skse);

		DME::LoadSettings();
		SKSE::log::info("Settings loaded.");

		DME::InstallHooks();
		SKSE::log::info("Hooks installed.");

		SKSE::log::info("Dialogue Movement Enabler loaded.");

		return true;
	}
};
