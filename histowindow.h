#ifndef HISTOWINDOW_H
#define HISTOWINDOW_H

#include <QDialog>
#include "darw.hpp"

namespace Ui {
class HistoWindow;
}

class HistoWindow : public QDialog
{
    Q_OBJECT

public:
    explicit HistoWindow(QWidget *parent = 0);

    bool setData(std::vector <unsigned> &input, const std::string &I);
    bool draw();
    ~HistoWindow();

private slots:
    void on_pushButton_H_clicked();

private:
    Ui::HistoWindow *ui;
    QVector <double> source = QVector<double>();
    QString hisTitle;
    QCPTextElement *title;
};

#endif // HISTOWINDOW_H
