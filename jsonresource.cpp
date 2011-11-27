#include "jsonresource.h"
#include <QMutexLocker>
#include <QDebug>
#include <QBuffer>
#include "qjson/parser.h"
#include "qjson/serializer.h"

JsonResource::JsonResource(QUrl url, QObject *parent): QObject(parent), url(url) {
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

void JsonResource::POST(QVariant payload) {
    QNetworkRequest request(url);
    doRequest(request, "POST", payload);
}

void JsonResource::PUT(QVariant payload) {
    QNetworkRequest request(url);
    doRequest(request, "PUT", payload);
}

void JsonResource::DELETE() {
    QNetworkRequest request(url);
    doRequest(request, "DELETE");
}

void JsonResource::doRequest(QNetworkRequest &request, QString verb, QVariant payload) {
    QMutexLocker mutexLocker(&lock);

    QByteArray outData =  QJson::Serializer().serialize(payload);

    QBuffer data(&outData);
    QNetworkReply *reply = qnam.sendCustomRequest(request, verb.toAscii(), &data);
    qDebug() << "request " << verb << " afsendt";

    loop.exec();

    error = false;
    if (reply->error() == QNetworkReply::NoError) {
        bool ok;
        QJson::Parser parser;
        response = parser.parse(reply->readAll(), &ok);
        if (! ok) {
            error = true;
            errorMsg = parser.errorString();
        }
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
