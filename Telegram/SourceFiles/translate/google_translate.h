/*
This file is part of 64Gram Desktop fork,
the unofficial app based on Telegram Desktop.
For license and copyright information please follow this link:
https://github.com/frknkrc44/tdesktop-x64/blob/dev/LEGAL
*/
#ifndef __GOOGLE_TRANSLATE_H__
#define __GOOGLE_TRANSLATE_H__

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QString>

#include <cstring>
#include <iostream>
#include <string>

using TranslationCallback = std::function<void(QString)>;

inline QByteArray QByteArray::fromStdString(const std::string &s)
{ return QByteArray(s.data(), int(s.size())); }

class GTranslate : public QObject {
    Q_OBJECT
    public:
        explicit GTranslate();
        ~GTranslate() {
            delete manager;
        }
        void translate(QString from, QString to, QString query, TranslationCallback onFinished);


    private:
        QNetworkAccessManager* manager;
};

#endif // __GOOGLE_TRANSLATE_H__
