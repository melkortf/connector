#include "eventstream.h"
#include <fstream>

EventStream::EventStream(const std::string& fileName)
{
    m_output.open(fileName, std::ios::app);
}

EventStream::~EventStream()
{
    m_output.close();
}

EventStream& EventStream::operator <<(const std::string& event)
{
    m_output << event << std::endl << std::flush;
    return *this;
}
