#ifndef SCANLOTSDIALOG_H
#define SCANLOTSDIALOG_H

#include <QDialog>
#include <QDir>


class QFileDialog;
class QComboBox;
class scanLotsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit scanLotsDialog(QWidget *parent = nullptr);
    ~scanLotsDialog();
    int result();
    QDir directory() const;
    QString format() const;
private slots:
    void filterChange(QString filter);

private:
    QFileDialog* a;
    QComboBox* box;
};

#endif // SCANLOTSDIALOG_H

