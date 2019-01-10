#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <cstdint>
#include "interface.h"
#include "engine/iserverplugin.h"

class Connector: public IServerPluginCallbacks {
public:
    Connector();
    virtual ~Connector();

    // Initialize the plugin to run
    // Return false if there is an error during startup.
    bool Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) override;

    // Called when the plugin should be shutdown
    void Unload() override;

    // called when a plugins execution is stopped but the plugin is not unloaded
    void Pause() override;

    // called when a plugin should start executing again (sometime after a Pause() call)
    void UnPause() override;

    // Returns string describing current plugin.  e.g., Admin-Mod.
    const char* GetPluginDescription() override;

    // Called any time a new level is started (after GameInit() also on level transitions within a game)
    void LevelInit(char const *pMapName) override;

    // The server is about to activate
    void ServerActivate(edict_t *pEdictList, int edictCount, int clientMax) override;

    // The server should run physics/think on all edicts
    void GameFrame(bool simulating) override;

    // Called when a level is shutdown (including changing levels)
    void LevelShutdown() override;

    // Client is going active
    void ClientActive(edict_t *pEntity) override;

    // Client is disconnecting from server
    void ClientDisconnect(edict_t *pEntity) override;

    // Client is connected and should be put in the game
    void ClientPutInServer(edict_t *pEntity, char const *playername) override;

    // Sets the client index for the client who typed the command into their console
    void SetCommandClient(int index) override;

    // A player changed one/several replicated cvars (name etc)
    void ClientSettingsChanged(edict_t *pEdict) override;

    // Client is connecting to server ( set retVal to false to reject the connection )
    //	You can specify a rejection message by writing it into reject
    PLUGIN_RESULT ClientConnect(bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen) override;

    // The client has typed a command at the console
    PLUGIN_RESULT ClientCommand(edict_t *pEntity, const CCommand &args) override;

    // A user has had their network id setup and validated
    PLUGIN_RESULT NetworkIDValidated(const char *pszUserName, const char *pszNetworkID) override;

    // This is called when a query from IServerPluginHelpers::StartQueryCvarValue is finished.
    // iCookie is the value returned by IServerPluginHelpers::StartQueryCvarValue.
    // Added with version 2 of the interface.
    void OnQueryCvarValueFinished(QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue) override;

    // added with version 3 of the interface.
    void OnEdictAllocated(edict_t *edict) override;
    void OnEdictFreed(const edict_t *edict) override;

private:
    int m_loadCount = 0;
    int m_clientCommandIndex;

};

#endif // CONNECTOR_H
