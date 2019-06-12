#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "about.h"
#include "histowindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->canovas->addGraph(ui->canovas->xAxis, ui->canovas->yAxis2);              //Set movement chart   (Graph 0)
//    ui->canovas->graph(0)->setName("血氧");
    ui->canovas->addGraph(ui->canovas->xAxis, ui->canovas->yAxis);             //Set heart rate chart (Graph 1)
//    ui->canovas->graph(1)->setName("活動量");
    ui->canovas->addGraph(ui->canovas->xAxis, ui->canovas->yAxis);
//    ui->canovas->graph(2)->setName("活動量L1");
    ui->canovas->addGraph(ui->canovas->xAxis, ui->canovas->yAxis);
//    ui->canovas->graph(3)->setName("活動量L2");
    ui->canovas->addGraph(ui->canovas->xAxis, ui->canovas->yAxis2);
    ui->canovas->graph(4)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 10));
//    ui->canovas->graph(4)->setName("Desaturation");
    ui->canovas_3->addGraph(ui->canovas_3->xAxis, ui->canovas_3->yAxis);              //Set blood oxygen chart (Graph 2)
//    ui->canovas_3->graph(0)->setName("心率");
    title = new QCPTextElement(ui->canovas);
    AnylTitle = new QCPTextElement(ui->canovas);
    ui->canovas->plotLayout()->insertRow(0);
    ui->canovas->plotLayout()->insertRow(0);
    ui->canovas->plotLayout()->addElement(0, 0, title);
    ui->canovas->plotLayout()->addElement(1, 0, AnylTitle);

    ui->canovas->setBackground(QBrush(QColor(240,240,240)));
    ui->canovas_3->setBackground(QBrush(QColor(240,240,240)));

    QSharedPointer<QCPAxisTickerTime>TimeTicker(new QCPAxisTickerTime);
    TimeTicker->setTimeFormat("Day %d\n%h:%m");
    TimeTicker->setTickCount(10);
    QSharedPointer<QCPAxisTickerLog>Log10Ticker(new QCPAxisTickerLog);

    ui->canovas->xAxis->setRange(3600*44, 3600*56);
    ui->canovas->xAxis->setLabel("Time(hh:mm)");
    ui->canovas->xAxis->setTicker(TimeTicker);

    //Set up y axis on the left
    ui->canovas->yAxis->setRange(0.4, 45);
    ui->canovas->yAxis->grid()->setSubGridVisible(false);
    ui->canovas->yAxis->setScaleType(QCPAxis::stLogarithmic);
    ui->canovas->yAxis->setTicker(Log10Ticker);
    ui->canovas->yAxis->setLabel("MV");
    //Set up y axis on the right
    ui->canovas->yAxis2->setRange(61, 101);
    ui->canovas->yAxis2->grid()->setSubGridVisible(false);
    ui->canovas->yAxis2->setLabel("OX(%)");
    ui->canovas->yAxis2->setVisible(true);


    ui->canovas_3->xAxis->setRange(3600*44, 3600*56);
    ui->canovas_3->xAxis->setTicker(TimeTicker);
    //Set up y axis on the left
    ui->canovas_3->yAxis->setLabel("Heart rate");
    ui->canovas_3->yAxis->setRange(30, 90);
    ui->canovas_3->yAxis->setVisible(true);

    DrawBoundOX(ui->canovas, 3, 20);
