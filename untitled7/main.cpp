#include "mainwindow.h"
#include "med.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    // Загрузка данных из бинарных файлов
    MED<Drug> M1;
    MED<Price> K1;

    QFile file1("drugs.bin");
    if (file1.open(QIODevice::ReadOnly)) {
        QDataStream in1(&file1);
        M1.ReadData(in1);
        file1.close();
    }

    QFile file2("prices.bin");
    if (file2.open(QIODevice::ReadOnly)) {
        QDataStream in2(&file2);
        K1.ReadData(in2);
        file2.close();
    }

    // Вывод исходных данных
    QTextStream cout(stdout);
    cout << "=== Исходные данные (Drugs) ===\n" << M1 << "\n";
    cout << "=== Исходные данные (Prices) ===\n" << K1 << "\n";

    // Объединение данных
    QMap<QString, Med> mergedMap;

    // Обработка лекарств
    for (int i = 0; i < M1.GetCount(); ++i) {
        const Drug& d = M1[i];
        Med m;
        m.name = d.name;
        m.myDate = d.myDate;
        m.section = d.section;
        m.count = d.count;
        m.price = 0.0;
        mergedMap.insert(d.name, m);
    }

    // Обработка цен
    for (int i = 0; i < K1.GetCount(); ++i) {
        const Price& p = K1[i];
        if (mergedMap.contains(p.name)) {
            Med& m = mergedMap[p.name];
            if (dateToInt(p.myDate) > dateToInt(m.myDate)) {
                m.myDate = p.myDate;
            }
            m.price = p.price;
        } else {
            Med m;
            m.name = p.name;
            m.myDate = p.myDate;
            m.section = "unknown";
            m.count = 0;
            m.price = p.price;
            mergedMap.insert(p.name, m);
        }
    }

    // Создание объединенного массива
    MED<Med> R(mergedMap.size());
    int idx = 0;
    for (auto it = mergedMap.begin(); it != mergedMap.end(); ++it) {
        R[idx++] = it.value();
    }

    // Исправленная сортировка через публичный метод
    R.Sort([](const Med& a, const Med& b) {
        return a.name > b.name;
    });



    // Сохранение результатов
    QFile binOut("meds.bin");
    if (binOut.open(QIODevice::WriteOnly)) {
        QDataStream outBin(&binOut);
        R.WriteData(outBin);
        binOut.close();
    }

    QFile txtOut("meds.txt");
    if (txtOut.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream outTxt(&txtOut);
        outTxt << R;
        txtOut.close();
    }

    // Вывод объединенных данных
    cout << "=== Объединенные данные (Med) ===\n" << R << "\n";

    // Поиск просроченных лекарств
    cout << "Введите дату для проверки срока (ММ.гггг): ";
    QTextStream cin(stdin);
    QString checkDate;
    cin >> checkDate;

    bool found = false;
    for (int i = 0; i < R.GetCount(); ++i) {
        if (dateToInt(R[i].myDate) < dateToInt(checkDate)) {
            cout << "Просрочено: " << R[i].name << " (до " << R[i].myDate << ")\n";
            found = true;
        }
    }
    if (!found) cout << "Просроченных лекарств не найдено.\n";

    return 0;
}
