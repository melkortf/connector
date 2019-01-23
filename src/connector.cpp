#include "connector.h"
#include "config.h"

#include "eiface.h"
#include "convar.h"
#include "game/server/iplayerinfo.h"
#include "tier1.h"
#include "filesystem.h"
#include "valve_minmax_off.h"

#include "gameserver.h"
#include <QtDBus>
#include <algorithm>
#include <fstream>
#include <string>

namespace {
Connector connector;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(Connector, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, connector)

void printVersion(const CCommand& /*args*/)
{
    Msg("morgoth-connector version: %s\n", MORGOTH_CONNECTOR_VERSION);
}

ConCommand printVersionCommand("connector_version", printVersion, "Prints morgoth-connector plugin version");

char** getProgramArguments(int* argc)
{
    // get arguments that were passed to the current process (like argc + argv, but without main())
    static char** argv = nullptr;
    static int _argc = 0;
    if (argv) {
        *argc = _argc;
        return argv;
    }

    std::ifstream cmdline("/proc/self/cmdline");
    if (!cmdline.is_open())
        throw std::runtime_error("cannot read launch arguments");

    std::vector<std::string> arguments;
    std::string arg;
    while (std::getline(cmdline, arg, '\0'))
        arguments.push_back(arg);

    argv = new char*[arguments.size()];
    for (size_t i = 0; i < arguments.size(); ++i) {
        argv[i] = new char[arguments.at(i).length() + 1];
        ::memset(argv[i], '\0', arguments.at(i).length() + 1);
        ::memcpy(argv[i], arguments.at(i).c_str(), arguments.at(i).length());
    }

    _argc = static_cast<int>(arguments.size());
    *argc = _argc;
    return argv;
}

void srcdsMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    Q_UNUSED(context);

    static const char* Prefix = "[morgoth-connector]";

    switch (type) {
        case QtFatalMsg:
            Error("%s %s\n", Prefix, qPrintable(msg));
            break;

        case QtCriticalMsg:
        case QtWarningMsg:
            Warning("%s %s\n", Prefix, qPrintable(msg));
            break;

        case QtInfoMsg:
        case QtDebugMsg:
            Msg("%s %s\n", Prefix, qPrintable(msg));
            break;
    }
}

} // ns

Connector::Connector()
{

}

Connector::~Connector()
{

}

bool Connector::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory)
{
    if (!m_loadCount) {
        m_loadCount += 1;

        ConnectTier1Libraries(&interfaceFactory, 1);
        ConVar_Register();

        m_engine = reinterpret_cast<IVEngineServer*>(interfaceFactory(INTERFACEVERSION_VENGINESERVER, nullptr));
        m_playerInfoManager = reinterpret_cast<IPlayerInfoManager*>(gameServerFactory(INTERFACEVERSION_PLAYERINFOMANAGER, nullptr));

        if (!m_playerInfoManager) {
            Warning("Could not load all interfaces\n");
            return false;
        }

        m_globalVars = m_playerInfoManager->GetGlobalVars();

        // Qt initialization
        qInstallMessageHandler(::srcdsMessageHandler);

        int argc;
        char** argv = ::getProgramArguments(&argc);
        m_application.reset(new QCoreApplication(argc, argv));

        // http://stackoverflow.com/questions/25661295/why-does-qcoreapplication-call-setlocalelc-all-by-default-on-unix-linux
        setlocale(LC_NUMERIC, "C");

        if (!QDBusConnection::sessionBus().isConnected()) {
            qCritical("Cannot connect to the D-Bus session bus");
            return false;
        }

        m_gameServer.reset(new morgoth::GameServer);

//        ConVarRef hostname("hostname");
//        static_cast<ConVar*>(hostname.GetLinkedConVar())->InstallChangeCallback(
//            [](IConVar *var, const char *pOldValue, float flOldValue) {
//                connector.gameServer()
//            });

        qInfo("Running");

        return true;
    } else {
        qInfo("morgoth connector already loaded; ignoring.");
        return false;
    }
}

void Connector::Unload()
{
    m_loadCount -= 1;

    if (m_loadCount == 0) {
        m_application->processEvents();
        m_application->quit();
        m_application->processEvents();
        delete m_application.take();

        ConVar_Unregister();
        DisconnectTier1Libraries();
    }
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
    m_gameServer->setMap(pMapName);
}

void Connector::ServerActivate(edict_t* /*pEdictList*/, int /*edictCount*/, int /*clientMax*/)
{

}

void Connector::GameFrame(bool /*simulating*/)
{
    QCoreApplication::processEvents();
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

Connector* Connector::instance()
{
    return &connector;
}
