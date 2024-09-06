//#include "TestField.h"
#include "Hooks.h"
#include "CombatStyleManager.h"
#include "StyleOverride.h"
#include "SettingsManager.h"
#include "EventSingleton.h"

#include "Serialization/SerializationTypePlayground.h"


using namespace SKSE;
using namespace SKSE::log;
using namespace SKSE::stl;

using namespace SOS;
using namespace RGL;

void InitializeLogging() {
    auto path = log_directory();
    if (!path) {
        report_and_fail("Unable to lookup SKSE logs directory.");
    }
    *path /= PluginDeclaration::GetSingleton()->GetName();
    *path += L".log";

    std::shared_ptr<spdlog::logger> log;

    if (IsDebuggerPresent()) {
        log = std::make_shared<spdlog::logger>(
            "Global", std::make_shared<spdlog::sinks::msvc_sink_mt>());
    }
    else {
        log = std::make_shared<spdlog::logger>(
            "Global", std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true));
    }


#ifdef _DEBUG
    const auto level = spdlog::level::trace;
#else
    const auto level = spdlog::level::info;
#endif

    log->set_level(level);
    log->flush_on(level);

    spdlog::set_default_logger(std::move(log));
    
    //Sometime I would like to go through and set specific loggers for each where relevant
    //log->sinks()[0]->set_pattern(">>>>>>>>> %H:%M:%S %z %v <<<<<<<<<");
  
#ifdef _DEBUG
    spdlog::set_pattern(">{%r} %s(%#): [%^%l%$] %v"s);
#else
    spdlog::set_pattern("> %s(%#): [%^%l%$] %v"s);
#endif
}

uint32_t RGL::GetProjectVersion()
{
    constexpr REL::Version version(1, 0, 0, 0);

    //1.0.0.0 Use the Commonlib versioning later. It has a packing function.
    return version.pack();
}


void InitializeMessaging() {
    if (!GetMessagingInterface()->RegisterListener([](MessagingInterface::Message* message) {
        switch (message->type) {
        case MessagingInterface::kPostLoad:
            ArithmeticAPI::RequestInterface();
            ActorValueGeneratorAPI::RequestInterface();
            StyleOverride::Init1();
            break;
            // It is now safe to do multithreaded operations, or operations against other plugins.

        case MessagingInterface::kPostPostLoad: // Called after all kPostLoad message handlers have run.
            Hooks::Install();
            EventSingleton::Install();
            break;

        case MessagingInterface::kDataLoaded:
            CombatStyleManager::Init();
            StyleOverride::Init2();
            //auto singleton = RE::BGSDefaultObjectManager::GetSingleton();
//#undef GetObject 
//Fingers crossed this does nothing.
            //if (singleton)
            //    defaultStyle = singleton->GetObject<RE::TESCombatStyle>()
            break;
        }
        })) {
        stl::report_and_fail("Unable to register message listener.");
    }
}


SKSEPluginLoad(const LoadInterface* skse) {
#ifdef _DEBUG
    //Need a way to only have this happen when holding down a key
    if (GetKeyState(VK_RCONTROL) & 0x800) {
        constexpr auto text1 = L"Request for debugger detected. If you wish to attach one and press Ok, do so now if not please press Cancel.";
        constexpr auto text2 = L"Debugger still not detected. If you wish to continue without one please press Cancel.";
        constexpr auto caption = L"Debugger Required";

        int input = 0;

        do
        {
            input = MessageBox(NULL, !input ? text1 : text2, caption, MB_OKCANCEL);
        } while (!IsDebuggerPresent() && input != IDCANCEL);
    }
#endif

    InitializeLogging();
    
    const auto* plugin = PluginDeclaration::GetSingleton();
    auto version = plugin->GetVersion();
    log::info("{} {} is loading...", plugin->GetName(), version);
    Init(skse);


    InitializeMessaging();
    SettingsManager::Init();
    MainSerializer::Initialize('SOVR');

    log::info("{} has finished loading.", plugin->GetName());
    return true;
}
