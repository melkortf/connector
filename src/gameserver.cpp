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
    m_wrapper(new SrcdsWrapper),
    m_gameLocation(QCoreApplication::applicationDirPath())
{
    new GameServerAdaptor(this);

    QDBusConnection conn = QDBusConnection::sessionBus();
    conn.registerObject("/", this);

    QString serviceName = ::findFirstAvailableServerId(conn);
    conn.registerService(serviceName);

    connect(qApp, &QCoreApplication::aboutToQuit, this, &GameServer::aboutToQuit);
}

GameServer::~GameServer()
{

}

void GameServer::setMap(const QString& map)
{
    m_map = map;
    emit mapChanged(m_map);
}

QString GameServer::getConVarValue(const QString& conVarName)
{
    return m_wrapper->getConVarString(conVarName.toLocal8Bit().constData());
}

} // namespace morgoth
