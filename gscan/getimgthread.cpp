#include "getimgthread.h"
#include <opencv2/opencv.hpp>
#include <thread>
#include <QDebug>

GetImgThread::GetImgThread()
{
}
GetImgThread::~GetImgThread()
{
}

//void GetImgThread::setThreadPara(std::shared_ptr<IDev> scanner, QString dirpath, QString format, QDir tmpdir, HG_OCR *ocrPDF, int dpi, int paperSize)
//{
//    m_scanner = scanner;
//    m_dirpath = dirpath;
//    m_format = format;
//    m_tmpdir = tmpdir;
//    //m_ocrPDF = ocrPDF;
//    m_dpi = dpi;
//    m_paperSize = paperSize;
//}

void GetImgThread::setThreadOptPara(bool isOCR, int ocrType, int jpg_quality)
{
    m_isOCR = isOCR;
    m_ocrType = ocrType;
    m_jpg_quality = jpg_quality;
}

void GetImgThread::setThreadToPreMode()
{
    m_isPreScan = true;
    m_inputTmpList.clear();
    m_inputMatList.clear();
}

void GetImgThread::setInputList(QStringList inputTmpList, QList<cv::Mat> inputMatList)
{
    m_inputTmpList = inputTmpList;
    m_inputMatList = inputMatList;
}

int GetImgThread::getIndex()
{
    return index;
}

void GetImgThread::run()
{
//    index = 0;
//    cv::Mat matTemp;
//    QStringList pdfFileList,tifFileList,ofdFileList,preTmpList;
//    QList<QSize> pdfSizeList;
//    QList<cv::Mat> preMatList;
//    FileExport *fx_convert = new FileExport;
//    //if inputMatList contains images, save preview images first
//    if(!m_inputMatList.isEmpty())
//    {
//        for(int i = 0; i < m_inputMatList.size(); i++)
//        {
//            imgSave(m_inputMatList.at(i), m_inputTmpList.at(i), pdfFileList, tifFileList, ofdFileList, pdfSizeList, fx_convert);
//        }
//        m_inputTmpList.clear();
//        m_inputMatList.clear();
//    }
//    //waiting for captured images, timeout means there is no image captured this time
//    int timeout = 0;
//    while(imgIsEmpty && m_scanner->is_run())
//    {
//        if(timeout++ == 10)
//        {
//            timeout = 0;
//            break;
//        }
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//        checkImgIsEmpty();
//    }
//    //get images loop
//    while(!imgIsEmpty)
//    {
//        int name_index = index;
//        while(m_scanner->count_image() != 0)
//        {
//            qDebug()<<"count image = "<<m_scanner->count_image();
//            m_scanner->get_image(matTemp);
//            if(matTemp.cols == 0 || matTemp.rows == 0) continue;
//            matTemp = matTemp.clone();
//            index++;
//            QString tmpFilePath = tmpFileNameCheck(name_index);
//            cv::imwrite(tmpFilePath.toStdString(),matTemp);
//            if(m_isPreScan)
//            {
//                preTmpList.append(tmpFilePath);
//                preMatList.append(matTemp);
//            }
//            else
//            {
//                emit loadImgToListview(tmpFilePath);
//                if(m_dirpath != m_tmpdir.path())//images saved in non-tmp dir when using 'scan lots to' function
//                {
//                    imgSave(matTemp, tmpFilePath, pdfFileList, tifFileList,ofdFileList, pdfSizeList, fx_convert);
//                }
//            }
//        }
//        std::this_thread::sleep_for(std::chrono::seconds(2));
//        checkImgIsEmpty();
//    }
//    emit viewFit();
//    //multi-page file creating
//    if(!pdfFileList.isEmpty())
//    {
//        QString format = m_format.remove("[multi]");
//        QString dirFilePath = fileNameCheck(format);
//        QString dirFileName = dirFilePath.remove(dirFilePath.right(4));
//        if(m_isOCR)
//            m_ocrPDF->save(pdfFileList, m_dpi, dirFileName, m_ocrType);
//        else
//            fx_convert->toPDFs(pdfFileList,m_dpi,pdfSizeList,dirFilePath);
//    }
//    if(!tifFileList.isEmpty())
//    {
//        QString format = m_format.remove("[multi]");
//        QString dirFilePath = fileNameCheck(format);
//        fx_convert->toTIFFs(tifFileList,dirFilePath);
//    }
//    if(!ofdFileList.isEmpty())
//    {
//        QString format = m_format.remove("[multi]");
//        QString dirFilePath = fileNameCheck(format);
//        fx_convert->toOFDs(ofdFileList,dirFilePath);
//    }
//    m_isPreScan = false;//clear pre-scan flag
//    emit getImgFinished(index);
//    delete fx_convert;
//    //pre-scan tmp files list sending
//    if(!preTmpList.isEmpty())
//        emit preScanList(preTmpList, preMatList, m_dirpath, m_format);
}

