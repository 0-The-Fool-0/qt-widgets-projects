#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QList>
#include <QDebug>
#include <unordered_map>
#include <QGroupBox>
bool compareMedsByNameDesc(const Med& a, const Med& b)
{
    return a.name > b.name;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_drugFileLoaded(false), m_priceFileLoaded(false)
{
    setupUi();
    createInitialFiles();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUi()
{
    setWindowTitle("Экр");
    resize(900, 700);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    // --- Загрузка файлов ---
    QGroupBox *loadGroup = new QGroupBox("Загрузка файлов", this);
    QHBoxLayout *loadLayout = new QHBoxLayout(loadGroup);
    loadLayout->setSpacing(20);
    m_step1aButton = new QPushButton("Загрузить Drug (*.bin)", this);
    m_step1bButton = new QPushButton("Загрузить Price (*.bin)", this);
    loadLayout->addWidget(m_step1aButton);
    loadLayout->addWidget(m_step1bButton);

    // --- Обработка данных ---
    QGroupBox *processGroup = new QGroupBox("Обработка данных", this);
    QHBoxLayout *processLayout = new QHBoxLayout(processGroup);
    processLayout->setSpacing(20);
    m_step2Button = new QPushButton("Объединить данные", this);
    m_step3Button = new QPushButton("Отсортировать", this);
    m_step4Button = new QPushButton("Сохранить результат", this);
    processLayout->addWidget(m_step2Button);
    processLayout->addWidget(m_step3Button);
    processLayout->addWidget(m_step4Button);

    // --- Поиск просроченных ---
    QGroupBox *searchGroup = new QGroupBox("Поиск просроченных", this);
    QHBoxLayout *searchLayout = new QHBoxLayout(searchGroup);
    QLabel *searchLabel = new QLabel("Дата (мм.гггг):", this);
    m_dateInput = new QLineEdit(this);
    m_dateInput->setPlaceholderText("Введите дату");
    m_findButton = new QPushButton("Найти", this);
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(m_dateInput);
    searchLayout->addWidget(m_findButton);
    searchLayout->addStretch();

    // --- Лог ---
    m_logDisplay = new QTextEdit(this);
    m_logDisplay->setReadOnly(true);
    m_logDisplay->setFont(QFont("Courier", 10));
    m_logDisplay->setMinimumHeight(300);

    // --- Статус ---
    m_statusLabel = new QLabel("Готов к работе. Сначала загрузите файлы.", this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("color: green; font-weight: bold;");

    // --- Добавляем в основной лэйаут ---
    mainLayout->addWidget(loadGroup);
    mainLayout->addWidget(processGroup);
    mainLayout->addWidget(m_logDisplay, 1);  // лог занимает все оставшееся пространство
    mainLayout->addWidget(searchGroup);
    mainLayout->addWidget(m_statusLabel);

    // --- Подключаем слоты ---
    connect(m_step1aButton, &QPushButton::clicked, this, &MainWindow::onStep1a_LoadDrugFile);
    connect(m_step1bButton, &QPushButton::clicked, this, &MainWindow::onStep1b_LoadPriceFile);
    connect(m_step2Button, &QPushButton::clicked, this, &MainWindow::onStep2_MergeData);
    connect(m_step3Button, &QPushButton::clicked, this, &MainWindow::onStep3_SortData);
    connect(m_step4Button, &QPushButton::clicked, this, &MainWindow::onStep4_SaveResults);
    connect(m_findButton, &QPushButton::clicked, this, &MainWindow::onStep5_FindExpired);

    updateButtonStates();
}


void MainWindow::updateButtonStates()
{
    m_step1aButton->setEnabled(!m_drugFileLoaded);
    m_step1bButton->setEnabled(!m_priceFileLoaded);
    m_step2Button->setEnabled(m_drugFileLoaded && m_priceFileLoaded);
    m_step3Button->setEnabled(false);
    m_step4Button->setEnabled(false);
    m_findButton->setEnabled(false);
}

void MainWindow::createInitialFiles()
{
    MED<Drug> drugs;
    QList<Drug> drugList = {
        {"Aspirin", "12.2024", "Painkiller"}, {"Paracetamol", "10.2023", "Painkiller"},
        {"Ibuprofen", "08.2025", "Painkiller"}, {"Amoxicillin", "05.2023", "Antibiotic"},
        {"Cufrex", "10.2022", "Medicine"}, {"Alphabet", "11.2019", "Vitamin"}
    };
    drugs.setData(drugList);
    QFile drugFile("F1.bin");
    if (drugFile.open(QIODevice::WriteOnly)) {
        QDataStream out(&drugFile);
        drugs.WriteData(out);
    }
    drugFile.close();

    MED<Price> prices;
    QList<Price> priceList = {
        {"Aspirin", "12.2024", 50.75}, {"Paracetamol", "10.2023", 30.00},
        {"Loratadine", "01.2026", 120.00}, {"Multivitamin", "05.2024", 200.00},
        {"Alphabet", "11.2019", 20.30}
    };
    prices.setData(priceList);
    QFile priceFile("F2.bin");
    if (priceFile.open(QIODevice::WriteOnly)) {
        QDataStream out(&priceFile);
        prices.WriteData(out);
    }
    priceFile.close();
}

void MainWindow::onStep1a_LoadDrugFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Выберите файл Drug (*.bin)", "", "Binary Files (*.bin);;All Files (*)");
    if (filePath.isEmpty()) {
        return;
    }

    QFile drugFile(filePath);
    if (drugFile.open(QIODevice::ReadOnly)) {
        QDataStream in(&drugFile);
        m_drugs.ReadData(in);
        drugFile.close();

        m_logDisplay->append("--- Загружен массив Drug ---");
        for (const auto& drug : m_drugs) {
            m_logDisplay->append(QString("Название: %1, Срок: %2, Секция: %3")
                                     .arg(drug.name, -15).arg(drug.myDate, -10).arg(drug.section));
        }
        m_drugFileLoaded = true;
        m_statusLabel->setText("Файл Drug загружен. Теперь загрузите файл Price.");
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл: " + drugFile.errorString());
        m_drugFileLoaded = false;
    }
    updateButtonStates();
}

void MainWindow::onStep1b_LoadPriceFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Выберите файл Price (*.bin)", "", "Binary Files (*.bin);;All Files (*)");
    if (filePath.isEmpty()) {
        return;
    }

    QFile priceFile(filePath);
    if (priceFile.open(QIODevice::ReadOnly)) {
        QDataStream in(&priceFile);
        m_prices.ReadData(in);
        priceFile.close();

        m_logDisplay->append("\n--- Загружен массив Price ---");
        for (const auto& price : m_prices) {
            m_logDisplay->append(QString("Название: %1, Срок: %2, Цена: %3")
                                     .arg(price.name, -15).arg(price.myDate, -10).arg(price.pr));
        }
        m_priceFileLoaded = true;
        m_statusLabel->setText("Файл Price загружен. Теперь можно объединить данные.");
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл: " + priceFile.errorString());
        m_priceFileLoaded = false;
    }
    updateButtonStates();
}


void MainWindow::onStep2_MergeData()
{
    std::unordered_map<QString, Med> medMap;

    for(const auto& drug : m_drugs) {
        QString key = drug.name + "@" + drug.myDate;
        Med newMed;
        newMed.name = drug.name;
        newMed.myDate = drug.myDate;
        newMed.section = drug.section;
        newMed.pr = 0.0;
        newMed.count = 10;
        medMap[key] = newMed;
    }


    for(const auto& price : m_prices) {
        QString key = price.name + "@" + price.myDate;
        auto it = medMap.find(key);
        if (it != medMap.end()) {
            it->second.pr = price.pr;
        } else {
            Med newMed;
            newMed.name = price.name;
            newMed.myDate = price.myDate;
            newMed.section = "Unknown";
            newMed.pr = price.pr;
            newMed.count = 20;
            medMap[key] = newMed;
        }
    }
    QList<Med> mergedList;
    mergedList.reserve(medMap.size());
    for (const auto& pair : medMap) {
        mergedList.append(pair.second);
    }

    m_meds.setData(mergedList);

    m_logDisplay->append("\n--- б) Объединенный массив Med ---");
    for (const auto& med : m_meds) {
        m_logDisplay->append(QString("Название: %1, Срок: %2, Секция: %3, Цена: %4, Кол-во: %5")
                                 .arg(med.name, -15).arg(med.myDate, -10).arg(med.section, -10)
                                 .arg(med.pr).arg(med.count));
    }

    m_statusLabel->setText("Шаг 'б' выполнен. Данные объединены.");
    m_step2Button->setEnabled(false);
    m_step3Button->setEnabled(true);
}

void MainWindow::onStep3_SortData()
{
    std::sort(m_meds.begin(), m_meds.end(), compareMedsByNameDesc);

    m_logDisplay->append("\n--- в) Массив Med отсортирован ---");
    for (const auto& med : m_meds) {
        m_logDisplay->append(QString("Название: %1, Срок: %2, Секция: %3, Цена: %4, Кол-во: %5")
                                 .arg(med.name, -15).arg(med.myDate, -10).arg(med.section, -10)
                                 .arg(med.pr).arg(med.count));
    }

    m_statusLabel->setText("Шаг 'в' выполнен. Данные отсортированы.");
    m_step3Button->setEnabled(false);
    m_step4Button->setEnabled(true);
    m_findButton->setEnabled(true);
}

void MainWindow::onStep4_SaveResults()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Сохранить результат", "Med_result.bin", "Binary Files (*.bin);;All Files (*)");
    if (filePath.isEmpty()) {
        return;
    }

    QFile binFile(filePath);
    if (binFile.open(QIODevice::WriteOnly)) {
        QDataStream out(&binFile);
        m_meds.WriteData(out);
        binFile.close();
        m_logDisplay->append("\n--- г) Результат сохранен в бинарный файл: " + filePath);
    }

    QString txtFilePath = filePath;
    if (txtFilePath.endsWith(".bin")) {
        txtFilePath.chop(4);
    }
    txtFilePath += ".txt";

    QFile txtFile(txtFilePath);
    if (txtFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&txtFile);
        out << m_meds;
        txtFile.close();
        m_logDisplay->append("--- г) Результат сохранен в текстовый файл: " + txtFilePath);
    }

    m_statusLabel->setText("Шаг 'г' выполнен. Результаты сохранены.");
    m_step4Button->setEnabled(false);
}

