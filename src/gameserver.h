#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QtCore/QObject>

namespace morgoth {

class SrcdsWrapper;

class GameServer : public QObject {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.morgoth.connector.GameServer")

    Q_PROPERTY(QString gameLocation READ gameLocation CONSTANT)
    Q_PROPERTY(QString map READ map NOTIFY mapChanged)

signals:
    void aboutToQuit();
    void mapChanged(const QString& map);

public:
    GameServer(QObject* parent = nullptr);
    virtual ~GameServer();

    const QString& gameLocation() const { return m_gameLocation; }

    const QString& map() const { return m_map; }
    void setMap(const QString& map);

public slots:
    QString getConVarValue(const QString& conVarName);

private:
    QScopedPointer<SrcdsWrapper> m_wrapper;
    QString m_gameLocation;
    QString m_map;

};

} // namespace morgoth

#endif // GAMESERVER_H
