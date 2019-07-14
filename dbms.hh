#ifndef __DBMS_HH__
#define __DBMS_HH__
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

namespace dbms{
  struct movie {
    unsigned long awards;
    char title[40];
    unsigned short year;
    char director[40];
    char country[30];
  };

  class dataBase{
  public:
    dataBase();
    ~dataBase();

    //stores a table of movies
    vector<movie> table;
    int rowsUsed;

    movie* get(int);
    //removes a movie from the table with the given id
    bool remove(unsigned long int);
    //adds a movie to the table
    bool add(movie);
    //loads a csv into the database
    bool loadCSV(const char *infn);
    //Returns a string containing all the information
    //a movie in the database contains
    std::string getMovieToString(int, bool getAll);
    //returns amount of rows used in the table
    int rows(){
      return rowsUsed;
    };
  };
}
#endif
