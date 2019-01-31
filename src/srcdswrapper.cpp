#include "srcdswrapper.h"
#include "interfacestore.h"
#include <convar.h>
#include <steam/steam_api.h>
#include <map>
#include <string>

namespace {

std::map<std::string, ConVarRef> conVars;
std::map<std::string, std::function<void (std::string)>> conVarHandlers;

edict_t* getPlayerBySlotId(int slotId)
{
    for (int i = 1; i < InterfaceStore::playerInfoManager->GetGlobalVars()->maxClients; ++i) {
        edict_t* player = InterfaceStore::engineServer->PEntityOfEntIndex(i);
        IPlayerInfo* playerInfo = InterfaceStore::playerInfoManager->GetPlayerInfo(player);
        if (playerInfo && playerInfo->GetUserID() == slotId)
            return player;
    }

    return nullptr;
}

}

namespace morgoth {

const char* SrcdsWrapper::getConVarString(const char* cvarName)
{
    // cache all ConVarRefs, as the lookup is expensive
    if (!conVars.count(cvarName)) {
        conVars.insert(std::make_pair(cvarName, ConVarRef(cvarName)));
    }

    return conVars.at(cvarName).GetString();
}

int SrcdsWrapper::getConVarInt(const char* cvarName)
{
    // cache all ConVarRefs, as the lookup is expensive
    if (!conVars.count(cvarName)) {
        conVars.insert(std::make_pair(cvarName, ConVarRef(cvarName)));
    }

    return conVars.at(cvarName).GetInt();
}

void SrcdsWrapper::trackConVar(const char* cvarName, std::function<void (std::string)> handler)
{
    conVarHandlers.insert(std::make_pair(cvarName, handler));
}

const char* SrcdsWrapper::getCurrentMap()
{
    return InterfaceStore::playerInfoManager->GetGlobalVars()->mapname.ToCStr();
}

int SrcdsWrapper::getMaxPlayers()
{
    return InterfaceStore::playerInfoManager->GetGlobalVars()->maxClients;
}

const char* SrcdsWrapper::getPlayerName(int userId)
{
    edict_t* player = getPlayerBySlotId(userId);
    IPlayerInfo* playerInfo = InterfaceStore::playerInfoManager->GetPlayerInfo(player);
    return playerInfo ? playerInfo->GetName() : nullptr;
}

uint64_t SrcdsWrapper::getPlayerSteamId(int userId)
{
    edict_t* player = getPlayerBySlotId(userId);
    const CSteamID* steamId = InterfaceStore::engineServer->GetClientSteamID(player);
    return steamId->ConvertToUint64();
}

void SrcdsWrapper::conVarChangeHandler(IConVar* var, const char* /*pOldValue*/, float /*flOldValue*/)
{
    std::string name(var->GetName());
    auto handler = conVarHandlers.find(name);
    if (handler != conVarHandlers.end()) {
        handler->second(static_cast<ConVar*>(var)->GetString());
    }
}

} // namespace morgoth
