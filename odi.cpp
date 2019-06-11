#include "odi.hpp"

odi::odi()
{
    StartTime=0;
    duration=0;
}

odi::odi(int time, int length){
    StartTime = time;
    duration = length;
}

void odi::Modify(unsigned newTime, unsigned newDuration){
    StartTime = newTime;
    duration  = newDuration;
}

void odi::extend(){
    ++duration;
}

void odi::extend(int seg){
    duration+=seg;
}

unsigned odi::GetStartTime(){
    return StartTime;
}

unsigned odi::GetDuration(){
    return duration;
}

void OListtoMap(std::list <odi> &source, std::map <unsigned, unsigned> &target){
    std::list <odi>::iterator needle = source.begin();
    target.clear();
    while(needle!=source.end()){
        target.insert(target.end(), std::pair<unsigned, unsigned>(needle->GetStartTime(),needle->GetDuration()));
        ++needle;
    }
}


odList::odList(){}

bool odList::isEmpty(){
    return (BlueFace.empty());
}

size_t odList::Size(){
    return BlueFace.size();
}

void odList::HeadCount(std::queue <unsigned> &Target){
    Crane = BlueFace.begin();
    while(Crane!=BlueFace.end()){
        Target.push(Crane->GetStartTime());
        ++Crane;
    }
}

bool odList::Update(std::map<int, double> &Source, int threshold){
    BlueFace.clear();

    odi* tmpODI;
    int tmpOX;
    int timeF, timeB;
    std::map<int, double> ::iterator needle = Source.begin();

    while(needle!=Source.end()){

        tmpOX = needle->second;
        if(tmpOX<threshold){
            timeF = needle->first;

            while(tmpOX<threshold&&needle!=Source.end()){
                ++needle;
                tmpOX = needle->second;
            }
            timeB = needle->first;
            if (needle!=Source.end()){
                tmpODI = new odi(timeF, (timeB-timeF));
                BlueFace.push_back(*tmpODI);
            }
        }
        else
        {++needle;}
    }
    return 1;
}

bool odList::RemoveWake(std::list<odi> &ALAN){
    if (BlueFace.empty()||ALAN.empty())
    {return 0;}
    else{
        std::list<odi> ::iterator WAKE_needle, OD_Remove;
        unsigned WAKEtimeF, WAKEtimeB;
        Crane = BlueFace.begin();
        WAKE_needle=ALAN.begin();
//        WAKEtimeF = WAKE_needle->GetStartTime();
        while(Crane!=BlueFace.end()&&WAKE_needle!=ALAN.end()){
            WAKEtimeF = WAKE_needle->GetStartTime();
            while((WAKEtimeF > Crane->GetStartTime())&&Crane!=BlueFace.end()){
                ++Crane;
            }
            WAKEtimeB = WAKEtimeF + WAKE_needle->GetDuration();
            while ((WAKEtimeB > Crane->GetStartTime())&&Crane!=BlueFace.end()){ //Crane found the time spot that the subject is awake
                OD_Remove = Crane;                                              //Mark the spot for removal
                --Crane;                                                        //Crane backs away from the spot
                BlueFace.erase(OD_Remove);                                            //Remove the marked spot
                ++Crane;
            }
            ++WAKE_needle;
        }
        return 1;

    }

}

bool odList::Merge(odi *root, odi *stem){
    unsigned tmpStart = root->GetStartTime();
    unsigned tmpDuration = (stem->GetStartTime()+stem->GetDuration()) - root->GetStartTime();
    root->Modify(tmpStart, tmpDuration);

//    Crane = stem;
//    BlueFace.erase(Crane);
    return 1;
}

void odList::OutputAsString(std::string &output, const unsigned MinTime, const unsigned MaxTime, unsigned density){
    std::list<odi> ::iterator Probe = BlueFace.begin();
    unsigned counter = 1;
    unsigned token   = 0;
    unsigned length;
//    ldiv_t div1, div2;
    unsigned dd;
    unsigned hh;
    unsigned mm;
    std::string StringBuffer;
    StringBuffer.reserve(5);

    while (Probe->GetStartTime()<MinTime&&Probe!=BlueFace.end()){
        Probe++;
    }
    token = Probe->GetStartTime();


    while (Probe!=BlueFace.end()&&token<MaxTime){
/*
        div1 = ldiv(token, (3600*24));
        dd = div1.quot;
        div2 = ldiv(div1.rem, 3600);
        hh = div2.quot;
        mm = ldiv(div2.rem, 60).quot;
*/
        dd = token/(3600*24);
        hh = (token%(3600*24))/3600;
        mm = (token%3600)/60;
        length = Probe->GetDuration();

        output+="\n";
        Int2String(counter, StringBuffer);
        output.append(StringBuffer);
//        output+=toStringKang(counter);
        output+=". ";

        if (dd==1)
        {output+="Day1 ";}
        else
        {output+="Day2 ";}
        Int2String(hh, StringBuffer);
        output.append(StringBuffer);
//        output+=toStringKang(hh);
        output+=":";
        Int2String(mm, StringBuffer);
        output.append(StringBuffer);
//        output+=toStringKang(mm);

        output+=" ";

        Int2String_alt(length, StringBuffer);
        output.append(StringBuffer);
//        output+=toStringKang_alt(length);
        output+="sec";

        ++Probe;
        ++counter;
        token = Probe->GetStartTime();
    }
}

