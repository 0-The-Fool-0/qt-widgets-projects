// mainwindow.cpp
#include "mainwindow.h"
#include "med.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->actionLoad_Drug, &QAction::triggered, this, &MainWindow::loadDrug);
    connect(ui->actionLoad_Price, &QAction::triggered, this, &MainWindow::loadPrice);
    connect(ui->actionCombine_Data, &QAction::triggered, this, &MainWindow::combineData);
    connect(ui->actionSort_Data, &QAction::triggered, this, &MainWindow::sortData);
    connect(ui->actionFind_Expired, &QAction::triggered, this, &MainWindow::findExpired);
    connect(ui->actionSave_Text, &QAction::triggered, this, &MainWindow::saveText);
    connect(ui->actionSave_Binary, &QAction::triggered, this, &MainWindow::saveBinary);

    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels({"Name", "Date", "Section", "Count", "Price"});
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::displayMedTable() {
    ui->tableWidget->setRowCount(R.GetCount());
    for (int i = 0; i < R.GetCount(); ++i) {
        const Med &m = R[i];
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(m.name));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString("%1.%2").arg(m.myDate.month, 2, 10, QChar('0')).arg(m.myDate.year)));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(m.section));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(m.count)));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(m.pr, 'f', 2)));
    }
}

void MainWindow::loadDrug() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Drug File");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {
            QDataStream in(&file);
            M1.ReadData(in);
            QMessageBox::information(this, "Success", "Drug data loaded.");
        }
    }
}

void MainWindow::loadPrice() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Price File");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {
            QDataStream in(&file);
            K1.ReadData(in);
            QMessageBox::information(this, "Success", "Price data loaded.");
        }
    }
}

void MainWindow::combineData() {
    QMap<QString, Med> map;
    for (int i = 0; i < M1.GetCount(); ++i) {
        const Drug &d = M1[i];
        Med m;
        m.name = d.name;
        m.myDate = d.myDate;
        m.section = d.section;
        m.count = d.count;
        m.pr = 0;
        map[d.name] = m;
    }
    for (int i = 0; i < K1.GetCount(); ++i) {
        const Price &p = K1[i];
        if (map.contains(p.name)) {
            if (map[p.name].myDate < p.myDate) {
                map[p.name].myDate = p.myDate;
            }
            map[p.name].pr = p.pr;
        } else {
            Med m;
            m.name = p.name;
            m.myDate = p.myDate;
            m.section = "Unknown";
            m.count = 0;
            m.pr = p.pr;
            map[p.name] = m;
        }
    }

    QVector<Med> result = map.values().toVector();
    if (result.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Нет данных для объединения");
        return;
    }

    R = MED<Med>(result.size());

    for (int i = 0; i < result.size(); ++i)
        R[i] = result[i];

    displayMedTable();
    QMessageBox::information(this, "Combined", "Data combined into MED<Med>.");
}

void MainWindow::sortData() {
    std::sort(&R[0], &R[0] + R.GetCount(), [](const Med &a, const Med &b) {
        return a.name > b.name;
    });
    displayMedTable();
    QMessageBox::information(this, "Sorted", "Data sorted in reverse alphabetical order.");
}

void MainWindow::findExpired() {
    bool ok;
    QString input = QInputDialog::getText(this, "Enter Date", "MM.YYYY:", QLineEdit::Normal, "06.2025", &ok);
    if (ok && !input.isEmpty()) {
        QStringList parts = input.split(".");
        if (parts.size() != 2) {
            QMessageBox::warning(this, "Error", "Введите дату в формате ММ.ГГГГ");
            return;
        }
        MyDate now = { parts[0].toInt(), parts[1].toInt() };
        QString result;
        QTextStream stream(&result);  // создаем поток один раз

        for (int i = 0; i < R.GetCount(); ++i) {
            if (R[i].myDate < now) {
                stream << R[i] << "\n";  // используем перегруженный оператор
            }
        }
        if (result.isEmpty())
            result = "Нет истекших лекарств на дату: " + input;
        QMessageBox::information(this, "Expired", result);
    }
}


void MainWindow::saveText() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save to Text File");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            R.PutData(out);
            QMessageBox::information(this, "Saved", "Saved to text file.");
        }
    }
}

void MainWindow::saveBinary() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save to Binary File");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            QDataStream out(&file);
            R.WriteData(out);
            QMessageBox::information(this, "Saved", "Saved to binary file.");
        }
    }
}
