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
    JsonResource(QUrl url, QObject *parent = 0);

    void GET(ParameterSet parameters = ParameterSet());
    void POST(QVariant data);
    void PUT(QVariant data);
    void DELETE();

    QVariant response;
    bool    error;
    QString errorMsg;

public slots:
    void requestFinished();

private:
    void doRequest(QNetworkRequest &request, QString verb, QVariant payload = QVariant());
    QNetworkAccessManager qnam;
    QUrl url;
    QMutex lock; // Only one thread at a time in each object-instance.
    QEventLoop loop; // We go into this awaiting responses
};

#endif // JSONRESOURCE_H
