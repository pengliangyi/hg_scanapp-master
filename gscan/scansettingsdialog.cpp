#include "scansettingsdialog.h"
#include "ui_scansettingsdialog.h"
//#include "curvedialog.h"
//#include "scanareadialog.h"
#include "setpicclrtool.h"
#include "cutpapertool.h"
#include <QButtonGroup>
#include <QMessageBox>
#include "mainwindow.h"
#include <QDesktopServices>
#include <QVariant>
#include <QInputDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QSettings>
#include <QDir>
#include <QDebug>

enum{
    DEV_G200 = 1,
    DEV_G300 = 0
};

scanSettingsDialog::scanSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::scanSettingsDialog)
{
    ui->setupUi(this);

    set_globalLogic();
    set_defaults();
    set_funVisible();
//    get_settingsPara(dynamic_cast<MainWindow*>(parent)->settingsPara,dynamic_cast<MainWindow*>(parent)->localPara);
    m_paperSize = ui->cbox_paperSize->currentIndex();
    m_sysDirPath = dynamic_cast<MainWindow*>(parent)->sysDirPath;
    m_appDirPath = dynamic_cast<MainWindow*>(parent)->appDirPath;
    m_iniDirPath = m_appDirPath+"/settings";
    QDir dir;
    dir.mkdir(m_iniDirPath);
    load_iniList();
}

scanSettingsDialog::~scanSettingsDialog()
{
    delete ui;
    save_iniList();
}

