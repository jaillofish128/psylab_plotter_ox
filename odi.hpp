#ifndef ODI_HPP
#define ODI_HPP

#include <string>
#include <list>
#include "libpsyclass.h"

class odi
{
private:
    unsigned StartTime;
    unsigned duration;
public:
    odi();
    odi(int time, int length);
    void extend();
    void extend(int seg);
    void Modify(unsigned newTime, unsigned newDuration);
    unsigned GetStartTime();
    unsigned GetDuration();
};

void OListtoMap(std::list <odi> &source, std::map<unsigned, unsigned> &target);

class odList{
private:
    std::list <odi> BlueFace;
    std::list <odi>::iterator Crane;
public:
    odList();
    bool isEmpty();
    size_t Size();
    bool Update(std::map<int, double> &Source, int threshold);
    bool RemoveWake(std::list<odi> &ALAN);
    bool Merge(odi *root, odi *stem);
    void HeadCount(std::queue<unsigned> &Target);
    void OutputAsString(std::string &output, const unsigned MinTime, const unsigned MaxTime, unsigned density);
    void OutputAsStringZwei(std::string &output1, std::string &output2, const unsigned MinTime, const unsigned MaxTime, unsigned density);
    unsigned CalcOxTimeBelow(int threshold, int density, std::map<int, double> &Base);
};


bool FilterWake(std::map<int, double>& Source, std::list <odi>& Target);

bool ODI_Remove_Wake(std::list<odi> &OD, std::list<odi> &WAKE);

unsigned CalcListTime(std::list<odi> &Source);

#endif // ODI_HPP
