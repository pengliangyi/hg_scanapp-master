#ifndef SCANSETTINGSDIALOG_H
#define SCANSETTINGSDIALOG_H

#include <QDialog>
#include <QDir>
#include <QListWidget>
#include "settings.h"


//class IDev;

namespace Ui {
class scanSettingsDialog;
}

class scanSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit scanSettingsDialog(QWidget *parent = nullptr);
    ~scanSettingsDialog();

private slots:
    void on_pbtn_help_clicked();
    void on_pbtn_default_clicked();
    void on_pbtn_ok_clicked();
    void on_pbtn_cancel_clicked();

    void on_pBtn_usrSetting_app_clicked();
    void on_pBtn_usrSetting_add_clicked();
    void on_pBtn_usrSetting_del_clicked();
    void on_pBtn_usrSetting_clr_clicked();
    void on_pBtn_usrSetting_inp_clicked();
    void on_pBtn_usrSetting_exp_clicked();
    void on_pBtn_usrSetting_save_clicked();

    void on_listWidget_itemSelectionChanged();
    void on_listWidget_currentRowChanged(int currentRow);
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_cbox_colorMode_currentIndexChanged(int index);
    void on_cbtn_multiStream_stateChanged(int arg1);
    void on_cbox_multiStream_currentIndexChanged(int index);
    void on_cbox_paperSize_currentIndexChanged(int index);
    void on_cbtn_sizeAreaSwitch_stateChanged(int arg1);
    void on_cbox_scanPage_currentIndexChanged(int index);
    void on_cbtn_colorLineSwitch_stateChanged(int arg1);
    void on_cbtn_deHole_stateChanged(int arg1);
    void on_cbtn_sharAndBlur_stateChanged(int arg1);
    void on_rbtn_pageNum_toggled(bool checked);
    void on_cbox_pageDirection_currentIndexChanged(int index);
    void on_cbtn_skewCheck_stateChanged(int arg1);
    void on_cbtn_jpgPercent_stateChanged(int arg1);
    void on_gbox_ocr_toggled(bool arg1);

    void on_Slider_brightness_valueChanged(int value);
    void on_spin_brightness_valueChanged(int arg1);
    void on_Slider_contrast_valueChanged(int value);
    void on_spin_contrast_valueChanged(int arg1);
    void on_Slider_gamma_valueChanged(int value);
    void on_dSpin_gamma_valueChanged(double arg1);
    void on_Slider_deHole_valueChanged(int value);
    void on_Slider_skewCheck_valueChanged(int value);
    void on_Slider_jpgPercent_valueChanged(int value);

    void showColorLineSetDialog();
    void showCutDialog();  

private:
    Ui::scanSettingsDialog *ui;
    QRectF sizeAreaRect;
    QString m_sysDirPath,m_appDirPath,m_iniDirPath,currIniPath;
    int m_paperSize;
    bool m_paperSizeChanged = false;
    bool isFirstRun = true;
    bool isUsrSettingChange = false;
    void set_globalLogic();
    void set_defaults();
    void set_funVisible();
//    bool set_settingsPara(ScanParam *param,LocalParams *lparam);
//    void get_settingsPara(ScanParam *param,LocalParams *lparam);
    void add_usrSetting(QString name);
    bool add_ini(QString name, QString dirPath);
    void delete_ini(QString name);
    void load_ini(QString name);
    void load_iniList();
    void save_iniList();
    QString showIniInputDialog();
    QStringList check_ini(QString dirPath);
    QVector<QList<QPoint>> rgbKeyPoint;
    QList<QPoint> grayKeyPoint;

};

#endif // SCANSETTINGSDIALOG_H
