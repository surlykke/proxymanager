#ifndef JSONRESOURCE_H
#define JSONRESOURCE_H
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMutex>
#include <QEventLoop>

typedef QMap<QString, QString> ParameterSet;

class JsonResource: public QObject
{
    Q_OBJECT
public:
    JsonResource(QString url, QObject *parent = 0);

    void GET(ParameterSet parameters = ParameterSet());
    void POST(QByteArray payload = QByteArray());
    void PUT(QByteArray payload = QByteArray());
    void DELETE();

    QByteArray response;
    bool    error;
    QString errorMsg;

public slots:
    void requestFinished();

private:
    void doRequest(QNetworkRequest &request, QString verb, QByteArray payload = QByteArray());
    QNetworkAccessManager qnam;
    QUrl url;
    QMutex lock; // Only one thread at a time in each object-instance.
    QEventLoop loop; // We go into this awaiting responses
};

#endif // JSONRESOURCE_H
