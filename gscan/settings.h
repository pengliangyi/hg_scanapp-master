#pragma once

#include <QList>
#include <QVariant>

enum errCode
{
    HAVE_ERROR,
    OPEN_COVER,
    NO_FEED,
    FEED_IN_ERROR,
    PAPER_JAM,
    DETECT_DOUBLE_FEED,
    DETECT_STAPLE,
    PAPER_SKEW,
    FPGA_ERROR,
    PC_SCAN_BUSY_or_ERROR
};

struct tagJPGQuality
{
    bool isSetQuality;
    int Quality;
};
typedef struct tagJPGQuality JPGQuality;

struct tagColorLine
{
    QList<QVariant> RGBlist;
    QList<QVariant> Rlist;
    QList<QVariant> Glist;
    QList<QVariant> Blist;
};
typedef struct tagColorLine ColorLine;

struct tagGreyLine
{
    QList<QVariant> greylist;
};
typedef struct tagGreyLine GreyLine;

struct tagLocalParams
{
    JPGQuality jpgQuality;
    bool isOcrPDF;
    int ocrType;//1:only PDF;  2:only txt;  3:both
    ColorLine colorLine;
    GreyLine greyLine;
    bool isPreScan;
};
typedef struct tagLocalParams LocalParams;

