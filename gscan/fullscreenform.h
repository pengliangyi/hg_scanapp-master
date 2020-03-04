#ifndef FULLSCREENFORM_H
#define FULLSCREENFORM_H

#include <QDialog>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include "mygraphicsscene.h"

namespace Ui {
class fullScreenForm;
}

class MainWindow;
class fullScreenForm : public QDialog
{
    Q_OBJECT

public:
    explicit fullScreenForm(QWidget *parent = nullptr);
    ~fullScreenForm();

protected:
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent* event);

private slots:
    void on_pbtn_prePage_clicked();
    void on_pbtn_nextPage_clicked();
    void hideBtn();//���ؼ�ʱtimeout��Ӧ�ۣ�����������ť
    void btnFade();//������ʱtimeout��Ӧ�ۣ��𲽵�����ť
    void subWinMouseMoved();//��ӦgraphicsScene����ƶ��¼�
private:
    Ui::fullScreenForm *ui;
    MainWindow* mainW;//MainWindowָ��
    QImage srcImg;//��Դͼ�񸱱�
    MyGraphicsScene *src_pic;//ȫ������
    int index;//��ǰͼ����MainWindow�б��е����
    QStringList namesList;//MainWindow�б�������ͼ����ļ�·��
    QTimer timer_hide,timer_fade;//��ť���ؼ�ʱ������ť������ʱ��   
	QGraphicsOpacityEffect *effect1,*effect2;//��ť��Ӧ�ĵ���Ч��   
	void setBtnVisible(int index);//���ݵ�ǰͼ�����ˢ�°�ť�ɼ����
    void setBtnOpacity(qreal opacity);//���ð�ť͸����   
	void freshImg();//ˢ�»���ͼ��
    void initWindow();//��ʼ��ȫ������
};

#endif // FULLSCREENFORM_H
