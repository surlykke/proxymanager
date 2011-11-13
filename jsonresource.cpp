#include "jsonresource.h"
#include <QMutexLocker>
#include <QDebug>
#include <QBuffer>

JsonResource::JsonResource(QString url, QObject *parent): QObject(parent), url(url) {
    connect(&qnam, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished()));
}

void JsonResource::GET(ParameterSet parameters) {
    QUrl url(this->url);
    ParameterSet::const_iterator it;
    for (it = parameters.constBegin(); it != parameters.constEnd(); it++) {
        url.addQueryItem(it.key(), it.value());
    }

    QNetworkRequest request(url);
    doRequest(request, "GET");
}

void JsonResource::POST(QByteArray payload) {
    QNetworkRequest request(url);
    doRequest(request, "POST", payload);
}

void JsonResource::PUT(QByteArray payload) {
    QNetworkRequest request(url);
    doRequest(request, "PUT", payload);
}

void JsonResource::DELETE() {
    QNetworkRequest request(url);
    doRequest(request, "DELETE");
}

void JsonResource::doRequest(QNetworkRequest &request, QString verb, QByteArray payload) {
    QMutexLocker mutexLocker(&lock);
    QBuffer data(&payload);
    QNetworkReply *reply = qnam.sendCustomRequest(request, verb.toAscii(), &data);
    qDebug() << "request " << verb << " afsendt";

    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        error = false;
        response = reply->readAll();
    }
    else {
        error = true;
        errorMsg = reply->errorString();
    }

    reply->deleteLater();
}


void JsonResource::requestFinished() {
    qDebug() << "finished signal received...";
    loop.exit();
}
