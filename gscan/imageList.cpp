#include "./imageList.h"
#include <QFile>
#include <QImage>
#include <QDebug>
#include <QFileInfo>
#include <QListWidgetItem>
#include <QMatrix>
#include <QMessageBox>
#include <QApplication>
#include <QString>

ImageList::ImageList(QWidget* parent)
    :QListWidget (parent),
      imageList(new QList<ImageInfo>())
{
   // this->setFixedWidth(175);
    this->setResizeMode(QListView::Adjust);
    this->setIconSize(QSize(this->width(), 150));
    this->setAutoScroll(true);
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setSpacing(10);

    initRightBtnMenu();
	connect(this, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
        this, SLOT(selectedChangedHandler(QListWidgetItem*, QListWidgetItem*)));

    connect(this, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(itemCilckedSlot(QListWidgetItem *)));
}

ImageList::~ImageList()
{
    delete imageList;
}

//////////////////////////////////////////////////////////////////////////

void ImageList::addImage(const QString& filename)
{
    if (QFile::exists(filename))
    {
        QImage image(filename);
        addImage(filename, image);
    }
}

void ImageList::addImage(const QString& filename, const QImage& img)
{
    QString itemname;
    if (QFile::exists(filename))
    {
        //if file is already in list, delete it first  
		for (int i=0; i<this->count(); i++)
        {
            QListWidgetItem* item = this->item(i);
            itemname = item->data(Qt::WhatsThisRole).toString();
            if (itemname == filename)
            {
                this->removeItemWidget(this->item(i));
                break;
            }
        }
        QListWidgetItem* item = new QListWidgetItem(iconFromImage(img), "");
        item->setToolTip(filename);
        item->setData(Qt::WhatsThisRole, filename);
        struct ImageInfo imageTmp;
        imageTmp.m_image = QImage(img);
        imageTmp.image_name = filename;
        imageTmp.image_size = QString::number(img.width()) + " * " + QString::number(img.height());
        imageTmp.image_type = filename.split(".").last();
        imageTmp.image_data = QString::number(QFile(filename).size());
        imageList->append(imageTmp);
        this->insertItem(0, item);
        this->setCurrentItem(item);
    }
}

void ImageList::changeImage(const QString& filename, const QImage& img)
{
    QString itemname;
    QListWidgetItem* item;
    if (QFile::exists(filename))
    {
        //if file is already in list, delete it first      
		for (int i=0; i<this->count(); i++)
        {
            item = this->item(i);
            itemname = item->data(Qt::WhatsThisRole).toString();
            if (itemname == filename)
            {
                break;
            }
        }
        item->setIcon(iconFromImage(img));
//        item->setToolTip(filename);
//        item->setData(Qt::WhatsThisRole, filename);
        //this->setCurrentItem(item);
    }
}

void ImageList::insertImage(const QString& filename, const QImage& img, int index)
{

}

void ImageList::removeSelected()
{
    int curow = this->currentRow();
    if (this->count() > 0)
    {
        if (this->count() == 1)//only 1 item           
		emit imageChanged("");
        else
        {
            if (curow == (this->count() - 1))//last item
			this->setCurrentRow(curow - 1);
            else
                this->setCurrentRow(curow + 1);
        }
        this->removeItemWidget(item(curow));
    }
}

QString ImageList::currentFilename() const
{
    QListWidgetItem* item = this->currentItem();
    if (item != 0)
    {
        return item->data(Qt::WhatsThisRole).toString();
    }

    return "";
}

QList<ImageInfo>* ImageList::getImageInfo()
{
    return imageList;
}

QList<ImageInfo>* ImageList::updateImageInfoList()
{
    if(this->count()==0) return 0;
    QStringList lst;
    for(int i=0;i<this->count();i++)
    {
        lst.append(this->item(i)->data(Qt::WhatsThisRole).toString());
    }
    imageList->clear();
    struct ImageInfo imageTmp;
    for(int i=0;i<lst.size();i++)
    {
        QImage img = QImage(lst.at(i));
        imageTmp.m_image = img;
        imageTmp.image_name = lst.at(i);
        imageTmp.image_size = QString::number(img.width()) + " * " + QString::number(img.height());
        imageTmp.image_type = lst.at(i).split(".").last();
        imageTmp.image_data = QString::number(QFile(lst.at(i)).size());
        imageList->append(imageTmp);
    }
    return imageList;
}

QStringList ImageList::getImgNamesList()
{
    QStringList lst;
    for(int i=0;i<this->count();i++)
    {
        lst.append(this->item(i)->data(Qt::WhatsThisRole).toString());
    }
    return lst;
}

QStringList ImageList::getImgNamesList(QList<int> indexlist)
{
    QStringList lst;
    for(int i=0;i<indexlist.size();i++)
    {
        lst.append(this->item(indexlist.at(i))->data(Qt::WhatsThisRole).toString());
    }
    return lst;
}

//////////////////////////////////////////////////////////////////////////

void ImageList::selectedChangedHandler(QListWidgetItem* current, QListWidgetItem* previous)
{
    QString filename("");
    if (current != 0)
    {
        filename = current->data(Qt::WhatsThisRole).toString();
    }

    emit imageChanged(filename);
}

