#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scansettingsdialog.h"
#include "aboutdialog.h"
#include "localbcdialog.h"
#include "chosenpagedialog.h"
#include "scanlotsdialog.h"
#include "PreScanDialog.h"
#include "AdminDialog.h"
#include "fullscreenform.h"
#include "mygraphicsscene.h"
#include "consumestatusdialog.h"
#include "SleepTimeDialog.h"
#include "FileExport.h"
#include "CallbackNotice.h"
#include <QFileDialog>
#include <QDebug>
#include <QToolBar>
#include <QtMath>
#include <QDateTime>
#include <QPushButton>
#include <QDataStream>
//#include "GScanLists.h"
#include <QDesktopServices>
#include <QPdfWriter>
#include <QMarginsF>
#include <QCloseEvent>
#ifndef WIN32
#include <pwd.h>
#include <unistd.h>
#endif

QString getUsrName()
{
#ifndef WIN32
    struct passwd *pwd;
    pwd = getpwuid(getuid());
    return pwd->pw_name;
#else
    return QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
#endif // !WIN32
}

static QString usrName = getUsrName();

//CallbackNotice callback_notice;
//callback_message callback_errCode;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    RGBLineTable(new uchar[768]),
    greyLineTable(new uchar[256]),
    ui(new Ui::MainWindow),
    imageInfoList(nullptr),
    mes_box(new QMessageBox(this)),
    m_thread(new GetImgThread),
#ifndef WIN32
    ocrPDF(new HG_OCR()),
#endif // !WIN32
    imgInfoD(nullptr)
{
    ui->setupUi(this);

    globalImg = nullptr;//clear global image
    globalImgPath = "";//clear global image path
    appVersion = "2.0.0";
#ifndef WIN32
    appDirPath = "/home/" + usrName + "/HuaGoScan-" + appVersion;
    sysDirPath = "/usr/share/doc/HuaGoScan-" + appVersion;
#else
    appDirPath = "D:/Program Files/HuaGoScan-" + appVersion;
    sysDirPath = usrName + "/AppData/Local/HuaGoScan-" + appVersion;
#endif
    pic = new MyGraphicsScene;
    ui->graphicsView->setScene(pic);
    toolBar = new QToolBar(this);
    isViewFit = false;//whether the image auto scaled to fit window when window resizing
//    m_scanner = nullptr;
 //   settingsPara = new ScanParam;
    localPara = new LocalParams;
    //sacn processing dialog settings
    //mes_box->setWindowFlag(Qt::FramelessWindowHint);
    mes_box->setText(tr("扫描中..."));
    mes_box->addButton(QMessageBox::No)->setText(tr("取消扫描"));
    mes_box->addButton(QMessageBox::Cancel);
    mes_box->setDefaultButton(QMessageBox::Cancel);
    mes_box->button(QMessageBox::Cancel)->setVisible(false);

  //  callback_errCode = sendCallbackErr;
  //  connect(&callback_notice,&CallbackNotice::errorCodeSig,this,&MainWindow::on_getDevError);

    initFiles();
	initImageList();
    initToolBar();
	initStatusBar();
    initInterface();
	initUsbDevice();
    initScanParam();
    initGetImgThread();
}

MainWindow::~MainWindow()
{
    tmpdir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = tmpdir.entryInfoList();
    foreach (QFileInfo file, fileList)
    {
        file.dir().remove(file.fileName());
    }
    tmpdir.rmpath(tmpdir.absolutePath());
	//delete settingsPara;
    delete localPara;
    delete settings_Ini;
    if(!m_thread->isFinished())
    {
        m_thread->terminate();
        m_thread->wait();
    }
    delete m_thread;
    delete [] greyLineTable;
    delete [] RGBLineTable;
#ifndef WIN32
    delete ocrPDF;
#endif // !WIN32
    delete ui;
}


//Events (2)
void MainWindow::resizeEvent(QResizeEvent *event)
{
    if(isViewFit)
    {
        viewFit();
    }
}
void MainWindow::closeEvent(QCloseEvent *event)
  {
      if (writeIni() && writeColorTableFile() && writeGreyTableFile())
      {
          writeLog("application exit.");
          event->accept();
      }
      else
      {
          int a = QMessageBox::question(this,
                                tr("warning"),
                                tr("Settings' saving failed. Continue to exit?"),
                                QMessageBox::Yes|QMessageBox::No,
                                QMessageBox::Yes);
          if(a == QMessageBox::No)
            event->ignore();
      }
}