//my Functions (6)
void scanSettingsDialog::set_globalLogic()
{
    QButtonGroup *bg1 = new QButtonGroup(this);
    bg1->addButton(ui->rbtn_maxPage,0);
    bg1->addButton(ui->rbtn_pageNum,1);
    QButtonGroup *bg2 = new QButtonGroup(this);
    bg2->addButton(ui->rbtn_sharpening1,0);
    bg2->addButton(ui->rbtn_sharpening2,1);
    bg2->addButton(ui->rbtn_blur1,2);
    bg2->addButton(ui->rbtn_blur2,3);
    QButtonGroup *bg3 = new QButtonGroup(this);
    bg3->addButton(ui->rbtn_ocrPDF,0);
    bg3->addButton(ui->rbtn_ocrTxt,1);
    bg3->addButton(ui->rbtn_ocrPDF_txt,2);
    connect(ui->tbtn_colorCurve,SIGNAL(clicked()),this,SLOT(showColorLineSetDialog()));
    connect(ui->tbtn_sizeArea,SIGNAL(clicked()),this,SLOT(showCutDialog()));
    ui->pBtn_usrSetting_del->setEnabled(false);
    ui->pBtn_usrSetting_clr->setEnabled(false);
    ui->pBtn_usrSetting_app->setEnabled(false);
    ui->pBtn_usrSetting_save->setEnabled(false);
}
void scanSettingsDialog::set_defaults()
{
    ui->cbox_colorMode->setCurrentIndex(0);//������ɫ��ģʽ��24λ��ɫ    
	ui->cbtn_multiStream->setCheckState(Qt::Unchecked);//�������������������ѡ    
	ui->cbox_multiStream->setCurrentIndex(0);//�����������������ɫ+�Ҷ�+�ڰ�
    ui->cbox_multiStream->setEnabled(false);
    ui->cbox_paperSize->setCurrentIndex(1);//������ֽ�ųߴ磺A3
    ui->cbtn_sizeAreaSwitch->setChecked(false);//�������Զ������򣺲���ѡ    
	ui->tbtn_sizeArea->setEnabled(false);
    ui->cbox_resolution->setCurrentIndex(2);//�������ֱ���DPI��200
    ui->cbox_scanPage->setCurrentIndex(1);//������ɨ���棺˫��
    ui->Slider_brightness->setValue(128);//���ȣ�128
    ui->spin_brightness->setValue(128);
    ui->Slider_contrast->setValue(4);//�Աȶȣ�4
    ui->spin_contrast->setValue(4);
    ui->Slider_gamma->setValue(10);//٤��ֵ��1.0
    ui->dSpin_gamma->setValue(1.0);
    ui->cbtn_colorLineSwitch->setChecked(false);//ɫ�����ߣ�����ѡ    
	ui->tbtn_colorCurve->setEnabled(false);

    ui->cbtn_deBlack->setCheckState(Qt::Unchecked);//����ͼ���������ڿ򣺲���ѡ    
	ui->cbtn_adjust->setCheckState(Qt::Unchecked);//����ͼ�����Զ���ƫ������ѡ    
	ui->cbtn_deMoire->setCheckState(Qt::Unchecked);//����ͼ����ȥ��Ħ���ƣ�����ѡ    
	ui->cbtn_antiInfi->setCheckState(Qt::Unchecked);//����ͼ������ֹ��͸������ѡ    
	ui->cbtn_deHole->setCheckState(Qt::Unchecked);//����ͼ���������Ƴ�������ѡ    
	ui->label_deHole->setEnabled(false);
    ui->Slider_deHole->setValue(1);
    ui->Slider_deHole->setEnabled(false);
    ui->label_deHolePara->setText("10%");
    ui->label_deHolePara->setEnabled(false);
    ui->cbox_deColor->setCurrentIndex(0);//�˾�����ɫ������ɫ    
	ui->label_deColor->setEnabled(false);
    ui->cbox_deColor->setEnabled(false);
    ui->cbtn_setBWDot->setCheckState(Qt::Unchecked);//�˾���������ɢ������ѡ    
	ui->cbtn_setBWDot->setEnabled(false);
    ui->cbtn_colorAdjust->setCheckState(Qt::Unchecked);//�˾���ɫ������������ѡ    
	ui->cbtn_noise->setCheckState(Qt::Unchecked);//�˾�������Ż�������ѡ    
	ui->cbtn_noise->setEnabled(false);
    ui->cbtn_texRmv->setCheckState(Qt::Unchecked);//�˾���ȥ���ƣ�����ѡ
    ui->gbox_sharAndBlur->setChecked(false);//�˾�������ģ��������ѡ    
	ui->rbtn_sharpening1->setChecked(true);//�˾�������ģ�����񻯣�ѡ��
    ui->gbox_sharAndBlur->setEnabled(true);
    ui->cbtn_specialMulti->setCheckState(Qt::Unchecked);//����ͼ�����������������ѡ    
	ui->cbtn_deRed_asheet->setCheckState(Qt::Unchecked);//����ͼ�������⿨���죺����ѡ
    ui->rbtn_maxPage->setChecked(true);//��ֽ������ɨ�裺ѡ��
    ui->rbtn_pageNum->setChecked(false);//��ֽ��ָ����������ѡ��
    ui->spin_pageNum->setValue(1);
    ui->spin_pageNum->setEnabled(false);
    ui->cbtn_doubleCheck->setCheckState(Qt::Checked);//��ֽ��˫�ż�⣺��ѡ    
	ui->cbtn_skewCheck->setCheckState(Qt::Unchecked);//��ֽ����б��⣺����ѡ    
	ui->Slider_skewCheck->setValue(3);
    ui->Slider_skewCheck->setEnabled(false);
    ui->label_skewCheck->setText(QStringLiteral("����"));
    ui->label_skewCheck->setEnabled(false);
    ui->cbtn_stapleCheck->setCheckState(Qt::Unchecked);//��ֽ�����鶤��⣺����ѡ    
	ui->cbox_pageDirection->setCurrentIndex(0);//��ֽ���ĸ巽�򣺲���ת    
	ui->cbtn_backRotate_180->setCheckState(Qt::Unchecked);//��ֽ��������ת180�㣺����ѡ
    ui->cbtn_imgSplit->setCheckState(Qt::Unchecked);//������ͼ���֣�����ѡ    
	ui->cbtn_jpgPercent->setCheckState(Qt::Unchecked);//������JPGѹ���ȣ�����ѡ    
	ui->Slider_jpgPercent->setValue(10);//������JPGѹ���ȣ�100%
    ui->label_jpgPercentPara->setText("100%");
    ui->label_jpgPercent->setEnabled(false);
    ui->Slider_jpgPercent->setEnabled(false);
    ui->label_jpgPercentPara->setEnabled(false);
    ui->gbox_ocr->setChecked(false);//������˫��pdf���ܣ�����ѡ
	ui->cbtn_preScan->setChecked(false);//������Ԥ��ɨ�裺����ѡ
}
void scanSettingsDialog::set_funVisible()
{
    ui->cbtn_deMoire->setVisible(false);
    if(DEV_G300)
    {
        ui->cbtn_stapleCheck->setVisible(false);
        ui->cbtn_skewCheck->setVisible(false);
        ui->Slider_skewCheck->setVisible(false);
        ui->label_skewCheck->setVisible(false);
        QVariant v(6);
        ui->cbox_paperSize->setItemData(1,v,Qt::UserRole - 1);
        ui->cbox_paperSize->setItemData(3,v,Qt::UserRole - 1);
        ui->cbox_paperSize->setItemData(8,v,Qt::UserRole - 1);
        ui->cbox_paperSize->setItemData(10,v,Qt::UserRole - 1);
        ui->cbox_paperSize->setItemData(14,v,Qt::UserRole - 1);
        ui->cbox_paperSize->setItemData(15,v,Qt::UserRole - 1);
        ui->cbox_paperSize->setItemData(16,v,Qt::UserRole - 1);
        ui->cbox_paperSize->setItemData(17,v,Qt::UserRole - 1);
    }
}
//bool scanSettingsDialog::set_settingsPara(ScanParam *param,LocalParams *lparam)
//{
//    param->ColorMode = ui->cbox_colorMode->currentIndex();
//    param->ImageProcessParam.PixType = ui->cbox_colorMode->currentIndex();
//    param->ImageProcessParam.multiOutput.canMultiOutput = ui->cbtn_multiStream->isChecked();
//    param->ImageProcessParam.multiOutput.MutltiOutputType = ui->cbox_multiStream->currentIndex();
//    param->paperSize = ui->cbox_paperSize->currentIndex();
//    param->ImageProcessParam.paperSize = ui->cbox_paperSize->currentIndex();
//    param->ImageProcessParam.cropRect.isCustomDefine = ui->cbtn_sizeAreaSwitch->isChecked();
//    param->ImageProcessParam.cropRect.x = sizeAreaRect.x();
//    param->ImageProcessParam.cropRect.y = sizeAreaRect.y();
//    param->ImageProcessParam.cropRect.width = sizeAreaRect.width();
//    param->ImageProcessParam.cropRect.height = sizeAreaRect.height();
//    switch(ui->cbox_resolution->currentIndex())
//    {
//    case 0:
//        param->ImageProcessParam.DestResulution = 100.0f;
//        break;
//    case 1:
//        param->ImageProcessParam.DestResulution = 150.0f;
//        break;
//    case 2:
//        param->ImageProcessParam.DestResulution = 200.0f;
//        break;
//    case 3:
//        param->ImageProcessParam.DestResulution = 240.0f;
//        break;
//    case 4:
//        param->ImageProcessParam.DestResulution = 300.0f;
//        break;
//    default:
//        param->ImageProcessParam.DestResulution = 200.0f;
//    }
//    param->IsDuplex = !(ui->cbox_scanPage->currentIndex() == 0);
//    param->ImageProcessParam.AutoDiscardBlank = (ui->cbox_scanPage->currentIndex() == 2);
//    param->ImageProcessParam.AutoDiscardBlankVince = (ui->cbox_scanPage->currentIndex() == 3);
//    param->ImageProcessParam.IsFold = (ui->cbox_scanPage->currentIndex() == 4);
//    param->ImageProcessParam.Brightness = ui->spin_brightness->value();
//    param->ImageProcessParam.Contrast = ui->spin_contrast->value();
//    param->ImageProcessParam.Gamma = ui->dSpin_gamma->value();
//    param->ImageProcessParam.customGamma.isDefined = ui->cbtn_colorLineSwitch->isChecked();
//    if(param->ColorMode == 0)
//    {
//        param->ImageProcessParam.customGamma.tableLength = 768;
//        param->ImageProcessParam.customGamma.table = dynamic_cast<MainWindow*>(parent())->RGBLineTable;
//    }
//    else
//    {
//        param->ImageProcessParam.customGamma.tableLength = 256;
//        param->ImageProcessParam.customGamma.table = dynamic_cast<MainWindow*>(parent())->greyLineTable;
//    }
//    if(param->ColorMode == 0)
//    {
//        lparam->colorLine.RGBlist.clear();
//        lparam->colorLine.Rlist.clear();
//        lparam->colorLine.Glist.clear();
//        lparam->colorLine.Blist.clear();
//        for(int i=0;i<rgbKeyPoint.size();i++)
//        {
//            for(int j=0;j<rgbKeyPoint[i].size();j++)
//            {
//                QVariant v(rgbKeyPoint[i][j]);
//                switch(i)
//                {
//                case 0:
//                    lparam->colorLine.RGBlist.append(v);
//                    break;
//                case 1:
//                    lparam->colorLine.Rlist.append(v);
//                    break;
//                case 2:
//                    lparam->colorLine.Glist.append(v);
//                    break;
//                case 3:
//                    lparam->colorLine.Blist.append(v);
//                    break;
//                default:
//                    break;
//                }
//            }
//        }
//    }
//    else
//    {
//        lparam->greyLine.greylist.clear();
//        for(int i=0;i<grayKeyPoint.size();i++)
//        {
//            QVariant v(grayKeyPoint[i]);
//            lparam->greyLine.greylist.append(v);
//        }
//    }
//    param->ImageProcessParam.FillBlackRect = ui->cbtn_deBlack->isChecked();
//    param->ImageProcessParam.AutoDescrew = ui->cbtn_adjust->isChecked();
//    param->ImageProcessParam.RefuseInflow = ui->cbtn_antiInfi->isChecked();
//    param->ImageProcessParam.OutHoleParam.OutHole = ui->cbtn_deHole->isChecked();
//    param->ImageProcessParam.OutHoleParam.OutHoleValue = ui->Slider_deHole->value();
//    param->ImageProcessParam.Filter = ui->cbox_deColor->currentIndex();
//    param->ImageProcessParam.ColorCorrection = ui->cbtn_colorAdjust->isChecked();
//    param->ImageProcessParam.RemoveMorr = ui->cbtn_deMoire->isChecked();
//    param->ImageProcessParam.ErrorExtention = ui->cbtn_setBWDot->isChecked();
//    param->ImageProcessParam.NosieDetach = ui->cbtn_noise->isChecked();
//    param->ImageProcessParam.TextureRemove = ui->cbtn_texRmv->isChecked();
//    param->ImageProcessParam.imageSharpen.canSharpen = ui->gbox_sharAndBlur->isChecked();
//    if(ui->rbtn_sharpening1->isChecked()) param->ImageProcessParam.imageSharpen.sharpenType = SHARPEN;
//    else if(ui->rbtn_sharpening2->isChecked()) param->ImageProcessParam.imageSharpen.sharpenType = SHARPENMORE;
//    else if(ui->rbtn_blur1->isChecked()) param->ImageProcessParam.imageSharpen.sharpenType = BLUR;
//    else param->ImageProcessParam.imageSharpen.sharpenType = BLURMORE;
//    param->ImageProcessParam.MultiOutRed = ui->cbtn_specialMulti->isChecked();
//    param->ImageProcessParam.AnswerSheetFilter = ui->cbtn_deRed_asheet->isChecked();
//    if(ui->rbtn_maxPage->isChecked())
//    {
//        param->ScanCount = 999;
//    }
//    else
//    {
//        param->ScanCount = ui->spin_pageNum->value();
//    }
//    param->ImageProcessParam.Orentation = ui->cbox_pageDirection->currentIndex();
//    param->ImageProcessParam.BackRotate180 = ui->cbtn_backRotate_180->isChecked();
//    param->UltrasonicDetect = ui->cbtn_doubleCheck->isChecked();
//    param->BindingDetect = ui->cbtn_stapleCheck->isChecked();
//    param->ScrewDetect = ui->cbtn_skewCheck->isChecked();
//    param->ScrewTopLevel = ui->Slider_skewCheck->value();
//    param->ImageProcessParam.SplitImage = ui->cbtn_imgSplit->isChecked();
//    lparam->jpgQuality.isSetQuality = ui->cbtn_jpgPercent->isChecked();
//    lparam->jpgQuality.Quality = ui->Slider_jpgPercent->value()*10;
//    lparam->isOcrPDF = ui->gbox_ocr->isChecked();
//    if(ui->rbtn_ocrPDF->isChecked()) lparam->ocrType = 1;
//    else if(ui->rbtn_ocrTxt->isChecked()) lparam->ocrType = 2;
//    else lparam->ocrType = 3;
//    lparam->isPreScan = ui->cbtn_preScan->isChecked();
//    return true;
//}
//void scanSettingsDialog::get_settingsPara(ScanParam *param,LocalParams *lparam)
//{
//    ui->cbox_colorMode->setCurrentIndex(param->ColorMode);
//    ui->cbtn_multiStream->setChecked(param->ImageProcessParam.multiOutput.canMultiOutput);
//    ui->cbox_multiStream->setCurrentIndex(param->ImageProcessParam.multiOutput.MutltiOutputType);
//    ui->cbox_paperSize->setCurrentIndex(param->paperSize);
//    ui->cbtn_sizeAreaSwitch->setChecked(param->ImageProcessParam.cropRect.isCustomDefine);
//    sizeAreaRect.setX(param->ImageProcessParam.cropRect.x);
//    sizeAreaRect.setY(param->ImageProcessParam.cropRect.y);
//    sizeAreaRect.setWidth(param->ImageProcessParam.cropRect.width);
//    sizeAreaRect.setHeight(param->ImageProcessParam.cropRect.height);
//    QList<double> list;
//    list<<100.0f<<150.0f<<200.0f<<240.0f<<300.0f;
//    for(int i=0;i<list.size();i++)
//    {
//        if(list.at(i) == param->ImageProcessParam.DestResulution)
//        {
//            ui->cbox_resolution->setCurrentIndex(i);
//            break;
//        }
//    }
//    if(!param->IsDuplex) ui->cbox_scanPage->setCurrentIndex(0);
//    else if(param->ImageProcessParam.AutoDiscardBlank) ui->cbox_scanPage->setCurrentIndex(2);
//    else if(param->ImageProcessParam.AutoDiscardBlankVince) ui->cbox_scanPage->setCurrentIndex(3);
//    else if(param->ImageProcessParam.IsFold) ui->cbox_scanPage->setCurrentIndex(4);
//    else ui->cbox_scanPage->setCurrentIndex(1);
//    ui->spin_brightness->setValue(param->ImageProcessParam.Brightness);
//    ui->spin_contrast->setValue(param->ImageProcessParam.Contrast);
//    ui->dSpin_gamma->setValue(param->ImageProcessParam.Gamma);
//    ui->cbtn_colorLineSwitch->setChecked(param->ImageProcessParam.customGamma.isDefined);
//    rgbKeyPoint.clear();
//    for(int i=0;i<4;i++)
//    {
//        QList<QPoint> list;
//        switch(i)
//        {
//        case 0:
//            for(int j=0;j<lparam->colorLine.RGBlist.size();j++)
//                list.append(lparam->colorLine.RGBlist[j].toPoint());
//            break;
//        case 1:
//            for(int j=0;j<lparam->colorLine.Rlist.size();j++)
//                list.append(lparam->colorLine.Rlist[j].toPoint());
//            break;
//        case 2:
//            for(int j=0;j<lparam->colorLine.Glist.size();j++)
//                list.append(lparam->colorLine.Glist[j].toPoint());
//            break;
//        case 3:
//            for(int j=0;j<lparam->colorLine.Blist.size();j++)
//                list.append(lparam->colorLine.Blist[j].toPoint());
//            break;
//        default:
//            break;
//        }
//        rgbKeyPoint.append(list);
//    }
//    grayKeyPoint.clear();
//    for(int i=0;i<lparam->greyLine.greylist.size();i++)
//        grayKeyPoint.append(lparam->greyLine.greylist[i].toPoint());
//    ui->cbtn_deBlack->setChecked(param->ImageProcessParam.FillBlackRect);
//    ui->cbtn_adjust->setChecked(param->ImageProcessParam.AutoDescrew);
//    ui->cbtn_antiInfi->setChecked(param->ImageProcessParam.RefuseInflow);
//    ui->cbtn_deHole->setChecked(param->ImageProcessParam.OutHoleParam.OutHole);
//    ui->Slider_deHole->setValue(param->ImageProcessParam.OutHoleParam.OutHoleValue);
//    ui->cbox_deColor->setCurrentIndex(param->ImageProcessParam.Filter);
//    ui->cbtn_colorAdjust->setChecked(param->ImageProcessParam.ColorCorrection);
//    ui->cbtn_deMoire->setChecked(param->ImageProcessParam.RemoveMorr);
//    ui->cbtn_setBWDot->setChecked(param->ImageProcessParam.ErrorExtention);
//    ui->cbtn_noise->setChecked(param->ImageProcessParam.NosieDetach);
//    ui->cbtn_texRmv->setChecked(param->ImageProcessParam.TextureRemove);
//    ui->gbox_sharAndBlur->setChecked(param->ImageProcessParam.imageSharpen.canSharpen);
//    switch(param->ImageProcessParam.imageSharpen.sharpenType)
//    {
//    case SHARPEN:
//        ui->rbtn_sharpening1->setChecked(true);
//        break;
//    case SHARPENMORE:
//        ui->rbtn_sharpening2->setChecked(true);
//        break;
//    case BLUR:
//        ui->rbtn_blur1->setChecked(true);
//        break;
//    case BLURMORE:
//        ui->rbtn_blur2->setChecked(true);
//        break;
//    default:
//        ui->rbtn_sharpening1->setChecked(true);
//        break;
//    }
//    ui->cbtn_specialMulti->setChecked(param->ImageProcessParam.MultiOutRed);
//    ui->cbtn_deRed_asheet->setChecked(param->ImageProcessParam.AnswerSheetFilter);
//    if(param->ScanCount == 999) ui->rbtn_maxPage->setChecked(true);
//    else
//    {
//        ui->rbtn_pageNum->setChecked(true);
//        ui->spin_pageNum->setValue(param->ScanCount);
//    }
//    ui->cbox_pageDirection->setCurrentIndex(param->ImageProcessParam.Orentation);
//    ui->cbtn_backRotate_180->setChecked(param->ImageProcessParam.BackRotate180);
//    ui->cbtn_doubleCheck->setChecked(param->UltrasonicDetect);
//    ui->cbtn_stapleCheck->setChecked(param->BindingDetect);
//    ui->cbtn_skewCheck->setChecked(param->ScrewDetect);
//    ui->Slider_skewCheck->setValue(param->ScrewTopLevel);
//    ui->cbtn_imgSplit->setChecked(param->ImageProcessParam.SplitImage);
//    ui->cbtn_jpgPercent->setChecked(lparam->jpgQuality.isSetQuality);
//    ui->Slider_jpgPercent->setValue(lparam->jpgQuality.Quality/10);
//    ui->gbox_ocr->setChecked(lparam->isOcrPDF);
//    switch(lparam->ocrType)
//    {
//    case 1:
//        ui->rbtn_ocrPDF->setChecked(true);
//        break;
//    case 2:
//        ui->rbtn_ocrTxt->setChecked(true);
//        break;
//    case 3:
//        ui->rbtn_ocrPDF_txt->setChecked(true);
//        break;
//    default:
//        ui->rbtn_ocrPDF_txt->setChecked(true);
//        break;
//    }
//    ui->cbtn_preScan->setChecked(lparam->isPreScan);
//}

