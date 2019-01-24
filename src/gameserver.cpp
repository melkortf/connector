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

    // Just after first map change, fetch this server's IP address and port
    connect(this, &GameServer::mapChanged, this, &GameServer::fetchUrl);
}

GameServer::~GameServer()
{

}

void GameServer::setMap(const QString& map)
{
    m_map = map;
    emit mapChanged(m_map);
}

int GameServer::maxPlayers() const
{
    return SrcdsWrapper::getMaxPlayers();
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

void GameServer::fetchUrl()
{
    int ip = SrcdsWrapper::getConVarInt("hostip");
    int quads[] = { ip >> 24 & 0x000000FF, ip >> 16 & 0x000000FF, ip >> 8 & 0x000000FF, ip & 0x000000FF };

    int port = SrcdsWrapper::getConVarInt("hostport");
    m_address = QStringLiteral("%1.%2.%3.%4:%5").arg(QString::number(quads[0]), QString::number(quads[1]),
            QString::number(quads[2]), QString::number(quads[3]), QString::number(port));
    emit addressChanged(m_address);

    disconnect(this, &GameServer::mapChanged, this, &GameServer::fetchUrl);
}

} // namespace morgoth
