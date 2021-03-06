#pragma once

#include <QObject>
#include <QList>
#include <QTcpSocket>
#include <QTimer>
#include "sshchannel.h"
#include "sshkey.h"

Q_DECLARE_LOGGING_CATEGORY(sshclient)

class  SshClient : public QObject {
    Q_OBJECT

private:
    LIBSSH2_SESSION    * m_session;
    LIBSSH2_KNOWNHOSTS * m_knownHosts;
    QString m_name;
    QMap<QString, SshChannel *> m_channels;
    QTcpSocket m_socket;

    quint16 m_port;
    int m_errorcode;
    bool m_sshConnected;

    QString m_hostname;
    QString m_username;
    QString m_passphrase;
    QString m_privateKey;
    QString m_publicKey;
    QString m_errorMessage;
    SshKey  m_hostKey;
    qint64 m_cntTxData;
    qint64 m_cntRxData;
    QTimer m_cntTimer;
    QTimer m_keepalive;

public:
    SshClient(const QString &name = "noname", QObject * parent = nullptr);
    virtual ~SshClient();

    QString getName() const;

public slots:
    int connectToHost(const QString & username, const QString & hostname, quint16 port = 22);
    void disconnectFromHost();
    QString runCommand(const QString &command);
    quint16 openLocalPortForwarding(const QString &servicename, quint16 port, quint16 bind);
    quint16 openRemotePortForwarding(const QString &servicename, quint16 port);
    void closePortForwarding(const QString &servicename);
    void setKeys(const QString &publicKey, const QString &privateKey);
    bool loadKnownHosts(const QString &file);
    QString sendFile(const QString &src, const QString &dst);
    void setPassphrase(const QString & pass);
    bool saveKnownHosts(const QString &file);
    bool addKnownHost  (const QString &hostname, const SshKey &key);
    QString banner();
    void waitSocket();

public slots:

    LIBSSH2_SESSION *session();
    bool channelReady();
    bool loopWhileBytesWritten(int msecs);
    bool getSshConnected() const;


signals:
    void connected();
    void unexpectedDisconnection();
    void disconnected();
    void xfer_rate(qint64 tx, qint64 rx);
    void sshDataReceived();
    void sshReset();
    void _connectionFailed();
    void sFtpXfer();


public slots:
    void askDisconnect();


private slots:
    void _readyRead();
    void _disconnected();
    void _getLastError();
    void _tcperror(QAbstractSocket::SocketError err);
    void _cntRate();
    void _sendKeepAlive();
};
