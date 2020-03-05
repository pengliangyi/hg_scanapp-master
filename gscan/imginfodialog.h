#ifndef IMGINFODIALOG_H
#define IMGINFODIALOG_H

#include <QDialog>
#include <QFileInfo>

namespace Ui {
class imgInfoDialog;
}

class imgInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit imgInfoDialog(QWidget *parent,QString globalImgPath,QString tmpAbPath);
    ~imgInfoDialog();

private:
    Ui::imgInfoDialog *ui;
    QString makeImgData(QFileInfo info);
    bool isTmpFile;
    QString m_globalImgPath,m_tmpAbPath;
};

#endif // IMGINFODIALOG_H