void MainWindow::onStep5_FindExpired()
{
    QString dateStr = m_dateInput->text();
    QStringList dateParts = dateStr.split('.');

    if (dateParts.size() != 2) {
        QMessageBox::warning(this, "Ошибка ввода", "Пожалуйста, введите дату в формате мм.гггг.");
        return;
    }

    bool monthOk, yearOk;
    int userMonth = dateParts[0].toInt(&monthOk);
    int userYear = dateParts[1].toInt(&yearOk);

    if (!monthOk || !yearOk || dateParts[0].length() != 2 || dateParts[1].length() != 4) {
        QMessageBox::warning(this, "Ошибка ввода", "Части даты должны быть числами в формате мм.гггг.");
        return;
    }

    if (userMonth < 1 || userMonth > 12) {
        QMessageBox::warning(this, "Ошибка ввода", "Некорректный месяц. Введите значение от 01 до 12.");
        return;
    }

    m_logDisplay->append(QString("\n--- д) Поиск просроченных лекарств на %1 ---").arg(dateStr));

    QList<Med> expiredList;
    for (const auto& med : m_meds) {
        QStringList parts = med.myDate.split('.');
        if (parts.size() != 2) {
            continue;
        }

        bool medMonthOk, medYearOk;
        int medMonth = parts[0].toInt(&medMonthOk);
        int medYear = parts[1].toInt(&medYearOk);

        if (!medMonthOk || !medYearOk) {
            continue;
        }

        if (medYear < userYear || (medYear == userYear && medMonth < userMonth)) {
            expiredList.append(med);
        }
    }

    MED<Med> expiredMeds;
    expiredMeds.setData(expiredList);

    if (expiredMeds.GetCount() == 0) {
        m_logDisplay->append("Просроченных лекарств не найдено.");
        m_statusLabel->setText(QString("Поиск на %1 завершен. Просроченных нет.").arg(dateStr));
    } else {
        for (const auto& med : expiredMeds) {
            m_logDisplay->append(QString("Найдено: %1, срок истек: %2")
                                     .arg(med.name, -15).arg(med.myDate));
        }
        m_statusLabel->setText(QString("Поиск на %1 завершен. Найдено %2 шт.").arg(dateStr).arg(expiredMeds.GetCount()));
    }
}
