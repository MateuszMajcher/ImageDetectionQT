#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctime>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
#include <typeinfo>
#include "VocabTree.h"
#include "keys2.h"
#include <QList>
#include "defines.h"
#include "qsort.h"
#include "match.h"




/*
 * Dopasowywanie obrazu
 *
 * inputs:
 *  database_file     :sciezka do bazy
 *  imageList_file    :sciezka do pliku z obrazami uczacymi
 *
 * */
QList<Res> mathImage(const QString& key_file, const QString& database_file, const QString& image_list_file, int gcs) {
    QList<Res> r;
    qDebug()<<"Debug:: "<<key_file;
    qDebug()<<"Debug:: "<<database_file;
    qDebug()<<"Debug:: "<<image_list_file;
    qDebug()<<"Debug:: "<<gcs;

    DistanceType distance_type = DistanceMin;
     bool normalize = true;



     /* Wczytanie drzewa*/
     printf("[VocabMatch] Reading database...\n");
     fflush(stdout);

     clock_t start = clock();
     VocabTree tree;
     tree.Read("a.db");




    return r;
}

/* Read in a set of keys from a file
 *
 * Inputs:
 *   keyfile      : file from which to read keys
 *   dim          : dimensionality of descriptors
 *
 * Outputs:
 *   num_keys_out : number of keys read
 *
 * Return value   : pointer to array of descriptors.  The descriptors
 *                  are concatenated together in one big array of
 *                  length num_keys_out * dim
 */
unsigned char *ReadKeys(const char *keyfile, int dim, int &num_keys_out)
{
    short int *keys;
    keypt_t *info = NULL;
    int num_keys = ReadKeyFile(keyfile, &keys, &info);

    unsigned char *keys_char = new unsigned char[num_keys * dim];

    for (int j = 0; j < num_keys * dim; j++) {
        keys_char[j] = (unsigned char) keys[j];
    }

    delete [] keys;

    if (info != NULL)
        delete [] info;

    num_keys_out = num_keys;

    return keys_char;
}

/* Read name of model  from a file
 *
 * Inputs:
 *   modelCount      : number of classes
 *   modelCountFile  : name file
 *
 *
 * Return value   : intervals following classes
 */
Range QueryRangesTemp(int modelCount, Range count) {



    Range ranges(modelCount);
    for (unsigned int i = 1; i < count.count_query.size(); i++) {

        int tmp_train = 0;
        for (unsigned int j = 0; j < i; j++) {
            tmp_train += count.count_train[j];
        }
        ranges.count_train[i - 1] = tmp_train;
        ranges.name[i - 1] = count.name[i-1];
        std::cout << "ranges "<<i<<" : "  << " train: " << ranges.count_train[i - 1] <<" model "<<ranges.name[i - 1]<< endl;
    }


    return ranges;

}

vector<Res> GroupResult(vector<Res> res) {
    vector<Res> tmp;								//pomacniczy wektor przechowujacy pogrupowane wyniki
    tmp.push_back(res[0]);							//dodnie pierwszego

    for (unsigned int i = 1; i < res.size(); i++)			//przetwarzanie wszystkich pozycji (najlepszych)
    {
        bool isVal = false;							//pamietanie czy
        int localIdx = -1;
        for (unsigned int j = 0; j < tmp.size(); j++)
            if (res[i].res == tmp[j].res)
            {
                isVal = true;
                localIdx = j;
            }

        if (!isVal){
            //std::cout<<"not val"<<res[i].val<<" "<<res[i].res<<std::endl;
        tmp.push_back(res[i]);}
        else
        {
            //std::cout<<"lokalidx: "<<localIdx<<" val: "<<res[i].val<<" res: "<<res[i].res<<std::endl;
            tmp[localIdx].val += res[i].val;
            //std::cout<<"sum: "<<tmp[localIdx].val<<std::endl;

        }

    }

    sort(tmp.begin(), tmp.end(), [](Res d1, Res d2) { return d1.val > d2.val; });
    return tmp;
}

int CheckClassQuery(vector<int> ranges, int idx, int classCount) { //klasyfikacja na podstawie przediałów
    for (unsigned int i = 0; i < ranges.size(); i++)
        if (idx < ranges[i])
            return i+1;

    if (idx > ranges[ranges.size() - 1]);
    return classCount;
}

int BasifyFilename(const char *filename, char *base)
{
    strcpy(base, filename);
    base[strlen(base) - 4] = 0;

    return 0;
}

void split(const std::string &s, char delim, vector<std::string> &e) {
    stringstream ss;
    ss.str(s);
    string v;
    while (getline(ss, v, delim)) {
        e.push_back(v);
    }
}

vector<std::string> split(const std::string &s, char delim) {
    vector<std::string> e;
    split(s, delim, e);
    return e;
}

void show_args(std::initializer_list<std::string> args) {
    for (auto a : args) printf("%s \n", a.c_str());
}
