#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //Исходное состояние виджетов
    ui->setupUi(this);
    ui->lb_statusConnect->setStyleSheet("color:red");
    ui->pb_request->setEnabled(false);

    /*
     * Выделим память под необходимые объекты. Все они наследники
     * QObject, поэтому воспользуемся иерархией.
    */

    //dataDb = new DbData(this);
    dialog = new Dialog(this);
    dataBase = new DataBase(this);
    msg = new QMessageBox(this);

    //Установим размер вектора данных для подключения к БД
    dataForConnect.resize(NUM_DATA_FOR_CONNECT_TO_DB);

    /*
     * Добавим БД используя стандартный драйвер PSQL и зададим имя.
    */
    dataBase->AddDataBase(POSTGRE_DRIVER, DB_NAME);

    /*
     * Устанавливаем данные для подключениея к БД.
     * Поскольку метод небольшой используем лямбда-функцию.
     */
    //connect(dataDb, &DbData::sig_sendData, this, [&](QVector<QString> receivData){
        //dataForConnect = receivData;
        //dataDb->close();
        //dialog->close();
    //});

    connect(dialog, &Dialog::sig_sendData, this, [&](QVector<QString> receivData){
        dataForConnect = receivData;
        //dataDb->close();
        //dialog->close();
    });

    /*
     * Соединяем сигнал, который передает ответ от БД с методом, который отображает ответ в ПИ
     */
    connect(dataBase, &::DataBase::sig_SendQueryFromDB, this, &MainWindow::ScreenQueryFromDB);
    connect(dataBase, &::DataBase::sig_SendTableFromDB, this, &MainWindow::ScreenTableFromDB);
    /*
     *  Сигнал для подключения к БД
     */
    connect(dataBase, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * @brief Слот отображает форму для ввода данных подключения к БД
 */
void MainWindow::on_act_addData_triggered()
{
    //Отобразим диалоговое окно. Какой метод нужно использовать?
    //dataDb->setGeometry(500,500,400,200);
    //dataDb->show();
    //dialog->setGeometry(1000,500,400,200);
    dialog->show();
}

/*!
 * @brief Слот выполняет подключение к БД. И отображает ошибки.
 */

void MainWindow::on_act_connect_triggered()
{
    /*
     * Обработчик кнопки у нас должен подключаться и отключаться от БД.
     * Можно привязаться к надписи лейбла статуса. Если он равен
     * "Отключено" мы осуществляем подключение, если "Подключено" то
     * отключаемся
    */

    if(ui->lb_statusConnect->text() == "Отключено"){

        ui->lb_statusConnect->setText("Подключение");
        ui->lb_statusConnect->setStyleSheet("color : black");


        auto conn = [&]{dataBase->ConnectToDataBase(dataForConnect);};
        QtConcurrent::run(conn);

    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        ui->lb_statusConnect->setText("Отключено");
        ui->act_connect->setText("Подключиться");
        ui->lb_statusConnect->setStyleSheet("color:red");
        ui->pb_request->setEnabled(false);
    }

}

/*!
 * \brief Обработчик кнопки "Получить"
 */
void MainWindow::on_pb_request_clicked()
{
    QString category;
    category = ui->cb_category->currentText();
    ///Тут должен быть код ДЗ
    if (category == "Комедия"){
        request = "SELECT title, description FROM film f JOIN film_category "
                  "fc on f.film_id = fc.film_id JOIN category c"
                  " on c.category_id = fc.category_id WHERE c.name = 'Comedy' ";
    }
    else if (category == "Ужасы"){
        request = "SELECT title, description FROM film f JOIN film_category "
                  "fc on f.film_id = fc.film_id JOIN category c"
                  " on c.category_id = fc.category_id WHERE c.name = 'Horror' ";
    }
    else
    {
        request = "QsglTableModel";
    }

    auto req = [&]{dataBase->RequestToDB(request);};
    QtConcurrent::run(req);


}

/*!
 * \brief Слот отображает значение в QTableWidget
 * \param widget
 * \param typeRequest
 */
void MainWindow::ScreenQueryFromDB(QSqlQueryModel *model)
{
    ui->tb_result->showColumn(0);
    ui->tb_result->setModel(model);
    ///Тут должен быть код ДЗ
}

void MainWindow::ScreenTableFromDB(QSqlTableModel *model)
{
    ui->tb_result->setModel(model);
    ui->tb_result->hideColumn(0);
    for (int i=3;i<14;i++){
    ui->tb_result->hideColumn(i);
    }
    ///Тут должен быть код ДЗ QTableView::hideColumn
}
/*!
 * \brief Метод изменяет стотояние формы в зависимости от статуса подключения к БД
 * \param status
 */
void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if(status){
        ui->act_connect->setText("Отключиться");
        ui->lb_statusConnect->setText("Подключено к БД");
        ui->lb_statusConnect->setStyleSheet("color:green");
        ui->pb_request->setEnabled(true);
    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        msg->setIcon(QMessageBox::Critical);
        msg->setText(dataBase->GetLastError().text());
        ui->lb_statusConnect->setText("Отключено");
        ui->lb_statusConnect->setStyleSheet("color:red");
        msg->exec();
    }

}

void MainWindow::on_pb_clear_clicked()
{
    ui->tb_result->setModel(NULL);
}

