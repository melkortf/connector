#ifndef SRCDSWRAPPER_H
#define SRCDSWRAPPER_H

#include <functional>

namespace morgoth {

/**
 * A utility class to hide all the srcds compilation warnings in one place.
 */
class SrcdsWrapper {
public:
    static const char* getConVarString(const char* cvarName);
    static void trackConVar(const char* cvarName, std::function<void(std::string)> handler);

    SrcdsWrapper() = delete;

};

} // namespace morgoth

#endif // SRCDSWRAPPER_H