void odList::OutputAsStringZwei(std::string &output1, std::string &output2, const unsigned MinTime, const unsigned MaxTime, unsigned density){
    std::list<odi> ::iterator Probe = BlueFace.begin();
    unsigned counter = 1;
    unsigned token   = 0;
    unsigned length;
//    ldiv_t div1, div2;
    unsigned dd;
    unsigned hh;
    unsigned mm;
    size_t capacity = 1+(BlueFace.size()/2);
    std::string StringBuffer;
    StringBuffer.reserve(5);

    while (Probe->GetStartTime()<MinTime&&Probe!=BlueFace.end()){
        Probe++;
        --capacity;
    }
    token = Probe->GetStartTime();
;

    while (Probe!=BlueFace.end()&&counter<capacity&&token<MaxTime){
/*
        div1 = ldiv(token, (3600*24));
        dd = div1.quot;
        div2 = ldiv(div1.rem, 3600);
        hh = div2.quot;
        mm = ldiv(div2.rem, 60).quot;
*/
        dd = token/(3600*24);
        hh = (token%(3600*24))/3600;
        mm = (token%3600)/60;
        length = Probe->GetDuration();

        output1+="\n";
        Int2String_alt(counter, StringBuffer);
        output1.append(StringBuffer);
//        output1+=toStringKang(counter);
        output1+=". ";

        if (dd==1)
        {output1+="Day1 ";}
        else
        {output1+="Day2 ";}
        Int2String(hh, StringBuffer);
        output1.append(StringBuffer);
//        output1+=toStringKang(hh);
        output1+=":";
        Int2String(mm, StringBuffer);
        output1.append(StringBuffer);
//        output1+=toStringKang(mm);

        output1+=" ";

        Int2String_alt(length, StringBuffer);
        output1.append(StringBuffer);
//        output1+=toStringKang_alt(length);
        output1+="sec";

        ++Probe;
        ++counter;
        token = Probe->GetStartTime();
    }
    while (Probe!=BlueFace.end()&&token<MaxTime){
/*
        div1 = ldiv(token, (3600*24));
        dd = div1.quot;
        div2 = ldiv(div1.rem, 3600);
        hh = div2.quot;
        mm = ldiv(div2.rem, 60).quot;
*/
        dd = token/(3600*24);
        hh = (token%(3600*24))/3600;
        mm = (token%3600)/60;
        length = Probe->GetDuration();

        output2+="\n";
        Int2String(counter, StringBuffer);
        output2.append(StringBuffer);
//        output2+=toStringKang(counter);
        output2+=". ";

        if (dd==1)
        {output2+="Day1 ";}
        else
        {output2+="Day2 ";}
        Int2String(hh, StringBuffer);
        output2.append(StringBuffer);
//        output2+=toStringKang(hh);
        output2+=":";
        Int2String(mm, StringBuffer);
        output2.append(StringBuffer);
//        output2+=toStringKang(mm);

        output2+=" ";

        Int2String_alt(length, StringBuffer);
        output2.append(StringBuffer);
//        output2+=toStringKang_alt(length);
        output2+="sec";

        ++Probe;
        ++counter;
        token = Probe->GetStartTime();
    }
}

unsigned odList::CalcOxTimeBelow(int threshold, int density, std::map<int, double> &Base){
    if (BlueFace.empty())
    {return 0;}
    else{
        std::map<int, double> ::iterator MapSeeker = Base.begin();
        std::list<odi> ::iterator ListGuider = BlueFace.begin();
        unsigned timeF, timeB;
        unsigned tmpTime = 0;

        while(MapSeeker!=Base.end() && ListGuider!=BlueFace.end()){
            timeF = ListGuider->GetStartTime();
            timeB = timeF + ListGuider->GetDuration();
            while(MapSeeker->first<timeF)
            {++MapSeeker;}
            while(MapSeeker->first<timeB){
                tmpTime = tmpTime + ((MapSeeker->second)<threshold);
                ++MapSeeker;
            }
            ++ListGuider;
        }
        return (tmpTime*density);
    }
}


bool FilterWake(std::map<int, double> &Source, std::list<odi> &Target){
    if (Source.empty())
    {return 0;}
    else{
        odi* tmpWK;
        int tmpMV;
        int timeF, timeB, timeDif;
        std::map<int, double> ::iterator needle = Source.begin();

        while(needle!=Source.end()){

            tmpMV = needle->second;
            if(tmpMV>10){
                timeF = needle->first;

                while(tmpMV>10&&needle!=Source.end()){
                    ++needle;
                    tmpMV = needle->second;
                }
                timeB = needle->first;
                timeDif = timeB-timeF;
                if (needle!=Source.end()&&timeDif<=129600){
                    tmpWK = new odi(timeF, timeDif);
                    Target.push_back(*tmpWK);
                }
            }
            else
            {++needle;}
        }
        return 1;

    }
}

unsigned CalcListTime(std::list<odi> &Source){
    if (Source.empty())
    {return 0;}
    else{
        unsigned tmp = 0;
        std::list<odi> ::iterator needle = Source.begin();
        while (needle!=Source.end()){
            tmp+=needle->GetDuration();
            ++needle;
        }
        return tmp;
    }
}

