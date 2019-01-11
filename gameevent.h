#ifndef GAMEEVENT_H
#define GAMEEVENT_H

#include <list>
#include <string>
#include <json.hpp>

class GameEvent {
public:
    explicit GameEvent(const std::string& name);

    std::string toJson() const;

    const std::string& name() const { return m_name; }

    void setArguments(const nlohmann::json& arguments);
    const nlohmann::json& arguments() const { return m_arguments; }

private:
    std::string m_name;
    nlohmann::json m_arguments;

};

#endif // GAMEEVENT_H
