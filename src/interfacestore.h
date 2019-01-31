#ifndef INTERFACESTORE_H
#define INTERFACESTORE_H

#include <eiface.h>
#include <game/server/iplayerinfo.h>

/**
 * Stores srcds interfaces
 */
class InterfaceStore {
public:
    InterfaceStore() = delete;

    /**
     * Returns true if all interfaces are good to use.
     */
    static bool isValid();

    static IVEngineServer* engineServer;
    static IPlayerInfoManager* playerInfoManager;

};

#endif // INTERFACESTORE_H
