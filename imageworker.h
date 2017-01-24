#ifndef IMAGEWORKER_H
#define IMAGEWORKER_H
#include <QWidget>
#include <QMutex>
#include <QListWidget>>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QErrorMessage>
#include <QApplication>
#include "match.h"

class QListWidget;
class QFileInfo;
class item;


class ImageWorker : public QWidget {
    Q_OBJECT

public:
    ImageWorker(QWidget* parent);
    void add(QString image);
    QListWidget* getImage();
    void run();
    bool convertPGM(const QString& image, const QString &output_pgm, QString &error);
    bool convertKEY(const QString& pgm, const QString &output_key, QString &error);

    //pomocnicze
    void runCommand(const QString& command, int timeoutMillisec, QString &error);
    QString getFileName(const QString& path);
    bool fileExists(QString path);

    void setCountFile(QList<item> count_file);
    void setDatabaseFile( QString file);
    void setImageListFile( QString file);
    void setGSC(int);

private:

    QListWidget *listImages;
    QString convert_process_pnm = "jpegtopnm ";
    QString extract_process = "./sift";
    QErrorMessage error;

    QList<item> count_file;
    QString database_file;
    QString image_list_file;
    int gsc;

};
#endif // IMAGEWORKER_H
