#include "gameserver.h"
#include <QtCore>

namespace morgoth {

GameServer::GameServer(QObject* parent)  :
    QObject(parent)
{
}

void GameServer::setMap(const QString& map)
{
    qDebug() << Q_FUNC_INFO;
    m_map = map;
    emit mapChanged(m_map);
}

} // namespace morgoth
