#include "gameserver.h"
#include "config.h"
#include "gameserveradaptor.h"
#include "srcdswrapper.h"
#include "servermanagerinterface.h"
#include <QtCore>
#include <QtDBus>

namespace {

org::morgoth::ServerManager* getServerManager(QDBusConnection connection)
{
    return new org::morgoth::ServerManager("org.morgoth", "/servers", connection, qApp);
}

org::morgoth::ServerManager* findServerManager()
{
    org::morgoth::ServerManager* serverManager;
    if (QDBusConnection::sessionBus().isConnected()) {
        serverManager = ::getServerManager(QDBusConnection::sessionBus());
        if (serverManager->isValid()) {
            qDebug("Connected to morgoth on session bus");
            return serverManager;
        }
    }

    if (QDBusConnection::systemBus().isConnected()) {
        serverManager = ::getServerManager(QDBusConnection::systemBus());
        if (serverManager->isValid()) {
            qDebug("Connected to morgoth on system bus");
            return serverManager;
        } else {
            qWarning("morgoth not running; unable to register this game server");
            return nullptr;
        }
    }

    qWarning("Could not connect to D-Bus system bus. Is D-Bus daemon running?");
    return nullptr;
}

}

namespace morgoth {

GameServer::GameServer(QObject* parent)  :
    QObject(parent),
    m_gameLocation(QCoreApplication::applicationDirPath())
{
    new GameServerAdaptor(this);

    // this is done via queued connection, as the plugin might become
    // unresponsive for a couple of seconds
    connect(this, &GameServer::mapChanged, this, &GameServer::registerService, Qt::QueuedConnection);
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

void GameServer::registerService()
{
    if (!m_registered) {
        org::morgoth::ServerManager* serverManager = findServerManager();
        if (!serverManager)
            return;

        // find the location where morgoth is running its own D-Bus session and register to it
        QString address = serverManager->dbusServerAddress();
        QDBusConnection connection = QDBusConnection::connectToPeer(address, "morgoth-server");
        if (!connection.isConnected()) {
            auto error = connection.lastError();
            qWarning("unable to connect to %s: %s", qPrintable(address), qPrintable(error.message()));
            return;
        }

        m_registered = connection.registerObject("/", this);
        if (!m_registered) {
            auto error = connection.lastError();
            qWarning("unable to register game server at %s: %s", qPrintable(address), qPrintable(error.message()));
        }
    }

}

} // namespace morgoth
