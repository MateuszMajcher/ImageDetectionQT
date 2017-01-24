#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QErrorMessage>
#include <QMap>
#include "imageworker.h"


class QAction;
class QListWidget;
class QTableWidget;
class QPushButton;
class QLineEdit;
class QMenu;
class QTextEdit;
class QColor;
class QProgressBar;
class QLabel;
class ImageWorker;
class item;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();

private slots:
    void newMatch(); //Wczytanie nowych obrazów
    void loadCountFile(); //Wczytanie pliku count
    void loadDatabaseFile(); //wczytanie bazy danych
    void loadImageListFile(); // wczytanie listy obrazow uzytych do uczenia
    void clear(); //Wyczyszczenie obrazow
    void runMatch(); //rozpoznawanie
    void about(); //o aplikacji
    void setGSC(const QString &);


private:
    void createActions();
    void createStatusBar();
    void createDockWindows();
    void showProgressBar();

    QErrorMessage msg;

    ImageWorker* worker;

    QList<item> count_file;
    QString database_file;
    QString image_list_file;
    int gsc;

    //panel
    QWidget *settingsPanel;
    QTableWidget *countTable;

    //Settings panel
    QPushButton *loadCountFile_btn;
    QLineEdit *countFile_edit;
    QPushButton *loadDatabaseFile_btn;
    QLineEdit *database_edit;
    QPushButton *loadImageListFile_btn;
    QLineEdit *imageList_edit;
    QLabel *gsc_label;
    QLineEdit *gsc_edit;

    //Progress bar panel
    QDockWidget *dockProgressBar;
    QProgressBar *progressBar;

    //menu
    QMenu *viewMenu;

    QString t_path = "/home/mateusz/workspace/dr_cars/library";
};

#endif // MAINWINDOW_H