//init Functions (9)
void MainWindow::initFiles()
{
    //create app user dir
    tmpdir.mkpath(appDirPath);
    //create tmp images dir
	tmpdir.mkdir(appDirPath+"/tmp");
    tmpdir = QDir(appDirPath+"/tmp");
    //appoint settings files
    log.setFileName(appDirPath+"/scanlog.log");
    colorTableFile.setFileName(appDirPath+"/colorLine.table");
    greyTableFile.setFileName(appDirPath+"/greyLine.table");
    settings_Ini = new QSettings(appDirPath+"/GSCAN_paraConfig.ini",QSettings::IniFormat);
    //read table files
    readColorTableFile();
    readGreyTableFile();

    //appoint ocr trainning files
	QString str = "/usr/share/doc/HuaGoScan-"+appVersion+"/chi_sim.traineddata";
#ifndef WIN32
    ocrPDF->init(str.toLatin1().data(), HG_OCR::OCR);
#endif // !WIN32

    //write log
    writeLog("-------------------------------------------------\n",false);
    writeLog("application open.");
}
void MainWindow::initImageList()
{
    connect(ui->listWidget->rotate_n,&QAction::triggered,this,&MainWindow::on_imgListRClick_rotateAntiClock);
    connect(ui->listWidget->rotate_s,&QAction::triggered,this,&MainWindow::on_imgListRClick_rotateClock);
    connect(ui->listWidget->merge,&QAction::triggered,this,&MainWindow::on_imgListRClick_multiExport);
    connect(ui->listWidget->first_page,&QAction::triggered,this,&MainWindow::on_act_firstPage_triggered);
    connect(ui->listWidget->last_page,&QAction::triggered,this,&MainWindow::on_act_lastPage_triggered);
    connect(ui->listWidget->close,&QAction::triggered,this,&MainWindow::on_act_close_triggered);
}
void MainWindow::initToolBar()
{
    toolBar->addWidget(ui->btn_settings);
    toolBar->addWidget(ui->btn_scan);
    toolBar->addWidget(ui->btn_zoomFitH);
    toolBar->addWidget(ui->btn_zoomFitHV);
    toolBar->addWidget(ui->btn_zoomIn);
    toolBar->addWidget(ui->btn_zoomOut);
    toolBar->addWidget(ui->btn_zoomOri);
    toolBar->addWidget(ui->btn_antiClockRotate);
    toolBar->addWidget(ui->btn_clockRotate);
    toolBar->addWidget(ui->btn_prePage);
    toolBar->addWidget(ui->btn_nextPage);
    toolBar->addWidget(ui->btn_help);
    QSize toolBarIconSize = {40,40};
    toolBar->setIconSize(toolBarIconSize);
    toolBar->setMovable(false);
    this->addToolBar(toolBar);
}
void MainWindow::initStatusBar()
{
    this->statusBar()->addWidget(ui->lab_admin);
    this->statusBar()->addWidget(ui->lab_scanCnetTxt);
    this->statusBar()->addWidget(ui->lab_scanCnet);
    this->statusBar()->addWidget(ui->line_statusBarV1);
    this->statusBar()->addWidget(ui->lab_scanStatusTxt);
    this->statusBar()->addWidget(ui->lab_scanStatus);
    this->statusBar()->addWidget(ui->line_statusBarV2);
    this->statusBar()->addWidget(ui->lab_pageNumTxt);
    this->statusBar()->addWidget(ui->lab_currPageNum);
    this->statusBar()->addWidget(ui->lab_pageNumSep);
    this->statusBar()->addWidget(ui->lab_totalPageNum);
    this->statusBar()->addWidget(ui->line_statusBarV3);
    this->statusBar()->addWidget(ui->lab_selectPageNumTxt);
    this->statusBar()->addWidget(ui->lab_selectPageNum);
    connect(ui->listWidget,SIGNAL(selectedItemCountChange(int)),this,SLOT(on_selectedItemCountChange(int)));
    connect(&t_connection,SIGNAL(timeout()),this,SLOT(checkUsbConnection()));
    t_connection.start(1000);
}
void MainWindow::initInterface()
{
    setInterfaceVisible();
    ui->act_logOutput->setEnabled(false);
    ui->act_logClr->setEnabled(false);
    connect(ui->act_scanSettings,SIGNAL(triggered()),this,SLOT(showSettingsDialog()));
    connect(ui->act_verInfo,SIGNAL(triggered()),this,SLOT(showAboutDialog()));
    connect(ui->act_consumeStatus,SIGNAL(triggered()),this,SLOT(showConsumeStatusDialog()));
    connect(ui->act_BCG,SIGNAL(triggered()),this,SLOT(showLocalBCDialog()));
    connect(ui->act_fullScreen,SIGNAL(triggered()),this,SLOT(showFullScreenForm()));
    connect(this,SIGNAL(ImgViewAvailable(bool)),this,SLOT(on_ImgViewAvailable_changed(bool)));
    connect(ui->listWidget,&QListWidget::itemClicked,this,&MainWindow::showImage);
    connect(ui->btn_settings,SIGNAL(clicked()),this,SLOT(showSettingsDialog()));
    connect(ui->act_imgInfo,SIGNAL(triggered()),this,SLOT(showImgInfoDialog()));
    connect(ui->btn_zoomFitHV,SIGNAL(clicked()),this,SLOT(on_act_zoomFitHV_triggered()));
    connect(ui->btn_zoomFitH,SIGNAL(clicked()),this,SLOT(on_act_zoomFitH_triggered()));
    connect(ui->btn_zoomIn,SIGNAL(clicked()),this,SLOT(on_act_zoomIn_triggered()));
    connect(ui->btn_zoomOut,SIGNAL(clicked()),this,SLOT(on_act_zoomOut_triggered()));
    connect(ui->btn_zoomOri,SIGNAL(clicked()),this,SLOT(on_act_zoomOri_triggered()));
    connect(ui->btn_antiClockRotate,SIGNAL(clicked()),this,SLOT(on_act_antiClock_currPage_triggered()));
    connect(ui->btn_clockRotate,SIGNAL(clicked()),this,SLOT(on_act_clock_currPage_triggered()));
    connect(ui->btn_prePage,SIGNAL(clicked()),this,SLOT(on_act_prePage_triggered()));
    connect(ui->btn_nextPage,SIGNAL(clicked()),this,SLOT(on_act_nextPage_triggered()));
    connect(ui->btn_help,SIGNAL(clicked()),this,SLOT(on_act_help_triggered()));
    emit ImgViewAvailable(false);
}
void MainWindow::setInterfaceVisible()
{
    //unfunction items
    ui->act_print->setVisible(false);
    ui->act_devSelect->setVisible(false);
    ui->menu_splitView->menuAction()->setVisible(false);
    ui->act_deletePage->setVisible(false);
    ui->menu_insertPage->menuAction()->setVisible(false);

    //default not visible items
    ui->lab_admin->setVisible(false);
}
void MainWindow::initUsbDevice()
{
   /* auto scanners = GScanLists::FindAll();
    if (scanners.empty())
        return;
    m_scanner = *scanners.begin();

    if (m_scanner.get() && m_scanner->open())
    {
        qDebug("device opened");
        ui->lab_scanCnet->setText(tr(""已连接 "));
        ui->lab_scanStatus->setText(tr("空闲 "));
        f_connection = true;
        writeLog("device connected.");
        checkRollerScanNum();
    }
    else
    {
        scanEnable(false);
        qDebug("open failed");
    }*/
}
void MainWindow::initScanParam()
{
    /* settingsPara->ColorMode = settings_Ini->value("base_colorModeIndex",0).toInt();
    settingsPara->ImageProcessParam.PixType = settings_Ini->value("img_pixType",0).toInt();
    settingsPara->ImageProcessParam.multiOutput.canMultiOutput = settings_Ini->value("base_isMultiOutput",false).toBool();
    settingsPara->ImageProcessParam.multiOutput.MutltiOutputType = settings_Ini->value("base_multiOutType",0).toInt();
    settingsPara->paperSize = settings_Ini->value("base_paperSizeIndex",1).toInt();
    settingsPara->ImageProcessParam.paperSize = settings_Ini->value("img_paperSize",1).toInt();
    settingsPara->ImageProcessParam.cropRect.isCustomDefine = settings_Ini->value("base_isCustomArea",false).toBool();
    settingsPara->ImageProcessParam.cropRect.x = settings_Ini->value("base_customAreaX",0).toInt();
    settingsPara->ImageProcessParam.cropRect.y = settings_Ini->value("base_customAreaY",0).toInt();
    settingsPara->ImageProcessParam.cropRect.width = settings_Ini->value("base_customAreaW",2338).toInt();
    settingsPara->ImageProcessParam.cropRect.height = settings_Ini->value("base_customAreaH",3307).toInt();
    settingsPara->Resolution = 200.0f;
    settingsPara->ImageProcessParam.NativeResulution = 200.0f;
    settingsPara->ImageProcessParam.DestResulution = settings_Ini->value("base_resolution",200.0f).toDouble();
    settingsPara->IsDuplex = settings_Ini->value("base_isDuplex",true).toBool();
    settingsPara->ImageProcessParam.AutoDiscardBlank = settings_Ini->value("base_isDiscardBlank",false).toBool();
    settingsPara->ImageProcessParam.AutoDiscardBlankVince = settings_Ini->value("base_isDiscardBlankV",false).toBool();
    settingsPara->ImageProcessParam.IsFold = settings_Ini->value("base_isFold",false).toBool();
    settingsPara->ImageProcessParam.Brightness = settings_Ini->value("BCG_brightness",128.0f).toDouble();
    settingsPara->ImageProcessParam.Contrast = settings_Ini->value("BCG_contrast",4.0f).toDouble();
    settingsPara->ImageProcessParam.Gamma = settings_Ini->value("BCG_gamma",1.0f).toDouble();
    settingsPara->ImageProcessParam.customGamma.isDefined = settings_Ini->value("BCG_isCustomGama",false).toBool();
    QPoint start(0,0);
    QPoint end(255,255);
    QList<QVariant> list;
    list.append(QVariant(start));
    list.append(QVariant(end));
    localPara->colorLine.RGBlist = settings_Ini->value("BCG_RGBline",list).toList();
    localPara->colorLine.Rlist = settings_Ini->value("BCG_Rline",list).toList();
    localPara->colorLine.Glist = settings_Ini->value("BCG_Gline",list).toList();
    localPara->colorLine.Blist = settings_Ini->value("BCG_Bline",list).toList();
    localPara->greyLine.greylist = settings_Ini->value("BCG_greyline",list).toList();
    settingsPara->ImageProcessParam.FillBlackRect = settings_Ini->value("imgProc_isFillBlack",false).toBool();
    settingsPara->ImageProcessParam.AutoDescrew = settings_Ini->value("imgProc_isDescrew",false).toBool();
    settingsPara->ImageProcessParam.RefuseInflow = settings_Ini->value("imgProc_isAntiInfiltration",false).toBool();
    settingsPara->ImageProcessParam.OutHoleParam.OutHole = settings_Ini->value("imgProc_isOutHole",false).toBool();
    settingsPara->ImageProcessParam.OutHoleParam.OutHoleValue = settings_Ini->value("imgProc_outHoleValue",10).toInt();
    settingsPara->ImageProcessParam.Filter = settings_Ini->value("imgProc_filterIndex",0).toInt();
    settingsPara->ImageProcessParam.ColorCorrection = settings_Ini->value("imgProc_isColorCorrection",false).toBool();
    settingsPara->ImageProcessParam.RemoveMorr = settings_Ini->value("imgProc_isRemoveMoire",false).toBool();
    settingsPara->ImageProcessParam.ErrorExtention = settings_Ini->value("imgProc_isErrorExtention",false).toBool();
    settingsPara->ImageProcessParam.NosieDetach = settings_Ini->value("imgProc_isNoiseOptimization",false).toBool();
    settingsPara->ImageProcessParam.TextureRemove = settings_Ini->value("imgProc_isTextureRemove",false).toBool();
    settingsPara->ImageProcessParam.imageSharpen.canSharpen = settings_Ini->value("imgProc_isSharOrBlur",false).toBool();
    settingsPara->ImageProcessParam.imageSharpen.sharpenType = settings_Ini->value("imgProc_sharpenType",0).toInt();
    settingsPara->ImageProcessParam.MultiOutRed = settings_Ini->value("imgProc_isMultiOutRed",false).toBool();
    settingsPara->ImageProcessParam.AnswerSheetFilter = settings_Ini->value("imgProc_isAnswerSheetFilter",false).toBool();
    settingsPara->ScanCount = settings_Ini->value("feeding_scanCount",999).toInt();
    settingsPara->ImageProcessParam.Orentation = settings_Ini->value("feeding_pageOrientationIndex",0).toInt();
    settingsPara->ImageProcessParam.BackRotate180 = settings_Ini->value("feeding_isBackRotate180",false).toBool();
    settingsPara->UltrasonicDetect = settings_Ini->value("feeding_isUltDetect",true).toBool();
    settingsPara->BindingDetect = settings_Ini->value("feeding_isBindDetect",false).toBool();
    settingsPara->ScrewDetect = settings_Ini->value("feeding_isScrewDetect",false).toBool();
    settingsPara->ScrewTopLevel = settings_Ini->value("feeding_screwLevel",3).toInt();
    settingsPara->ImageProcessParam.SplitImage = settings_Ini->value("other_isSplitImage",false).toBool();
    localPara->jpgQuality.isSetQuality = settings_Ini->value("other_isSetJpgQuality",false).toBool();
    localPara->jpgQuality.Quality = settings_Ini->value("other_jpgQuality",100).toInt();
    localPara->isOcrPDF = settings_Ini->value("other_isOCRPDF",false).toBool();
    localPara->ocrType = settings_Ini->value("other_ocrType",0).toInt();
    localPara->isPreScan = settings_Ini->value("other_isPreScan",false).toBool(); */
}
void MainWindow::initGetImgThread()
{
    connect(m_thread,SIGNAL(loadImgToListview(QString)),this,SLOT(loadImgToListview(QString)));
    connect(m_thread,SIGNAL(viewFit()),this,SLOT(viewFit()));
    connect(m_thread,SIGNAL(getImgFinished(int)),this,SLOT(on_getImgFinished(int)));
    qRegisterMetaType<QList<cv::Mat>>("QList<cv::Mat>");//regist customize data type, or SLOT will not answer
    connect(m_thread,SIGNAL(preScanList(QStringList, QList<cv::Mat>, QString, QString)),this,SLOT(on_preScanList(QStringList, QList<cv::Mat>, QString, QString)));
}

