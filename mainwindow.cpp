#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QtWidgets>


MainWindow::MainWindow() {

    worker = new ImageWorker(this);

   // listImages = new QListWidget(this);
    setCentralWidget(worker->getImage());
    pal = palette();
    createActions();
    createStatusBar();
    createDockWindows();

    setWindowTitle(tr("SIFT"));
    setUnifiedTitleAndToolBarOnMac(true);

    setMinimumSize(480, 320);
    resize(760, 480);

}

void MainWindow::newMatch() {
    clear();
    QStringList fileName = QFileDialog::getOpenFileNames(this,tr("Open Image"), "/home/mateusz/image", tr("Image Files(*.png *.jpg *.bmp *.avi *.gif)"));

    worker->getImage()->setViewMode(QListWidget::IconMode);
    worker->getImage()->setIconSize(QSize(200, 200));
    worker->getImage()->setResizeMode(QListWidget::Adjust);

    QStringListIterator it(fileName);
    while (it.hasNext()) {
        worker->add(it.next());
    }

}

void MainWindow::loadCountFile()
{
    QString countFileName = QFileDialog::getOpenFileName(this, tr("Open count file"), t_path, "Text files (*.txt)");
    count_file.clear();
    QFile inputFile(countFileName);
    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        bool success = true;
        while (!in.atEnd()) {

            QStringList tokens = in.readLine().split(":", QString::SkipEmptyParts);
            qDebug()<<tokens;
            if (tokens.size() == 2) {
                bool is_number = false;
                int v = tokens.at(1).toInt(&is_number);
                ;
                if (is_number) {
                    item i;
                    i.name = tokens.at(0);
                    i.count = v;
                    count_file.append(i);

                }
                else {
                    success = false;
                }
            }
            else {
                success = false;
            }
        }
        inputFile.close();

        if (success) {
            worker->setCountFile(count_file);
            countFile_edit->setText(countFileName);
            //ustawienie ilosci wierszy w tabeli
            countTable->setRowCount(count_file.size());

            QListIterator<item> i(count_file);
            int row = 0;
            while (i.hasNext()) {

               item u =  i.next();

                countTable->setItem(row, 0, new QTableWidgetItem(u.name));
                countTable->setItem(row, 1, new QTableWidgetItem(QString::number(u.count)));
                row++;
            }
        }
        else {
            count_file.clear();
            msg.showMessage("plik " + countFileName + " jest niepoprawny");
        }
    }
}

void MainWindow::loadDatabaseFile() {
    QString path;
    if (database_file.isEmpty())
        path = t_path;
    else
        path = database_file;

    database_file = QFileDialog::getOpenFileName(this, tr("Open database file"), path, "Database files (*.db)");
    worker->setDatabaseFile(database_file);
    database_edit->setText(database_file);
}

void MainWindow::loadImageListFile() {
    QString path;
    if (image_list_file.isEmpty())
        path = t_path;
    else
        path = image_list_file;

    image_list_file = QFileDialog::getOpenFileName(this, tr("Open file with list image using to learn"), path, "Text file (*.txt)");
    worker->setImageListFile(image_list_file);
    imageList_edit->setText(image_list_file);
}

void MainWindow::setGSC(const QString &number) {
    gsc = number.toInt();
    worker->setGSC(gsc);
}

void MainWindow::clear() {
    worker->getImage()->clear();
}

//start przetwarzania
void MainWindow::runMatch() {
    showProgressBar();
    steps = 0;
    progressBar->setMinimum(0);
    progressBar->setMaximum(worker->getImage()->count());

    qApp->processEvents();
    worker->run();
    hideProgressBar();
}

//ustawienie wartosci dla progressBar
void MainWindow::imageTask() {
    steps++;
 progressBar->setValue(steps);


 if (steps > progressBar->maximum()) {
       steps = 0;
 }

}

void MainWindow::showProgressBar() {
      dockProgressBar->show();
}

void MainWindow::hideProgressBar() {
      dockProgressBar->hide();
}

//zmiana tla panelu ustawien jesli wszystkie dane zoztaly uzupelnione
void MainWindow::checkCorrect() {
    if (!countFile_edit->text().isEmpty() &&
            !database_edit->text().isEmpty() &&
              !imageList_edit->text().isEmpty() &&
                  !gsc_edit->text().isEmpty()) {
                        pal.setColor(QPalette::Background, QColor(0, 255, 0, 127));
                        runAct->setEnabled(true);

    } else {
        pal.setColor(QPalette::Background, QColor(255, 0, 0, 127));


    }

    settingsPanel->setAutoFillBackground(true);
    settingsPanel->setPalette(pal);
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("Klasyfikacja modeli samochodów"),
                      tr("Analiza i wizualizacja danych\n"
                         "Wykonali\n"
                         "\n"
                         "Mateusz Majcher\n"
                         "Ariel Trybek"));

}

