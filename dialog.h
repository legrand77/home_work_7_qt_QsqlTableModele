#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

signals:
    void sig_sendData(QVector<QString> dbData);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Dialog *ui;
    QVector<QString> data;

};

#endif // DIALOG_H
