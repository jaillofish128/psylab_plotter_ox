#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QClipboard>
#include "libpsyclass.h"
#include "darw.hpp"
#include "odi.hpp"

extern std::string prefix;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_AZ_clicked();

    void on_pushButton_6_clicked();

    void on_radioButton_Remote_clicked();

    void on_radioButton_Local_clicked();

    void on_pushButton_DL_clicked();

    void on_actionExit_triggered();

    void on_actionReset_plot_triggered();

    void on_actionAbout_triggered();

    void on_pushButton_F_clicked();

    void on_pushButton_O_clicked();

    void on_action120_126_32_35_triggered();

    void on_actionxenon_ym_edu_tw_triggered();

    void on_action_white_ox_toggled(bool arg1);

    void on_checkBox_ODI_Dots_toggled(bool checked);

    void on_checkBox_ODI_List_toggled(bool checked);

    void on_pushButton_clipper_released();

    void on_action_zh_tw_triggered();

    void on_actionEnglish_en_US_triggered();

private:
    Ui::MainWindow *ui;

    std::string filename, tmpString, numStringBuffer;
    std::string sHH1, sHH2, sMM1, sMM2;

    TriTable DB_source;
    PSGTable DB_PSG;

    std::vector <unsigned> VC_O, VC_O_1step;
    std::map <int, double> DB_M, DB_H, DB_O, DB_O1, DB_O2, DB_PSGM;
    std::map <unsigned, unsigned> WKMap;
    std::list <odi> WKList;
    odList OD90;
    std::queue<unsigned> ODIHeads;

    bool toDrawACurve(std::map<int, double> &DrawBase, int slot, int density, int &density_pre);

    //Strings related to downloading function
    QString KnYserver = "http://120.126.32.35/spo2/";
    const QString id = "xid=";
    QString URL_G, URL_D;
    QString serial, date;
    QString filename_templateQ;
    unsigned serverLatency = 3000;

    void OrderFile(const QString& serialQ, const QString& dateQ, const bool serverChanged);
    bool FetchFile(const QString &serialQ, const QString &dateQ, int &ErrorType, unsigned retries, const bool serverChanged);
    bool server_changed = true;

    //Static strings that are commonly used across different functions
    const std::string underline = "_";
    const std::string blankSpace= " ";
    const std::string dash      = "-";

    //Variable related to IO
    std::string prefix="~/";
    std::string postfix;
    QString ext = "??.png";
    QString PNG;

    //Various flags, values and strings related to data anylisis
//    std::string lowestOXtxt, OD90timEtxt, OD85timEtxt, ODItxt, AnylTxt;
    QString AnylTxtQ, lowestOXtxtQ, OD90timEtxtQ, OD85timEtxtQ, ODItxtQ;

    int densityM_pre, densityH_pre, densityO_pre, densityODI_pre;
    bool newContact = false;
    bool OD_Changed = true;
    bool WK_changed = true;
    bool GreenLight = true;
	int MaxTime=0;
    int MinTime=0;
    double OD_counter=0;
    double OD_Duration_AVG=0;
    double ODI=0;
    double totalTime = 0;
    double sleepTime = 0;
    unsigned OD90Time= 0;
    unsigned OD85Time= 0;
    unsigned lowestOX=255;
    QCPTextElement *title;
    QCPTextElement *AnylTitle;
    QCPItemText *ODIText0, *ODIText1, *MVText, *OXText, *HRText;
    std::string ODI_result, ODI_result1;

    QClipboard *clipper = QApplication::clipboard();
    std::string clipperString;
    QString clipperQString;

    QFont FontPlaceholder;
    QPen tmpPen;
};

void SetPath(std::string&, const QString &, std::string&, std::string&);

#endif // MAINWINDOW_H
