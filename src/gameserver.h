#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QtCore/QObject>

namespace morgoth {

class GameServer : public QObject {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.morgoth.connector.GameServer")

    Q_PROPERTY(QString map READ map NOTIFY mapChanged)

signals:
    void mapChanged(const QString& map);

public:
    GameServer(QObject* parent = nullptr);

    const QString& map() const { return m_map; }
    void setMap(const QString& map);

private:
    QString m_map;

};

} // namespace morgoth

#endif // GAMESERVER_H