void GetImgThread::imgSave(cv::Mat srcMat, QString tmpFilePath, QStringList &pdfFileList, QStringList &tifFileList, QStringList &ofdFileList, QList<QSize> &pdfSizeList, FileExport *fx_convert)
{
    //diff converting with diff formats
	if(m_format == ".pdf")
    {
        if(m_isOCR)
        {
            QString dirFilePath = fileNameCheck(m_format);
            QStringList list;
            list.append(tmpFilePath);
            fx_convert->toOcrPDFs(list,1,dirFilePath,0);
        }
        else
        {
            QString dirFilePath = fileNameCheck(m_format);
            fx_convert->toPDF(tmpFilePath,m_dpi,srcMat.cols,srcMat.rows,dirFilePath);
        }
    }
    else if(m_format == ".ofd")
    {
        QString dirFilePath = fileNameCheck(m_format);
        fx_convert->toOFD(tmpFilePath,dirFilePath);
    }
    else if(m_format == ".pdf[multi]")
    {
        pdfFileList.append(tmpFilePath);
        QSize mat_size(srcMat.cols,srcMat.rows);
        pdfSizeList.append(mat_size);
    }
    else if(m_format == ".tif[multi]")
    {
        tifFileList.append(tmpFilePath);
    }
    else if(m_format == ".ofd[multi]")
    {
        ofdFileList.append(tmpFilePath);
    }
    else//non pdf/multi pdf/multi tif formats, creat file using cv::imwrite directly 
    {
        QString dirFilePath = fileNameCheck(m_format);
        if(m_format == ".jpg" || m_format == ".jpeg")//jpg/jpeg formats need to set quality
		{
            std::vector<int> quality;
            quality.push_back(CV_IMWRITE_JPEG_QUALITY);
            quality.push_back(m_jpg_quality);
            cv::imwrite(dirFilePath.toStdString(),srcMat,quality);
        }
        else
        {
            cv::imwrite(dirFilePath.toStdString(),srcMat);
        }
    }
}

void GetImgThread::checkImgIsEmpty()
{
//    imgIsEmpty = (m_scanner->count_image() == 0);
}

QString GetImgThread::fileNameCheck(QString format)
{
    //creat dst file
	int name_index = 1;
    QString dstFileName = m_dirpath+"/HGScan_"+QString::number(name_index)+format;
    while(QFileInfo::exists(dstFileName))
    {
        name_index++;
        dstFileName = m_dirpath+"/HGScan_"+QString::number(name_index)+format;
    }
    return dstFileName;
}

QString GetImgThread::tmpFileNameCheck(int index)
{
    //creat dst file path
	int name_index = index;
    QString dstFileName = m_tmpdir.path()+"/HGScan_"+QString::number(name_index)+".jpg";
    //check if path is existed, auto add number in file name
	while(QFileInfo::exists(dstFileName))
    {
        name_index++;
        dstFileName = m_tmpdir.path()+"/HGScan_"+QString::number(name_index)+".jpg";
    }
    return dstFileName;
}
