#include "CallbackNotice.h"

CallbackNotice::CallbackNotice(QObject *parent) : QObject(parent)
{
}

void CallbackNotice::callback_DevErr(int err)
{
    emit errorCodeSig(err);
}