//my Functions (33)
void MainWindow::showSettingsDialog()
{
    scanSettingsDialog d(this);
    d.exec();
}
void MainWindow::showAboutDialog()
{
    AboutDialog d(appVersion,this);
    d.exec();
}
void MainWindow::showLocalBCDialog()
{
    LocalBCDialog d(this,globalImg,appDirPath);
    bool a = d.exec();
    if(a)
    {
        //check the lastest copy file
        int index = 1;
        QString copyPath = appDirPath+"/tmp/bcAdjustCopy"+QString::number(index)+".jpg";
        while(QFileInfo::exists(copyPath))
        {
            index++;
            copyPath = appDirPath+"/tmp/bcAdjustCopy"+QString::number(index)+".jpg";
        }
        copyPath = appDirPath+"/tmp/bcAdjustCopy"+QString::number(index-1)+".jpg";
        //fresh shown copy image
        QImage* img = new QImage(copyPath);
        ui->listWidget->changeImage(globalImgPath,*img);
        ui->listWidget->currentItem()->setToolTip(copyPath);
        ui->listWidget->currentItem()->setData(Qt::WhatsThisRole, copyPath);
        imageInfoList = ui->listWidget->updateImageInfoList();
        freshImgInfo(imageInfoList->at(ui->listWidget->currentRow()));
        delete img;
        delete globalImg;
        globalImg = new QImage(copyPath);
        globalImgPath = copyPath;
        freshDisplayImg(globalImg,ui->graphicsView->width(),ui->graphicsView->height());
        //showImage(ui->listWidget->currentItem());
    }
}
void MainWindow::showFullScreenForm()
{
    fullScreenForm f(this);
    f.exec();
}
void MainWindow::showConsumeStatusDialog()
{
//    consumeStatusDialog d(this,m_scanner);
//    d.exec();
}
void MainWindow::freshDisplayImg(QImage *img,int width,int height)
{
    *img = img->scaled(width,height,Qt::KeepAspectRatio);
    pic->setImage(globalImg,img->width(),img->height());
    emit ImgViewAvailable(true);
}
void MainWindow::freshStatusBar()
{
    if(ui->listWidget->count() > 0)
    {
        ui->lab_currPageNum->setText(QString::number(ui->listWidget->currentRow()+1));//fresh current number in statusbar when click an item of listview
        ui->lab_totalPageNum->setText(QString::number(ui->listWidget->count()));//fresh total number when add images
    }
    else
    {
        ui->lab_currPageNum->setText("-");
        ui->lab_totalPageNum->setText("-");
    }

}
void MainWindow::freshImgInfo(ImageInfo imgInfo)
{
    ui->lab_imgSize->setText(imgInfo.image_size);
    ui->lab_imgFormat->setText(imgInfo.image_type);
    double data = imgInfo.image_data.toDouble();
    if(data <= 1024.0)
    {
        ui->lab_imgData->setText(QString::number(data, 'f', 0).toUpper() + "B");
    }
    else if(data <= 1048576.0)
    {
        ui->lab_imgData->setText(QString::number(data / 1024, 'f', 2).toUpper() + "KB");

    }
    else
    {
        ui->lab_imgData->setText(QString::number(data / 1024 / 1024, 'f', 2).toUpper() + "MB");
    }
}
void MainWindow::viewFit()
{
    if (globalImg == nullptr) return;
    isViewFit = true;
    QImage img = *globalImg;
    freshDisplayImg(&img,ui->graphicsView->width()-18,ui->graphicsView->height()-3);
}
void MainWindow::showImage(QListWidgetItem *item)
{
    if(item == nullptr) {
        pic->clear();
        delete globalImg;
        globalImg = nullptr;
        globalImgPath = "";
        ui->lab_imgData->setText("");
        ui->lab_imgFormat->setText("");
        ui->lab_imgSize->setText("");
        pic->setSceneRect(0,0,0,0);
        emit ImgViewAvailable(false);
        return;
    }
    QString itemName = item->toolTip();
    if(globalImg != nullptr){
        delete globalImg;
    }
    globalImg = new QImage(itemName);
    globalImgPath = itemName;
    QList<ImageInfo>::iterator iter;
    for (iter = imageInfoList->begin();iter != imageInfoList->end();iter++) {
        if((*iter).image_name == itemName)
        {
            freshImgInfo(*iter);
            QImage img((*iter).image_name);
            freshDisplayImg(&img,ui->graphicsView->width()-18,ui->graphicsView->height()-3);
            freshStatusBar();
            break;
        }
    }
}
void MainWindow::showImgInfoDialog()
{
    if(imgInfoD != nullptr)
        delete imgInfoD;
    imgInfoD = new imgInfoDialog(this,globalImgPath,tmpdir.absolutePath());
    imgInfoD->show();
}
void MainWindow::loadImgToListview(QString filePath)
{
    if(globalImg != nullptr)
    {
        delete globalImg;
        globalImg = nullptr;
    }
    globalImg = new QImage(filePath);
    if(globalImg->isNull())
    {
        delete globalImg;
        globalImg = nullptr;
        QMessageBox::information(this,tr("信息 "),tr("加载图像失败! "));
        writeLog("load an empty image.");
    }
    else
    {
        globalImgPath = filePath;
        ui->listWidget->addImage(filePath);
        imageInfoList = ui->listWidget->getImageInfo();
        freshImgInfo(imageInfoList->last());
        freshStatusBar();
    }
}
void MainWindow::scanLots(QString dirpath, bool isPreScan, QString format)
{
//    m_thread->wait();
//    //check if pre-scanning
//    int scanNum = settingsPara->ScanCount;
//    if(isPreScan)
//        settingsPara->ScanCount = 1;
//    m_scanner->config_scan(*settingsPara);
//    m_scanner->start();
//    if(m_scanner->get_err_code() == COUNT_MODE)
//    {
//        m_scanner->rst_err_code();
//        settingsPara->ScanCount = scanNum;
//        QMessageBox::information(this,tr("信息 "),tr("扫描仪处于计数模式，请先退出计数模式再扫描。 "));
//    }
//    else
//    {
//        this->toolBar->setEnabled(false);
//        this->menuBar()->setEnabled(false);
//        ui->lab_scanStatus->setText(tr("扫描中 "));
//        ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
//        m_thread->setThreadPara(m_scanner,dirpath,format,tmpdir,ocrPDF,settingsPara->ImageProcessParam.DestResulution,settingsPara->paperSize);
//        //check if ocr or jpg compression function is chosen
//        if(localPara->isOcrPDF || localPara->jpgQuality.isSetQuality)
//            m_thread->setThreadOptPara(localPara->isOcrPDF,localPara->jpgQuality.Quality);
//        //check if pre-scanning
//        if(isPreScan)
//            m_thread->setThreadToPreMode();
//        //check if pre_scan lists contains images
//        if(!prescan_matList.isEmpty())
//        {
//            if(dirpath != tmpdir.path())
//                m_thread->setInputList(prescan_tmpList,prescan_matList);
//            prescan_tmpList.clear();
//            prescan_matList.clear();
//        }
//        m_thread->start();
//        settingsPara->ScanCount = scanNum;
//        if(mes_box->exec() == QMessageBox::No)
//            m_scanner->stop();
//        this->toolBar->setEnabled(true);
//        this->menuBar()->setEnabled(true);
//        ui->lab_scanStatus->setText(tr("空闲 "));
//        ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
//    }
}
void MainWindow::scanEnable(bool enable)
{
    if(enable)
    {
        ui->btn_scan->setEnabled(true);
        ui->act_scanSingle->setEnabled(true);
        ui->act_scanLot->setEnabled(true);
    }
    else
    {
        ui->btn_scan->setEnabled(false);
        ui->act_scanSingle->setEnabled(false);
        ui->act_scanLot->setEnabled(false);
    }
}
bool MainWindow::writeIni()
{
    if(settings_Ini->isWritable())
    {
          settings_Ini->setValue("base_colorModeIndex",0);//test
//        settings_Ini->setValue("base_colorModeIndex",settingsPara->ColorMode);
//        settings_Ini->setValue("img_pixType",settingsPara->ImageProcessParam.PixType);
//        settings_Ini->setValue("base_isMultiOutput",settingsPara->ImageProcessParam.multiOutput.canMultiOutput);
//        settings_Ini->setValue("base_multiOutType",settingsPara->ImageProcessParam.multiOutput.MutltiOutputType);
//        settings_Ini->setValue("base_paperSizeIndex",settingsPara->paperSize);
//        settings_Ini->setValue("img_paperSize",settingsPara->ImageProcessParam.paperSize);
//        settings_Ini->setValue("base_isCustomArea",settingsPara->ImageProcessParam.cropRect.isCustomDefine);
//        settings_Ini->setValue("base_customAreaX",settingsPara->ImageProcessParam.cropRect.x);
//        settings_Ini->setValue("base_customAreaY",settingsPara->ImageProcessParam.cropRect.y);
//        settings_Ini->setValue("base_customAreaW",settingsPara->ImageProcessParam.cropRect.width);
//        settings_Ini->setValue("base_customAreaH",settingsPara->ImageProcessParam.cropRect.height);
//        settings_Ini->setValue("resolution",settingsPara->Resolution);
//        settings_Ini->setValue("img_resolution",settingsPara->ImageProcessParam.NativeResulution);
//        settings_Ini->setValue("base_resolution",settingsPara->ImageProcessParam.DestResulution);
//        settings_Ini->setValue("base_isDuplex",settingsPara->IsDuplex);
//        settings_Ini->setValue("base_isDiscardBlank",settingsPara->ImageProcessParam.AutoDiscardBlank);
//        settings_Ini->setValue("base_isDiscardBlankV",settingsPara->ImageProcessParam.AutoDiscardBlankVince);
//        settings_Ini->setValue("base_isFold",settingsPara->ImageProcessParam.IsFold);
//        settings_Ini->setValue("BCG_brightness",settingsPara->ImageProcessParam.Brightness);
//        settings_Ini->setValue("BCG_contrast",settingsPara->ImageProcessParam.Contrast);
//        settings_Ini->setValue("BCG_gamma",settingsPara->ImageProcessParam.Gamma);
//        settings_Ini->setValue("BCG_isCustomGama",settingsPara->ImageProcessParam.customGamma.isDefined);
//        settings_Ini->setValue("BCG_RGBline",localPara->colorLine.RGBlist);
//        settings_Ini->setValue("BCG_Rline",localPara->colorLine.Rlist);
//        settings_Ini->setValue("BCG_Gline",localPara->colorLine.Glist);
//        settings_Ini->setValue("BCG_Bline",localPara->colorLine.Blist);
//        settings_Ini->setValue("BCG_greyline",localPara->greyLine.greylist);
//        settings_Ini->setValue("imgProc_isFillBlack",settingsPara->ImageProcessParam.FillBlackRect);
//        settings_Ini->setValue("imgProc_isDescrew",settingsPara->ImageProcessParam.AutoDescrew);
//        settings_Ini->setValue("imgProc_isAntiInfiltration",settingsPara->ImageProcessParam.RefuseInflow);
//        settings_Ini->setValue("imgProc_isOutHole",settingsPara->ImageProcessParam.OutHoleParam.OutHole);
//        settings_Ini->setValue("imgProc_outHoleValue",settingsPara->ImageProcessParam.OutHoleParam.OutHoleValue);
//        settings_Ini->setValue("imgProc_filterIndex",settingsPara->ImageProcessParam.Filter);
//        settings_Ini->setValue("imgProc_isColorCorrection",settingsPara->ImageProcessParam.ColorCorrection);
//        settings_Ini->setValue("imgProc_isRemoveMoire",settingsPara->ImageProcessParam.RemoveMorr);
//        settings_Ini->setValue("imgProc_isErrorExtention",settingsPara->ImageProcessParam.ErrorExtention);
//        settings_Ini->setValue("imgProc_isNoiseOptimization",settingsPara->ImageProcessParam.NosieDetach);
//        settings_Ini->setValue("imgProc_isTextureRemove",settingsPara->ImageProcessParam.TextureRemove);
//        settings_Ini->setValue("imgProc_isSharOrBlur",settingsPara->ImageProcessParam.imageSharpen.canSharpen);
//        settings_Ini->setValue("imgProc_sharpenType",settingsPara->ImageProcessParam.imageSharpen.sharpenType);
//        settings_Ini->setValue("imgProc_isMultiOutRed",settingsPara->ImageProcessParam.MultiOutRed);
//        settings_Ini->setValue("imgProc_isAnswerSheetFilter",settingsPara->ImageProcessParam.AnswerSheetFilter);
//        settings_Ini->setValue("feeding_scanCount",settingsPara->ScanCount);
//        settings_Ini->setValue("feeding_pageOrientationIndex",settingsPara->ImageProcessParam.Orentation);
//        settings_Ini->setValue("feeding_isBackRotate180",settingsPara->ImageProcessParam.BackRotate180);
//        settings_Ini->setValue("feeding_isUltDetect",settingsPara->UltrasonicDetect);
//        settings_Ini->setValue("feeding_isBindDetect",settingsPara->BindingDetect);
//        settings_Ini->setValue("feeding_isScrewDetect",settingsPara->ScrewDetect);
//        settings_Ini->setValue("feeding_screwLevel",settingsPara->ScrewTopLevel);
//        settings_Ini->setValue("other_isSplitImage",settingsPara->ImageProcessParam.SplitImage);
//        settings_Ini->setValue("other_isSetJpgQuality",localPara->jpgQuality.isSetQuality);
//        settings_Ini->setValue("other_jpgQuality",localPara->jpgQuality.Quality);
//        settings_Ini->setValue("other_isOCRPDF",localPara->isOcrPDF);
//        settings_Ini->setValue("other_ocrType",localPara->ocrType);
//        settings_Ini->setValue("other_isPreScan",localPara->isPreScan);
        return 1;
    }
    else
    {
        return 0;
    }
}
bool MainWindow::writeLog(QString contents,bool withDataTime)
{
    if(!log.exists())
    {
        QString content= "";
        log.open(QIODevice::ReadWrite | QIODevice::Text);
        QByteArray str = content.toUtf8();
        log.write(str);
        log.close();
    }
    if(log.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        if(withDataTime)
        {
            QDateTime time = QDateTime::currentDateTime();
            QString timeStr = time.toString("yyyy-MM-dd hh:mm:ss");
            contents += (" at "+timeStr+"\n");
            QByteArray str = contents.toUtf8();
            log.write(str);
            log.close();
        }
        else
        {
            QByteArray str = contents.toUtf8();
            log.write(str);
            log.close();
        }
        return true;
    }
    else
    {
        return false;
    }
}
bool MainWindow::writeColorTableFile()
{
    if(!colorTableFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QMessageBox::warning(this,tr("警告 "),tr("写colorTable失败，请重试！ "));
        return 0;
    }
    QDataStream stream(&colorTableFile);
    char* ptr = reinterpret_cast<char*>(RGBLineTable);
    uint len = 768;
    stream.writeRawData(ptr,len);
    colorTableFile.close();
    return 1;
}
bool MainWindow::writeGreyTableFile()
{
    if(!greyTableFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QMessageBox::warning(this,tr("警告 "),tr("写greyTable失败，请重试！ "));
        return 0;
    }
    QDataStream stream(&greyTableFile);
    char* ptr = reinterpret_cast<char*>(greyLineTable);
    uint len = 256;
    stream.writeRawData(ptr,len);
    greyTableFile.close();
    return 1;
}
void MainWindow::readColorTableFile()
{
    if(!colorTableFile.exists())
    {
        for(int i=0;i<256;i++)
        {
            RGBLineTable[i * 3 + 0] = i;
            RGBLineTable[i * 3 + 1] = i;
            RGBLineTable[i * 3 + 2] = i;
        }
        return;
    }
    if(!colorTableFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,tr("警告 "),tr("读取colorTable失败，请重启应用程序！ "));
        close();
        return;
    }
    if(colorTableFile.size() == 0)
    {
        for(int i=0;i<256;i++)
        {
            RGBLineTable[i * 3 + 0] = i;
            RGBLineTable[i * 3 + 1] = i;
            RGBLineTable[i * 3 + 2] = i;
        }
    }
    QDataStream stream(&colorTableFile);
    char* ptr = reinterpret_cast<char*>(RGBLineTable);
    uint len = 768;
    stream.readRawData(ptr,len);
    colorTableFile.close();
}
void MainWindow::readGreyTableFile()
{
    if(!greyTableFile.exists())
    {
        for(int i=0;i<256;i++)
        {
            greyLineTable[i] = i;
        }
        return;
    }
    if(!greyTableFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,tr("警告 "),tr("读取greyTable失败，请重启应用程序！ "));
        close();
        return;
    }
    if(greyTableFile.size() == 0)
    {
        for(int i=0;i<256;i++)
        {
            greyLineTable[i] = i;
        }
    }
    QDataStream stream(&greyTableFile);
    char* ptr = reinterpret_cast<char*>(greyLineTable);
    uint len = 256;
    stream.readRawData(ptr,len);
    greyTableFile.close();
}
void MainWindow::saveAsFunc()
{
    QFileDialog saveAsDialog(nullptr,tr("图像另存为 "),"","(*.jpg);;(*.jpeg);;(*.png);;(*.bmp);;(*.pdf);;(*.tif)");
    saveAsDialog.setAcceptMode(QFileDialog::AcceptSave);
    QIcon icon("url(:/image_rsc/logo.ico)");
    saveAsDialog.setWindowIcon(icon);
    QString filter,filename;
    if(saveAsDialog.exec())
    {
        filename = saveAsDialog.selectedFiles().last();
        if(!filename.isEmpty())
        {
            filter = saveAsDialog.selectedNameFilter().remove("(").remove("*").remove(")");
            if (!filename.endsWith(filter))
            {
                filename += filter;
            }
            if(filter == ".pdf")
            {
                int dpi = qRound(double(globalImg->dotsPerMeterX()*0.0254));
                FileExport pdf;
                pdf.toPDF(globalImgPath,dpi,globalImg->width(),globalImg->height(),filename);
            }
            else if(filter == ".tif")
            {
                FileExport tif;
                tif.toTIFF(globalImgPath,filename);
            }
            else if(filter == ".ofd")
            {
                FileExport ofd;
                ofd.toOFD(globalImgPath,filename);
            }
            else
            {
                globalImg->save(filename);
            }
        }
    }
}
void MainWindow::on_getImgFinished(int index)
{
 /*   int timeout = 0;
    while(m_scanner->is_run())
    {
        if(timeout++ == 10)
        {
            timeout = 0;
            on_getDevError(HAVE_ERROR);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    mes_box->close();
    QString str = "scan finished("+QString::number(index)+").";
    writeLog(str);*/
}
void MainWindow::on_getDevError(int err_code)
{
    QString str;
    switch(err_code)
    {
    case HAVE_ERROR:
        QMessageBox::warning(this,tr("警告 "),tr("发生未知错误，扫描中止。"));
        str = "Undefined error,scan stopped. ERRCODE="+QString::number(err_code)+" .";
        writeLog(str);
        break;
    case OPEN_COVER:
        QMessageBox::warning(this,tr("警告 "),tr("机盖被打开！ "));
        str = "Cover opened,scan stopped. ERRCODE="+QString::number(err_code)+" .";
        writeLog(str);
        break;
    case NO_FEED:
        QMessageBox::information(this,tr("提示 "),tr("未检测到文稿，请放入需要扫描的文稿。 "));
        break;
    case FEED_IN_ERROR:
        QMessageBox::information(this,tr("提示 "),tr("进纸失败，请打开机盖重新整理文稿。 "));
        str = "Feeding failed. ERRCODE="+QString::number(err_code)+" .";
        writeLog(str);
        break;
    case PAPER_JAM:
        QMessageBox::warning(this,tr("警告 "),tr("卡纸，请打开机盖清理文稿。 "));
        str = "Paper jammed,scan stopped. ERRCODE="+QString::number(err_code)+" .";
        writeLog(str);
        break;
    case DETECT_DOUBLE_FEED:
        QMessageBox::information(this,tr("提示 "),tr("检测到双张进纸，请取走双张文稿。 "));
        str = "Double feeding,scan stopped. ERRCODE="+QString::number(err_code)+" .";
        writeLog(str);
        break;
    case DETECT_STAPLE:
        QMessageBox::warning(this,tr("警告 "),tr("检测到订书钉，请打开机盖清理文稿。 "));
        str = "Detected staple,scan stopped. ERRCODE="+QString::number(err_code)+" .";
        writeLog(str);
        break;
    case PAPER_SKEW:
        QMessageBox::information(this,tr("提示 "),tr("检测到进纸歪斜，请重新整理文稿。 "));
        str = "Skewed feeding. ERRCODE="+QString::number(err_code)+" .";
        writeLog(str);
        break;
    case FPGA_ERROR:
        QMessageBox::warning(this,tr("警告 "),tr("发生固件错误，扫描中止。 "));
        str = "Firmware error(FPGA),scan stopped. ERRCODE="+QString::number(err_code)+" .";
        writeLog(str);
        break;
    case PC_SCAN_BUSY_or_ERROR:
        QMessageBox::warning(this,tr("警告 "),tr("计算机繁忙或发生错误。 "));
        str = "Host busy or error,scan stopped. ERRCODE="+QString::number(err_code)+" .";
        writeLog(str);
        break;
    default:
        break;
    }
    //m_scanner->rst_err_code();//***important***
}
void MainWindow::on_selectedItemCountChange(int count)
{
    ui->lab_selectPageNum->setText(QString::number(count));
}
void MainWindow::on_imgListRClick_rotateClock()
{
    QStringList nameslist = ui->listWidget->getSelectedImages();
    rotateImgs(nameslist,90);
}
void MainWindow::on_imgListRClick_rotateAntiClock()
{
    QStringList nameslist = ui->listWidget->getSelectedImages();
    rotateImgs(nameslist,-90);
}
void MainWindow::on_imgListRClick_multiExport()
{
    QMessageBox messbox;
    messbox.setWindowTitle(tr("询问 "));
    messbox.setText("选择要保存的文件格式 ");
    QAbstractButton* pdfBtn = messbox.addButton("PDF",QMessageBox::ActionRole);
    QAbstractButton* tifBtn = messbox.addButton("TIFF",QMessageBox::ActionRole);
    QAbstractButton* ofdBtn = messbox.addButton("OFD",QMessageBox::ActionRole);
    messbox.addButton(QMessageBox::Cancel);
    messbox.button(QMessageBox::Cancel)->setVisible(false);
    messbox.exec();
    if(messbox.clickedButton() == pdfBtn)
        on_act_export_multiPdf_triggered();
    else if(messbox.clickedButton() == tifBtn)
        on_act_export_multiTif_triggered();
    else if(messbox.clickedButton() == ofdBtn)
        on_act_export_multiOfd_triggered();
    messbox.close();
}
void MainWindow::on_preScanList(QStringList preTmpList, QList<cv::Mat> preMatList, QString dirpath, QString format)
{
    mes_box->close();
    PreScanDialog d(nullptr);
    d. setImages(preTmpList[0],preTmpList[preTmpList.size()/2]);
    if(d.exec())
    {
        prescan_tmpList.clear();
        prescan_matList.clear();
        prescan_tmpList = preTmpList;
        prescan_matList = preMatList;
        for(int i=0;i<preTmpList.size();i++)
            loadImgToListview(preTmpList[i]);
//        if(settingsPara->ScanCount > 1)
//        {
//            f_prescan_continue = true;
//        }
//        else if(settingsPara->ScanCount == 1)
//        {
//            m_thread->setThreadPara(m_scanner,dirpath,format,tmpdir,ocrPDF,settingsPara->ImageProcessParam.DestResulution,settingsPara->paperSize);
//            //判断jpg压缩比例和ocr是否开启
//            if(localPara->isOcrPDF || localPara->jpgQuality.isSetQuality)
//                m_thread->setThreadOptPara(localPara->isOcrPDF,localPara->jpgQuality.Quality);
//            //判断是否存在预览扫描图像列表
//            if(!prescan_matList.isEmpty())
//            {
//                if(dirpath != tmpdir.path())
//                    m_thread->setInputList(prescan_tmpList,prescan_matList);
//                prescan_tmpList.clear();
//                prescan_matList.clear();
//            }
//            m_thread->start();
//        }
    }
    else
    {
        prescan_tmpList.clear();
        prescan_matList.clear();
    }
}
void MainWindow::rotateImgs(QStringList namelist, int rotateDir)
{
    //rotate images in listview
    QMatrix m;
    m.rotate(rotateDir);
    QImage *img;
    for(int i=0;i<namelist.size();i++)
    {
        QString filename = namelist.at(i);
        img = new QImage(filename);
        *img = img->transformed(m,Qt::FastTransformation);
        img->save(filename);
        ui->listWidget->changeImage(filename,*img);
        delete img;
    }
    img = nullptr;//destroy ptr
	imageInfoList = ui->listWidget->updateImageInfoList();
    freshImgInfo(imageInfoList->at(ui->listWidget->currentRow()));
    showImage(ui->listWidget->currentItem());//fresh image in main window
}
void MainWindow::singleSelectImage(int index)
{
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listWidget->setCurrentRow(index);
    ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
}
int MainWindow::getListWidgetCount()
{
    return ui->listWidget->count();
}
int MainWindow::getListWidgetCurrIndex()
{
    return ui->listWidget->currentRow();
}
QStringList MainWindow::getListWidgetFileNames()
{
    QList<int> indexlist;
    for(int i=0;i<ui->listWidget->count();i++)
    {
        indexlist.append(i);
    }
    return ui->listWidget->getImgNamesList(indexlist);
}
void MainWindow::checkRollerScanNum()
{
//    int num = m_scanner->get_roller_scaned();
//    if(num > 450000)
//    {
//        QMessageBox::warning(this,
//                             tr("耗材更换提示 "),
//                             tr("扫描仪耗材已达到更换值( ") + QString::number(num) + tr(")/450000)，请及时更换耗材。\n""(继续使用扫描仪可能出现进纸不顺畅，卡纸，双张等问题)"));
//    }
}
void MainWindow::sendCallbackErr(int err)
{
  //  callback_notice.callback_DevErr(err);
}
void MainWindow::checkUsbConnection()
{
   /* if(m_scanner->is_open() != f_connection)
    {
        if(f_connection)
        {
            ui->lab_scanCnet->setText(tr("未连接 "));
            ui->lab_scanStatus->setText(tr("--"));
            QMessageBox::warning(this,
                                 tr("连接中断 "),
                                 tr("扫描仪连接中断。 "));
            while(m_thread->isRunning())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
            m_scanner->close();
            qDebug()<<"device usb disconnected.";
            writeLog("device usb disconnected.");
            scanEnable(false);
        }
        else
        {
            m_scanner->open();
            ui->lab_scanCnet->setText(tr("已连接 "));
            ui->lab_scanStatus->setText(tr("空闲 "));
            qDebug()<<"device usb connected.";
            writeLog("device usb connected.");
            scanEnable(true);
        }
        f_connection = !f_connection;
    }*/
}

