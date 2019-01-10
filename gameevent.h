#ifndef GAMEEVENT_H
#define GAMEEVENT_H

#include <list>
#include <string>

class GameEvent {
public:
    explicit GameEvent(const std::string& name);

    void addArgument(const std::string& argument);

    std::string toJson() const;

    const std::string& name() const { return m_name; }
    const std::list<std::string>& arguments() const { return m_arguments; }

private:
    std::string m_name;
    std::list<std::string> m_arguments;

};

#endif // GAMEEVENT_H
