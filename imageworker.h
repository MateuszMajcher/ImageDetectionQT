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
class QListWidget;
class QFileInfo;
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



private:

    QListWidget *listImages;
    QString convert_process_pnm = "jpegtopnm ";
    QString extract_process = "./sift";
    QErrorMessage error;

};
#endif // IMAGEWORKER_H
