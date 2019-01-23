#ifndef SRCDSWRAPPER_H
#define SRCDSWRAPPER_H

#include <memory>

namespace morgoth {

class SrcdsWrapperPrivate;

/**
 * A utility class to hide all the srcds compilation warnings in one place.
 */
class SrcdsWrapper {
public:
    SrcdsWrapper();
    virtual ~SrcdsWrapper();

    const char* getConVarString(const char* cvarName) const;

private:
    std::unique_ptr<SrcdsWrapperPrivate> d;

};

} // namespace morgoth

#endif // SRCDSWRAPPER_H