//    QPen tmpPen;
    tmpPen.setWidth(1);

    tmpPen.setColor(QColor(8, 0, 89));
    ui->canovas->graph(1)->setPen(tmpPen);                     //Paint MV curve blue
    tmpPen.setColor(QColor(209,52,46));
    ui->canovas_3->graph(0)->setPen(tmpPen);                   //Paint HR curve red
    tmpPen.setColor(QColor(0, 184, 30));
    ui->canovas->graph(0)->setPen(tmpPen);                     //Paint OX Curve green

    tmpPen.setColor(Qt::black);
    ODIText0 = new QCPItemText(ui->canovas);
    ODIText0->position->setType(QCPItemPosition::ptAxisRectRatio);
    ODIText0->setPositionAlignment(Qt::AlignRight|Qt::AlignBottom);
    ODIText0->position->setCoords(0.99, 0.99);
    ODIText0->setTextAlignment(Qt::AlignLeft);
    ODIText0->setPadding(QMargins(1,0,0,0));
    ODIText0->setBrush(QBrush(Qt::white));
    ODIText0->setPen(tmpPen);

    ODIText1 = new QCPItemText(ui->canovas);
    ODIText1->position->setType(QCPItemPosition::ptAxisRectRatio);
    ODIText1->setPositionAlignment(Qt::AlignRight|Qt::AlignBottom);
    ODIText1->position->setCoords(0.85, 0.99);
    ODIText1->setTextAlignment(Qt::AlignLeft);
    ODIText1->setPadding(QMargins(1,0,0,0));
    ODIText1->setBrush(QBrush(Qt::white));
    ODIText1->setPen(tmpPen);
    ODIText1->setVisible(false);


    MVText = new QCPItemText(ui->canovas);
    MVText->position->setType(QCPItemPosition::ptAxisRectRatio);
    MVText->setPositionAlignment(Qt::AlignLeft|Qt::AlignBottom);
    MVText->position->setCoords(0.01, 0.99);
    MVText->setTextAlignment(Qt::AlignLeft);
    MVText->setColor(QColor(8,0,127));

    OXText = new QCPItemText(ui->canovas);
    OXText->position->setType(QCPItemPosition::ptAxisRectRatio);
    OXText->setPositionAlignment(Qt::AlignLeft|Qt::AlignTop);
    OXText->position->setCoords(0.01, 0.01);
    OXText->setTextAlignment(Qt::AlignLeft);
    OXText->setColor(QColor(0, 92, 30));

    HRText = new QCPItemText(ui->canovas_3);
    HRText->position->setType(QCPItemPosition::ptAxisRectRatio);
    HRText->setPositionAlignment(Qt::AlignLeft|Qt::AlignTop);
    HRText->position->setCoords(0.01, 0.01);
    HRText->setTextAlignment(Qt::AlignLeft);
    HRText->setColor(QColor(104, 26, 23));

    //Reserve memory spaces for various vectors to prevent performance loss due to frequent memory allocation
    URL_G.reserve(80);
    URL_D.reserve(80);
    filename_templateQ.reserve(23);
    postfix.reserve(50);
    tmpString.reserve(200);
    PNG.reserve(200);
    VC_O.reserve(13);
    VC_O_1step.reserve(55);