void scanSettingsDialog::showColorLineSetDialog()
{
    setPicClrTool d(this);
    d.setColorMode(ui->cbox_colorMode->currentIndex());
    if(ui->cbox_colorMode->currentIndex()==0)
    {
        if(rgbKeyPoint.size() > 0)
            d.setRGBKeyTable(rgbKeyPoint);
    }
    else{
        if(grayKeyPoint.size() > 0)
            d.setGrayKeyTable(grayKeyPoint);
    }

    d.exec();
    if(ui->cbox_colorMode->currentIndex()==0)
    {
        d.getRGBTable(dynamic_cast<MainWindow*>(parent())->RGBLineTable);
        rgbKeyPoint = d.getRGBKeyTable();
    }
    else
    {
        d.getGrayTable(dynamic_cast<MainWindow*>(parent())->greyLineTable);
        grayKeyPoint = d.getGrayKeyTable();
    }
}

void scanSettingsDialog::showCutDialog()
{
    QList<int> list;
    list<<100<<150<<200<<240<<300;
    int dpi = list[ui->cbox_resolution->currentIndex()];
    QString size = ui->cbox_paperSize->currentText();
    int dialog_width = 300;
    CutPaperTool d(this);
    d.setPaperType(dpi,size,dialog_width);
    if(isFirstRun)
    {
        d.setCutRect(sizeAreaRect);
        m_paperSizeChanged = false;
        isFirstRun = false;
    }
    else if(m_paperSizeChanged)
    {
        d.setSizeInit();
        m_paperSizeChanged = false;
    }
    else
        d.setCutRect(sizeAreaRect);
    d.exec();
    qDebug() << sizeAreaRect;
    sizeAreaRect = d.getCutRectPixel();
}

