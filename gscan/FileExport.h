#ifndef FILEEXPORT_H
#define FILEEXPORT_H
#include <QString>
#include <QSize>
#include "tiffio.h"
#include "suwellofdconvert.h"
//#include "HG_OCR.h"

class FileExport
{
public:
	FileExport();
	~FileExport();
	void toPDF(QString src_file, int dpi, int page_width, int page_height, QString dst_file);
	void toPDFs(QStringList src_files, QList<int> files_dpi, QList<QSize> files_size, QString dst_file);
	void toPDFs(QStringList src_files, int dpi, QList<QSize> files_size, QString dst_file);
    void toOcrPDFs(QStringList src_files, int pageCount, QString dst_file, int saveMode);
	void toTIFF(QString src_file, QString dst_file);
	void toTIFFs(QStringList src_files, QString dst_file);
    void toOFD(QString src_file, QString dst_file);
    void toOFDs(QStringList src_files, QString dst_file);
private:
	void jpg2tiff(TIFF* tif_file, int pageIndex, QString jpg_file);

};

#endif // FILEEXPORT_H
