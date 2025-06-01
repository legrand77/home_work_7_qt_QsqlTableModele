#include "dialog.h"
#include "database.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //Ресайзим вектор значений, по количеству полей необходимых для
    //подключения к БД
    data.resize(NUM_DATA_FOR_CONNECT_TO_DB);
    ui->lineEdit->setText("981757-ca08998.tmweb.ru");
    ui->lineEdit_2->setText("netology_cpp");
    ui->lineEdit_3->setText("netology_usr_cpp");
    ui->lineEdit_4->setText("CppNeto3");
    ui->spinBox->setValue(5432);




}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_buttonBox_accepted()
{
    //Добавляем данные в контейнер и передаем в главное окно
    data[hostName] = ui->lineEdit->text();
    data[dbName] = ui->lineEdit_2->text();
    data[login] = ui->lineEdit_3->text();
    data[pass] = ui->lineEdit_4->text();
    data[port] = ui->spinBox->text();

    emit sig_sendData(data);

}

