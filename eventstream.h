#ifndef EVENTSTREAM_H
#define EVENTSTREAM_H

#include <fstream>
#include <string>

class EventStream {
public:
    EventStream(const std::string& fileName);
    virtual ~EventStream();

    bool isOK() const { return m_output.is_open(); }

    EventStream& operator <<(const std::string& event);

private:
    std::ofstream m_output;

};

#endif // EVENTSTREAM_H
