#ifndef GETIMGTHREAD_H
#define GETIMGTHREAD_H

#include <QThread>
#include <QDir>
#include <memory>
#include "opencv2/opencv.hpp"
#include "FileExport.h"

//class IDev;

class GetImgThread : public QThread
{
    Q_OBJECT
public:
    GetImgThread();
    ~GetImgThread();
//    void setThreadPara(std::shared_ptr<IDev> scanner, QString dirpath, QString format, QDir tmpdir, HG_OCR* ocrPDF,int dpi, int paperSize);
    void setThreadOptPara(bool isOCR = false, int ocrType = 0, int jpg_quality = 100);
    void setThreadToPreMode();
    void setInputList(QStringList inputTmpList, QList<cv::Mat> inputMatList);
    int getIndex();

signals:
    void getImgFinished(int index);
    void loadImgToListview(QString filePath);
    void viewFit();
    void preScanList(QStringList preTmpList, QList<cv::Mat> preMatList, QString dirpath, QString format);

protected:
    virtual void run();

private slots:
    void checkImgIsEmpty();
private:
    QString m_dirpath,m_format;
    QStringList m_inputTmpList;
    QList<cv::Mat> m_inputMatList;
//    std::shared_ptr<IDev> m_scanner;
    QDir m_tmpdir;
    int index;
    int m_jpg_quality = 100;
    int m_dpi;
    int m_paperSize;
    bool m_isPreScan = false;
    bool imgIsEmpty = true;
    bool m_isOCR = false;
    int m_ocrType = 0;
    QString fileNameCheck(QString format);
    QString tmpFileNameCheck(int index);
    void imgSave(cv::Mat srcMat, QString tmpFilePath, QStringList &pdfFileList, QStringList &tifFileList, QStringList &ofdFileList, QList<QSize> &pdfSizeList, FileExport *fx_pdf_tif);

};

#endif // GETIMGTHREAD_H
