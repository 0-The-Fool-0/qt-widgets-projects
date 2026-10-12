#include "mainwindow.h"
#include <QKeyEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QPixmap>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUi() {
    setWindowTitle("Телефонная книга");
    resize(900, 700);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    lineEdit = new QLineEdit();
    lineEdit->setPlaceholderText("Введите размер массива");

    btnLoad = new QPushButton("Загрузить");
    btnAdd = new QPushButton("Добавить");
    btnDelete = new QPushButton("Удалить");
    btnSort = new QPushButton("Сортировать");

    comboBox = new QComboBox();
    comboBox->addItems({"Фамилия", "Адрес"});

    tableWidget = new QTableWidget(0, 3);
    tableWidget->setHorizontalHeaderLabels({"Адрес", "Телефон", "Фамилия"});

    listWidget = new QListWidget();

    imageLabel = new QLabel();
    imageLabel->setPixmap(QPixmap(":/img/photo.jpg").scaled(200, 200, Qt::KeepAspectRatio));

    QHBoxLayout *controlsLayout = new QHBoxLayout();
    controlsLayout->addWidget(lineEdit);
    controlsLayout->addWidget(btnLoad);
    controlsLayout->addWidget(btnAdd);
    controlsLayout->addWidget(btnDelete);
    controlsLayout->addWidget(btnSort);
    controlsLayout->addWidget(comboBox);

    mainLayout->addLayout(controlsLayout);
    mainLayout->addWidget(tableWidget);
    mainLayout->addWidget(listWidget);
    mainLayout->addWidget(imageLabel);

    setCentralWidget(centralWidget);

    connect(btnLoad, &QPushButton::clicked, this, &MainWindow::loadOwners);
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::addOwner);
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::deleteOwner);
    connect(btnSort, &QPushButton::clicked, this, &MainWindow::sortOwners);
    connect(comboBox, &QComboBox::currentTextChanged, this, &MainWindow::sortList);
}

void MainWindow::loadOwners() {
    int n = lineEdit->text().toInt();
    owners.clear();
    tableWidget->setRowCount(0);
    listWidget->clear();

    for (int i = 0; i < n; ++i) {
        Owner o = {"Город" + QString::number(i), 375291112233 + i, "Фамилия" + QString::number(i)};
        owners.push_back(o);
        addToTable(o);
        listWidget->addItem(o.surname);
    }
}

void MainWindow::addOwner() {
    Owner o = {"Минск", 375297654321, "Иванов"};
    owners.push_back(o);
    addToTable(o);
    listWidget->addItem(o.surname);
}

void MainWindow::deleteOwner() {
    int row = tableWidget->currentRow();
    if (row >= 0) {
        tableWidget->removeRow(row);
        listWidget->takeItem(row);
        if (row < owners.size()) owners.remove(row);
    }
}

void MainWindow::sortOwners() {
    quickSort(owners, 0, owners.size() - 1);
    tableWidget->setRowCount(0);
    listWidget->clear();
    for (const Owner &o : owners) {
        addToTable(o);
        listWidget->addItem(o.surname);
    }
}

void MainWindow::addToTable(const Owner &o) {
    int row = tableWidget->rowCount();
    tableWidget->insertRow(row);
    tableWidget->setItem(row, 0, new QTableWidgetItem(o.address));
    tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(o.phone)));
    tableWidget->setItem(row, 2, new QTableWidgetItem(o.surname));
}

void MainWindow::quickSort(QVector<Owner> &arr, int left, int right) {
    int i = left, j = right;
    Owner pivot = arr[(left + right) / 2];

    while (i <= j) {
        while (arr[i].address < pivot.address) ++i;
        while (arr[j].address > pivot.address) --j;
        if (i <= j) {
            qSwap(arr[i], arr[j]);
            ++i; --j;
        }
    }

    if (left < j) quickSort(arr, left, j);
    if (i < right) quickSort(arr, i, right);
}

void MainWindow::sortList() {
    listWidget->sortItems();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Delete) deleteOwner();
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);
    menu.addAction("Удалить", this, SLOT(deleteOwner()));
    menu.exec(event->globalPos());
}
