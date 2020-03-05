#ifndef CHOSENPAGEDIALOG_H
#define CHOSENPAGEDIALOG_H

#include <QDialog>

namespace Ui {
class chosenPageDialog;
}

class chosenPageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit chosenPageDialog(QWidget *parent = nullptr);
    ~chosenPageDialog();
    int chosen_page = 0;
    void setParam(int chosenPage, int listCount);

private slots:
    virtual void accept();

private:
    Ui::chosenPageDialog *ui;
    int count = 0;
};

#endif // CHOSENPAGEDIALOG_H
