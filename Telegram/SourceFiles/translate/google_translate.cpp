/*
This file is part of 64Gram Desktop fork,
the unofficial app based on Telegram Desktop.
For license and copyright information please follow this link:
https://github.com/frknkrc44/tdesktop-x64/blob/dev/LEGAL
*/

#include "google_translate.h"

GTranslate::GTranslate() {
    manager = new QNetworkAccessManager(this);
}

void GTranslate::translate(QString from, QString to, QString query, TranslationCallback onFinished) {
    QEventLoop eventLoop;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    query.replace(QRegularExpression("\\s"), "+");
    std::stringstream ss;
    ss  << "https://translate.google.com/m?sl=" << from.toStdString()
        << "&tl=" << to.toStdString()
        << "&q=" << query.toStdString();

    auto url = ss.str();

    auto request = QNetworkRequest(QUrl(QString::fromStdString(url)));
    auto setHeader = [&request](QString key, QString value) {
        request.setRawHeader(key.toUtf8(), value.toUtf8());
    };

    setHeader("User-Agent", "Mozilla/5.0 (X11; U; Linux armv7l; no-NO; rv:1.9.2.3pre) Gecko/20100723 Firefox/3.5 Maemo Browser 1.7.4.8 RX-51 N900");
    setHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8");
    setHeader("Accept-Language", "en-US");
    setHeader("Alt-Used", "translate.google.com");
    setHeader("Connection", "keep-alive");
    setHeader("Upgrade-Insecure-Requests", "1");
    setHeader("Sec-Fetch-Dest", "document");
    setHeader("Sec-Fetch-Mode", "navigate");
    setHeader("Sec-Fetch-Site", "cross-site");

    QNetworkReply *reply = manager->get(request);
    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        auto all = reply->readAll();
        auto rc = QString::fromStdString("<div class=\"result-container\">");
        auto idx = all.indexOf(rc.toStdString());
        auto right = all.mid(idx, idx + rc.size());
        auto left = right.mid(rc.size(), right.indexOf("</div>") - rc.size());

        onFinished(left);
    } else {
        auto error = reply->errorString();

        onFinished(error);
    }

    reply->deleteLater();
}
