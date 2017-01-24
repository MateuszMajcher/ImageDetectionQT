#ifndef MATCH_H
#define MATCH_H
#include<vector>
#include <QList>
#include "mainwindow.h"
using namespace std;

class item {
public:
    QString name;
    int count;
};

class ImageMatch {
public:
    int i; //index obrazu train
    double v; //podobienstwo
};


//klasa opisuje dopasowanie
class Res {
public:
    int res;  //id klasa obrazu train+
    float val;  //wartows dopasowania
};

//klasa repezentujaca countfile
class Range{
public:
    Range();
    Range(int size) {
        name.resize(size);
        count_query.resize(size);
        count_train.resize(size);
    }

    vector<string> name;
    vector<int> count_train;
    vector<int> count_query;
};

QList<Res> mathImage(const QString& key_file, const QString& database_file, const QString& image_list_file, QList<item> count_file, int gcs);
unsigned char *ReadKeys(const char *, int , int &);
vector<int> QueryRanges(int);
void getDataStat(string);
void checkModel(string, int, float,bool, string, string, string, Range);
vector<Res> GroupResult(vector<Res>);
int CheckClassQuery(vector<int>, int ,int);
string ToModel(int);
Range QueryRangesTemp(int, Range);
void split(const string &s, char delim, vector<string> &elems);
vector<string> split(const string &s, char delim);
void show_args(std::initializer_list<std::string> );


#endif // MATCH_H
