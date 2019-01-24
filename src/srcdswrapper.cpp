#include "srcdswrapper.h"
#include <convar.h>
#include <map>
#include <string>

namespace {

std::map<std::string, ConVarRef> conVars;
std::map<std::string, std::function<void (std::string)>> conVarHandlers;

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

void SrcdsWrapper::conVarChangeHandler(IConVar* var, const char* /*pOldValue*/, float /*flOldValue*/)
{
    std::string name(var->GetName());
    auto handler = conVarHandlers.find(name);
    if (handler != conVarHandlers.end()) {
        handler->second(static_cast<ConVar*>(var)->GetString());
    }
}

} // namespace morgoth
