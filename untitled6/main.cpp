// main.cpp
#include "med.h"
#include <QCoreApplication>
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QDate>
#include <QMap>

bool isExpired(const MyDate &d, const MyDate &now) {
    return d.year < now.year || (d.year == now.year && d.month < now.month);
}

int main() {
    QTextStream cin(stdin);
    QTextStream cout(stdout);

    QFile file1("F1.bin");
    QFile file2("F2.bin");
    QFile textOut1("Ob1.txt");
    QFile textOut2("Ob2.txt");
    QFile textOutR("ROb.txt");
    QFile binOutR("Med.bin");

    if (!file1.open(QIODevice::ReadOnly)) {
        qCritical() << "Не удалось открыть F1.bin:" << file1.errorString();
        return -1;
    }
    if (!file2.open(QIODevice::ReadOnly)) {
        qCritical() << "Не удалось открыть F2.bin:" << file2.errorString();
        return -1;
    }
    if (!textOut1.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical() << "Не удалось открыть Ob1.txt:" << textOut1.errorString();
        return -1;
    }
    if (!textOut2.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical() << "Не удалось открыть Ob2.txt:" << textOut2.errorString();
        return -1;
    }
    if (!textOutR.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical() << "Не удалось открыть ROb.txt:" << textOutR.errorString();
        return -1;
    }
    if (!binOutR.open(QIODevice::WriteOnly)) {
        qCritical() << "Не удалось открыть Med.bin:" << binOutR.errorString();
        return -1;
    }


    QDataStream in1(&file1);
    QDataStream in2(&file2);
    QDataStream outBin(&binOutR);
    QTextStream out1(&textOut1);
    QTextStream out2(&textOut2);
    QTextStream outR(&textOutR);

    MED<Drug> M1;
    M1.ReadData(in1);
    M1.PutData(out1);
    M1.PutData(cout);

    MED<Price> K1;
    K1.ReadData(in2);
    K1.PutData(out2);
    K1.PutData(cout);

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

    std::sort(result.begin(), result.end(), [](const Med &a, const Med &b) {
        return a.name > b.name; // обратный алфавитный порядок
    });

    MED<Med> R(result.size());
    for (int i = 0; i < result.size(); ++i) {
        R[i] = result[i];
    }

    R.PutData(outR);
    R.PutData(cout);
    R.WriteData(outBin);

    cout << "Введите дату (мм.гггг) для проверки истекших лекарств: ";
    QString inputDate;
    cin >> inputDate;
    QStringList parts = inputDate.split(".");
    MyDate currentDate = { parts[0].toInt(), parts[1].toInt() };

    bool found = false;
    for (int i = 0; i < R.GetCount(); ++i) {
        if (isExpired(R[i].myDate, currentDate)) {
            cout << R[i];
            found = true;
        }
    }
    if (!found) {
        cout << "Нет лекарств с истекшим сроком годности на " << inputDate << "\n";
    }

    return 0;
}