//    AnylTxt.reserve(90);
    AnylTxtQ.reserve(90);
    clipperString.reserve(1000);
    clipperQString.reserve(1000);
    //Assuming that there will be nor more than 12hrs worth of data (12*60*60=43200 (seconds))

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_AZ_clicked()
{
    ui->pushButton_AZ->setCheckable(0);
    if (newContact){
        DB_source.Reset();
//        DB_PSG.Reset();
//        DB_PSGM.clear();
        DB_O1.clear();
        VC_O.clear();
        VC_O_1step.clear();
        clipper->setText("");
        tmpString = ui->lineEdit_d->text().toStdString();
        GreenLight = DB_source.ReadFile(filename, tmpString, ui->radioButton_Remote->isChecked());
        DB_source.removeSingleSpike_OX(ui->spinBox_Filter->value());
        DB_source.removeOrphanedSpike_OX(ui->spinBox_Filter->value());
        if (ui->action_white_ox->isChecked())
        {DB_source.removeWhiteNoise_OX(70, 60);}
        //        DB_source.CalcOxDistribute(63, 3, VC_O);
        DB_source.CalcOxDistributeZwei(63, 50, 102, 3, VC_O, 1, VC_O_1step);
//        DB_source.extract2MAP_TimeAware(DB_O1, 0, 1);
        totalTime = DB_source.Size();
        densityM_pre  =-1;
        densityH_pre  =-1;
        densityO_pre  =-1;
        densityODI_pre=-1;        
    }
    FontPlaceholder.setPointSize(8);


    if(!GreenLight){
        QMessageBox::critical(this, tr("Error"), tr("File not found. Please sellect/download again"));
    }
    else{
        ODI_result.clear();

        MinTime = 86400 + QTime(0,0,0).secsTo(ui->timeEdit_L_2->time());
        if (1==ui->comboBox_L->currentIndex()){
            MinTime+=86400;
        }
        MaxTime = 86400 + QTime(0,0,0).secsTo(ui->timeEdit_U_2->time());
        if (1==ui->comboBox_U->currentIndex()){                                           //If upper limit's date slot is on the second index (Day2)
            MaxTime+=86400;
        }

        ui->canovas->xAxis->setRangeUpper(MaxTime);
        ui->canovas->xAxis->setRangeLower(MinTime);
        ui->canovas_3->xAxis->setRangeUpper(MaxTime);
        ui->canovas_3->xAxis->setRangeLower(MinTime);
        ui->canovas_3->yAxis->setRangeUpper(ui->spinBox_U->value());
        ui->canovas_3->yAxis->setRangeLower(ui->spinBox_L->value());

        WK_changed = toDrawACurve(DB_M, 2, ui->spinBox_m->value(), densityM_pre);        //Mark that the Wake List has changed
        if (WK_changed){
            WKList.clear();
            FilterWake(DB_M, WKList);                                                     //Find out awake sections
            sleepTime = totalTime-CalcListTime(WKList);
            DB_source.extract2MAP_TimeAware(DB_O1, 0, 1);
            RemoveWKMAP(DB_O1, WKMap);
            sleepTime = DB_O1.size();
        }

        toDrawACurve(DB_H, 1, ui->spinBox_H->value(), densityH_pre);

        toDrawACurve(DB_O, 0, ui->spinBox_O->value(), densityO_pre);

        //Try to idtify desaturation events

        if (ui->spinBox_OD->value()!=densityODI_pre){
            DB_O2.clear();
            densityODI_pre = ui->spinBox_OD->value();
            DB_source.extract2MAP_wLowestValue(DB_O2, 0, densityODI_pre, lowestOX);
            OD90.Update(DB_O2, 90);                                                       //Update the Desaturation List
            OD_Changed = true;                                                               //Mark that the Desaturation List has changed
        }

        if (OD_Changed|WK_changed){
            OD90Time = 0;
            OD90.RemoveWake(WKList);
            OD_counter = OD90.Size();
            ODI = OD_counter/(sleepTime/3600);

            OD90Time = OD90.CalcOxTimeBelow(90, densityODI_pre, DB_O2);
            OD85Time = OD90.CalcOxTimeBelow(85, densityODI_pre, DB_O2);

            lowestOXtxtQ = QString::number(lowestOX);
            OD90timEtxtQ = QString::number(OD90Time);
            OD85timEtxtQ = QString::number(OD85Time);
            ODItxtQ      = QString::number(ODI,'g', 3);

            AnylTxtQ="OX<90:  sec     |     OX<85:  sec     |     Lowest OX:      |     OD events/hr: ";
            AnylTxtQ.insert(7, OD90timEtxtQ);
            AnylTxtQ.insert(29+OD90timEtxtQ.length(), OD85timEtxtQ);
            AnylTxtQ.insert(55+OD90timEtxtQ.length()+OD85timEtxtQ.length(), lowestOXtxtQ);
            AnylTxtQ.append(ODItxtQ);


            AnylTitle->setText(AnylTxtQ);

            OD_Changed = false;
            WK_changed = false;
        }

        OD90.HeadCount(ODIHeads);

        DrawDots(ODIHeads, 90, ui->canovas, 4, QColor(Qt::black));                         //Mark ODIs

        if (OD90.Size()>20){
            OD90.OutputAsStringZwei(ODI_result1, ODI_result, MinTime, MaxTime, densityODI_pre);
            ODIText0->setText(QString::fromStdString(ODI_result));
            ODIText1->setText(QString::fromStdString(ODI_result1));

        }
        else{
            OD90.OutputAsString(ODI_result, MinTime, MaxTime, densityODI_pre);
            ODIText0->setText(QString::fromStdString(ODI_result));
        }

        ODIText0->setVisible(ui->checkBox_ODI_List->isChecked());                            //Show ODI results if ODI box is checked
        ODIText1->setVisible((ui->checkBox_ODI_List->isChecked())&&(OD90.Size()>20));
        ui->canovas->graph(4)->setVisible(ui->checkBox_ODI_Dots->isChecked());

        if (newContact){
            postfix.erase(19);
            title->setText(QString::fromStdString(postfix));

            ui->canovas->replot();
            ui->canovas_3->replot();

            HistoWindow Histo1;
            Histo1.setModal(0);
            Histo1.setData(VC_O, postfix);
            Histo1.draw();
            Histo1.exec();}

    }
    ui->canovas->replot();
    ui->canovas_3->replot();
    newContact = false;
    ui->pushButton_AZ->setCheckable(true);

}

