#include "gameserver.h"
#include "gameserveradaptor.h"
#include <QtCore>
#include <QtDBus>

namespace morgoth {

GameServer::GameServer(QObject* parent)  :
    QObject(parent)
{
    new GameServerAdaptor(this);
    QDBusConnection::sessionBus().registerObject("/", this);
}

void GameServer::setMap(const QString& map)
{
    qDebug() << Q_FUNC_INFO;
    m_map = map;
    emit mapChanged(m_map);
}

} // namespace morgoth
