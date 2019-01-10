#include "connector.h"
#include "eventstream.h"
#include "convar.h"
#include "tier1.h"
#include "valve_minmax_off.h"
#include <algorithm>
#include <string>

namespace {
Connector connector;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(Connector, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, connector)

void addOutput(const CCommand& args)
{
    if (args.ArgC() <= 1) {
        Msg("Usage: connector_add_output <file>\n");
        return;
    }

    std::string fileName(args.Arg(1));
    EventStream* eventStream = new EventStream(fileName);
    if (eventStream->isOK()) {
        connector.addEventStream(eventStream);
        Msg("Printing morgoth connector events to %s\n", fileName.c_str());
    } else {
        Warning("Could not open %s\n", fileName.c_str());
        delete eventStream;
    }
}

void printVersion(const CCommand& /*args*/)
{
    Msg("morgoth-connector version: 0.0.1\n");
}

ConCommand addOutputCommand("connector_add_output", addOutput, "Outputs connector events to the given file");
ConCommand printVersionCommand("connector_version", printVersion, "Prints morgoth-connector plugin version");

} // ns

Connector::Connector()
{

}

Connector::~Connector()
{
    std::for_each(m_eventStreams.begin(), m_eventStreams.end(), [](auto e) { delete e; });
}

bool Connector::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn /*gameServerFactory*/)
{
    if (!m_loadCount) {
        m_loadCount += 1;

        ConnectTier1Libraries(&interfaceFactory, 1);
        ConVar_Register();
        return true;
    } else {
        Msg("morgoth connector already loaded; ignoring.\n");
        return false;
    }
}

void Connector::Unload()
{
    ConVar_Unregister();
    DisconnectTier1Libraries();
    m_loadCount -= 1;
}

void Connector::Pause()
{

}

void Connector::UnPause()
{

}

const char* Connector::GetPluginDescription()
{
    static const std::string PluginDescription("morgoth connector");
    return PluginDescription.c_str();
}

void Connector::LevelInit(const char* pMapName)
{
    std::string mapName(pMapName);
    std::for_each(m_eventStreams.begin(), m_eventStreams.end(), [&mapName](EventStream* eventStream) {
        *eventStream << mapName;
    });
}

void Connector::ServerActivate(edict_t* /*pEdictList*/, int /*edictCount*/, int /*clientMax*/)
{

}

void Connector::GameFrame(bool /*simulating*/)
{

}

void Connector::LevelShutdown()
{

}

void Connector::ClientActive(edict_t* /*pEntity*/)
{

}

void Connector::ClientDisconnect(edict_t* /*pEntity*/)
{

}

void Connector::ClientPutInServer(edict_t* /*pEntity*/, const char* /*playername*/)
{

}

void Connector::SetCommandClient(int index)
{
    m_clientCommandIndex = index;
}

void Connector::ClientSettingsChanged(edict_t* /*pEdict*/)
{

}

PLUGIN_RESULT Connector::ClientConnect(bool* /*bAllowConnect*/, edict_t* /*pEntity*/,
                                       const char* /*pszName*/, const char* /*pszAddress*/,
                                       char* /*reject*/, int /*maxrejectlen*/)
{
    return PLUGIN_CONTINUE;
}

PLUGIN_RESULT Connector::ClientCommand(edict_t* /*pEntity*/, const CCommand& /*args*/)
{
    return PLUGIN_CONTINUE;
}

PLUGIN_RESULT Connector::NetworkIDValidated(const char* /*pszUserName*/, const char* /*pszNetworkID*/)
{
    return PLUGIN_CONTINUE;
}

void Connector::OnQueryCvarValueFinished(QueryCvarCookie_t /*iCookie*/, edict_t* /*pPlayerEntity*/,
                                         EQueryCvarValueStatus /*eStatus*/, const char* /*pCvarName*/,
                                         const char* /*pCvarValue*/)
{

}

void Connector::OnEdictAllocated(edict_t* /*edict*/)
{

}

void Connector::OnEdictFreed(const edict_t* /*edict*/)
{

}

void Connector::addEventStream(EventStream* eventStream)
{
    m_eventStreams.push_back(eventStream);
}
