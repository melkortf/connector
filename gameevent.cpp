#include "gameevent.h"
#include "json.hpp"

using json = nlohmann::json;

GameEvent::GameEvent(const std::string& name) :
    m_name(name)
{

}

void GameEvent::addArgument(const std::string& argument)
{
    m_arguments.push_back(argument);
}

std::string GameEvent::toJson() const
{
    json json;
    json["name"] = name();
    json["arguments"] = arguments();

    return json.dump();
}
