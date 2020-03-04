#include "SleepTimeDialog.h"
#include "ui_SleepTimeDialog.h"

SleepTimeDialog::SleepTimeDialog(QWidget *parent) :
    QDialog(parent),
//    m_scanner(scanner),
    ui(new Ui::SleepTimeDialog)
{
    ui->setupUi(this);
    //creat time value fits comboBoxIndex
	m_map.insert(0,MIN_5);
    m_map.insert(1,MIN_10);
    m_map.insert(2,MIN_20);
    m_map.insert(3,MIN_30);
    m_map.insert(4,HOUR_1);
    m_map.insert(5,HOUR_2);
    m_map.insert(6,HOUR_4);
    m_map.insert(7,NEVER);
    //read lastest saved param
//    int time_key = m_map.key(m_scanner->get_sleep_time());
    int time_key = 0;
    //check if time value is in m_map
	if(m_map.contains(time_key))
        ui->comboBox->setCurrentIndex(time_key);
    else
        ui->comboBox->setCurrentIndex(0);
}

SleepTimeDialog::~SleepTimeDialog()
{
    m_map.clear();
    delete ui;
}

void SleepTimeDialog::on_buttonBox_accepted()
{
//    m_scanner->set_sleep_time(m_map.value(ui->comboBox->currentIndex()));
}