//exclution logic slots(11)
void scanSettingsDialog::on_cbox_colorMode_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:
        ui->label_deColor->setEnabled(false);
        ui->cbox_deColor->setEnabled(false);
        ui->cbox_deColor->setCurrentIndex(0);
        ui->cbtn_colorAdjust->setEnabled(true);
        ui->cbtn_deMoire->setEnabled(true);
        ui->cbtn_texRmv->setEnabled(true);
        ui->cbtn_setBWDot->setEnabled(false);
        ui->cbtn_setBWDot->setChecked(false);
        ui->cbtn_noise->setEnabled(false);
        ui->cbtn_noise->setChecked(false);
        ui->gbox_sharAndBlur->setEnabled(true);
        ui->gbox_specialImgProc->setEnabled(true);
        break;
    case 1:
        ui->label_deColor->setEnabled(true);
        ui->cbox_deColor->setEnabled(true);
        ui->cbtn_setBWDot->setEnabled(false);
        ui->cbtn_colorAdjust->setEnabled(true);
        ui->cbtn_noise->setEnabled(false);
        ui->cbtn_deMoire->setEnabled(true);
        ui->cbtn_texRmv->setEnabled(true);
        ui->gbox_sharAndBlur->setEnabled(true);
        ui->gbox_specialImgProc->setEnabled(false);
        ui->cbtn_specialMulti->setChecked(false);
        ui->cbtn_deRed_asheet->setChecked(false);
        break;
    case 2:
        ui->label_deColor->setEnabled(true);
        ui->cbox_deColor->setEnabled(true);
        ui->cbtn_colorAdjust->setEnabled(false);
        ui->cbtn_colorAdjust->setChecked(false);
        ui->cbtn_deMoire->setEnabled(false);
        ui->cbtn_deMoire->setChecked(false);
        ui->cbtn_texRmv->setEnabled(false);
        ui->cbtn_texRmv->setChecked(false);
        ui->cbtn_setBWDot->setEnabled(true);
        ui->cbtn_noise->setEnabled(true);
        ui->gbox_sharAndBlur->setEnabled(false);
        ui->gbox_specialImgProc->setEnabled(false);
        ui->cbtn_specialMulti->setChecked(false);
        ui->cbtn_deRed_asheet->setChecked(false);
        break;
    default:
        ui->label_deColor->setEnabled(false);
        ui->cbox_deColor->setEnabled(false);
        ui->cbox_deColor->setCurrentIndex(0);
        ui->cbtn_colorAdjust->setEnabled(true);
        ui->cbtn_deMoire->setEnabled(true);
        ui->cbtn_texRmv->setEnabled(true);
        ui->cbtn_setBWDot->setEnabled(false);
        ui->cbtn_setBWDot->setChecked(false);
        ui->cbtn_noise->setEnabled(false);
        ui->cbtn_noise->setChecked(false);
        ui->gbox_sharAndBlur->setEnabled(true);
        ui->gbox_specialImgProc->setEnabled(true);
        break;
    }
}
void scanSettingsDialog::on_cbtn_multiStream_stateChanged(int arg1)
{
    switch (arg1)
    {
    case 0:
        ui->cbox_multiStream->setEnabled(false);
        ui->label_colorMode->setEnabled(true);
        ui->cbox_colorMode->setEnabled(true);
        ui->gbox_filter->setEnabled(true);
        ui->cbtn_colorLineSwitch->setEnabled(true);
        switch (ui->cbox_colorMode->currentIndex())
        {
        case 0:
            ui->cbtn_colorAdjust->setEnabled(true);
            ui->cbtn_deMoire->setEnabled(true);
            ui->cbtn_texRmv->setEnabled(true);
            ui->gbox_sharAndBlur->setEnabled(true);
            ui->gbox_specialImgProc->setEnabled(true);
            break;
        case 1:
            ui->label_deColor->setEnabled(true);
            ui->cbox_deColor->setEnabled(true);
            ui->cbtn_deMoire->setEnabled(true);
            ui->cbtn_texRmv->setEnabled(true);
            ui->gbox_sharAndBlur->setEnabled(true);
            break;
        case 2:
            ui->label_deColor->setEnabled(true);
            ui->cbox_deColor->setEnabled(true);
            ui->cbtn_setBWDot->setEnabled(true);
            ui->cbtn_noise->setEnabled(true);
            break;
        default:
            break;
        }
        break;
    case 1:
    case 2:
        ui->cbox_multiStream->setEnabled(true);
        ui->label_colorMode->setEnabled(false);
        ui->cbox_colorMode->setEnabled(false);
        ui->gbox_filter->setEnabled(false);
        ui->cbox_deColor->setCurrentIndex(0);
        ui->cbtn_colorAdjust->setChecked(false);
        ui->cbtn_texRmv->setChecked(false);
        ui->cbtn_deMoire->setChecked(false);
        ui->cbtn_setBWDot->setChecked(false);
        ui->cbtn_noise->setChecked(false);
        ui->gbox_sharAndBlur->setChecked(false);
        ui->gbox_specialImgProc->setEnabled(false);
        ui->cbtn_specialMulti->setChecked(false);
        ui->cbtn_deRed_asheet->setChecked(false);
        ui->cbtn_colorLineSwitch->setEnabled(false);
        ui->cbtn_colorLineSwitch->setChecked(false);
        ui->tbtn_colorCurve->setEnabled(false);
        if(ui->cbox_multiStream->currentIndex() == 3)
            ui->cbox_colorMode->setCurrentIndex(1);
        else
            ui->cbox_colorMode->setCurrentIndex(0);
        break;
    default:
        break;
    }
}
void scanSettingsDialog::on_cbox_multiStream_currentIndexChanged(int index)
{
    switch(index)
    {
    case 0:
    case 1:
    case 2:
        ui->cbox_colorMode->setCurrentIndex(0);
        break;
    case 3:
        ui->cbox_colorMode->setCurrentIndex(1);
        break;
    default:
        break;
    }
}
void scanSettingsDialog::on_cbox_paperSize_currentIndexChanged(int index)
{
    if(isUsrSettingChange)
    {
        isUsrSettingChange = false;
        return;
    }
    if(m_paperSize == index)
        return;
    if(ui->cbtn_sizeAreaSwitch->isChecked())
    {
        int m = QMessageBox::question(this,
                                      QStringLiteral("��Ҫ"),
                                      QStringLiteral("��ǰ�ĸ�ߴ��������Զ���ɨ�������������ĸ�ߴ磬��������ѡ���Զ���ɨ���������á�\n""�Ƿ�ȷ��Ҫ�����ĸ�ߴ磿"));
        if(m == QMessageBox::No)
        {
            ui->cbox_paperSize->setCurrentIndex(m_paperSize);
            return;
        }
    }
    if(index == 0)
    {
        ui->cbtn_sizeAreaSwitch->setChecked(false);
        ui->cbtn_sizeAreaSwitch->setEnabled(false);
        ui->tbtn_sizeArea->setEnabled(false);
    }
    else
    {
        ui->cbtn_sizeAreaSwitch->setEnabled(true);
    }
    m_paperSize = index;
    m_paperSizeChanged = true;
}
void scanSettingsDialog::on_cbtn_sizeAreaSwitch_stateChanged(int arg1)
{
    switch (arg1)
    {
    case 0:
        ui->tbtn_sizeArea->setEnabled(false);
        break;
    case 1:
    case 2:
        ui->tbtn_sizeArea->setEnabled(true);
        break;
    default:
        break;
    }
}
void scanSettingsDialog::on_cbox_scanPage_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:
        ui->cbtn_backRotate_180->setEnabled(false);
        ui->cbtn_adjust->setEnabled(true);
        break;
    case 1:
    case 2:
    case 3:
        ui->cbtn_backRotate_180->setEnabled(true);
        ui->cbtn_adjust->setEnabled(true);
        break;
    case 4:
        ui->cbtn_adjust->setChecked(true);
        ui->cbtn_adjust->setEnabled(false);
        ui->cbtn_backRotate_180->setEnabled(false);
        break;
    default:
        break;
    }
}
void scanSettingsDialog::on_cbtn_colorLineSwitch_stateChanged(int arg1)
{
    switch (arg1)
    {
    case 0:
        ui->tbtn_colorCurve->setEnabled(false);
        ui->Slider_brightness->setEnabled(true);
        ui->Slider_contrast->setEnabled(true);
        ui->Slider_gamma->setEnabled(true);
        ui->spin_brightness->setEnabled(true);
        ui->spin_contrast->setEnabled(true);
        ui->dSpin_gamma->setEnabled(true);
        break;
    case 1:
    case 2:
        ui->tbtn_colorCurve->setEnabled(true);
        ui->Slider_brightness->setEnabled(false);
        ui->Slider_contrast->setEnabled(false);
        ui->Slider_gamma->setEnabled(false);
        ui->spin_brightness->setEnabled(false);
        ui->spin_contrast->setEnabled(false);
        ui->dSpin_gamma->setEnabled(false);
        break;
    default:
        break;
    }
}
void scanSettingsDialog::on_cbtn_deHole_stateChanged(int arg1)
{
    switch (arg1)
    {
    case 0:
        ui->label_deHole->setEnabled(false);
        ui->Slider_deHole->setEnabled(false);
        ui->label_deHolePara->setEnabled(false);
        break;
    case 1:
    case 2:
        ui->label_deHole->setEnabled(true);
        ui->Slider_deHole->setEnabled(true);
        ui->label_deHolePara->setEnabled(true);
        break;
    default:
        break;
    }
}
void scanSettingsDialog::on_cbtn_sharAndBlur_stateChanged(int arg1)
{
    switch (arg1)
    {
    case 0:
        ui->rbtn_sharpening1->setEnabled(false);
        ui->rbtn_sharpening2->setEnabled(false);
        ui->rbtn_blur1->setEnabled(false);
        ui->rbtn_blur2->setEnabled(false);
        break;
    case 1:
    case 2:
        ui->rbtn_sharpening1->setEnabled(true);
        ui->rbtn_sharpening2->setEnabled(true);
        ui->rbtn_blur1->setEnabled(true);
        ui->rbtn_blur2->setEnabled(true);
        break;
    default:
        break;
    }
}
void scanSettingsDialog::on_rbtn_pageNum_toggled(bool checked)
{
    if(checked)
    {
        ui->spin_pageNum->setEnabled(true);
    }
    else
    {
        ui->spin_pageNum->setEnabled(false);
    }
}
void scanSettingsDialog::on_cbtn_skewCheck_stateChanged(int arg1)
{
    switch (arg1)
    {
    case 0:
        ui->label_skewCheck->setEnabled(false);
        ui->Slider_skewCheck->setEnabled(false);
        break;
    case 1:
    case 2:
        ui->label_skewCheck->setEnabled(true);
        ui->Slider_skewCheck->setEnabled(true);
        break;
    default:
        break;
    }
}
void scanSettingsDialog::on_cbox_pageDirection_currentIndexChanged(int index)
{
    if(index == 4)
    {
        ui->cbtn_backRotate_180->setCheckState(Qt::Unchecked);
        ui->cbtn_backRotate_180->setEnabled(false);
    }
    else if(ui->cbox_scanPage->currentIndex() != 0)
    {
        ui->cbtn_backRotate_180->setEnabled(true);
    }
}
void scanSettingsDialog::on_cbtn_jpgPercent_stateChanged(int arg1)
{
    switch (arg1)
    {
    case 0:
        ui->label_jpgPercent->setEnabled(false);
        ui->Slider_jpgPercent->setEnabled(false);
        ui->label_jpgPercentPara->setEnabled(false);
        break;
    case 1:
    case 2:
        ui->label_jpgPercent->setEnabled(true);
        ui->Slider_jpgPercent->setEnabled(true);
        ui->label_jpgPercentPara->setEnabled(true);
        break;
    default:
        break;
    }
}
void scanSettingsDialog::on_gbox_ocr_toggled(bool arg1)
{
    if(arg1)
    {
        ui->cbox_pageDirection->setEnabled(false);
        ui->cbox_pageDirection->setCurrentIndex(4);
        ui->cbtn_backRotate_180->setEnabled(false);
        ui->cbtn_backRotate_180->setChecked(false);
    }
    else
    {
        ui->cbox_pageDirection->setEnabled(true);
        ui->cbtn_backRotate_180->setEnabled(true);
    }
}

