#include "histowindow.h"
#include "ui_histowindow.h"

HistoWindow::HistoWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistoWindow)
{
    ui->setupUi(this);
    title = new QCPTextElement(ui->OxHisto);
    QSharedPointer<QCPAxisTickerFixed> FixedTicker(new QCPAxisTickerFixed);
    FixedTicker->setTickStep(3);
    ui->OxHisto->xAxis->setTicker(FixedTicker);
    ui->OxHisto->yAxis->setLabel("Time (seconds)");
    ui->OxHisto->xAxis->setLabel("Oxygenation(%)");
}
/*
bool HistoWindow::setData(std::vector<double> &input, QString I1, QString I2){
    for(unsigned k=0; k<input.size(); k++){
        source.push_back(input[k]);
    }
    DaTe = I1;
    Cereal =I2;
    return 1;
}
*/
bool HistoWindow::setData(std::vector <unsigned> &input, const std::string &I){
    for(unsigned k=0; k<input.size(); k++){
        source.push_back(input[k]);
    }
    hisTitle = QString::fromStdString(I);
    return 1;
}

bool HistoWindow::draw(){
    DrawHisto(source, ui->OxHisto, 63, 100, 3);
    title->setText(hisTitle);
//    title->setText(Cereal+" "+DaTe);
    return 1;
}

HistoWindow::~HistoWindow()
{
    delete ui;
}

void HistoWindow::on_pushButton_H_clicked()
{
    QString ext = "_OX_histo.png";
//    QString PNG = Cereal+"-"+DaTe+ext;
    QString PNG = hisTitle+ext;
    ui->OxHisto->savePng(PNG, 500, 500, 1, -1);
    QMessageBox::information(this, tr("Graph saved"), tr("OX histogram is saved"));
}
