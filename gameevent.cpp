#include "gameevent.h"
#include "json.hpp"

using json = nlohmann::json;

GameEvent::GameEvent(const std::string& name) :
    m_name(name)
{

}

std::string GameEvent::toJson() const
{
    json json;
    json["name"] = name();
    json["arguments"] = arguments();

    return json.dump();
}

void GameEvent::setArguments(const nlohmann::json& arguments)
{
    m_arguments = arguments;
}
