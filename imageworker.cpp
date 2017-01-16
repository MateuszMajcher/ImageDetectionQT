#include "imageworker.h"
#include "match.h"

ImageWorker::ImageWorker(QWidget *parent)
{
    listImages = new QListWidget(parent);
}

void ImageWorker::add(QString image) {
    QListWidgetItem *newItem = new QListWidgetItem(QIcon(image), "czxc");

    QString fullFilePath(image);
    QVariant fullFilePathData(fullFilePath);
    newItem->setData(Qt::UserRole, fullFilePathData);
    newItem->setText("Not Classsifed");

    listImages->addItem( newItem);
}

QListWidget* ImageWorker::getImage() {
    return listImages;
}


void ImageWorker::run() {
    for(int i = 0; i < listImages->count(); ++i)
    {
        QString msg;
        QListWidgetItem* item = listImages->item(i);
        QVariant data = item->data(Qt::UserRole);
        QString FullFilePath  = data.toString();
        QString output_pgm( QString::number(i) + ".pgm");
        QString output_key( QString::number(i) + ".key");
        if (!convertPGM(FullFilePath, output_pgm, msg)) {
           if (convertKEY(output_pgm, output_key, msg))
                mathImage(output_key);
           else
               error.showMessage("Image " + FullFilePath + " convert error\n" + msg);
        } else {
            error.showMessage("Image " + FullFilePath + " convert error\n" + msg);
        }
    }
}


bool ImageWorker::convertPGM(const QString &image, const QString &output_pgm, QString &error) {
    QString command = convert_process_pnm + image + " | ppmtopgm >" + output_pgm;
     runCommand(command, 5000, error);
    return fileExists(output_pgm);
}

bool ImageWorker::convertKEY(const QString &pgm, const QString &output_key, QString &error) {
    QString command = extract_process + " <" + pgm + "> " + output_key;
    runCommand(command, 10000, error);
    return fileExists(output_key);
}

void ImageWorker::runCommand(const QString &command, int timeoutMillisec, QString &error) {
    QProcess process;
    process.start("sh", QStringList() << "-c" << command);
    process.waitForFinished(timeoutMillisec);
    QString stdout = process.readAllStandardOutput();
    QString stderr = process.readAllStandardError();
    qDebug()<<command;
    qDebug()<< stdout;
    qDebug()<< stderr;
    error = stderr;
}

QString ImageWorker::getFileName(const QString &path) {
    QFileInfo fileInfo(path);
    return fileInfo.fileName();
}

bool ImageWorker::fileExists(QString path) {
    QFileInfo check_file(path);
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}
