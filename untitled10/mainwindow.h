#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "absfanat.h"
#include "fanatdatabase.h"
#include "fanatsworldcup.h"
#include "mainviolators.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoadButtonClicked();
    void onSortUpButtonClicked();
    void onSortDownButtonClicked();
    void onFindButtonClicked();
    void onSaveButtonClicked();

private:
    Ui::MainWindow *ui;
    FanDatabaseManager dbManager;
    FanWorldCupManager wcManager;
    MainViolatorsManager violatorsManager;

    QStandardItemModel* dbModel;
    QStandardItemModel* wcModel;
    QStandardItemModel* violatorsModel;

    void setupModels();
    void updateViews();
    void logMessage(const QString& message);
};

#endif // MAINWINDOW_H
