#include "interfacestore.h"

IVEngineServer* InterfaceStore::engineServer = nullptr;
IPlayerInfoManager* InterfaceStore::playerInfoManager = nullptr;

bool InterfaceStore::isValid()
{
    return engineServer
            && playerInfoManager;
}
