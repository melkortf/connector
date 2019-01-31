#include "gameserver.h"
#include "gameserveradaptor.h"
#include "srcdswrapper.h"
#include <QtCore>
#include <QtDBus>

namespace {

QString findFirstAvailableServerId(const QDBusConnection& connection)
{
    QDBusConnectionInterface* iface = connection.interface();
    int id = 0;

    while (true) {
        QString serviceName = QStringLiteral("org.morgoth.connector.gameserver_%1").arg(QString::number(id));
        if (!iface->isServiceRegistered(serviceName))
            return serviceName;
    }
}

}

namespace morgoth {

GameServer::GameServer(QObject* parent)  :
    QObject(parent),
    m_gameLocation(QCoreApplication::applicationDirPath())
{
    new GameServerAdaptor(this);

    QDBusConnection conn = QDBusConnection::sessionBus();
    conn.registerObject("/", this);

    QString serviceName = ::findFirstAvailableServerId(conn);
    // FIXME Register to DBus after a second or so to avoid
    //  the lack of responsivenes
    conn.registerService(serviceName);
}

GameServer::~GameServer()
{

}

QString GameServer::map() const
{
    return SrcdsWrapper::getCurrentMap();
}

QString GameServer::address() const
{
    static const int ip = SrcdsWrapper::getConVarInt("hostip");
    int quads[] = { ip >> 24 & 0x000000FF, ip >> 16 & 0x000000FF, ip >> 8 & 0x000000FF, ip & 0x000000FF };

    static const int port = SrcdsWrapper::getConVarInt("hostport");

    return QStringLiteral("%1.%2.%3.%4:%5").arg(QString::number(quads[0]), QString::number(quads[1]),
            QString::number(quads[2]), QString::number(quads[3]), QString::number(port));
}

int GameServer::maxPlayers() const
{
    return SrcdsWrapper::getMaxPlayers();
}

void GameServer::onMapChange(const QString& map)
{
    emit mapChanged(map);
}

QString GameServer::getConVarValue(const QString& conVarName)
{
    return SrcdsWrapper::getConVarString(conVarName.toLocal8Bit().constData());
}

void GameServer::watchConVar(const QString& conVarName)
{
    SrcdsWrapper::trackConVar(conVarName.toLocal8Bit().constData(), [this, conVarName](std::string newValue) {
        emit conVarChanged(conVarName, newValue.c_str());
    });
}

QString GameServer::getPlayerName(int userId)
{
    return SrcdsWrapper::getPlayerName(userId);
}

quint64 GameServer::getPlayerSteamId(int userId)
{
    return SrcdsWrapper::getPlayerSteamId(userId);
}

} // namespace morgoth