void MainWindow::on_pushButton_6_clicked()
{
    FontPlaceholder.setPointSize(12);
    ODIText0->setFont(FontPlaceholder);
    ODIText1->setFont(FontPlaceholder);
    ext = "_MR.png";
    int hhL = ui->timeEdit_L_2->time().hour();
    int mmL = ui->timeEdit_L_2->time().minute();
    int hhU = ui->timeEdit_U_2->time().hour();
    int mmU = ui->timeEdit_U_2->time().minute();
    size_t stringLenth;

    tmpString.assign(filename, 0, filename.find_last_of("."));
    stringLenth = tmpString.length();
    tmpString+="---";
    PNG = QString::fromStdString(tmpString);          //Convert half-made PNG filename string to QString
    
    Int2String(hhL, sHH1);                            //Convert HH and MM to string
    Int2String(mmL, sMM1);                            //The strings are stored and reserved to avoid constant mem allocation
    Int2String(hhU, sHH2);
    Int2String(mmU, sMM2);
    PNG.insert(stringLenth+1, sHH1.c_str());          //Insert the strings into PNG filename
    PNG.insert(stringLenth+3, sMM1.c_str());
    PNG.insert(stringLenth+6, sHH2.c_str());
    PNG.insert(stringLenth+8, sMM2.c_str());
    
    PNG+=ui->spinBox_L->text();                  //Continue adding QStrings on the interface to PNG
    PNG+="-";
    PNG+=ui->spinBox_U->text();
    PNG+=ext;                                    //PNG is finished

    ui->canovas->savePng(PNG, 1280, 720, 1, -1);
    QMessageBox::information(this, tr("Graph saved"), tr("MV/OX graph have been saved in working directory"));
}

