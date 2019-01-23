#include "srcdswrapper.h"
#include <convar.h>
#include <map>
#include <string>

namespace morgoth {

class SrcdsWrapperPrivate {
public:
    std::map<std::string, ConVarRef> conVars;

};

SrcdsWrapper::SrcdsWrapper() : d(new SrcdsWrapperPrivate)
{

}

SrcdsWrapper::~SrcdsWrapper()
{

}

const char* SrcdsWrapper::getConVarString(const char* cvarName) const
{
    // cache all ConVarRefs, as the lookup is expensive
    if (!d->conVars.count(cvarName)) {
        d->conVars.insert(std::make_pair(cvarName, ConVarRef(cvarName)));
    }

    return d->conVars.at(cvarName).GetString();
}

} // namespace morgoth
