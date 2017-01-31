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
QList<result> mathImage(const QString& key_file, const QString& database_file, const QString& image_list_file, QList<item> count_file, int gcs) {
    QList<Res> r;
    qDebug()<<"Debug:: "<<key_file;
    qDebug()<<"Debug:: "<<database_file;
    qDebug()<<"Debug:: "<<image_list_file;
    qDebug()<<"Debug:: "<<gcs;

    const int dim = 128;
    const int num_nbrs = 1000;
    vector<Res> match; //wyniki

    DistanceType distance_type = DistanceMin;
     bool normalize = true;

     /* Wczytanie drzewa*/
     printf("[VocabMatch] Reading database...\n");
     fflush(stdout);

     clock_t start = clock();
     VocabTree tree;
     tree.Read(database_file.toStdString().c_str());
     clock_t end = clock();
     printf("Reading datavase in %0.3fs\n", (double)(end-start) / CLOCKS_PER_SEC );

#if 1
     tree.Flatten();
#endif

     tree.SetDistanceType(distance_type);
     tree.SetInteriorNodeWeight(0, 0.0);

     /* Read database file*/
       std::vector<std::string> db_files;

        QFile file(image_list_file);
           if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
               qDebug()<<"cant open: "<<image_list_file;
           } else {
               QTextStream in(&file);
                  QString line = in.readLine();
                  while (!line.isNull()) {
                      qDebug()<<line;
                      db_files.push_back(line.toStdString());
                      line = in.readLine();
                  }
              file.close();
           }

            //read count file

           Range count(count_file.size()); //ilosc klas
           QListIterator<item> i(count_file);
           int row = 0;
           while (i.hasNext()) {
               item temp = i.next();
               count.name[row] = temp.name.toStdString();
               count.count_train[row] = temp.count;

               row++;
           }
            qDebug()<<count.count_query.size();
           /* Parse count File*/
            Range ranges = QueryRangesTemp(count_file.size(), count);
            int num_db_images = db_files.size();


            printf("[VocabMatch] Read %d database images\n", num_db_images);
            /* Now score each query keyfile */
            // printf("[VocabMatch] Scoring %d query images...\n", num_query_images);

            float *scores = new float[num_db_images];
            double *scores_d = new double[num_db_images];
            int *perm = new int[num_db_images];



            // for (int i = 0; i < num_query_images; i++) {
            start = clock();

            /* Clear scores */
            for (int j = 0; j < num_db_images; j++)
                scores[j] = 0.0;

            unsigned char *keys;
            int num_keys;
            //wczytanie pliku key
            keys = ReadKeys(key_file.toStdString().c_str(), dim, num_keys);
            //punktowanie
            clock_t start_score = clock();
            double mag = tree.ScoreQueryKeys(num_keys, normalize, keys, scores);
            clock_t end_score = end = clock();

            printf("[VocabMatch] Scored image %s in %0.3fs "
                   "( %0.3fs total, mag = %0.3f )\n",
                   key_file.toStdString().c_str(),
                   (double) (end_score - start_score) / CLOCKS_PER_SEC,
                   (double) (end - start) / CLOCKS_PER_SEC, mag);

            //zapis wynikow do tablicy
            for (int j = 0; j < num_db_images; j++) {
                scores_d[j] = (double) scores[j];
            }

            qsort_descending();
            qsort_perm(num_db_images, scores_d, perm);

            for (int j = 0; j < num_db_images; j++) {
               // qDebug()<<scores_d[j] << " "<<perm[j];
            }

            int top = MIN(num_nbrs, num_db_images);

            /* jesli brak statystyki*/
            if (gcs == 0) {
                gcs = top;
            }

            /* analiza ze statystyka */
            match.resize(gcs);
            //double sum[25] = {0.0};
            for (int j = 0; j < gcs; j++) {
                match[j].res = CheckClassQuery(ranges.count_train, perm[j], count_file.size());
                match[j].val = scores_d[j];
                qDebug()<<"-"<<match[j].res<<" = "<<perm[j]<<" "<<match[j].val<<" > "<< QString::fromStdString(count.name.at(match[j].res-1));

            }

            vector<Res> tmp = GroupResult(match);
            QList<result> res;


            for (auto const& value: tmp) {
               // qDebug()<<"result: "<<QString::fromStdString(count.name[value.res-1])<<" "<<value.val;
               result tmp;
               tmp.name = QString::fromStdString(count.name[value.res-1]);
               tmp.score = value.val;
               res.append(tmp);
           }

    return res;
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
    for (unsigned int i = 1; i < count.count_query.size()+1; i++) {

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

/* Read keypoints from the given file pointer and return the list of
 * keypoints.  The file format starts with 2 integers giving the total
 * number of keypoints and the size of descriptor vector for each
 * keypoint (currently assumed to be 128). Then each keypoint is
 * specified by 4 floating point numbers giving subpixel row and
 * column location, scale, and orientation (in radians from -PI to
 * PI).  Then the descriptor vector for each keypoint is given as a
 * list of integers in range [0,255]. */
int ReadKeyss(FILE *fp, short int **keys, keypt_t **info)
{
    int i, num, len;

    std::vector<Keypoint *> kps;

    if (fscanf(fp, "%d %d", &num, &len) != 2) {
    printf("Invalid keypoint file\n");
    return 0;
    }

    if (len != 128) {
    printf("Keypoint descriptor length invalid (should be 128).");
    return 0;
    }

    *keys = new short int[128 * num];

    if (info != NULL)
        *info = new keypt_t[num];

    short int *p = *keys;
    for (i = 0; i < num; i++) {
    /* Allocate memory for the keypoint. */
    // short int *d = new short int[128];
    float x, y, scale, ori;

    if (fscanf(fp, "%f %f %f %f\n", &y, &x, &scale, &ori) != 4) {
        printf("Invalid keypoint file format y.");
        return 0;
    }

        if (info != NULL) {
            (*info)[i].x = x;
            (*info)[i].y = y;
            (*info)[i].scale = scale;
            (*info)[i].orient = ori;
        }

    char buf[1024];
    for (int line = 0; line < 7; line++) {
        fgets(buf, 1024, fp);

        if (line < 6) {
        sscanf(buf,
               "%hu %hu %hu %hu %hu %hu %hu %hu %hu %hu "
               "%hu %hu %hu %hu %hu %hu %hu %hu %hu %hu",
               p+0, p+1, p+2, p+3, p+4, p+5, p+6, p+7, p+8, p+9,
               p+10, p+11, p+12, p+13, p+14,
               p+15, p+16, p+17, p+18, p+19);

        p += 20;
        } else {
        sscanf(buf,
               "%hu %hu %hu %hu %hu %hu %hu %hu",
               p+0, p+1, p+2, p+3, p+4, p+5, p+6, p+7);
        p += 8;
        }
    }
    }

    return num; // kps;
}