void MainWindow::on_pushButton_O_clicked()
{
    ext = "_O.png";
    int hhL = ui->timeEdit_L_2->time().hour();
    int mmL = ui->timeEdit_L_2->time().minute();
    int hhU = ui->timeEdit_U_2->time().hour();
    int mmU = ui->timeEdit_U_2->time().minute();
    size_t stringLenth;

    tmpString.assign(filename, 0, filename.find_last_of("."));
    stringLenth = tmpString.length();
    tmpString+="---";
    PNG = QString::fromStdString(tmpString);          //Convert half-made PNG filename string to QString
    
    Int2String(hhL, sHH1);                            //Convert HH and MM to string
    Int2String(mmL, sMM1);                            //The strings are stored and reserved to avoid constant mem allocation
    Int2String(hhU, sHH2);
    Int2String(mmU, sMM2);
    PNG.insert(stringLenth+1, sHH1.c_str());          //Insert the strings into PNG filename
    PNG.insert(stringLenth+3, sMM1.c_str());
    PNG.insert(stringLenth+6, sHH2.c_str());
    PNG.insert(stringLenth+8, sMM2.c_str());    

    PNG+=ui->spinBox_L->text();                  //Continue adding QStrings on the interface to PNG
    PNG+="-";
    PNG+=ui->spinBox_U->text();
    PNG+=ext;                                    //PNG is finished

    ui->canovas_3->savePng(PNG, 1214, 240, 1, -1);
    QMessageBox::information(this, tr("Graph is saved"), tr("HR graph have been saved in working directory"));
}

void MainWindow::on_radioButton_Remote_clicked()
{
    ui->radioButton_Local->setChecked(false);
    ui->pushButton_F->setEnabled(false);

    ui->pushButton_DL->setEnabled(true);
}

void MainWindow::on_radioButton_Local_clicked()
{
    ui->radioButton_Remote->setChecked(false);
    ui->pushButton_F->setEnabled(true);

    ui->pushButton_DL->setEnabled(false);
}

void MainWindow::on_pushButton_DL_clicked()
{
    ui->pushButton_DL->setEnabled(false);
    ui->pushButton_DL->setText("Waiting for server…");
    ui->pushButton_AZ->setEnabled(false);
    ui->action120_126_32_35->setEnabled(false);
    ui->actionxenon_ym_edu_tw->setEnabled(false);
    newContact = true;

    serial = (ui->lineEdit_s->text());
    date   = (ui->lineEdit_d->text());
    QEventLoop holder;
    bool DownloadCompleted = 0;
    int ErrorNum = 0;

    if (serial.length()==8 && date.length()==10){
        //Order file of specific serial and date from server
        OrderFile(serial, date, server_changed);

        QTimer::singleShot(serverLatency, &holder, SLOT(quit()));
        holder.exec();

        ui->pushButton_DL->setText("Downloading…");
        DownloadCompleted = FetchFile(serial, date, ErrorNum, 5, server_changed);

        if (DownloadCompleted == false){
            switch(ErrorNum) {
            case 1:
                QMessageBox::critical(this, tr("Download failed"), tr("Connection interuppted"));
                break;
            case 2:                                                   //The connection kept getting interupted
                QMessageBox::critical(this, tr("Download failed"), tr("No valid record on the sever. Check your serial and date, or choose a different server"));
                break;
            default:
                QMessageBox::critical(this, tr("Download failed"), tr("Unknown error"));
                break;
            }
        }
        else
            {QMessageBox::information(this, tr("Download complete"), tr("Download complete"));}
        server_changed = false;

    }
    else{
        QMessageBox::critical(this, tr("Invalid argument"), tr("Invalid serial or date format."));
    }
     ui->pushButton_DL->setText("Download file");
     ui->pushButton_DL->setEnabled(true);
     ui->pushButton_AZ->setEnabled(true);
     ui->action120_126_32_35->setEnabled(true);
     ui->actionxenon_ym_edu_tw->setEnabled(true);
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionReset_plot_triggered()
{
    prefix="~/";
//    density=1;
    DB_source.Reset();
    title->setText(" ");
    toDrawACurve(DB_H, 1, 100000, densityH_pre);
    toDrawACurve(DB_O, 0, 100000, densityO_pre);
    toDrawACurve(DB_M, 2, 100000, densityM_pre);
    DrawDots(ODIHeads, 90, ui->canovas, 4, QColor(Qt::black));
    ODIText0->setText("");
    ODIText1->setText("");
    AnylTitle->setText("OX<90:  sec     |     OX<85:  sec     |     Lowest OX:      |     OD events/hr: ");
    newContact = true;
    ui->canovas->replot();
    ui->canovas_3->replot();
}

void MainWindow::on_actionAbout_triggered()
{
   About AboutWindow;
   AboutWindow.setModal(true);
   AboutWindow.exec();
}

void MainWindow::on_pushButton_F_clicked()
{
    QString filenameQ = QFileDialog::getOpenFileName(this,
                                                      tr("Open file..."),  QString::fromStdString(prefix), tr("Raw Data (*.txt)"));
    SetPath(filename, filenameQ, prefix, postfix);
    ui->labelF->setText(QString::fromStdString(postfix));
    newContact = true;
}

bool MainWindow::toDrawACurve(std::map<int, double> &DrawBase, int slot, int density, int &density_pre){
    if (density == density_pre)
    {return 0;}
    else{
         int LineNum;
         QCustomPlot *canova;
         QCPItemText *GraphTxT;

        switch (slot){
        case 0:                         //Extract & Draw OX
            LineNum = 0;
            canova = ui->canovas;
            GraphTxT = OXText;
            break;
        case 1:                         //Extract & Draw HR
            LineNum = 0;
            canova = ui->canovas_3;
            GraphTxT = HRText;
            break;
        case 2:                         //Extract & Draw MV
            LineNum = 1;
            canova = ui->canovas;
            GraphTxT = MVText;
            break;
        default:
            return 0;
        }
        density_pre = density;
        DrawBase.clear();
        DB_source.extract2MAP_TimeAware(DrawBase, slot, density);
        Int2String_alt(density_pre, numStringBuffer);
        GraphTxT->setText(QString::fromStdString(numStringBuffer));
        canova->graph(LineNum)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond, 3*(density_pre>=60)));
        DrawCurve_MAP(DrawBase, canova, LineNum);                      //Draw OX
        return 1;
    }
}

