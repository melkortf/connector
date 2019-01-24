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

    Q_PROPERTY(QString address READ address NOTIFY addressChanged)

signals:
    void aboutToQuit();
    void mapChanged(const QString& map);
    void addressChanged(const QString& address);
    void conVarChanged(QString conVarName, QString newValue);

public:
    GameServer(QObject* parent = nullptr);
    virtual ~GameServer();

    const QString& gameLocation() const { return m_gameLocation; }

    const QString& map() const { return m_map; }
    void setMap(const QString& map);

    const QString& address() const { return m_address; }

public slots:
    /**
     * \brief Queries for the current value of the given ConVar.
     */
    QString getConVarValue(const QString& conVarName);

    void watchConVar(const QString& conVarName);

private slots:
    void fetchUrl();

private:
    QString m_gameLocation;
    QString m_map;
    QString m_address;

};

} // namespace morgoth

#endif // GAMESERVER_H
