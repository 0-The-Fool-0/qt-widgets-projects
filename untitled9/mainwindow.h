#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "med_template_class.h"

class QTextEdit;
class QPushButton;
class QLineEdit;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStep1a_LoadDrugFile();
    void onStep1b_LoadPriceFile();
    void onStep2_MergeData();
    void onStep3_SortData();
    void onStep4_SaveResults();
    void onStep5_FindExpired();

private:
    void setupUi();
    void createInitialFiles();
    void updateButtonStates();

    QTextEdit *m_logDisplay;
    QPushButton *m_step1aButton;
    QPushButton *m_step1bButton;
    QPushButton *m_step2Button;
    QPushButton *m_step3Button;
    QPushButton *m_step4Button;
    QPushButton *m_findButton;
    QLineEdit *m_dateInput;
    QLabel *m_statusLabel;

    MED<Drug> m_drugs;
    MED<Price> m_prices;
    MED<Med> m_meds;

    bool m_drugFileLoaded;
    bool m_priceFileLoaded;
};

#endif // MAINWINDOW_H
