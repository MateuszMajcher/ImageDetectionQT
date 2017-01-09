#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QErrorMessage>
#include <QMap>

class QAction;
class QListWidget;
class QTableWidget;
class QPushButton;
class QLineEdit;
class QMenu;
class QTextEdit;
class QColor;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();

private slots:
    void newMatch(); //Wczytanie nowych obrazów
    void loadCountFile(); //Wczytanie pliku count
    void clear();
    void runMatch();
    void save();
    void print();
    void undo();
    void about();
    void insertCustomer(const QString& customer);
    void addParagraph(const QString &paragraph);

private:
    void createActions();
    void createStatusBar();
    void createDockWindows();

    QErrorMessage msg;

    QListWidget *listImages;
    QMap<QString, int> count_file;

    //panel
    QWidget *settingsPanel;
    QTableWidget *countTable;

    //Settings panel
    QPushButton *loadCountFile_btn;
    QLineEdit *countFile_edit;



    QMenu *viewMenu;
};

#endif // MAINWINDOW_H
