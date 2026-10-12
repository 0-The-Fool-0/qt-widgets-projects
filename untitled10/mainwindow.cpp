#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Инициализация моделей
    dbModel = new QStandardItemModel(this);
    wcModel = new QStandardItemModel(this);
    violatorsModel = new QStandardItemModel(this);
    setupModels();

    // Подключение сигналов
    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::onLoadButtonClicked);
    connect(ui->sortUpButton, &QPushButton::clicked, this, &MainWindow::onSortUpButtonClicked);
    connect(ui->sortDownButton, &QPushButton::clicked, this, &MainWindow::onSortDownButtonClicked);
    connect(ui->findButton, &QPushButton::clicked, this, &MainWindow::onFindButtonClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::onSaveButtonClicked);

    logMessage("Application started. Click 'Load Data' to begin.");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupModels()
{
    // Настройка модели для базы данных фанатов
    dbModel->setHorizontalHeaderLabels({"Name", "Country", "Birth Year", "Detentions"});
    ui->databaseTableView->setModel(dbModel);
    ui->databaseTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Настройка модели для фанатов чемпионата мира
    wcModel->setHorizontalHeaderLabels({"Name", "Country", "Code", "Detentions"});
    ui->worldCupTableView->setModel(wcModel);
    ui->worldCupTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Настройка модели для главных нарушителей
    violatorsModel->setHorizontalHeaderLabels({"Name", "Country", "Birth Year", "Code", "Total Detentions"});
    ui->violatorsTableView->setModel(violatorsModel);
    ui->violatorsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::updateViews()
{
    // Обновление представления базы данных
    dbModel->removeRows(0, dbModel->rowCount());
    for (const auto& fan : dbManager.getFans()) {
        QList<QStandardItem*> items;
        items << new QStandardItem(fan.Name);
        items << new QStandardItem(fan.Country);
        items << new QStandardItem(QString::number(fan.BirthYear));
        items << new QStandardItem(QString::number(fan.Detentions));
        dbModel->appendRow(items);
    }

    // Обновление представления фанатов чемпионата
    wcModel->removeRows(0, wcModel->rowCount());
    for (const auto& fan : wcManager.getFans()) {
        QList<QStandardItem*> items;
        items << new QStandardItem(fan.Name);
        items << new QStandardItem(fan.Country);
        items << new QStandardItem(QString::number(fan.Code));
        items << new QStandardItem(QString::number(fan.Detentions));
        wcModel->appendRow(items);
    }

    // Обновление представления нарушителей
    violatorsModel->removeRows(0, violatorsModel->rowCount());
    for (const auto& violator : violatorsManager.getViolators()) {
        QList<QStandardItem*> items;
        items << new QStandardItem(violator.Name);
        items << new QStandardItem(violator.Country);
        items << new QStandardItem(QString::number(violator.BirthYear));
        items << new QStandardItem(QString::number(violator.Code));
        items << new QStandardItem(QString::number(violator.TotalDetentions));
        violatorsModel->appendRow(items);
    }
}

void MainWindow::logMessage(const QString& message)
{
    ui->logTextEdit->append(QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ") + message);
}

void MainWindow::onLoadButtonClicked()
{
    QString dbPath = QFileDialog::getOpenFileName(this, "Open Database Fans", "", "Text Files (*.txt)");
    QString wcPath = QFileDialog::getOpenFileName(this, "Open World Cup Fans", "", "Text Files (*.txt)");

    if (dbPath.isEmpty() || wcPath.isEmpty()) {
        logMessage("Operation canceled - files not selected");
        return;
    }

    QFile dbFile(dbPath);
    QFile wcFile(wcPath);

    if (!dbFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        logMessage("Error: Cannot open database file");
        return;
    }

    if (!wcFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        logMessage("Error: Cannot open world cup file");
        dbFile.close();
        return;
    }

    QTextStream dbStream(&dbFile);
    QTextStream wcStream(&wcFile);

    dbManager.GetFanats(dbStream);
    wcManager.GetFanats(wcStream);
    violatorsManager.createFrom(dbManager, wcManager);

    dbFile.close();
    wcFile.close();

    updateViews();
    logMessage("Data loaded successfully");
    logMessage(QString("Database fans: %1 records").arg(dbManager.getFans().size()));
    logMessage(QString("World Cup fans: %1 records").arg(wcManager.getFans().size()));
    logMessage(QString("Main violators: %1 records").arg(violatorsManager.getViolators().size()));
}

void MainWindow::onSortUpButtonClicked()
{
    int currentTab = ui->tabWidget->currentIndex();

    switch(currentTab) {
    case 0: // Database
        dbManager.SortUp();
        logMessage("Database fans sorted A-Z");
        break;
    case 1: // World Cup
        wcManager.SortUp();
        logMessage("World Cup fans sorted A-Z");
        break;
    case 2: // Violators
        violatorsManager.SortUp();
        logMessage("Main violators sorted A-Z");
        break;
    }

    updateViews();
}

void MainWindow::onSortDownButtonClicked()
{
    int currentTab = ui->tabWidget->currentIndex();

    switch(currentTab) {
    case 0: // Database
        dbManager.SortDown();
        logMessage("Database fans sorted Z-A");
        break;
    case 1: // World Cup
        wcManager.SortDown();
        logMessage("World Cup fans sorted Z-A");
        break;
    case 2: // Violators
        violatorsManager.SortDown();
        logMessage("Main violators sorted Z-A");
        break;
    }

    updateViews();
}

void MainWindow::onFindButtonClicked()
{
    QString name = ui->searchLineEdit->text().trimmed();
    if (name.isEmpty()) {
        logMessage("Please enter a name to search");
        return;
    }

    QString result;
    QTextStream stream(&result);

    dbManager.printFanInfo(name, stream);
    wcManager.printFanInfo(name, stream);
    violatorsManager.printFanInfo(name, stream);

    ui->logTextEdit->append(result);
    logMessage(QString("Search completed for: %1").arg(name));
}

void MainWindow::onSaveButtonClicked()
{
    QString savePath = QFileDialog::getSaveFileName(this, "Save Results", "", "Text Files (*.txt)");
    if (savePath.isEmpty()) {
        logMessage("Operation canceled - file not selected");
        return;
    }

    QFile file(savePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        logMessage("Error: Cannot open file for writing");
        return;
    }

    QTextStream out(&file);

    out << "=== DATABASE FANS ===\n";
    dbManager.PutFanats(out);

    out << "\n=== WORLD CUP FANS ===\n";
    wcManager.PutFanats(out);

    out << "\n=== MAIN VIOLATORS ===\n";
    violatorsManager.PutFanats(out);

    file.close();
    logMessage(QString("Results saved to: %1").arg(savePath));
}