//gorny panle akcje
void MainWindow::createActions() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));

    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newLetterAct = new QAction(newIcon, tr("&New match"), this);
    newLetterAct->setShortcuts(QKeySequence::New);
    newLetterAct->setStatusTip(tr("Create a new form letter"));
    connect(newLetterAct, &QAction::triggered, this, &MainWindow::newMatch);
    fileMenu->addAction(newLetterAct);
    fileToolBar->addAction(newLetterAct);

    QAction *clearAct = new QAction(QIcon(":/images/close.png"), tr("&clear"), this);
    clearAct->setShortcut(QKeySequence::Cancel);
    clearAct->setStatusTip(tr("Clear all images"));
    connect(clearAct, &QAction::triggered, this, &MainWindow::clear);
    fileMenu->addAction(clearAct);
    fileToolBar->addAction(clearAct);


    runAct = new QAction(QIcon(":/images/run.png"), tr("&Run"), this);
    runAct->setStatusTip(tr("Run match"));
    connect(runAct, &QAction::triggered, this, &MainWindow::runMatch);
    fileMenu->addAction(runAct);
    fileToolBar->addAction(runAct);
    runAct->setEnabled(false);

    viewMenu = menuBar()->addMenu(tr("View"));
    menuBar()->addSeparator();

    QMenu *helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(tr("O Aplikacji"), this, &MainWindow::about);
    helpMenu->addAction(tr("Wersja Qt"), this, &QApplication::aboutQt);

}

void MainWindow::createStatusBar() {
    statusBar()->showMessage(tr("Ready"));
}

//utworzenie bocznych doków
void MainWindow::createDockWindows() {

    //Panel sterowania
    QDockWidget *dock = new QDockWidget(tr("Settings panel"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    pal.setColor(QPalette::Background, QColor(255, 0, 0, 127));
    settingsPanel = new QWidget(dock);
    settingsPanel->setAutoFillBackground(true);
    settingsPanel->setPalette(pal);

    QFormLayout *layout = new QFormLayout;
    //ladowanie count file
    loadCountFile_btn = new QPushButton("Load count file");
    countFile_edit = new QLineEdit;
    layout->addRow(loadCountFile_btn, countFile_edit);
    //ladowanie bazy danych
    loadDatabaseFile_btn = new QPushButton("Load database");
    database_edit = new QLineEdit;
    layout->addRow(loadDatabaseFile_btn, database_edit);
    //ladowanie plika z lista obrazow
    loadImageListFile_btn = new QPushButton("load learn image");
    imageList_edit = new QLineEdit;
    layout->addRow(loadImageListFile_btn, imageList_edit);
    //gsc
    gsc_label = new QLabel("best matches");
    gsc_edit = new QLineEdit;
    gsc_edit->setValidator(new QIntValidator(0, 1000, this));
    layout->addRow(gsc_label, gsc_edit);


    settingsPanel->setLayout(layout);
    dock->setWidget(settingsPanel);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

    //syganały
    connect(loadCountFile_btn, SIGNAL(clicked()), this, SLOT(loadCountFile()));
    connect(loadDatabaseFile_btn, SIGNAL(clicked()), this, SLOT(loadDatabaseFile()));
    connect(loadImageListFile_btn, SIGNAL(clicked()), this, SLOT(loadImageListFile()));
    connect(gsc_edit, SIGNAL(textChanged(QString)), this, SLOT(setGSC(const QString &)));

    //checkCorrect
    connect(countFile_edit, SIGNAL(textChanged(QString)), this, SLOT(checkCorrect()));
    connect(database_edit, SIGNAL(textChanged(QString)), this, SLOT(checkCorrect()));
    connect(imageList_edit, SIGNAL(textChanged(QString)), this, SLOT(checkCorrect()));
    connect(gsc_edit, SIGNAL(textChanged(QString)), this, SLOT(checkCorrect()));

    // Panel z tabela
    dock = new QDockWidget(tr("Count file"), this);
    countTable = new QTableWidget(1,2,this);
    QTableWidgetItem *classHeader = new QTableWidgetItem(tr("Klasa"));
    QTableWidgetItem *countHeader = new QTableWidgetItem(tr("ilosc"));
    countTable->setHorizontalHeaderItem(0, classHeader);
    countTable->setHorizontalHeaderItem(1, countHeader);
    countTable->horizontalHeader()->setStretchLastSection(true);

    dock->setWidget(countTable);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());


    //Panel postepu
    dockProgressBar = new QDockWidget(tr("Progress"), this);

    progressBar = new QProgressBar();
    dockProgressBar->setWidget(progressBar);
    dockProgressBar->hide();
    connect(worker, SIGNAL(progress()),
        this, SLOT(imageTask()));

    addDockWidget(Qt::BottomDockWidgetArea, dockProgressBar);
    viewMenu->addAction(dockProgressBar->toggleViewAction());
}