//menu slots (33)
void MainWindow::on_act_open_triggered()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(nullptr,
                                                      tr("打开图片 "),
                                                      "",
                                                      "(*.jpg *.jpeg *.png *.bmp)");
    ui->listWidget->cancelCurSelItemsState();
    QString fileName;
    QStringList::iterator it;
    for(it = fileNames.begin();it != fileNames.end();it++)
    {
        fileName = *it;
        if(!fileName.isEmpty())
        {
            loadImgToListview(fileName);
            viewFit();
        }
    }
    ui->lab_selectPageNum->setText(QString::number(fileNames.size()));
}
void MainWindow::on_act_save_triggered()
{
    if(globalImgPath.isEmpty())
        saveAsFunc();
    else
    {
        QFileInfo info(globalImgPath);
        if(info.absolutePath() == tmpdir.absolutePath())
            saveAsFunc();
        else
        {
            globalImg->save(globalImgPath);
            QMessageBox::information(this,tr("信息 "),tr("图像保存完毕。 "));
        }
    }
}
void MainWindow::on_act_saveAs_triggered()
{
    saveAsFunc();
}
void MainWindow::on_act_export_multiPdf_triggered()
{
    QStringList filesList = ui->listWidget->getSelectedImages();
    QList<QSize> filesSize = ui->listWidget->getCurSelectedImagesSize();
    QList<int> filesDPI = ui->listWidget->getCurSelectedImageDPI();
    if(filesList.isEmpty() || filesSize.isEmpty() || filesDPI.isEmpty())
    {
        QMessageBox::warning(this,tr("警告 "),tr("选取图像失败，请重试！ "));
        return;
    }
    QString dst = QFileDialog::getSaveFileName(nullptr,tr("导出多页pdf文档 "),"","(*.pdf)") + ".pdf";
    FileExport pdf;
    if(filesList.size()==1)
        pdf.toPDF(filesList.at(0),filesDPI.at(0),filesSize.at(0).width(),filesSize.at(0).height(),dst);
    else
        pdf.toPDFs(filesList,filesDPI,filesSize,dst);
}
void MainWindow::on_act_export_multiTif_triggered()
{
    QStringList filesList = ui->listWidget->getSelectedImages();
    if(filesList.isEmpty())
    {
        QMessageBox::warning(this,tr("警告 "),tr("选取图像失败，请重试！ "));
        return;
    }
    QString dst = QFileDialog::getSaveFileName(nullptr,tr("导出多页tif文档 "),"","(*.tif)") + ".tif";
    FileExport tif;
    if(filesList.size()==1)
        tif.toTIFF(filesList.at(0),dst);
    else
        tif.toTIFFs(filesList,dst);
}
void MainWindow::on_act_export_multiOfd_triggered()
{
    QStringList filesList = ui->listWidget->getSelectedImages();
    if(filesList.isEmpty())
    {
        QMessageBox::warning(this,tr("警告 "),tr("选取图像失败，请重试！ "));
        return;
    }
    QString dst = QFileDialog::getSaveFileName(nullptr,tr("导出多页ofd文档 "),"","(*.ofd)") + ".ofd";
    FileExport ofd;
    if(filesList.size()==1)
        ofd.toOFD(filesList.at(0),dst);
    else
        ofd.toOFDs(filesList,dst);
}
void MainWindow::on_act_close_triggered()
{
    ui->listWidget->deleteSelectedItems();
    imageInfoList = ui->listWidget->updateImageInfoList();
}
void MainWindow::on_act_closeAll_triggered()
{
    ui->listWidget->deleteAllItems();
    imageInfoList = ui->listWidget->updateImageInfoList();
}
void MainWindow::on_act_quit_triggered()
{
    close();
}
void MainWindow::on_act_scanSingle_triggered()
{
//    checkRollerScanNum();
//    settingsPara->ScanCount = 1;
//    scanLots(tmpdir.path());
}
void MainWindow::on_act_scanLot_triggered()
{   
//    int scanNum = settingsPara->ScanCount;
//    checkRollerScanNum();
//    scanLotsDialog d(nullptr);
//    d.setWindowTitle(tr("选择批次文件保存的文件夹 "));
//    d.exec();
//    if (!d.result())return;
//    QString dirPath = d.directory().absolutePath();
//    if(dirPath.isEmpty())return;
//    if(localPara->isPreScan)
//    {
//        scanLots(dirPath, true, d.format());
//        if(f_prescan_continue)
//        {
//            settingsPara->ScanCount--;
//            scanLots(dirPath, false, d.format());
//            f_prescan_continue = false;
//        }
//    }
//    else
//    {
//        prescan_tmpList.clear();
//        prescan_matList.clear();
//        scanLots(dirPath, false, d.format());
//    }
//    settingsPara->ScanCount = scanNum;
}
void MainWindow::on_act_zoomIn_triggered()
{
    isViewFit = false;
    pic->zoomIn();
}
void MainWindow::on_act_zoomOut_triggered()
{
    isViewFit = false;
    pic->zoomOut();
}
void MainWindow::on_act_zoomOri_triggered()
{
    isViewFit = false;
    freshDisplayImg(globalImg,globalImg->width(),globalImg->height());
}
void MainWindow::on_act_zoomFitHV_triggered()
{
    viewFit();
}
void MainWindow::on_act_zoomFitH_triggered()
{
    QImage img = *globalImg;
    freshDisplayImg(&img,ui->graphicsView->width()-18,qCeil(img.height() * ui->graphicsView->width() / img.width()));
}
void MainWindow::on_act_antiClock_currPage_triggered()
{
    QList<int> indexlist;
    indexlist.append(ui->listWidget->currentRow());
    QStringList namelist = ui->listWidget->getImgNamesList(indexlist);
    rotateImgs(namelist,-90);
}
void MainWindow::on_act_antiClock_allPage_triggered()
{
    QList<int> indexlist;
    for(int i=0;i<ui->listWidget->count();i++)
    {
        indexlist.append(i);
    }
    QStringList namelist = ui->listWidget->getImgNamesList(indexlist);
    rotateImgs(namelist,-90);
}
void MainWindow::on_act_antiClock_oddPage_triggered()
{
    QList<int> indexlist;
    for(int i=0;i<ui->listWidget->count();i=i+2)
    {
        indexlist.append(i);
    }
    QStringList namelist = ui->listWidget->getImgNamesList(indexlist);
    rotateImgs(namelist,-90);
}
void MainWindow::on_act_antiClock_evenPage_triggered()
{
    QList<int> indexlist;
    for(int i=1;i<ui->listWidget->count();i=i+2)
    {
        indexlist.append(i);
    }
    QStringList namelist = ui->listWidget->getImgNamesList(indexlist);
    rotateImgs(namelist,-90);
}
void MainWindow::on_act_clock_currPage_triggered()
{
    QList<int> indexlist;
    indexlist.append(ui->listWidget->currentRow());
    QStringList namelist = ui->listWidget->getImgNamesList(indexlist);
    rotateImgs(namelist,90);
}
void MainWindow::on_act_clock_allPage_triggered()
{
    QList<int> indexlist;
    for(int i=0;i<ui->listWidget->count();i++)
    {
        indexlist.append(i);
    }
    QStringList namelist = ui->listWidget->getImgNamesList(indexlist);
    rotateImgs(namelist,90);
}
void MainWindow::on_act_clock_oddPage_triggered()
{
    QList<int> indexlist;
    for(int i=0;i<ui->listWidget->count();i=i+2)
    {
        indexlist.append(i);
    }
    QStringList namelist = ui->listWidget->getImgNamesList(indexlist);
    rotateImgs(namelist,90);
}
void MainWindow::on_act_clock_evenPage_triggered()
{
    QList<int> indexlist;
    for(int i=1;i<ui->listWidget->count();i=i+2)
    {
        indexlist.append(i);
    }
    QStringList namelist = ui->listWidget->getImgNamesList(indexlist);
    rotateImgs(namelist,90);
}
void MainWindow::on_act_toolBar_triggered(bool checked)
{
    if(checked)
    {
        toolBar->setVisible(true);
    }
    else
    {
        toolBar->setVisible(false);
    }
}
void MainWindow::on_act_statusBar_triggered(bool checked)
{
    if(checked)
    {
        this->statusBar()->setVisible(true);
    }
    else
    {
        this->statusBar()->setVisible(false);
    }
}
void MainWindow::on_act_thumBar_triggered(bool checked)
{
    if(checked)
    {
        ui->listWidget->setVisible(true);
        ui->lab_imgData->setVisible(true);
        ui->lab_imgDataTxt->setVisible(true);
        ui->lab_imgFormat->setVisible(true);
        ui->lab_imgFormatTxt->setVisible(true);
        ui->lab_imgSize->setVisible(true);
        ui->lab_imgSizeTxt->setVisible(true);
    }
    else
    {
        ui->listWidget->setVisible(false);
        ui->lab_imgData->setVisible(false);
        ui->lab_imgDataTxt->setVisible(false);
        ui->lab_imgFormat->setVisible(false);
        ui->lab_imgFormatTxt->setVisible(false);
        ui->lab_imgSize->setVisible(false);
        ui->lab_imgSizeTxt->setVisible(false);
    }
}
void MainWindow::on_act_nextPage_triggered()
{
    singleSelectImage(ui->listWidget->currentRow()+1);
    showImage(ui->listWidget->currentItem());
}
void MainWindow::on_act_prePage_triggered()
{
    singleSelectImage(ui->listWidget->currentRow()-1);
    showImage(ui->listWidget->currentItem());
}
void MainWindow::on_act_lastPage_triggered()
{
    singleSelectImage(ui->listWidget->count()-1);
    showImage(ui->listWidget->currentItem());
}
void MainWindow::on_act_firstPage_triggered()
{
    singleSelectImage(0);
    showImage(ui->listWidget->currentItem());
}
void MainWindow::on_act_chosenPage_triggered()
{
    chosenPageDialog d(this);
    int list_count = ui->listWidget->count();
    if(list_count < chosenPage)
        chosenPage = list_count;
    d.setParam(chosenPage, list_count);
    if(d.exec())
    {
        chosenPage = d.chosen_page;
        singleSelectImage(chosenPage-1);
        showImage(ui->listWidget->currentItem());
    }
}
void MainWindow::on_act_adminLogin_triggered()
{
    AdminDialog d(this, ui->lab_admin->isVisible());
    if(d.exec())
    {
        ui->act_logOutput->setEnabled(true);
        ui->act_logClr->setEnabled(true);
        ui->lab_admin->setVisible(true);
    }
    else
    {
        ui->act_logOutput->setEnabled(false);
        ui->act_logClr->setEnabled(false);
        ui->lab_admin->setVisible(false);
    }
}
void MainWindow::on_act_logOutput_triggered()
{
    QFileDialog logOutputDialog(nullptr,tr("导出日志文档.. "),".","(*.txt)");
    logOutputDialog.setAcceptMode(QFileDialog::AcceptSave);
    QString filter,filename;
    if(logOutputDialog.exec())
    {
        filename = logOutputDialog.selectedFiles().last();
        if(!filename.isEmpty())
        {
            filter = logOutputDialog.selectedNameFilter().remove("(").remove("*").remove(")");
            if (!filename.endsWith(filter))
            {
                filename += filter;
            }
            QFile::copy(log.fileName(),filename);
        }
    }
}
void MainWindow::on_act_logClr_triggered()
{
    if(!log.exists())
    {
        QString content = "";
        log.open(QIODevice::ReadWrite | QIODevice::Text);
        QByteArray str = content.toUtf8();
        log.write(str);
        log.close();
    }
    if(log.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QString contents = "";
        QByteArray str = contents.toUtf8();
        log.write(str);
        log.close();
        QMessageBox::information(this,tr("信息 "),tr("日志清空完毕。 "));
    }
    else
        QMessageBox::warning(this,tr("警告 "),tr("日志清空失败，请重试！ "));
}
void MainWindow::on_act_setSleepTime_triggered()
{
    SleepTimeDialog d;
    d.exec();
}
void MainWindow::on_act_cacheClr_triggered()
{
    int a = QMessageBox::question(this,
                          tr("警告 "),
                          tr("清空缓存将关闭所有非本地加载的图像，未保存的图像将会丢失！\n""是否继续清空缓存？ "),
                          QMessageBox::Yes|QMessageBox::No,
                          QMessageBox::No);
    if(a == QMessageBox::Yes)
    {
        //检查imglist中非本地图像并关闭
        QStringList all_list = ui->listWidget->getImgNamesList();
        QList<int> index_list;
        for(int i  = 0; i < all_list.size(); i++)
        {
            QString img_name = all_list.at(i);
            QString tmp_dirname = tmpdir.path();
            if(img_name.length() > tmp_dirname.length())
            {
                if(img_name.left(tmp_dirname.length()) == tmp_dirname)
                {
                    index_list.append(i);
                }
            }
        }
        if(!index_list.empty())
        {
            ui->listWidget->deleteItems(index_list);
            imageInfoList = ui->listWidget->updateImageInfoList();
        }
        //清空系统tmp下的缓存图像
        tmpdir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
        QFileInfoList fileList = tmpdir.entryInfoList(); // 获取所有的文件信息
        foreach (QFileInfo file, fileList)//遍历文件信息
        {
            file.dir().remove(file.fileName());
        }
        //下发硬件缓存清除指令
//        if(m_scanner->clr_cache())
//            QMessageBox::information(this,tr("信息 "),tr("缓存已清空！ "));
//        else
//            QMessageBox::warning(this,tr("警告 "),tr("硬件缓存清空失败！ "));
    }
}
void MainWindow::on_act_help_triggered()
{
    QString file = sysDirPath + "/HuaGoScan_App_Help_manual_V1_0.pdf";
    QFileInfo info(file);
    if(!info.exists())
        QMessageBox::warning(this,tr("警告 "),tr("找不到帮助文档！文档可能已被删除或重命名！ "));
    else
        QDesktopServices::openUrl(QUrl::fromLocalFile(file));
}