//slider logic slots(9)
void scanSettingsDialog::on_Slider_brightness_valueChanged(int value)
{
    ui->spin_brightness->setValue(value);
}
void scanSettingsDialog::on_spin_brightness_valueChanged(int arg1)
{
    ui->Slider_brightness->setValue(arg1);
}
void scanSettingsDialog::on_Slider_contrast_valueChanged(int value)
{
    ui->spin_contrast->setValue(value);
}
void scanSettingsDialog::on_spin_contrast_valueChanged(int arg1)
{
    ui->Slider_contrast->setValue(arg1);
}
void scanSettingsDialog::on_Slider_gamma_valueChanged(int value)
{
    ui->dSpin_gamma->setValue(double(value)/10);
}
void scanSettingsDialog::on_dSpin_gamma_valueChanged(double arg1)
{
    ui->Slider_gamma->setValue(int(arg1*10));
}
void scanSettingsDialog::on_Slider_deHole_valueChanged(int value)
{
    QString per = "";
    switch(value)
    {
    case 1:
        per = "10%";
        break;
    case 2:
        per = "15%";
        break;
    case 3:
        per = "20%";
        break;
    case 4:
        per = "25%";
        break;
    case 5:
        per = "30%";
        break;
    case 6:
        per = "35%";
        break;
    case 7:
        per = "40%";
        break;
    case 8:
        per = "45%";
        break;
    case 9:
        per = "50%";
        break;
    default:
        per = "10%";
    }
    ui->label_deHolePara->setText(per);
}
void scanSettingsDialog::on_Slider_skewCheck_valueChanged(int value)
{
    QString sk = "";
    switch(value)
    {
    case 1:
        sk = QStringLiteral("���׼�� ");
        break;
    case 2:
        sk = QStringLiteral("�׼�� ");
        break;
    case 3:
        sk = QStringLiteral("���� ");
        break;
    case 4:
        sk = QStringLiteral("�Ѽ�� ");
        break;
    case 5:
        sk = QStringLiteral("���Ѽ�� ");
        break;
    default:
        sk = QStringLiteral("���� ");
    }
    ui->label_skewCheck->setText(sk);
}
void scanSettingsDialog::on_Slider_jpgPercent_valueChanged(int value)
{
    QString per = "";
    switch(value)
    {
    case 1:
        per = "10%";
        break;
    case 2:
        per = "20%";
        break;
    case 3:
        per = "30%";
        break;
    case 4:
        per = "40%";
        break;
    case 5:
        per = "50%";
        break;
    case 6:
        per = "60%";
        break;
    case 7:
        per = "70%";
        break;
    case 8:
        per = "80%";
        break;
    case 9:
        per = "90%";
        break;
    default:
        per = "100%";
    }
    ui->label_jpgPercentPara->setText(per);
}


//global btn slots(4)
void scanSettingsDialog::on_pbtn_help_clicked()
{
    QString file = m_sysDirPath + "/HuaGoScan_scanSettings_Help_manual_V1_0.pdf";
    if(!QFileInfo(file).exists())
        QMessageBox::warning(this,QStringLiteral("���� "),QStringLiteral("�Ҳ��������ĵ����ĵ������ѱ�ɾ������������ "));
    QDesktopServices::openUrl(QUrl::fromLocalFile(file));
}
void scanSettingsDialog::on_pbtn_default_clicked()
{
    set_defaults();
}
void scanSettingsDialog::on_pbtn_ok_clicked()
{
//    set_settingsPara(dynamic_cast<MainWindow*>(parent())->settingsPara,dynamic_cast<MainWindow*>(parent())->localPara);
    close();
}
void scanSettingsDialog::on_pbtn_cancel_clicked()
{
    close();
}

