#ifndef SRCDSWRAPPER_H
#define SRCDSWRAPPER_H

#include <functional>

class Connector;
class IConVar;

namespace morgoth {

/**
 * A utility class to hide all the srcds compilation warnings in one place.
 */
class SrcdsWrapper {
public:
    static const char* getConVarString(const char* cvarName);
    static int getConVarInt(const char* cvarName);
    static void trackConVar(const char* cvarName, std::function<void(std::string)> handler);

    static const char* getCurrentMap();
    static int getMaxPlayers();

    SrcdsWrapper() = delete;

private:
    friend class ::Connector;
    static void conVarChangeHandler(IConVar *var, const char *pOldValue, float flOldValue);

};

} // namespace morgoth

#endif // SRCDSWRAPPER_H