//btn slots (1)
void MainWindow::on_btn_scan_clicked()
{
//    int scanNum = settingsPara->ScanCount;
//    checkRollerScanNum();
//    if(localPara->isPreScan)
//    {
//        scanLots(tmpdir.path(),true);
//        if(f_prescan_continue)
//        {
//            settingsPara->ScanCount--;
//            scanLots(tmpdir.path());
//            f_prescan_continue = false;
//        }

//    }
//    else
//    {
//        prescan_tmpList.clear();
//        prescan_matList.clear();
//        scanLots(tmpdir.path());
//    }
//    settingsPara->ScanCount = scanNum;
}

//exclution logic slot (1)
void MainWindow::on_ImgViewAvailable_changed(bool isAvailable)
{
    if(isAvailable)
    {
        ui->act_save->setEnabled(true);
        ui->act_saveAs->setEnabled(true);
        ui->act_close->setEnabled(true);
        ui->act_closeAll->setEnabled(true);
        ui->act_imgInfo->setEnabled(true);
        ui->menu_export->setEnabled(true);
        ui->menu_Edit->setEnabled(true);
        ui->act_zoomIn->setEnabled(true);
        ui->act_zoomOut->setEnabled(true);
        ui->act_zoomFitH->setEnabled(true);
        ui->act_zoomFitHV->setEnabled(true);
        ui->act_zoomOri->setEnabled(true);
        ui->act_fullScreen->setEnabled(true);
        ui->menu_Pages->setEnabled(true);
        ui->act_prePage->setEnabled(!(ui->listWidget->currentRow() == 0));
        ui->act_nextPage->setEnabled(!(ui->listWidget->currentRow() == (ui->listWidget->count()-1)));
        ui->act_firstPage->setEnabled((ui->listWidget->count() > 1)&&(ui->listWidget->currentRow() != 0));
        ui->act_lastPage->setEnabled((ui->listWidget->count() > 1)&&(ui->listWidget->currentRow() != ui->listWidget->count()-1));
        ui->act_chosenPage->setEnabled(true);
        ui->btn_zoomFitH->setEnabled(true);
        ui->btn_zoomFitHV->setEnabled(true);
        ui->btn_zoomIn->setEnabled(true);
        ui->btn_zoomOut->setEnabled(true);
        ui->btn_zoomOri->setEnabled(true);
        ui->btn_antiClockRotate->setEnabled(true);
        ui->btn_clockRotate->setEnabled(true);
        ui->btn_prePage->setEnabled(!(ui->listWidget->currentRow() == 0));
        ui->btn_nextPage->setEnabled(!(ui->listWidget->currentRow() == (ui->listWidget->count()-1)));
    }
    else
    {
        ui->act_save->setEnabled(false);
        ui->act_saveAs->setEnabled(false);
        ui->act_close->setEnabled(false);
        ui->act_closeAll->setEnabled(false);
        ui->act_imgInfo->setEnabled(false);
        ui->menu_export->setEnabled(false);
        ui->menu_Edit->setEnabled(false);
        ui->act_zoomIn->setEnabled(false);
        ui->act_zoomOut->setEnabled(false);
        ui->act_zoomFitH->setEnabled(false);
        ui->act_zoomFitHV->setEnabled(false);
        ui->act_zoomOri->setEnabled(false);
        ui->act_fullScreen->setEnabled(false);
        ui->menu_Pages->setEnabled(false);
        ui->btn_zoomFitH->setEnabled(false);
        ui->btn_zoomFitHV->setEnabled(false);
        ui->btn_zoomIn->setEnabled(false);
        ui->btn_zoomOut->setEnabled(false);
        ui->btn_zoomOri->setEnabled(false);
        ui->btn_antiClockRotate->setEnabled(false);
        ui->btn_clockRotate->setEnabled(false);
        ui->btn_prePage->setEnabled(false);
        ui->btn_nextPage->setEnabled(false);
    }
}