//usr_settings slots(10)
#define MAX_SETTINGS_NUM 10
void scanSettingsDialog::on_pBtn_usrSetting_app_clicked()
{
    int a = QMessageBox::question(this,
                          QStringLiteral("ѯ�� "),
                          QStringLiteral("Ӧ����ѡ�е����ã�ɨ��������������ģ��Ƿ������ "),
                          QMessageBox::Yes|QMessageBox::No,
                          QMessageBox::Yes);
    if(a == QMessageBox::Yes)
    {
        QString name = ui->listWidget->currentItem()->text();
        QString file_path = m_iniDirPath+"/"+name+".ini";
        QFileInfo ini_file(file_path);
        if(!ini_file.exists())
            QMessageBox::warning(this,QStringLiteral("���� "),QStringLiteral("���������ļ�ʧ�ܡ������ļ��������𻵻�ɾ���� "));
        else
            load_ini(name);
    }
}
void scanSettingsDialog::on_pBtn_usrSetting_add_clicked()
{
    QInputDialog d(this);
    d.setWindowTitle(QStringLiteral("��������"));
    d.setLabelText(QStringLiteral("��������������"));
    if(d.exec())
    {
        QString name = d.textValue();
        //����ɨ�����Ϊini�����ļ����ļ����������ļ�����ͬ
        if(!add_ini(name, m_iniDirPath))
        {
            QMessageBox::warning(this,QStringLiteral("���� "),QStringLiteral("���������ļ�ʧ�ܡ��������ʣ��ռ��ǰ�û���Ȩ�ޡ� "));
            return;
        }
        //�����б���
        add_usrSetting(name);
        if(ui->listWidget->count() >= MAX_SETTINGS_NUM)
        {
            ui->pBtn_usrSetting_add->setEnabled(false);
            ui->pBtn_usrSetting_inp->setEnabled(false);
        }
    }
}
void scanSettingsDialog::on_pBtn_usrSetting_del_clicked()
{
    int a = QMessageBox::question(this,
                          QStringLiteral("ѯ�� "),
                          QStringLiteral("������ɾ����ѡ�е����ã��Ƿ������ "),
                          QMessageBox::Yes|QMessageBox::No,
                          QMessageBox::Yes);
    if(a == QMessageBox::Yes)
    {
        QString name = ui->listWidget->currentItem()->text();
        delete_ini(name);
        ui->listWidget->takeItem(ui->listWidget->currentRow());
        ui->pBtn_usrSetting_add->setEnabled(true);
        ui->pBtn_usrSetting_inp->setEnabled(true);
    }
}
void scanSettingsDialog::on_pBtn_usrSetting_clr_clicked()
{
    int a = QMessageBox::question(this,
                          QStringLiteral("ѯ�� "),
                          QStringLiteral("������ɾ�������ѱ�������ã��Ƿ������ "),
                          QMessageBox::Yes|QMessageBox::No,
                          QMessageBox::Yes);
    if(a == QMessageBox::Yes)
    {
        for(int i = 0; i < ui->listWidget->count(); i++)
        {
            QString name = ui->listWidget->item(i)->text();
            delete_ini(name);
        }
        ui->listWidget->clear();
        ui->pBtn_usrSetting_add->setEnabled(true);
        ui->pBtn_usrSetting_inp->setEnabled(true);
    }
}
void scanSettingsDialog::on_pBtn_usrSetting_inp_clicked()
{
//    QString file_path = QFileDialog::getOpenFileName(nullptr,
//                                                     QStringLiteral("���������ļ� "),
//                                                     "",
//                                                     "(*.ini)");
//    if(file_path.isEmpty()) return;
//    QFileInfo file(file_path);
//    QString name = file.completeBaseName();
    QString name = showIniInputDialog();
    if(name.isEmpty()) return;
    add_usrSetting(name);
    if(ui->listWidget->count() >= MAX_SETTINGS_NUM)
    {
        ui->pBtn_usrSetting_add->setEnabled(false);
        ui->pBtn_usrSetting_inp->setEnabled(false);
    }
}
void scanSettingsDialog::on_pBtn_usrSetting_exp_clicked()
{
    QString filename = QFileDialog::getSaveFileName(nullptr,
                                                    QStringLiteral("���������ļ� "),
                                                    "",
                                                    "(*.ini)");
    if(filename.isEmpty()) return;
    if (!filename.endsWith(".ini"))
    {
        filename += ".ini";
    }
    //����ini�ļ�
    QSettings *new_ini = new QSettings(filename,QSettings::IniFormat);
    new_ini->setValue("base_colorModeIndex",ui->cbox_colorMode->currentIndex());//д��һ������������ini�ļ�������
    delete new_ini;
    QFileInfo file(filename);
    if(!file.exists())
    {
        QMessageBox::warning(this,QStringLiteral("���� "),QStringLiteral("���������ļ�ʧ�ܡ��������ʣ��ռ��ǰ�û���Ȩ�ޡ� "));
        return;
    }
    //���������ļ�
    QString name = file.completeBaseName();
    QString dir_path = file.absolutePath();
    add_ini(name, dir_path);
}
void scanSettingsDialog::on_pBtn_usrSetting_save_clicked()
{
    int a = QMessageBox::question(this,
                          QStringLiteral("ѯ�� "),
                          QStringLiteral("�����ǵ�ǰ��ѡ�е����ã��Ƿ������ "),
                          QMessageBox::Yes|QMessageBox::No,
                          QMessageBox::Yes);
    if(a == QMessageBox::Yes)
    {
        QString name = ui->listWidget->currentItem()->text();
        add_ini(name, m_iniDirPath);
    }
}
void scanSettingsDialog::on_listWidget_itemSelectionChanged()
{
    if(ui->listWidget->selectedItems().isEmpty())
    {
        ui->pBtn_usrSetting_del->setEnabled(false);
        ui->pBtn_usrSetting_exp->setEnabled(false);
    }
    else
    {
        ui->pBtn_usrSetting_del->setEnabled(true);
        ui->pBtn_usrSetting_exp->setEnabled(true);
    }
}
void scanSettingsDialog::on_listWidget_currentRowChanged(int currentRow)
{
    if(currentRow == -1)
    {
        ui->pBtn_usrSetting_clr->setEnabled(false);
        ui->pBtn_usrSetting_app->setEnabled(false);
        ui->pBtn_usrSetting_save->setEnabled(false);
    }
    else
    {
        ui->pBtn_usrSetting_clr->setEnabled(true);
        ui->pBtn_usrSetting_app->setEnabled(true);
        ui->pBtn_usrSetting_save->setEnabled(true);
    }
}
void scanSettingsDialog::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    ui->listWidget->setCurrentItem(item);
    on_pBtn_usrSetting_app_clicked();
}

//usr_settings functions(3)
void scanSettingsDialog::add_usrSetting(QString name)
{
    QList<QListWidgetItem*> list(ui->listWidget->findItems(name,Qt::MatchExactly));
    if(list.isEmpty())
    {
        int index = ui->listWidget->currentRow();
        if(index == ui->listWidget->count()-1)
            ui->listWidget->addItem(name);
        else
            ui->listWidget->insertItem(index+1,name);
        ui->listWidget->setCurrentRow(index+1);
    }
    else
    {
        int a = QMessageBox::question(this,
                              QStringLiteral("ѯ�� "),
                              QStringLiteral("������ͬ�������ã��Ƿ񸲸�ͬ�����ã� "),
                              QMessageBox::Yes|QMessageBox::No,
                              QMessageBox::Yes);
        if(a == QMessageBox::Yes)
        {
            QListWidgetItem* item = new QListWidgetItem;
            item = list.at(0);
            int index = ui->listWidget->row(item);
            ui->listWidget->takeItem(index);
            ui->listWidget->insertItem(index,name);
            ui->listWidget->setCurrentRow(index);
            delete item;
        }
    }
}
QString scanSettingsDialog::showIniInputDialog()
{
    QDialog* d = new QDialog(this);
    d->setWindowTitle(QStringLiteral("�ɵ��������б� "));
    QVBoxLayout* v = new QVBoxLayout(d);
    //���������б�
    QListWidget* listWidget = new QListWidget;
    listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    QStringList nameList = check_ini(m_iniDirPath);
    foreach(QString name,nameList)
    {
        listWidget->addItem(name);
    }
    v->addWidget(listWidget);
    //�������ڰ�ť
    QPushButton *inputBtn = new QPushButton(QStringLiteral("���� "),d);
    QPushButton *cancelBtn = new QPushButton(QStringLiteral("ȡ�� "),d);
    QHBoxLayout* h = new QHBoxLayout(d);
    h->addWidget(inputBtn);
    h->addWidget(cancelBtn);
    v->addLayout(h);
    connect(inputBtn,SIGNAL(clicked(bool)),d,SLOT(accept()));
    connect(cancelBtn,SIGNAL(clicked(bool)),d,SLOT(reject()));
    //�����߼�
    QString retName;
    if(d->exec())
    {
        retName = listWidget->currentItem()->text();
    }
    delete listWidget;
    delete inputBtn;
    delete cancelBtn;
    delete h;
    delete v;
    delete d;
    return retName;
}
QStringList scanSettingsDialog::check_ini(QString dirPath)
{
    //��ȡsettings�ļ����µ�ini�ļ��б�
    QDir dir(dirPath);
    QStringList filter;
    filter<<"*.ini";
    QFileInfoList ini_list;
    ini_list = dir.entryInfoList(filter, QDir::Files);
    //��ȡ��ǰ��ѡ���б��Ѽ��ص������б�
    QStringList name_list;
    for(int i = 0; i < ui->listWidget->count(); i++)
    {
        name_list.append(ui->listWidget->item(i)->text());
    }
    //�޳��Ѽ�������
    foreach(QFileInfo iniName,ini_list)
    {
        foreach(QString name,name_list)
        {
            if(name == iniName.baseName())
                ini_list.removeOne(iniName);
        }
    }
    //���ɷ����б�
    QStringList reList;
    foreach(QFileInfo iniName,ini_list)
    {
        reList.append(iniName.baseName());
    }
    return reList;
}

