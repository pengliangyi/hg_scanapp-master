#ifndef IMAGELIST_H
#define IMAGELIST_H

#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

#include <QListWidget>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QList>
#include <QMenu>
#include <QContextMenuEvent>
#include <QMouseEvent>
//////////////////////////////////////////////////////////////////////////
#ifndef _IMAGELIST_DLL 
#define _IMAGELIST_DLL Q_DECL_EXPORT
#else
#define _IMAGELIST_DLL Q_DECL_IMPORT
#endif

struct ImageInfo{
  QImage m_image;
  QString image_name;
  QString image_size;
  QString image_type;
  QString image_data;
};

class ImageList : public QListWidget
{
    Q_OBJECT

public:
    explicit ImageList(QWidget* parent);
	~ImageList();
    QAction* rotate_s;
    QAction* rotate_n;
    QAction* first_page;
    QAction* last_page;
    QAction* delSelect;
    QAction* merge;
    QAction* save_as;
    QAction* close;

public:
	void addImage(const QString& filename);
	void addImage(const QString& filename, const QImage& img);
	void changeImage(const QString& filename, const QImage& img);
	void insertImage(const QString& filename, const QImage& img, int index);
	void removeSelected();
    void imageRorate(QListWidgetItem *item, int angle);
    void saveImageToFile(QImage img, QString filename);   
    void saveImageList();
    void saveAllImageList();
	QString currentFilename() const;
    QList<ImageInfo>* getImageInfo();
    QList<ImageInfo> *updateImageInfoList();//更新imageList
    QStringList getImgNamesList();//获取列表所有图片路径名单
    QStringList getImgNamesList(QList<int> indexlist);//获取指定index的所有图片名单
    QStringList getSelectedImages();
    QList<QSize> getCurSelectedImagesSize();
    QList<int> getCurSelectedImageDPI();
    int getCurSelectedImageCount();
    void deleteItem(int index);//删除指定index的成员
    void deleteItems(QList<int> indexs);//删除指定index列表中的成员
    void deleteSelectedItems();
    void deleteAllItems();
    void cancelCurSelItemsState();

signals:
	void imageChanged(const QString& filename);
    void rotateSSig();
    void rotateNSig();
    void selectedItemCountChange(int count);

private slots:
	void selectedChangedHandler(QListWidgetItem* current, QListWidgetItem* previous);
    void itemCilckedSlot(QListWidgetItem *item);
    void rotateSSlot();
    void rotateNSlot();
    void firstPageSlot();
    void lastPageSlot();
    void delSelectSlot();
    void delAllSlot();
    void mergeSlot();

private:
	QPixmap iconFromImage(const QImage& img);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void contextMenuEvent(QContextMenuEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void initRightBtnMenu();
    void deleteItem(QListWidgetItem *item);
    void deleteFunction(QList<QListWidgetItem*> items);

private:
	const static int WIDTH_ICON   = 150;
	const static int HEIGHT_ICON  = 120;
	const static int WIDTH_IMAGE  = 140;
	const static int HEIGHT_IMAGE = 110;
    QList<ImageInfo>* imageList;
    QMenu m_menu;
    int selectedCount;
};

#endif /*IMAGELIST_H*/
