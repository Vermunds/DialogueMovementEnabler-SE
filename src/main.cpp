#include "Hooks.h"
#include "ModConfigUI.h"
#include "Settings.h"

#include "Version.h"

static void MessageHandler(SKSE::MessagingInterface::Message* a_message)
{
	if (a_message->type == SKSE::MessagingInterface::kPostLoad)
	{
		DME::InstallModConfigUI();
	}
}

extern "C"
{
	DLLEXPORT SKSE::PluginVersionData SKSEPlugin_Version = []() {
		SKSE::PluginVersionData v{};
		v.PluginVersion(REL::Version{ Version::MAJOR, Version::MINOR, Version::PATCH, 0 });
		v.PluginName(Version::NAME);
		v.AuthorName(Version::AUTHOR);
		v.UsesAddressLibrary();
		v.UsesUpdatedStructs();
		v.CompatibleVersions({ SKSE::RUNTIME_SSE_1_7_104 });
		return v;
	}();

	DLLEXPORT bool SKSEPlugin_Load(SKSE::LoadInterface* a_skse)
	{
		SKSE::InitInfo initInfo{};
		initInfo.logLevel = REX::ELogLevel::Trace;
		initInfo.logPattern = "%s(%#): [%^%l%$] %v";
		initInfo.trampoline = true;
		initInfo.trampolineSize = 1 << 5;
		SKSE::Init(a_skse, initInfo);

		logger::info("{} v{} -({})", Version::FORMATTED_NAME, Version::STRING, __TIMESTAMP__);

		const SKSE::MessagingInterface* messaging = SKSE::GetMessagingInterface();
		if (messaging->RegisterListener("SKSE", MessageHandler))
		{
			logger::info("Messaging interface registration successful.");
		}
		else
		{
			logger::critical("Messaging interface registration failed.");
			return false;
		}

		DME::LoadSettings();
		logger::info("Settings loaded.");

		DME::InstallHooks();
		logger::info("Hooks installed.");

		logger::info("Dialogue Movement Enabler loaded.");

		return true;
	}
};