void MainWindow::OrderFile(const QString &serialQ, const QString &dateQ, const bool serverChanged = true){
    QNetworkAccessManager control;
    QNetworkReply *buffer;
    QEventLoop ribbon;

    //Compose url to make server generate txt files
    if (serverChanged == true){
        URL_G.clear();
        URL_G.append(KnYserver);
    }
    else{
        URL_G.remove(KnYserver.length(), (URL_G.length()-KnYserver.length()));
    }
    URL_G+= "downspo2.php?xid=&date=&type=download"; // [KnYserver]/downspo2.php?xid=         &date=       &type=download
    URL_G.insert(KnYserver.length()+17, serialQ);    // [KnYserver]/downspo2.php?xid=[serialQ]&date=       &type=download
    URL_G.insert(KnYserver.length()+31, dateQ);      // [KnYserver]/downspo2.php?xid=[serialQ]&date=[dateQ]&type=download

    QNetworkRequest Recon(URL_G);
    buffer = control.get(Recon);
    connect(buffer, SIGNAL(finished()), &ribbon, SLOT(quit()));
    ribbon.exec();
}


bool MainWindow::FetchFile(const QString &serialQ, const QString &dateQ, int &ErrorType, unsigned retries = 1, const bool serverChanged = true){
    QNetworkAccessManager control;
    QNetworkReply *buffer;
    QEventLoop ribbon;
    bool noNetError = 0;
    bool DataRecieved = 0;

    if (serverChanged == true){
        URL_D.clear();
        URL_D.append(KnYserver);
    }
    else{
        URL_D.remove(KnYserver.length(), (URL_D.length()-KnYserver.length()));
    }
    URL_D+= "dlfile/dlfile.php?file=_(spo2).txt";         // [KnYserver]/dlfile/dlfile.php?file=         _       (spo2).txt
    URL_D.insert(KnYserver.length()+23, serialQ);         // [KnYserver]/dlfile/dlfile.php?file=[serialQ]_       (spo2).txt
    URL_D.insert(KnYserver.length()+32, dateQ);           // [KnYserver]/dlfile/dlfile.php?file=[serialQ]_[dateQ](spo2).txt

    filename_templateQ.clear();
    filename_templateQ.append("_.txt");
    filename_templateQ.insert(0,serialQ);
    filename_templateQ.insert(9, dateQ);

    unsigned k = 0;
    while (DataRecieved==false && k < retries){
        QNetworkRequest fetch = QNetworkRequest(URL_D);
        buffer = control.get(fetch);
        connect(buffer, SIGNAL(finished()), &ribbon, SLOT(quit()));
        ribbon.exec();
        noNetError = (buffer->error()==QNetworkReply::NoError);
//        bufSize = buffer->size();
        DataRecieved = (buffer->size()>400);
        k++;
    }

    if (DataRecieved&&noNetError){
      QFile dzBlack(filename_templateQ);
      dzBlack.open(QIODevice::WriteOnly);
      dzBlack.write(buffer->readAll());
      SetPath(filename, filename_templateQ, prefix, postfix);
      ui->labelF->setText(QString::fromStdString(postfix));

      return 1;
    }
    else{
      if (!noNetError)
      {ErrorType = 1;}
      else
      {ErrorType = 2;}
      return 0;
    }
}