void ImageList::itemCilckedSlot(QListWidgetItem *item)
{
    QList<QListWidgetItem*> items = this->selectedItems();
    selectedCount = items.size();
    emit selectedItemCountChange(selectedCount);
}

void ImageList::rotateSSlot()
{
    QList<QListWidgetItem*> items = this->selectedItems();
    for(int i = 0; i < items.size(); i++)
    {
        imageRorate(items[i],90);
    }
    emit rotateSSig();
}

void ImageList::rotateNSlot()
{
    QList<QListWidgetItem*> items = this->selectedItems();
    for(int i = 0; i < items.size(); i++)
    {
        imageRorate(items[i],-90);
    }
    emit rotateNSig();
}

void ImageList::firstPageSlot()
{
    setCurrentRow(0);
    emit itemClicked(item(0));
}

void ImageList::lastPageSlot()
{
    setCurrentRow(count()-1);
    emit itemClicked(item(count()-1));
}

void ImageList::delSelectSlot()
{
    QList<QListWidgetItem*> items = this->selectedItems();
    deleteFunction(items);
}

void ImageList::delAllSlot()
{
    QList<QListWidgetItem*> items;
    for(int i=0;i<this->count();i++)
    {
        items.append(this->item(i));
    }
    deleteFunction(items);
}

void ImageList::mergeSlot()
{

}

//////////////////////////////////////////////////////////////////////////

QPixmap ImageList::iconFromImage(const QImage& img)
{
    QImage imageIcon;
    if(img.width() < img.height())
        imageIcon = img.scaledToHeight(HEIGHT_IMAGE, Qt::SmoothTransformation);
    else
        imageIcon = img.scaledToWidth(WIDTH_IMAGE, Qt::SmoothTransformation);
    int image_width = imageIcon.width();
    int image_height = imageIcon.height();

    QRectF target((WIDTH_ICON - image_width) / 2, (HEIGHT_ICON - image_height) / 2,
        image_width, image_height);
    QRectF source(0, 0, image_width, image_height);

    QPixmap pixDraw(WIDTH_ICON, HEIGHT_ICON);
    QPainter painter(&pixDraw);

    painter.setBrush(Qt::NoBrush);
    painter.fillRect(QRect(0, 0, WIDTH_ICON, HEIGHT_ICON), Qt::white);
    painter.drawImage(target, imageIcon, source);
    painter.setPen(Qt::black);
    painter.drawRect(target);

    return pixDraw;
}

void ImageList::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        QListWidget::mousePressEvent(event);
    }
}
void ImageList::mouseReleaseEvent(QMouseEvent *event)
{
    QListWidget::mouseReleaseEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        if(!(QApplication::keyboardModifiers() == Qt::ControlModifier || QApplication::keyboardModifiers() == Qt::ShiftModifier)){
            if(this->count() > 0){
                currentItem()->setSelected(true);
            }
        }
        emit selectedItemCountChange(getCurSelectedImageCount());
    }
}

void ImageList::mouseMoveEvent(QMouseEvent *event)
{

}
void ImageList::contextMenuEvent(QContextMenuEvent *event)
{
    if(imageList->size() == 0) {
        rotate_s->setEnabled(false);
        rotate_n->setEnabled(false);
        first_page->setEnabled(false);
        last_page->setEnabled(false);
        close->setEnabled(false);
    }else if(imageList->size() == 1){
        rotate_s->setEnabled(true);
        rotate_n->setEnabled(true);
        first_page->setEnabled(true);
        last_page->setEnabled(true);
        close->setEnabled(true);
    }else{
        rotate_s->setEnabled(true);
        rotate_n->setEnabled(true);
        first_page->setEnabled(true);
        last_page->setEnabled(true);
        close->setEnabled(true);
    }
    merge->setEnabled(getCurSelectedImageCount()>1);
    if(currentRow() == 0) first_page->setEnabled(false);
    if(currentRow() == count()-1) last_page->setEnabled(false);
    m_menu.exec(event->globalPos());
}

void ImageList::initRightBtnMenu()
{
    rotate_s = new QAction(tr("顺时针90° "),this);
    rotate_n = new QAction(tr("逆时针旋转90° "),this);
    first_page = new QAction(tr("第一页 "),this);
    last_page = new QAction(tr("最后一页 "),this);
    delSelect = new QAction(tr("删除 "),this);
    merge = new QAction(tr("合并为多页文件 "),this);
    save_as = new QAction(tr("另存为... "),this);
    close = new QAction(tr("关闭 "),this);
    m_menu.addAction(rotate_s);
    m_menu.addAction(rotate_n);
    m_menu.addAction(first_page);
    m_menu.addAction(last_page);
//    m_menu.addAction(delSelect);
    m_menu.addAction(merge);
//    m_menu.addAction(save_as);
    m_menu.addAction(close);
//    connect(rotate_s,&QAction::triggered,this,&ImageList::rotateSSlot);
//    connect(rotate_n,&QAction::triggered,this,&ImageList::rotateNSlot);
//    connect(first_page,&QAction::triggered,this,&ImageList::firstPageSlot);
//    connect(last_page,&QAction::triggered,this,&ImageList::lastPageSlot);
//    connect(delSelect,&QAction::triggered,this,&ImageList::delSelectSlot);
//    connect(merge,&QAction::triggered,this,&ImageList::mergeSlot);
}

