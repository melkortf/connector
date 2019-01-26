#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QtCore/QObject>

namespace morgoth {

/**
 * \brief The GameServer class provides an easy access to the running game server.
 */
class GameServer : public QObject {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.morgoth.connector.GameServer")

    /**
     * Contains the location of the server installation.
     */
    Q_PROPERTY(QString gameLocation READ gameLocation CONSTANT)

    /**
     * The currently running map.
     */
    Q_PROPERTY(QString map READ map NOTIFY mapChanged)

    /**
     * The "ip:port" string which identifies what address the server is listening on.
     */
    Q_PROPERTY(QString address READ address CONSTANT)

    /**
     * The "+maxplayers" launch property. It is 24 by default.
     */
    Q_PROPERTY(int maxPlayers READ maxPlayers CONSTANT)

signals:
    /**
     * Emitted when the plugin is being unloaded, most likely during the game server shutdown.
     */
    void aboutToQuit();

    void mapChanged(QString map);
    void conVarChanged(QString conVarName, QString newValue);

public:
    GameServer(QObject* parent = nullptr);
    virtual ~GameServer();

    const QString& gameLocation() const { return m_gameLocation; }
    QString map() const;
    QString address() const;
    int maxPlayers() const;

    void onMapChange(const QString& map);

public slots:
    /**
     * \brief Queries for the current value of the given ConVar.
     */
    QString getConVarValue(const QString& conVarName);

    void watchConVar(const QString& conVarName);

private:
    QString m_gameLocation;

};

} // namespace morgoth

#endif // GAMESERVER_H
