#ifndef CALLBACKNOTICE_H
#define CALLBACKNOTICE_H

#include <QObject>

class CallbackNotice : public QObject
{
    Q_OBJECT
public:
    explicit CallbackNotice(QObject *parent = nullptr);
    void callback_DevErr(int err);
signals:
    void errorCodeSig(int err);

};

#endif // CALLBACKNOTICE_H
