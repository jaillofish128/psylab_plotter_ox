#ifndef RAWREAD_H
#define RAWREAD_H

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <list>
#include <map>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <sstream>

class DataShell2{
  private:
    std::string label;
    double value;
    bool D2 = 0;
  public:
    int token;
    std::string GetLabel();
    double GetValue();

    DataShell2();
    DataShell2(const std::string &T, double X, std::string &tmpH, std::string &tmpM, std::string &tmpS);
    DataShell2(const std::string &T, double X, std::string &tmpH, std::string &tmpM, std::string &tmpS, bool isD2);
    DataShell2(const std::string &T, double X, bool isD2, int offset);
    DataShell2(int T, double X);
    DataShell2(int T, double X, bool isD2);
//    DataShell2(int T, double X, bool isD2, int offset);
};

class TriShell{
    double Slots[3];
public:
  bool isDay2=0;
  int token;

  int GetOX();
  int GetHR();
  double GetMV();
  double GetValue(unsigned);

  TriShell();
  TriShell(const std::string &T, double X, double Y, double Z, bool D);
  TriShell(int T, double X, double Y, double Z, bool D);
  TriShell(int T, double X, double Y, double Z);
};

class PSGTable{
private:
    std::list <DataShell2> ::iterator jotan, footstep, sting;
    std::list <DataShell2> FurinBranchless;
public:
    PSGTable();
    size_t Size();
    void Reset();
    bool ReadFile(const std::string &File);
    void assign(const PSGTable& source);
//    bool push_back(const std::string&, double v);
//    bool push_back(int T, double v);
    bool push_back(DataShell2 *tmpShell);
    bool isEmpty();
    bool extract2MAP_TimeAware(std::map <int, double> &Target, int tensity);
//    bool extract2MAP_GapTimeWare(std::map <int, double> &Target, int slot, int tensity, int gapLimit);
//    bool extract2MAP_wLowestValue(std::map <int, double> &Target, int slot, int tensity, unsigned &lowestValue);
    bool CalcOxDistribute(int LB, int steps, std::vector <double> &Target);
//    bool CalcOxDistributeZwei(int LB, int Watershed, int UB, int stepsL, std::vector <unsigned> &TargetL, int stepsR, std::vector <unsigned> &TargetR);
};


class TriTable{
private:
    std::list <TriShell> ::iterator jotan, footstep, sting;
    std::list <TriShell> FurinL;
public:
//  std::vector <TriShell> Furin;
  TriTable();
  size_t Size();
  void Reset();
  bool ReadFile(const std::string &File, std::string, bool isRemote);
  bool removeWhiteNoise_OX(int threshold, int timeLimit);
  bool removeSingleSpike_OX(int gapLimit);
  bool removeOrphanedSpike_OX(int gapLimit);
  void assign(const TriTable& source);
//  bool push_back(const std::string&, double O, double H, double A, bool D);
//  bool push_back(int T, double O, double H, double A, bool D);
  bool push_back(TriShell *tmpShell);
  bool isEmpty();
//  bool extract2Q(std::queue <DataShell2> &Target, int slot, int tensity);
//  bool extract2Q_TimeAware(std::queue <DataShell2> &Target, int slot, int tensity, int LB, int UB);
//  bool extract2L_TimeAware(std::list <DataShell2> &Target, int slot, int tensity);
//  bool extract2L_GapTimeAware(std::list <DataShell2> &Target, int slot, int tensity, int gapLimit);
  bool extract2MAP_TimeAware(std::map <int, double> &Target, int slot, int tensity);
  bool extract2MAP_GapTimeWare(std::map <int, double> &Target, int slot, int tensity, int gapLimit);
  bool extract2MAP_wLowestValue(std::map <int, double> &Target, int slot, int tensity, unsigned &lowestValue);
  bool CalcOxDistribute(int LB, int steps, std::vector <double> &Target);
  bool CalcOxDistributeZwei(int LB, int Watershed, int UB, int stepsL, std::vector <unsigned> &TargetL, int stepsR, std::vector <unsigned> &TargetR);
};

bool OpenTargetZwei(const std::string &File, TriTable&, std::string, bool isRemote);

bool GenOxDistrString(std::vector <unsigned> &source, std::string &Target, const unsigned totalOX, const unsigned LB, const unsigned step);

//void CalcOxDistributeL(int LB, int steps, std::list <DataShell2> &source, std::vector <double> &Target);

void CalcOxDistributeMAP(int LB, int steps, std::map <int, double> &source, std::vector<unsigned> &Target);

bool RemoveWKMAP(std::map <int, double> &target, std::map <unsigned, unsigned> &reff);

void Int2String(int, std::string&);

void Int2String_alt(int, std::string&);

void Double2String(int, std::string&);

//void appendHHMMString(std::string& target, std::string& buffer, int hhL, int mmL, int hhU, int mmU);

#endif // RAWREAD_H