void MainWindow::on_action120_126_32_35_triggered()
{
    KnYserver = "http://xenon2.ym.edu.tw/spo2/";
    ui->action120_126_32_35->setChecked(true);
    ui->actionxenon_ym_edu_tw->setChecked(false);
//    serverLatency = 5000;
    server_changed = true;
}

void MainWindow::on_actionxenon_ym_edu_tw_triggered()
{
    KnYserver = "http://xenon.ym.edu.tw/spo2/";
    ui->actionxenon_ym_edu_tw->setChecked(true);
    ui->action120_126_32_35->setChecked(false);
//    serverLatency = 5000;
    server_changed = true;
}


void SetPath(std::string& filename, const QString &input, std::string& prefix, std::string& postfix){
    filename = input.toUtf8().constData();
    size_t dot = filename.find(".");
    prefix = filename.substr(0, dot-19);
    size_t dash= filename.find_last_of("/\\");
    postfix = filename.substr(dash+1);
}


void MainWindow::on_action_white_ox_toggled(bool arg1)
{
    newContact=true;
}


void MainWindow::on_checkBox_ODI_Dots_toggled(bool checked)
{
    ui->canovas->graph(4)->setVisible(checked);
    ui->canovas->replot();
}

void MainWindow::on_checkBox_ODI_List_toggled(bool checked)
{
    ODIText0->setVisible(checked);
    ODIText1->setVisible(checked&&(OD90.Size()>20));
    ui->canovas->replot();
}

void MainWindow::on_pushButton_clipper_released()
{
    if (GenOxDistrString(VC_O_1step, clipperString, totalTime, ui->spinBox_clipperLB->value(), ui->spinBox_clipperStep->value())){
        clipperQString = QString::fromStdString(clipperString);
        clipper->setText(clipperQString);
        QMessageBox::information(this, tr("String coppied"), tr("Oxygenation string has been coppied to your clippboard"));
    }
    else
    {QMessageBox::information(this, tr("String not coppied"), tr("No loaded data"));}
}
/*
void MainWindow::on_action_zh_tw_triggered()
{
    ui->actionEnglish_en_US->setChecked(false);
    QTranslator Trans;
    Trans.load("zh_TW.qm");
    ui->action_zh_tw->setChecked(true);
    qApp->installTranslator(&Trans);
    ui->retranslateUi(this);
}


void MainWindow::on_actionEnglish_en_US_triggered()
{
    ui->action_zh_tw->setChecked(false);
    QTranslator Trans;
    Trans.load("en-US.qm");
    ui->actionEnglish_en_US->setChecked(true);
    qApp->installTranslator(&Trans);
    ui->retranslateUi(this);
}
*/
