#include "imageworker.h"


ImageWorker::ImageWorker(QWidget *parent)
{
    listImages = new QListWidget(parent);

}

void ImageWorker::add(QString image) {
    QListWidgetItem *newItem = new QListWidgetItem(QIcon(image), "czxc");
    //ustawienie oryginalnej sciezki do obrazu
    QString fullFilePath(image);
    QVariant fullFilePathData(fullFilePath);
    newItem->setData(Qt::UserRole, fullFilePathData);
    newItem->setText("Not Classsifed");
    //dodanie obrazu do galerii
    listImages->addItem( newItem);
}

QListWidget* ImageWorker::getImage() {
    return listImages;
}

void ImageWorker::run() {
    for(int i = 0; i < listImages->count(); ++i)
    {
        QString msg;
        QListWidgetItem* it = listImages->item(i);
        QVariant data = it->data(Qt::UserRole);
        QString FullFilePath  = data.toString();

        //ustawienie sciezek wyjsciowych dla pgm oraz key
        QString output_pgm( QString::number(i) + ".pgm");
        QString output_key( QString::number(i) + ".key");

        //przetwarzanie
        if (convertPGM(FullFilePath, output_pgm, msg)) {
           if (convertKEY(output_pgm, output_key, msg)) {
               qApp->processEvents();
                  QList<result> result;
                   result = mathImage(output_key, database_file, image_list_file, count_file, gsc);
                   it->setText(result.at(0).name + " : " + QString::number(percentCalculate(result)) + "%");
                   for (auto const& v: result)
                      qDebug()<<"result: "<<v.name<<" "<<v.score;
                   emit progress();
                   deleteFile(output_key);
                   deleteFile(output_pgm);

           }
           else
               error.showMessage("Image " + FullFilePath + " convert error\n" + msg);
        } else {
            error.showMessage("Image " + FullFilePath + " convert error\n" + msg);
        }
    }
}

double ImageWorker::percentCalculate(QList<result> res) {
       double sum = 0.0;
       foreach (result s, res) {
           sum += s.score;
       }
       qDebug()<<sum;
       double per = res.at(0).score * 100 / sum;
       return per;
}

void ImageWorker::deleteFile(QString path) {
    QFile file (path);
    file.remove();
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

void ImageWorker::setCountFile(QList<item> map) {
    count_file = map;
}

void ImageWorker::setDatabaseFile( QString file) {
    database_file = file;
}

void ImageWorker::setImageListFile( QString file) {
    image_list_file  = file;
}

void ImageWorker::setGSC(int g) {
    gsc = g;
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
