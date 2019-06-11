#ifndef DARW_HPP
#define DARW_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <qcustomplot.h>
#include "libpsyclass.h"

void DrawBound(QCustomPlot *canovas, int cell, int number);

void DrawBoundOX(QCustomPlot *canovas, int num1, int num2);

bool DrawCurve_MAP(std::map <int, double> &BaseQ, QCustomPlot* canova, int LineNum);

bool DrawCurve_MAP(std::map <int, double> &BaseQ, QCustomPlot* canova, int LineNum, QColor PenColor);

bool DrawDots(std::queue<unsigned> &ListQ, int height, QCustomPlot* canova, int LineNum, QColor PenColor);

bool DrawHisto(QVector <double> &Source, QCustomPlot* canova, int LB, int UB, double step);

//void DrawTri(TriTable&, QCustomPlot* canova, QCustomPlot *canovas2);

#endif // DARW_HPP
