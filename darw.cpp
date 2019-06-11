#include "darw.hpp"

using namespace std;

void DrawBound(QCustomPlot *canovas, int cell, int number){
    QVector <double> tmpX;
    QVector <double> tmpY;
    for (int k=0; k!=2; k++){
        tmpX.push_back(k*3600*24);
        tmpY.push_back(cell);
    }
    canovas->graph(number)->setData(tmpX, tmpY);
    //Set boundary color and style
    QPen bahamaBlueDot;
    bahamaBlueDot.setStyle(Qt::DotLine);
    bahamaBlueDot.setColor(QColor(49, 94, 132, 150));
    bahamaBlueDot.setWidth(2);
    canovas->graph(number)->setPen(bahamaBlueDot);
    canovas->graph(number)->setLineStyle(QCPGraph::lsLine);
    canovas->graph(number)->setBrush(QBrush(QColor((255*(number-2))/4, 141, 30, 20)));
    //Hide boundary untill movement chart is drawn
    canovas->graph(number)->setVisible(false);
}

void DrawBoundOX(QCustomPlot *canovas, int num1, int num2){
    QVector <double> tmpX;
    QVector <double> tmpY1;
    QVector <double> tmpY2;
    for (int k=1; k!=4; k++){
        tmpX.push_back(k*3600*24);
        tmpY1.push_back(num1);
        tmpY2.push_back(num2);
    }
    canovas->graph(2)->setData(tmpX, tmpY1);
    canovas->graph(3)->setData(tmpX, tmpY2);
    QPen bahamaBlueDot;
    bahamaBlueDot.setStyle(Qt::DotLine);
    bahamaBlueDot.setColor(QColor(49, 94, 132, 150));
    bahamaBlueDot.setWidth(2);
    canovas->graph(2)->setPen(bahamaBlueDot);
    canovas->graph(3)->setPen(bahamaBlueDot);
}

bool DrawCurve_MAP(std::map <int, double> &BaseQ, QCustomPlot* canova, int LineNum){
  QVector <double>  tmpT;
  QVector <double>  tmpV;
  std::map <int, double> ::iterator probe=BaseQ.begin();

  while (probe!=BaseQ.end()) {
      tmpT.push_back(probe->first);
      tmpV.push_back(probe->second);
      ++probe;
  }

   canova->graph(LineNum)->setLineStyle(QCPGraph::lsLine);
   canova->graph(LineNum)->setData(tmpT,tmpV);
   canova->graph(LineNum)->setVisible(true);
//   canova->replot();

   return 1;
}

bool DrawCurve_MAP(std::map <int, double> &BaseQ, QCustomPlot* canova, int LineNum, QColor PenColor){
  QVector <double>  tmpT;
  QVector <double>  tmpV;
  std::map <int, double> ::iterator probe=BaseQ.begin();

  while (probe!=BaseQ.end()) {
      tmpT.push_back(probe->first);
      tmpV.push_back(probe->second);
      ++probe;
  }

   QPen CurvePen;
   CurvePen.setColor(PenColor);
   CurvePen.setWidth(1);

   canova->graph(LineNum)->setLineStyle(QCPGraph::lsLine);
   canova->graph(LineNum)->setPen(CurvePen);
   canova->graph(LineNum)->setData(tmpT,tmpV);
   canova->graph(LineNum)->setVisible(true);
//   canova->replot();

   return 1;
}

bool DrawDots(std::queue<unsigned> &ListQ, int height, QCustomPlot *canova, int LineNum, QColor PenColor){
    QVector <double>  tmpT;
    QVector <double>  tmpV;

    while (!ListQ.empty()) {
        tmpT.push_back(ListQ.front());
        tmpV.push_back(height);
        ListQ.pop();
    }

    QPen CurvePen;
    CurvePen.setColor(PenColor);
    CurvePen.setWidth(1);
//    CurvePen.setStyle(Qt::DotLine);

    canova->graph(LineNum)->setLineStyle(QCPGraph::lsNone);
    canova->graph(LineNum)->setPen(CurvePen);
    canova->graph(LineNum)->setData(tmpT,tmpV);
    canova->replot();

    return 1;
}

bool DrawHisto(QVector <double> &Source, QCustomPlot* canova, int LB, int UB, double step){
    int MaxValue = 0;
    double tmpD;
    QCPBars* HistoX = new QCPBars(canova->xAxis, canova->yAxis);
//    HistoX->setAntialiased(0);
    canova->xAxis->grid()->setVisible(0);
    canova->yAxis->grid()->setVisible(0);

    QVector <double>tmpX;
    QVector <double>tmpY;
    for (int i=0; i<Source.size(); i++){
        tmpX.push_back(step*i+LB+(step/2));
        tmpY.push_back(Source[i]);
        if (Source[i]>MaxValue)
        {MaxValue=Source[i];}
    }


    QCPItemText *tmpText;
    for (int k=0; k<Source.size(); k++){
        tmpText = new QCPItemText(canova);
        tmpD = Source[k];

        tmpText->position->setCoords(step*k+LB+(step/2), tmpD*1.05);
        tmpText->setPositionAlignment(Qt::AlignBottom|Qt::AlignHCenter);
        tmpText->setText(QString::number(tmpD));
    }

    HistoX->setData(tmpX, tmpY);
    HistoX->setWidth(step);
    canova->xAxis->setRange(LB, UB+(step/2));
    canova->yAxis->setRange(0, MaxValue*1.15);
    HistoX->setPen(QColor("#276191"));
    HistoX->setBrush(QColor("#6992b3"));
    canova->setBackground(QBrush(QColor(240,240,240)));
    canova->replot();

    return 1;
}
