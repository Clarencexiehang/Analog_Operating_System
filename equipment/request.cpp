#include "request.h"
#include "ui_request.h"

request::request(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::request)
{
    ui->setupUi(this);
}

request::~request()
{
    delete ui;
}

void request::on_requestButton_clicked()
{
    if (ui->blueEdit->text().isEmpty()||ui->keyEdit->text().isEmpty()||ui->mouseEdit->text().isEmpty()||ui->printEdit->text().isEmpty()){
        QMessageBox::warning(this,"警告","请求资源不能为空！");
        request::close();
}
    requestId = ui->idEdit->text().toInt();
    requestBlue = ui->blueEdit->text().toInt();
    requestKey = ui->keyEdit->text().toInt();
    requestMouse = ui->mouseEdit->text().toInt();
    requestPrint = ui->printEdit->text().toInt();
    requestTime = ui->timeEdit->text().toInt();
    int requestData[5];
    requestData[0] = requestBlue;
    requestData[1] = requestKey;
    requestData[2] = requestMouse;
    requestData[3] = requestPrint;
    emit sendToDequip(requestData, requestId, requestTime);
    request::close();
}
