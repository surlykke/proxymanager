#include "profile.h"

Profile::Profile()
{
}

Profile::Profile(QMap<QString, QVariant> serialized) {
    networkSignature = serialized["networkSignature"].toString();
    name = serialized["name"].toString();
    useProxy = serialized["useProxy"].toBool();
    proxyHost = serialized["proxyHost"].toString();
    proxyPort = serialized["proxyPort"].toInt();
    hostExceptions = serialized["hostExceptions"].toStringList();
    domainExceptions = serialized["domainExceptions"].toStringList();
}

Profile::~Profile() {
}

QMap<QString, QVariant> Profile::serialize() {
  QMap<QString, QVariant> result;
  result["networkSignature"] = networkSignature;
  result["name"] = name;
  result["useProxy"] = useProxy;
  result["proxyHost"] = proxyHost;
  result["proxyPort"] = proxyPort;
  result["hostExceptions"] = hostExceptions;
  result["domainExceptions"] = domainExceptions;
  return result;
}

void Profile::copy(Profile &otherProfile) {
  this->networkSignature = otherProfile.networkSignature;
  this->name = otherProfile.name;
  this->useProxy = otherProfile.useProxy;
  this->proxyHost = otherProfile.proxyHost;
  this->proxyPort = otherProfile.proxyPort;
  this->hostExceptions = otherProfile.hostExceptions;
  this->domainExceptions = otherProfile.domainExceptions;
}