void ImageList::deleteItem(QListWidgetItem *item)
{
    for(int i = 0; i < imageList->size(); i++){
        if(imageList->at(i).image_name == item->toolTip()){
            imageList->removeAt(i);
            this->removeItemWidget(item);
            delete item;
            return;
        }
    }
}

void ImageList::deleteFunction(QList<QListWidgetItem*> items)
{
    if (items.count() > 0)
    {
        if (QMessageBox::Yes == QMessageBox::question(nullptr, tr("关闭图像 "),
            tr("关闭%1张图像 ").arg(QString::number(items.count())), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
        {
            for(int j = 0; j < items.size(); j++)
            {
                deleteItem(items[j]);
                items.removeAt(j);
                j--;
            }
        }
    }
    if(count() > 0){
        currentItem()->setSelected(true);
        emit itemClicked(currentItem());
    }else
        emit itemClicked(nullptr);
}

void ImageList::imageRorate(QListWidgetItem *item,int angle)
{
    QImage img;
    int curItem;
    for(int i = 0;i < imageList->size(); i++)
    {
        if(imageList->at(i).image_name == item->toolTip())
        {
            img = imageList->at(i).m_image;
            curItem = i;
            break;
        }
    }
    QMatrix matrix;
    matrix.rotate(angle);
    img = img.transformed(matrix,Qt::SmoothTransformation);
    (*imageList)[curItem].m_image = img;
    (*imageList)[curItem].image_size = QString::number(img.width()) + " * " + QString::number(img.height());
    item->setIcon(iconFromImage(img));
}

void ImageList::saveImageToFile(QImage img, QString filename)
{
    img.save(filename);
}

void ImageList::saveAllImageList()
{
    for(int i = 0; i < imageList->size(); i++)
    {
        QImage image = imageList->at(i).m_image;
        image.save(imageList->at(i).image_name);
    }
}

QStringList ImageList::getSelectedImages()
{
    QList<QListWidgetItem*> items = this->selectedItems();
    QStringList imgPathList;
    if(items.size() > 0)
    {
        for(int i = 0; i < imageList->size(); i++)
        {
            for(int j = 0; j < items.size(); j++)
            {
                if(items.at(j)->toolTip() == imageList->at(i).image_name)
                {
                    imgPathList.push_front(items.at(j)->toolTip());
                    items.removeAt(j);
                    i = 0;
                    break;
                }
            }
        }
    }
    return imgPathList;
}

QList<QSize> ImageList::getCurSelectedImagesSize()
{
    QList<QSize> imagesSize;
    QStringList selectedImages(getSelectedImages());
    for(int i = 0; i < selectedImages.size(); i++)
    {
        QImage image(selectedImages.at(i));
        imagesSize.append(QSize(image.width(),image.height()));
    }
    return imagesSize;
}

QList<int> ImageList::getCurSelectedImageDPI()
{
    QList<int> imagesDPI;
    QStringList selectedImages(getSelectedImages());
    for(int i = 0; i < selectedImages.size(); i++)
    {
        QImage image(selectedImages.at(i));
        int dpi = qRound(double(image.dotsPerMeterX()*0.0254));
        imagesDPI.append(dpi);
    }
    return imagesDPI;
}

int ImageList::getCurSelectedImageCount()
{
    QList<QListWidgetItem*> items = this->selectedItems();
    selectedCount = items.count();
    return selectedCount;
}

void ImageList::deleteItem(int index)
{
    QList<QListWidgetItem*> items;
    items.append(this->item(index));
    deleteFunction(items);
}

void ImageList::deleteItems(QList<int> indexs)
{
    QList<QListWidgetItem*> items;
    for(int i=0;i<indexs.size();i++)
    {
        items.append(this->item(indexs.at(i)));
    }
    //deleteFunction(items) reuse: delete comfirm, close dialog directly
	if (items.count() > 0)
    {
        for(int j = 0; j < items.size(); j++)
        {
            deleteItem(items[j]);
            items.removeAt(j);
            j--;
        }
    }
    if(count() > 0){
        currentItem()->setSelected(true);
        emit itemClicked(currentItem());
    }else
        emit itemClicked(nullptr);
}

void ImageList::deleteSelectedItems()
{
    delSelectSlot();
}

void ImageList::deleteAllItems()
{
    delAllSlot();
}

void ImageList::cancelCurSelItemsState()
{
    QList<QListWidgetItem*> items = this->selectedItems();
    for(int i=0;i<items.size();i++)
    {
        items[i]->setSelected(false);
    }
}

void ImageList::keyPressEvent(QKeyEvent *event)
{
    if ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_A))
    {
        QListWidget::keyPressEvent(event);
        QList<QListWidgetItem*> items = this->selectedItems();
        selectedCount = items.size();
        emit selectedItemCountChange(selectedCount);
    }
}