//ini functions(5)
bool scanSettingsDialog::add_ini(QString name, QString dirPath)
{
    QSettings *new_ini = new QSettings(dirPath+"/"+name+".ini",QSettings::IniFormat);
    bool isAddSuccess = false;
    if(new_ini->isWritable())
    {
        new_ini->setValue("base_colorModeIndex",ui->cbox_colorMode->currentIndex());
        new_ini->setValue("base_isMultiOutput",ui->cbtn_multiStream->isChecked());
        new_ini->setValue("base_multiOutType",ui->cbox_multiStream->currentIndex());
        new_ini->setValue("base_paperSizeIndex",ui->cbox_paperSize->currentIndex());
        new_ini->setValue("base_isCustomArea",ui->cbtn_sizeAreaSwitch->isChecked());
        new_ini->setValue("base_customAreaX",sizeAreaRect.x());
        new_ini->setValue("base_customAreaY",sizeAreaRect.y());
        new_ini->setValue("base_customAreaW",sizeAreaRect.width());
        new_ini->setValue("base_customAreaH",sizeAreaRect.height());
        new_ini->setValue("base_resolution",ui->cbox_resolution->currentIndex());
        new_ini->setValue("base_scanPage",ui->cbox_scanPage->currentIndex());
        new_ini->setValue("BCG_brightness",ui->spin_brightness->value());
        new_ini->setValue("BCG_contrast",ui->spin_contrast->value());
        new_ini->setValue("BCG_gamma",ui->dSpin_gamma->value());
        new_ini->setValue("BCG_isCustomGama",ui->cbtn_colorLineSwitch->isChecked());
        QList<QVariant> RGBline,Rline,Gline,Bline,GRAYline;
        for(int i=0;i<rgbKeyPoint.size();i++)
        {
            for(int j=0;j<rgbKeyPoint[i].size();j++)
            {
                QVariant v(rgbKeyPoint[i][j]);
                switch(i)
                {
                case 0:
                    RGBline.append(v);
                    break;
                case 1:
                    Rline.append(v);
                    break;
                case 2:
                    Gline.append(v);
                    break;
                case 3:
                    Bline.append(v);
                    break;
                default:
                    break;
                }
            }
        }
        for(int i=0;i<grayKeyPoint.size();i++)
        {
            QVariant v(grayKeyPoint[i]);
            GRAYline.append(v);
        }
        new_ini->setValue("BCG_RGBline",RGBline);
        new_ini->setValue("BCG_Rline",Rline);
        new_ini->setValue("BCG_Gline",Gline);
        new_ini->setValue("BCG_Bline",Bline);
        new_ini->setValue("BCG_greyline",GRAYline);
        new_ini->setValue("imgProc_isFillBlack",ui->cbtn_deBlack->isChecked());
        new_ini->setValue("imgProc_isDescrew",ui->cbtn_adjust->isChecked());
        new_ini->setValue("imgProc_isAntiInfiltration",ui->cbtn_antiInfi->isChecked());
        new_ini->setValue("imgProc_isOutHole",ui->cbtn_deHole->isChecked());
        new_ini->setValue("imgProc_outHoleValue",ui->Slider_deHole->value());
        new_ini->setValue("imgProc_filterIndex",ui->cbox_deColor->currentIndex());
        new_ini->setValue("imgProc_isColorCorrection",ui->cbtn_colorAdjust->isChecked());
        new_ini->setValue("imgProc_isRemoveMoire",ui->cbtn_deMoire->isChecked());
        new_ini->setValue("imgProc_isErrorExtention",ui->cbtn_setBWDot->isChecked());
        new_ini->setValue("imgProc_isNoiseOptimization",ui->cbtn_noise->isChecked());
        new_ini->setValue("imgProc_isTextureRemove",ui->cbtn_texRmv->isChecked());
        new_ini->setValue("imgProc_isSharOrBlur",ui->gbox_sharAndBlur->isChecked());
        int sharpenType = 0;
        if(ui->rbtn_sharpening1->isChecked())
            sharpenType = 1;
        else if(ui->rbtn_sharpening2->isChecked())
            sharpenType = 2;
        else if(ui->rbtn_blur1->isChecked())
            sharpenType = 3;
        else if(ui->rbtn_blur2->isChecked())
            sharpenType = 4;
        else
            sharpenType = 0;
        new_ini->setValue("imgProc_sharpenType",sharpenType);
        new_ini->setValue("imgProc_isMultiOutRed",ui->cbtn_specialMulti->isChecked());
        new_ini->setValue("imgProc_isAnswerSheetFilter",ui->cbtn_deRed_asheet->isChecked());
        int ScanCount = 999;
        if(ui->rbtn_pageNum->isChecked())
            ScanCount = ui->spin_pageNum->value();
        new_ini->setValue("feeding_scanCount",ScanCount);
        new_ini->setValue("feeding_pageOrientationIndex",ui->cbox_pageDirection->currentIndex());
        new_ini->setValue("feeding_isBackRotate180",ui->cbtn_backRotate_180->isChecked());
        new_ini->setValue("feeding_isUltDetect",ui->cbtn_doubleCheck->isChecked());
        new_ini->setValue("feeding_isBindDetect",ui->cbtn_stapleCheck->isChecked());
        new_ini->setValue("feeding_isScrewDetect",ui->cbtn_skewCheck->isChecked());
        new_ini->setValue("feeding_screwLevel",ui->Slider_skewCheck->value());
        new_ini->setValue("other_isSplitImage",ui->cbtn_imgSplit->isChecked());
        new_ini->setValue("other_isSetJpgQuality",ui->cbtn_jpgPercent->isChecked());
        new_ini->setValue("other_jpgQuality",ui->Slider_jpgPercent->value());
        new_ini->setValue("other_isOCRPDF",ui->gbox_ocr->isChecked());
        int ocrType = 0;
        if(ui->rbtn_ocrPDF->isChecked())
            ocrType = 1;
        else if(ui->rbtn_ocrTxt->isChecked())
            ocrType = 2;
        else if(ui->rbtn_ocrPDF_txt->isChecked())
            ocrType = 3;
        new_ini->setValue("other_ocrType",ocrType);
        new_ini->setValue("other_isPreScan",ui->cbtn_preScan->isChecked());
        isAddSuccess = true;
    }
    delete new_ini;
    return isAddSuccess;
}
void scanSettingsDialog::delete_ini(QString name)
{
    QFileInfo file(m_iniDirPath+"/"+name+".ini");
    if(!file.exists()) return;
    file.dir().remove(file.fileName());
}
void scanSettingsDialog::load_ini(QString name)
{
    isUsrSettingChange = true;
    QSettings *local_ini = new QSettings(m_iniDirPath+"/"+name+".ini",QSettings::IniFormat);//��֧�ּ���appDir�µ�ini�����ļ�
    ui->cbox_colorMode->setCurrentIndex(local_ini->value("base_colorModeIndex",0).toInt());
    ui->cbtn_multiStream->setChecked(local_ini->value("base_isMultiOutput",false).toBool());
    ui->cbox_multiStream->setCurrentIndex(local_ini->value("base_multiOutType",0).toInt());
    ui->cbox_paperSize->setCurrentIndex(local_ini->value("base_paperSizeIndex",1).toInt());
    ui->cbtn_sizeAreaSwitch->setChecked(local_ini->value("base_isCustomArea",false).toBool());
    sizeAreaRect.setX(local_ini->value("base_customAreaX",0).toInt());
    sizeAreaRect.setY(local_ini->value("base_customAreaY",0).toInt());
    sizeAreaRect.setWidth(local_ini->value("base_customAreaW",2338).toInt());
    sizeAreaRect.setHeight(local_ini->value("base_customAreaH",3307).toInt());
    ui->cbox_resolution->setCurrentIndex(local_ini->value("base_resolution",2).toInt());
    ui->cbox_scanPage->setCurrentIndex(local_ini->value("base_scanPage",1).toInt());
    ui->spin_brightness->setValue(local_ini->value("BCG_brightness",128).toInt());
    ui->spin_contrast->setValue(local_ini->value("BCG_contrast",4).toInt());
    ui->dSpin_gamma->setValue(local_ini->value("BCG_gamma",1.0f).toDouble());
    ui->cbtn_colorLineSwitch->setChecked(local_ini->value("BCG_isCustomGama",false).toBool());
    //���ó�ʼ������point
    QPoint start(0,0);
    QPoint end(255,255);
    QList<QVariant> init_list;
    init_list.append(QVariant(start));
    init_list.append(QVariant(end));
    //��ȡ5������point
    QList<QVariant> RGBline,Rline,Gline,Bline,GRAYline;
    RGBline = local_ini->value("BCG_RGBline",init_list).toList();
    Rline = local_ini->value("BCG_Rline",init_list).toList();
    Gline = local_ini->value("BCG_Gline",init_list).toList();
    Bline = local_ini->value("BCG_Bline",init_list).toList();
    GRAYline = local_ini->value("BCG_greyline",init_list).toList();
    //��ֵKeyPoint
    rgbKeyPoint.clear();
    for(int i=0;i<4;i++)
    {
        QList<QPoint> list;
        switch(i)
        {
        case 0:
            for(int j=0;j<RGBline.size();j++)
                list.append(RGBline[j].toPoint());
            break;
        case 1:
            for(int j=0;j<Rline.size();j++)
                list.append(Rline[j].toPoint());
            break;
        case 2:
            for(int j=0;j<Gline.size();j++)
                list.append(Gline[j].toPoint());
            break;
        case 3:
            for(int j=0;j<Bline.size();j++)
                list.append(Bline[j].toPoint());
            break;
        default:
            break;
        }
        rgbKeyPoint.append(list);
    }
    grayKeyPoint.clear();
    for(int i=0;i<GRAYline.size();i++)
        grayKeyPoint.append(GRAYline[i].toPoint());
    ui->cbtn_deBlack->setChecked(local_ini->value("imgProc_isFillBlack",false).toBool());
    ui->cbtn_adjust->setChecked(local_ini->value("imgProc_isDescrew",false).toBool());
    ui->cbtn_antiInfi->setChecked(local_ini->value("imgProc_isAntiInfiltration",false).toBool());
    ui->cbtn_deHole->setChecked(local_ini->value("imgProc_isOutHole",false).toBool());
    ui->Slider_deHole->setValue(local_ini->value("imgProc_outHoleValue",1).toInt());
    ui->cbox_deColor->setCurrentIndex(local_ini->value("imgProc_filterIndex",0).toInt());
    ui->cbtn_colorAdjust->setChecked(local_ini->value("imgProc_isColorCorrection",false).toBool());
    ui->cbtn_deMoire->setChecked(local_ini->value("imgProc_isRemoveMoire",false).toBool());
    ui->cbtn_setBWDot->setChecked(local_ini->value("imgProc_isErrorExtention",false).toBool());
    ui->cbtn_noise->setChecked(local_ini->value("imgProc_isNoiseOptimization",false).toBool());
    ui->cbtn_texRmv->setChecked(local_ini->value("imgProc_isTextureRemove",false).toBool());
    ui->gbox_sharAndBlur->setChecked(local_ini->value("imgProc_isSharOrBlur",false).toBool());
    int sharpenType = local_ini->value("imgProc_sharpenType",0).toInt();
    switch(sharpenType)
    {
    case 0:
        break;
    case 1:
        ui->rbtn_sharpening1->setChecked(true);
        break;
    case 2:
        ui->rbtn_sharpening2->setChecked(true);
        break;
    case 3:
        ui->rbtn_blur1->setChecked(true);
        break;
    case 4:
        ui->rbtn_blur2->setChecked(true);
        break;
    default:
        break;
    }
    ui->cbtn_specialMulti->setChecked(local_ini->value("imgProc_isMultiOutRed",false).toBool());
    ui->cbtn_deRed_asheet->setChecked(local_ini->value("imgProc_isAnswerSheetFilter",false).toBool());
    int ScanCount = local_ini->value("feeding_scanCount",999).toInt();
    if(ScanCount == 999)
        ui->rbtn_maxPage->setChecked(true);
    else
    {
        ui->rbtn_pageNum->setChecked(true);
        ui->spin_pageNum->setValue(ScanCount);
    }
    ui->cbox_pageDirection->setCurrentIndex(local_ini->value("feeding_pageOrientationIndex",0).toInt());
    ui->cbtn_backRotate_180->setChecked(local_ini->value("feeding_isBackRotate180",false).toBool());
    ui->cbtn_doubleCheck->setChecked(local_ini->value("feeding_isUltDetect",true).toBool());
    ui->cbtn_stapleCheck->setChecked(local_ini->value("feeding_isBindDetect",false).toBool());
    ui->cbtn_skewCheck->setChecked(local_ini->value("feeding_isScrewDetect",false).toBool());
    ui->Slider_skewCheck->setValue(local_ini->value("feeding_screwLevel",3).toInt());
    ui->cbtn_imgSplit->setChecked(local_ini->value("other_isSplitImage",false).toBool());
    ui->cbtn_jpgPercent->setChecked(local_ini->value("other_isSetJpgQuality",false).toBool());
    ui->Slider_jpgPercent->setValue(local_ini->value("other_jpgQuality",10).toInt());
    ui->gbox_ocr->setChecked(local_ini->value("other_isOCRPDF",false).toBool());
    int ocrType = local_ini->value("other_ocrType",0).toInt();
    switch(ocrType)
    {
    case 1:
        ui->rbtn_ocrPDF->setChecked(true);
        break;
    case 2:
        ui->rbtn_ocrTxt->setChecked(true);
        break;
    case 3:
        ui->rbtn_ocrPDF_txt->setChecked(true);
        break;
    default:
        break;
    }
    ui->cbtn_preScan->setChecked(local_ini->value("other_isPreScan",false).toBool());
    delete local_ini;
}
void scanSettingsDialog::load_iniList()
{
    QSettings *app_ini = new QSettings(m_appDirPath+"/GSCAN_paraConfig.ini",QSettings::IniFormat);
    int index = app_ini->value("userSettings_index",0).toInt();
    if(index != -1)
    {
        QStringList list = app_ini->value("userSettings_list").toStringList();
        ui->listWidget->addItems(list);
        ui->listWidget->setCurrentRow(index);
    }
    delete app_ini;
}
void scanSettingsDialog::save_iniList()
{
    QSettings *app_ini = new QSettings(m_appDirPath+"/GSCAN_paraConfig.ini",QSettings::IniFormat);
    int index = -1;//��ʶ�б��Ƿ�Ϊ�գ�-1Ϊ�գ�����¼��ǰѡ��λ��
    int count = ui->listWidget->count();
    if(count > 0)
    {
        index = ui->listWidget->currentRow();
        QStringList list;
        for(int i = 0; i < count; i++)
        {
            list.append(ui->listWidget->item(i)->text());
        }
        app_ini->setValue("userSettings_list",list);
    }
    app_ini->setValue("userSettings_index",index);
    delete app_ini;
}
