

#include <cstdlib>
#include <vector>
#include <iostream>
#include <cstring>
#include "dbms.hh"
using namespace std;
using namespace dbms;


dataBase::dataBase(){rowsUsed=0;}

dataBase::~dataBase(){}

movie* dataBase::get(int rowNum){
  if(rowNum<0 || rowNum>rows())
  return nullptr;

  movie *ptr= &table.at(rowNum);
  return ptr;
}



bool dataBase::add(movie mov){
  table.push_back(mov);
  rowsUsed++;

  return true;
}

/**
*Loads data from file into the database
*/
bool dataBase::loadCSV(const char *infn){
  if(infn==nullptr)
  return false;

  //opening file and checking for errors
  ifstream csv;
  csv.open(infn);
  if(!csv.is_open()) {
    printf("Failed to open .csv file\n");
    return false;
  }

  movie m;
  //looping through contents of file
  while(!csv.eof()) {
    string t;

    // title
    if(!getline(csv, t, ',')) break;
    if(t.size()>50 ||t.size()==0){csv.close(); return false;}//invalid checker
    strncpy(m.title, t.c_str(), t.length()+1);

    //director
    if(!getline(csv, t, ',')) break;
    if(t.size()>50 ||t.size()==0) {csv.close(); return false;}//invalid checker
    strncpy(m.director, t.c_str(), t.length()+1);

    // year
    if(!getline(csv, t, ',')) break;
    if(t.size()!=4 ||atoi(t.c_str())==0) {csv.close(); return false;}//invalid checker
    m.year=atoi(t.c_str());


    // awards
    if(!getline(csv, t, ',')) break;
    if(atoi(t.c_str())==-1){csv.close(); return false;}//invalid checker
    m.awards=atoi(t.c_str());

    //country
    if(!getline(csv, t)) break;
    if(t.size()>50 ||t.size()==0) {csv.close(); return false;}//invalid checker
    strncpy(m.country, t.c_str(), t.length()+1);

    add(m);
  }
  csv.close();
  return true;
}

string dataBase::getMovieToString(int index, bool getAll){
  string out;
  if(getAll){
    index=0;
    for (auto i = table.begin(); i != table.end(); ++i){
      movie *temp = get(index++);
      char snum[20];
      out+="Title: ";
      out+=(*temp).title;
      out+=" Director: ";
      out+=(*temp).director;
      out+=" Awards: ";
      sprintf(snum, "%d",(*temp).awards);
      out+=snum;
      out+=" Year: ";
      sprintf(snum, "%d",(*temp).year);
      out+=snum;
      out+=" Country: ";
      out+=(*temp).country;
      out+="\n";
    }
  }else{
    movie *temp = get(index);
    char num[20];
    out+="Title: ";
    out+=(*temp).title;
    out+=" Director: ";
    out+=(*temp).director;
    out+=" Awards: ";
    sprintf(num, "%d",(*temp).awards);
    out+=num;
    out+=" Year: ";
    sprintf(num, "%d",(*temp).year);
    out+=num;
    out+=" Country: ";
    out+=(*temp).country;
  }
  return out;
}
