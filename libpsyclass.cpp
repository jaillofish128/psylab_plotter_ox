#include "libpsyclass.h"
using namespace std;

//Second Gen DataShell (Simplified)

  DataShell2::DataShell2(){}

  DataShell2::DataShell2(const std::string &T, double X, string &tmpH, string &tmpM, string &tmpS){
      label = T;
      value = X;

      tmpH=T.substr(0,2);
      tmpM=T.substr(3,2);
      tmpS=T.substr(6,2);

      token = (3600*24)+(3600*atof(tmpH.c_str()))+(60*atof(tmpM.c_str()))+atof(tmpS.c_str());
  }

  DataShell2::DataShell2(const std::string &T, double X,  string &tmpH, string &tmpM, string &tmpS, bool isD2){
      label = T;
      value = X;

      tmpH=T.substr(0,2);
      tmpM=T.substr(3,2);
      tmpS=T.substr(6,2);

      token = (3600*24*isD2)+(3600*atof(tmpH.c_str()))+(60*atof(tmpM.c_str()))+atof(tmpS.c_str());
      D2 = isD2;
  }

  DataShell2::DataShell2(int T, double X){
      token = T;
      value = X;
  }

  DataShell2::DataShell2(int T, double X, bool isD2){
      token = T;
      value = X;

      D2 = isD2;
      if (isD2)
          token+=24*3600;
  }

  double DataShell2::GetValue(){
      return value;
  }

  std::string DataShell2::GetLabel(){
      return label;
  }

  TriShell::TriShell(){
      isDay2 = 0;
  }

  TriShell::TriShell(const string &T, double X, double Y, double Z, bool D){
      Slots[0] = X;
      Slots[1] = Y;
      Slots[2] = Z;

      isDay2 = D;

//      label=T;
      std::string tmpH=T.substr(0,2);
      std::string tmpM=T.substr(3,2);
      std::string tmpS=T.substr(6,2);

      token = ((3600*24*D)+(3600*atof(tmpH.c_str()))+(60*atof(tmpM.c_str()))+atof(tmpS.c_str()));
  }

  TriShell::TriShell(int T, double X, double Y, double Z, bool D){
      Slots[0] = X;
      Slots[1] = Y;
      Slots[2] = Z;

      isDay2 = D;

      token = (3600*24*D)+T ;
  }

  TriShell::TriShell(int T, double X, double Y, double Z){
      Slots[0] = X;
      Slots[1] = Y;
      Slots[2] = Z;
      token = T ;

      isDay2 = (T>86399);
  }

  int TriShell::GetOX(){
      return Slots[0];
  }

  int TriShell::GetHR(){
      return Slots[1];
  }

  double TriShell::GetMV(){
      return Slots[2];
  }

  double TriShell::GetValue(unsigned slot){
      if (slot>2)
      {return -1;}
      else
      {return Slots[slot];}
  }

  PSGTable::PSGTable(){}

  void PSGTable::Reset(){
      FurinBranchless.clear();
      jotan=FurinBranchless.begin();
      footstep=jotan;
  }

  bool PSGTable::ReadFile(const string &File){
      std::ifstream Loader;
      std::string tmp, tmpD, tmpT, tmpH, tmpM, tmpS;
      DataShell2 *Shell_placeholder;
      bool isDay2=0;
      std::string Day1String = "????";
      double X=0;
      int T=0;
//      std::size_t possy;

      Loader.open(File, std::ios::in);

      if (!Loader.fail()){
          std::getline(Loader, tmp);
          std::getline(Loader, tmp);
          std::getline(Loader, tmp);
          std::getline(Loader, tmp);
          std::getline(Loader, tmp);
          Day1String = tmp.substr(0,tmp.find_first_of(" "));
          while (Loader.good()&&tmp.size()!=0){
              tmpD = tmp.substr(0,tmp.find_first_of(" "));
              isDay2 = (tmpD!=Day1String);

              tmp.erase(tmp.begin(), tmp.begin()+tmp.find_first_of(" ")+1);
              tmpH = tmp.substr(0,2);
              tmpM = tmp.substr(3,2);
              tmpS = tmp.substr(6,2);
              if (tmpH=="12")
              {tmpH="00";}
              T = 86400+43200+(86400*isDay2)-(43200*isDay2)+(3600*atof(tmpH.c_str()))+(60*atof(tmpM.c_str()))+atof(tmpS.c_str());

              tmp.erase(tmp.begin(),tmp.begin()+tmp.find_first_of('\t'));
              X = atof(tmp.c_str());

              if (X>0){
                  Shell_placeholder = new DataShell2(T, X);
                  this->push_back(Shell_placeholder);
              }
              std::getline(Loader, tmp);
          }
          Loader.close();
          return 1;
      }
      else
      {return 0;}
  }

  void PSGTable::assign(const PSGTable& source){
      FurinBranchless.assign(source.FurinBranchless.begin(), source.FurinBranchless.end());
      jotan = FurinBranchless.begin();
      footstep=jotan;
  }

  bool PSGTable::push_back(DataShell2 *tmpShell){
      FurinBranchless.push_back(*tmpShell);
      delete tmpShell;
      return 1;
  }

  bool PSGTable::isEmpty(){
      return FurinBranchless.empty();
  }

  bool PSGTable::extract2MAP_TimeAware(std::map <int, double> &Target, int tensity){
      double avgK, avgT, bufferK;
      int tokenBase, tokenNewest;
      int tickle;

      if (FurinBranchless.empty()){
          return 0;
      }
      else{
        jotan = FurinBranchless.begin();
        footstep = jotan;
        while(jotan!=this->FurinBranchless.end()&&footstep!=this->FurinBranchless.end()){
          tickle=0;
          footstep=jotan;
          avgK=0;
          avgT=0;
          tokenBase = jotan->token;
          tokenNewest = footstep->token;

          while(tensity>(tokenNewest-tokenBase)&&footstep!=this->FurinBranchless.end()){
              ++tickle;
              avgK+=footstep->GetValue();
              avgT+=tokenNewest;
              ++footstep;
              tokenNewest = footstep->token;
          }
          avgK = avgK/tickle;
          avgT = avgT/tickle;
          Target.insert(Target.end(), std::pair<int, double> (avgT, avgK));

          jotan = footstep;
        }
      return 1;
      }
  }

  TriTable::TriTable(){}

  size_t TriTable::Size(){
      return FurinL.size();
  }

  void TriTable::Reset(){
      FurinL.clear();
      jotan=FurinL.begin();
      footstep=FurinL.begin();
  }

  bool TriTable::ReadFile(const string &File, string DZ, bool isRemote){
      std::ifstream Loader;
      std::string tmp, tmp2, tmpT, tmpH, tmpM, tmpS;
      std::string tmpX, tmpY, tmpZ;
      TriShell *Shell_placeHolder;
      int Day2=0;
      int DayC;
      bool isNewYear=0;
      bool isDay2=0;
      double X;
      double Y;
      double Z;
      int T, previousT;
      std::size_t possy, possyX, possyY, possyZ; //Find the positions of "/\\", "," and "/r"

      Loader.open(File, std::ios::in);

      if (!Loader.fail()){
          std::getline(Loader, tmp);
          std::getline(Loader, tmp); //skip the header lines
          std::getline(Loader, tmp);
          if (isRemote){                                       //DZ: YYYY-MM-DD.txt
              DZ.erase(DZ.begin(), DZ.begin()+5);              // >> MM-DD.txt
              DZ.erase(2, 1);                                  // >> MMDD.txt
              DZ.erase(DZ.begin()+4, DZ.end());                // >> MMDD
//              DZ = DZ.substr(5,2)+DZ.substr(8,2);
              Day2 = atof(DZ.c_str());
          }
          else{
              possy = File.find_last_of("/\\");
              DZ = File.substr(possy+1);
              if (DZ.size()==23){                                  //DZ: NNNNNNNN_YYYY-MM-DD.txt
                  DZ.erase(DZ.begin(), DZ.begin()+14);             // >> MM-DD.txt
                  DZ.erase(2, 1);                                  // >> MMDD.txt
                  DZ.erase(DZ.begin()+4, DZ.end());                // >> MMDD
//                  DZ = DZ.substr(14,2)+DZ.substr(17,2);
                  Day2 = atof(DZ.c_str());
              }
              else{
                  DZ.erase(DZ.begin(), DZ.begin()+5);              // >> MM-DD.txt
                  DZ.erase(2, 1);                                  // >> MMDD.txt
                  DZ.erase(DZ.begin()+4, DZ.end());                // >> MMDD
//                  DZ = (tmp.substr(5,2)+tmp.substr(8,2));
                  Day2 = 1+atof(DZ.c_str());
              }
          }
          if(Day2==101||Day2==1232) //Check if it is new year
          {isNewYear=1;}
          previousT = 0;

          while (Loader.good()&&tmp.size()!=0){
              tmp2 = (tmp.substr(5,2)+tmp.substr(8,2));
              DayC = atof(tmp2.c_str());
              if (isNewYear)
              {isDay2=(DayC!=1231);}
              else
              {isDay2=(DayC==Day2);}

              tmpT = tmp.substr(11, 8);
              tmpH=tmpT.substr(0,2);
              tmpM=tmpT.substr(3,2);
              tmpS=tmpT.substr(6,2);
              T = 86400+(86400*isDay2)+(3600*atof(tmpH.c_str()))+(60*atof(tmpM.c_str()))+atof(tmpS.c_str());

              if (T > previousT){
                  tmp.erase(tmp.begin(), tmp.begin()+20);

                  possyX = tmp.find(",");
                  possyY = tmp.find(",", possyX+1);
                  possyZ = tmp.find("\r");

                  tmpX = tmp.substr(0, possyX);
                  tmpY = tmp.substr(possyX+1, possyY-possyX-1);
                  tmpZ = tmp.substr(possyY+1, possyZ-possyY-1);

                  X = atof(tmpX.c_str());         //Transit string into double number
                  Y = atof(tmpY.c_str());
                  Z = atof(tmpZ.c_str());

                  if(X>50 && X<101 && Y>40 && Y<150){
                    previousT = T;                                 //Update the last logged time
                    Shell_placeHolder = new TriShell(T, X, Y, Z);  //Ceate new entry of TriShell
                    this->push_back(Shell_placeHolder);            //Push the new entry into target table
                  }
              }
              std::getline(Loader, tmp);
          }
          Loader.close();
          return 1;
      }
      else
      {return 0;}
  }

  bool TriTable::removeWhiteNoise_OX(int threshold, int timeLimimt){
      std::list <TriShell> ::iterator primerF, primerB;
      jotan = FurinL.begin();
      footstep = jotan;
      ++footstep;
      int OXvalue = 0;
      int timeF, timeB;

      while (jotan!=FurinL.end()&&footstep!=FurinL.end()){
          OXvalue = jotan->GetOX();

          if (OXvalue<threshold){                                           //The node's OX value is lower than threshold
            timeF = jotan->token;                                           //Try to identify if there is white noise contamination
            footstep = jotan;

            while(footstep!=FurinL.end() && footstep->GetOX()==OXvalue){   //If the following nodes have the same lower-than-threshold OX value
                                                                            //as "jotan", expand the marker for possible white noise range
                ++footstep;
                timeB = footstep->token;
            }
            if (timeLimimt<(timeB-timeF)){                                 //If the low OX range is too large to be presented in living human
                                                                           //(>timeLimit), the program treat these nodes as contaminated by white
                                                                           //noise and proceed to remove currupted data

                primerF = jotan;                                           //Mark the start of currupted area with primerF
                --jotan;                                                   //Move "jotan" away from currupted area
                primerB = footstep;                                        //Mark the end of currupted area with primerB
                FurinL.erase(primerF, primerB);                            //Erase the currupted data
            }
          }
          ++jotan;                                                         //Continue scanning the data with jotan
      }
      return 1;
  }

  bool TriTable::removeSingleSpike_OX(int gapLimit){
      if (this->isEmpty()){
          return 0;
      }
      else{
          jotan = FurinL.begin();
          footstep = FurinL.begin();
          ++footstep;
          int OXGap = 0;
          int timeGap = 0;

          while (jotan!=FurinL.end()&&footstep!=FurinL.end()){
              OXGap = (jotan->GetOX() - footstep->GetOX());
              timeGap = (footstep->token - jotan->token);
              if (OXGap>gapLimit&&timeGap==1){
                  sting = footstep;
                  ++footstep;
                  OXGap = footstep->GetOX() - sting->GetOX();
                  timeGap = footstep->token - sting->token;
                  if (OXGap>gapLimit&&timeGap==1){
                      FurinL.erase(sting);
                  }
              }
              ++jotan;
              ++footstep;
          }
          return 1;
      }
  }

  bool TriTable::removeOrphanedSpike_OX(int gapLimit){
      if (this->isEmpty()){
          return 0;
      }
      else{
          jotan = FurinL.begin();
          footstep = FurinL.begin();
          ++footstep;
          int OXGap = 0;
          int timeGap = 0;

          while (jotan!=FurinL.end()&&footstep!=FurinL.end()){
              OXGap = (jotan->GetOX() - footstep->GetOX());
              timeGap = (footstep->token - jotan->token);
              if (OXGap>gapLimit){
                  sting = footstep;
                  ++footstep;
                  OXGap = footstep->GetOX() - sting->GetOX();
                  if ((OXGap)>gapLimit&&timeGap>1&&timeGap<60){
                      FurinL.erase(sting);
                  }
              }
              ++jotan;
              ++footstep;
          }
          return 1;
      }
  }

  void TriTable::assign(const TriTable& source){
      this->FurinL.assign(source.FurinL.begin(), source.FurinL.end());
  }


  bool TriTable::push_back(TriShell *tmpShell){
      FurinL.push_back(*tmpShell);
      delete  tmpShell;
      return 1;
  }

  bool TriTable::isEmpty(){
      return FurinL.empty();
  }

  bool TriTable::extract2MAP_TimeAware(std::map<int, double> &Target, int slot, int tensity){

          if (this->isEmpty()){
              return 0;
          }
          else{
              double avgK, avgT, bufferK;
              int tokenBase, tokenNewest;
              int tickle;
              jotan = this->FurinL.begin();
              footstep = jotan;
              while(jotan!=this->FurinL.end()&&footstep!=this->FurinL.end()){
                tickle=0;
                footstep=jotan;
                avgK=0;
                avgT=0;
                tokenBase = jotan->token;
                tokenNewest = footstep->token;
                while(tensity>(tokenNewest-tokenBase)&&footstep!=this->FurinL.end()){
                    ++tickle;
                    bufferK = footstep->GetValue(slot);
                    avgK+=bufferK;
                    avgT+=tokenNewest;
                    ++footstep;
                    tokenNewest = footstep->token;
                }
                avgK = avgK/tickle;
                avgT = avgT/tickle;
                Target.insert(Target.end(), std::pair<int, double> (avgT, avgK));
                jotan = footstep;
              }
            return 1;
          }
  }

  bool TriTable::extract2MAP_GapTimeWare(std::map<int, double> &Target, int slot, int tensity, int gapLimit){
      if (this->isEmpty()){
          return 0;
      }

      else{
          double avgK, avgT, bufferK;
          int tokenBase, tokenNewest;
          int tickle;
          double exSlot, currentSlot, gap;
          jotan = this->FurinL.begin();
          footstep = jotan;
          while(jotan!=this->FurinL.end()&&footstep!=this->FurinL.end()){
            tickle=0;
            footstep=jotan;
            avgK=0;
            avgT=0;
            tokenBase = jotan->token;
            tokenNewest = footstep->token;
            bufferK = footstep->GetValue(slot);
            exSlot = bufferK;
            while(tensity>(tokenNewest-tokenBase)&&footstep!=this->FurinL.end()){
                bufferK = footstep->GetValue(slot);
                currentSlot = bufferK;
                gap = abs(exSlot-currentSlot);
                if (gap<gapLimit){
                  ++tickle;
                  avgK+=currentSlot;
                  avgT+=tokenNewest;
                  exSlot = currentSlot;
                }
                  ++footstep;
                  tokenNewest = footstep->token;
            }
            avgK = avgK/tickle;
            avgT = avgT/tickle;
            Target.insert(Target.end(), std::pair<int, double> (avgT, avgK));

            jotan = footstep;
          }
        return 1;

      }
  }

  bool TriTable::extract2MAP_wLowestValue(std::map<int, double> &Target, int slot, int tensity,unsigned& lowestValue){

      if (this->isEmpty()){
          return 0;
      }
      else{
          double avgK, avgT, bufferK;
          int tokenBase, tokenNewest;
          int tickle;
        lowestValue = 255;
        jotan = this->FurinL.begin();
        footstep = jotan;
        while(jotan!=this->FurinL.end()&&footstep!=this->FurinL.end()){
            tickle=0;
            footstep=jotan;
            avgK=0;
            avgT=0;
            tokenBase = jotan->token;
            tokenNewest = footstep->token;
            while(tensity>(tokenNewest-tokenBase)&&footstep!=this->FurinL.end()){
                  ++tickle;
                bufferK = footstep->GetValue(slot);
                avgK+=bufferK;
                avgT+=tokenNewest;

                ++footstep;
                tokenNewest = footstep->token;
            }
            avgK = avgK/tickle;
            avgT = avgT/tickle;
            Target.insert(Target.end(), std::pair<int, double> (avgT, avgK));
          if (avgK<lowestValue)
          {lowestValue = avgK;}

          jotan = footstep;
        }
      return 1;
      }
  }


  bool TriTable::CalcOxDistribute(int LB, int steps, std::vector <double> &Target){
      if (this->isEmpty()){
          return 0;
      }
      else{
          int Flinger;
          std::list <TriShell> ::iterator Tracer = FurinL.begin();

          for (int j=102-LB; j>0; j=j-steps){
              Target.push_back(0);
          }

          while(Tracer!=FurinL.end()){
              Flinger = ((Tracer->GetOX()-LB))/steps;
              ++Target[Flinger];
              ++Tracer;
          }
          return 1;
      }
  }

  bool TriTable::CalcOxDistributeZwei(int LB, int Watershed, int UB, int stepsL, std::vector <unsigned> &TargetL, int stepsR, std::vector <unsigned> &TargetR){
      if (this->isEmpty()){
          return 0;
      }
      else{
          int FlingerL, FlingerR;
          double tmpOX = 0;
          std::list <TriShell> ::iterator Tracer = FurinL.begin();

          for (int j=UB-LB; j>0; j=j-stepsL){
              TargetL.push_back(0);
          }

          for (int j=UB-Watershed; j>0; j=j-stepsR){
              TargetR.push_back(0);
          }

          while(Tracer!=FurinL.end()){
              tmpOX = Tracer->GetOX();
              FlingerL = ((tmpOX-LB))/stepsL;
              FlingerR = ((tmpOX-Watershed))/stepsR;
              TargetL[FlingerL]+=(FlingerL>-1);
              TargetR[FlingerR]+=(FlingerR>-1);
              ++Tracer;
          }

          return 1;
      }
  }

      bool GenOxDistrString(std::vector <unsigned> &source, std::string &Target, const unsigned totalOX, const unsigned LB, const unsigned step){
          if (source.size() == 0)
          {return 0;}
          else{
              std::string bufferString;
              size_t ss = source.size()-2;
              Target.clear();
              unsigned OverHead = (100-LB)%step;
              unsigned AccuDataCount = totalOX;
              int      rollbacks = (99-LB)-OverHead;
              if (OverHead!=0){
                  for (int i=OverHead+1; i!=0;i--){
                      AccuDataCount -= source[ss];
                      ss--;
                  }
                  Int2String_alt(AccuDataCount, bufferString);
                  Target+=bufferString;
                  Target+=';';
              }

              while (rollbacks>0) {
                  for (int j = step; (rollbacks*j)>0; j--){
                      AccuDataCount -= source[ss];
                      rollbacks--;
                      ss--;
                  }
                  Int2String_alt(AccuDataCount, bufferString);
                  Target+=bufferString;
                  Target+=';';
              }
              return 1;
           }
      }



  void CalcOxDistributeMAP(int LB, int steps, std::map<int, double> &source, std::vector<unsigned> &Target){
      int Flinger;
      std::map <int, double> ::iterator Tracer = source.begin();

      for (int j=102-LB; j>0; j=j-steps){
          Target.push_back(0);
      }

      while(Tracer!=source.end()){
          Flinger = ((Tracer->second)-LB)/steps;
          Target[Flinger]+=(Flinger>-1);
          ++Tracer;
      }
  }


      bool RemoveWKMAP(std::map <int, double> &target, std::map <unsigned, unsigned> &reff){
          if (reff.empty()|target.empty()){
              return 0;
          }
          std::map <int,double> ::iterator probe = target.begin();
          std::map <int,double> ::iterator crane = probe;
          std::map <unsigned, unsigned> ::iterator WKneedle = reff.begin();
          unsigned tL = 0;
          unsigned tU = 0;
          while (probe!=target.end()&&WKneedle!=reff.end()){
              tL = WKneedle->first;
              tU = tL + WKneedle->second;
              while (probe->first < tL){
                  ++probe;
              }
              if (probe->first < tU){
                  while (probe->first <tU){
                      crane = probe;
                      ++probe;
                      target.erase(crane);
                  }
              }
              ++WKneedle;
          }
          return 1;
      }

  void Int2String(int input, std::string& target){
      target.clear();
      std::stringstream tmp;
      if (input>9){
          tmp << input;
      }
      else {
          tmp << "0" << input;
      }
      tmp >> target;
  }

  void Int2String_alt(int input, std::string& target){
      target.clear();
      std::stringstream tmp;
      if (input>9){
          tmp << input;
      }
      else {
          tmp << " " << input;
      }
      tmp >> target;
  }

  void Double2String(int input, std::string& target){
      target.clear();
      std::stringstream tmp;
      tmp << input;
      tmp >> target;
  }
