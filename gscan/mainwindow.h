#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <QTimer>
#include "imageList.h"
#include "imginfodialog.h"
#include "getimgthread.h"
#include "settings.h"
#include <QDir>
#include <QSettings>
#include <QMessageBox>
#include <QTimer>
#include <thread>


//class IDev;


namespace Ui {
class MainWindow;
}

class MyGraphicsScene;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //ScanParam *settingsPara;
    LocalParams *localPara;
    QSettings *settings_Ini;
    QDir tmpdir;
    QImage *globalImg;
    QString globalImgPath,appDirPath,sysDirPath;
    uchar* RGBLineTable;
    uchar* greyLineTable;
    int chosenPage = 1;
    int getListWidgetCurrIndex();
    int getListWidgetCount();
    QStringList getListWidgetFileNames();

signals:
    void ImgViewAvailable(bool isAvailable);

private slots:
    void on_act_open_triggered();
    void on_act_save_triggered();
    void on_act_saveAs_triggered();
    void on_act_export_multiPdf_triggered();
    void on_act_export_multiTif_triggered();
    void on_act_export_multiOfd_triggered();
    void on_act_close_triggered();
    void on_act_closeAll_triggered();
    void on_act_quit_triggered();
    void on_act_scanSingle_triggered();
    void on_act_scanLot_triggered();
    void on_act_zoomIn_triggered();
    void on_act_zoomOut_triggered();
    void on_act_zoomFitH_triggered();
    void on_act_zoomFitHV_triggered();
    void on_act_zoomOri_triggered();
    void on_act_antiClock_currPage_triggered();
    void on_act_antiClock_allPage_triggered();
    void on_act_antiClock_oddPage_triggered();
    void on_act_antiClock_evenPage_triggered();
    void on_act_clock_currPage_triggered();
    void on_act_clock_allPage_triggered();
    void on_act_clock_oddPage_triggered();
    void on_act_clock_evenPage_triggered();
    void on_act_toolBar_triggered(bool checked);
    void on_act_statusBar_triggered(bool checked);
    void on_act_thumBar_triggered(bool checked);
    void on_act_nextPage_triggered();
    void on_act_prePage_triggered();
    void on_act_lastPage_triggered();
    void on_act_firstPage_triggered();
    void on_act_chosenPage_triggered();
    void on_act_adminLogin_triggered();
    void on_act_logOutput_triggered();
    void on_act_logClr_triggered();
    void on_act_setSleepTime_triggered();
    void on_act_cacheClr_triggered();
    void on_act_help_triggered();
    void on_btn_scan_clicked();

    void showSettingsDialog();
    void showAboutDialog();
	void showLocalBCDialog();
    void showFullScreenForm();
    void showConsumeStatusDialog();
	void showImage(QListWidgetItem*);//add the given list image to main window
    void showImgInfoDialog();
    void on_ImgViewAvailable_changed(bool isAvailable);//functions switch
	void on_getImgFinished(int index);//lots scan thread finish 
	void on_getDevError(int err_code);//show error information
    void on_selectedItemCountChange(int count);//fresh image count in statusbar
    void on_imgListRClick_rotateClock();
	void on_imgListRClick_rotateAntiClock();
	void on_imgListRClick_multiExport();
    void on_preScanList(QStringList preTmpList, QList<cv::Mat> preMatList, QString dirpath, QString format);//show preview images

    void loadImgToListview(QString filePath);//add given image to listview
    void viewFit();//image fits window
    void checkUsbConnection();

protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
//    std::shared_ptr<IDev> m_scanner;
    MyGraphicsScene *pic;
    QToolBar *toolBar;
    QList<ImageInfo> *imageInfoList;
    bool isViewFit;
    QFile log,colorTableFile,greyTableFile;
    QMessageBox* mes_box;//scan processing
    GetImgThread* m_thread;//get images from lower modules
    bool f_connection = false;//usb connection flag
    bool f_prescan_continue = false;//continue scan flag
    QTimer t_connection;
    imgInfoDialog *imgInfoD;
    QString appVersion;
    QStringList prescan_tmpList;//pre-scan tmp images
    QList<cv::Mat> prescan_matList;//pre-scan mat images

private:
    //init functions
	void initFiles();
    void initImageList();
    void initToolBar();
    void initStatusBar();
    void initInterface();
    void setInterfaceVisible();//functions visible
	void initUsbDevice();
    void initScanParam();
    void initGetImgThread();
    //interface functions
    void freshDisplayImg(QImage *img,int width,int height);
	void freshStatusBar();
    void freshImgInfo(ImageInfo imgInfo);
	void rotateImgs(QStringList namelist, int rotateDir);
    void singleSelectImage(int index);
	//file functions
    bool writeIni();
    bool writeLog(QString contents,bool withDataTime = true);
	bool writeColorTableFile();
    bool writeGreyTableFile();
    void readColorTableFile();
    void readGreyTableFile();
    void saveAsFunc();
	//scan functions
    void scanLots(QString dirpath,bool isPreScan = false,QString format = ".jpg");//scan as multi documents mode
	void scanEnable(bool enable);//scan button enable
    //check functions
	void checkRollerScanNum();//check scanner history total-scaned numbers
	static void sendCallbackErr(int err);//callback for error occured
    //test functions

};
#endif // MAINWINDOW_H
