#include "FileExport.h"
#include <QFile>
#include <QPdfWriter>
#include <QPainter>
#include <QImage>
#include <QDebug>

FileExport::FileExport()
{    
}

FileExport::~FileExport()
{
}

void FileExport::toPDF(QString src_file, int dpi,int page_width, int page_height, QString dst_file)
{
    //convert width&height unit to Inch
    double w = ((double)page_width)/dpi;
    double h = ((double)page_height)/dpi;
    QFile pdfFile(dst_file);//create pdf file
    pdfFile.open(QIODevice::WriteOnly);
    QPdfWriter *pdf_writer = new QPdfWriter(&pdfFile);//create pdfwriter
    pdf_writer->setPageSize(QPageSize(QSizeF(w,h),QPageSize::Inch,"",QPageSize::ExactMatch));//ExactMatch:fobidden auto resize to standard size
    pdf_writer->setResolution(dpi);
    pdf_writer->setMargins(QPagedPaintDevice::Margins{0,0,0,0});
    QPainter *pdf_painter = new QPainter(pdf_writer);//create painter
    pdf_painter->drawImage(0,0,QImage(src_file));//paint image
    delete pdf_painter;
    delete pdf_writer;
    pdfFile.close();
}

void FileExport::toPDFs(QStringList src_files, QList<int> files_dpi, QList<QSize> files_size, QString dst_file)
{
    QFile pdfFile(dst_file);
    pdfFile.open(QIODevice::WriteOnly);
    QPdfWriter *pdf_writer = new QPdfWriter(&pdfFile);
    pdf_writer->setMargins(QPagedPaintDevice::Margins{0,0,0,0});
    //set the first page's size(painter's first size)
    pdf_writer->setResolution(files_dpi[0]);
    double w = ((double)files_size[0].width())/files_dpi[0];
    double h = ((double)files_size[0].height())/files_dpi[0];
    pdf_writer->setPageSize(QPageSize(QSizeF(w,h),QPageSize::Inch,"",QPageSize::ExactMatch));
    QPainter *pdf_painter = new QPainter(pdf_writer);
    //paint multiple pages loop
    for(int i=0;i<src_files.size();i++)
    {
        if(i != 0)//skip the first page
        {
            pdf_writer->setResolution(files_dpi[i]);
            double w = ((double)files_size[i].width())/files_dpi[i];
            double h = ((double)files_size[i].height())/files_dpi[i];
            pdf_writer->setPageSize(QPageSize(QSizeF(w,h),QPageSize::Inch,"",QPageSize::ExactMatch));
            pdf_writer->newPage();
        }
        pdf_painter->drawImage(0,0,QImage(src_files[i]));
    }
    delete pdf_painter;
    delete pdf_writer;
    pdfFile.close();
}

void FileExport::toPDFs(QStringList src_files, int dpi, QList<QSize> files_size, QString dst_file)
{
    QFile pdfFile(dst_file);
    pdfFile.open(QIODevice::WriteOnly);
    QPdfWriter *pdf_writer = new QPdfWriter(&pdfFile);
    pdf_writer->setResolution(dpi);
    //set the first page's size
    double w = ((double)files_size[0].width())/dpi;
    double h = ((double)files_size[0].height())/dpi;
    pdf_writer->setPageSize(QPageSize(QSizeF(w,h),QPageSize::Inch,"",QPageSize::ExactMatch));

    pdf_writer->setMargins(QPagedPaintDevice::Margins{0,0,0,0});
    QPainter *pdf_painter = new QPainter(pdf_writer);
    //paint multiple pages loop
    for(int i=0;i<src_files.size();i++)
    {
        if(i != 0)//skip the first page
        {
            double w = ((double)files_size[i].width())/dpi;
            double h = ((double)files_size[i].height())/dpi;
            pdf_writer->setPageSize(QPageSize(QSizeF(w,h),QPageSize::Inch,"",QPageSize::ExactMatch));
            pdf_writer->newPage();
        }
        pdf_painter->drawImage(0,0,QImage(src_files[i]));
    }
    delete pdf_painter;
    delete pdf_writer;
    pdfFile.close();
}

void FileExport::toOcrPDFs(QStringList src_files, int pageCount, QString dst_file, int saveMode)
{
    char** srcFiles = new char*[pageCount];
    for(int i = 0; i < src_files.size(); i++)
    {
        QByteArray qba_src = src_files.at(i).toLatin1();
        srcFiles[i] = qba_src.data();
    }
    QByteArray qba_dst = dst_file.toLatin1();
//    ocr_init();
//    ocr(srcFiles, pageCount, qba_dst.data(), saveMode);
    delete []srcFiles;
}

void FileExport::toTIFF(QString src_file, QString dst_file)
{
    TIFF *tif_file = TIFFOpen(dst_file.toStdString().c_str(),"w");//create tiff file
    if (!tif_file)
        return;
    jpg2tiff(tif_file,0,src_file);//single jpg to tiff
    TIFFClose(tif_file);
}

void FileExport::toTIFFs(QStringList src_files, QString dst_file)
{
    TIFF *tif_file = TIFFOpen(dst_file.toStdString().c_str(),"w");
    if (!tif_file)
        return;
    int pageIndex = 0;//init page number
    for (int i=0;i<src_files.size();i++)//convert jpgs to tiff loop
    {
        jpg2tiff(tif_file,pageIndex,src_files[i]);
        pageIndex++;
    }
    TIFFClose(tif_file);
}

void FileExport::toOFD(QString src_file, QString dst_file)
{
    QByteArray qba_src = src_file.toUtf8();
    QByteArray qba_dst = dst_file.toUtf8();
//    ImagetoOfd(qba_src.data(), qba_dst.data());
}

void FileExport::toOFDs(QStringList src_files, QString dst_file)
{
    QByteArray qba_dst = dst_file.toUtf8();
    for(int i = 0; i < src_files.size(); i++)
    {
        QByteArray qba_src = src_files.at(i).toUtf8();
//        ImagetoOfd(qba_src.data(), qba_dst.data());
    }
}


void FileExport::jpg2tiff(TIFF *tif_file, int pageIndex, QString jpg_file)
{
    if(!tif_file)
        return;
    QImage img(jpg_file);
    img = img.convertToFormat(QImage::Format_RGBA8888);//convert image format
    TIFFSetDirectory(tif_file, pageIndex);//set current operation page number

    TIFFSetField(tif_file, TIFFTAG_SUBFILETYPE, FILETYPE_PAGE);//set page type
    TIFFSetField(tif_file, TIFFTAG_IMAGEWIDTH, img.width());//set page width
    TIFFSetField(tif_file, TIFFTAG_IMAGELENGTH, img.height());//set page height
    TIFFSetField(tif_file, TIFFTAG_SAMPLESPERPIXEL, 4);//set every pixel channels
    TIFFSetField(tif_file, TIFFTAG_BITSPERSAMPLE, 8);//set every channel depth
    TIFFSetField(tif_file, TIFFTAG_COMPRESSION, COMPRESSION_LZW);//set compress type
    TIFFSetField(tif_file, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);//set color type
    TIFFSetField(tif_file, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);//set store style

    uchar *imgData = img.bits();//get jpg image data
    for (int i = 0; i < img.height(); i++)//write tiff by lines
    {
        TIFFWriteScanline(tif_file, imgData + img.bytesPerLine() * i, i);
    }
    TIFFWriteDirectory(tif_file);//write page
}

