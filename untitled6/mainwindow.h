// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "med.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadDrug();
    void loadPrice();
    void combineData();
    void sortData();
    void findExpired();
    void saveText();
    void saveBinary();

private:
    Ui::MainWindow *ui;
    MED<Drug> M1;
    MED<Price> K1;
    MED<Med> R;
    void displayMedTable();
};

#endif // MAINWINDOW_H
