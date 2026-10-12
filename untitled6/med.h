// med.h
#ifndef MED_TEMPLATE_CLASS_H
#define MED_TEMPLATE_CLASS_H

#include <QString>
#include <QDataStream>
#include <QTextStream>
#include <QFile>
#include <QVector>
#include <algorithm>


struct MyDate {
    int month;
    int year;

    friend QDataStream &operator>>(QDataStream &in, MyDate &d) {
        in >> d.month >> d.year;
        return in;
    }
    friend QDataStream &operator<<(QDataStream &out, const MyDate &d) {
        out << d.month << d.year;
        return out;
    }
    friend QTextStream &operator>>(QTextStream &in, MyDate &d) {
        QString str;
        in >> str;
        QStringList parts = str.split(".");
        if (parts.size() == 2) {
            d.month = parts[0].toInt();
            d.year = parts[1].toInt();
        }
        return in;
    }
    friend QTextStream &operator<<(QTextStream &out, const MyDate &d) {
        out << (d.month < 10 ? "0" : "") << d.month << "." << d.year;
        return out;
    }
    bool operator<(const MyDate &other) const {
        return year < other.year || (year == other.year && month < other.month);
    }
};

struct Drug {
    QString name;
    MyDate myDate;
    QString section;
    int count;

    friend QDataStream &operator>>(QDataStream &in, Drug &d) {
        in >> d.name >> d.myDate >> d.section >> d.count;
        return in;
    }
    friend QDataStream &operator<<(QDataStream &out, const Drug &d) {
        out << d.name << d.myDate << d.section << d.count;
        return out;
    }
    friend QTextStream &operator<<(QTextStream &out, const Drug &d) {
        out << d.name << " " << d.myDate << " " << d.section << " " << d.count << "\n";
        return out;
    }
    bool operator==(const Drug &other) const {
        return name == other.name;
    }
};

struct Price {
    QString name;
    MyDate myDate;
    double pr;

    friend QDataStream &operator>>(QDataStream &in, Price &p) {
        in >> p.name >> p.myDate >> p.pr;
        return in;
    }
    friend QDataStream &operator<<(QDataStream &out, const Price &p) {
        out << p.name << p.myDate << p.pr;
        return out;
    }
    friend QTextStream &operator<<(QTextStream &out, const Price &p) {
        out << p.name << " " << p.myDate << " " << p.pr << "\n";
        return out;
    }
    bool operator==(const Price &other) const {
        return name == other.name;
    }
};

struct Med {
    QString name;
    MyDate myDate;
    QString section;
    int count;
    double pr;

    friend QDataStream &operator>>(QDataStream &in, Med &m) {
        in >> m.name >> m.myDate >> m.section >> m.count >> m.pr;
        return in;
    }
    friend QDataStream &operator<<(QDataStream &out, const Med &m) {
        out << m.name << m.myDate << m.section << m.count << m.pr;
        return out;
    }

    bool operator==(const Med &other) const {
        return name == other.name;
    }
};
QTextStream &operator<<(QTextStream &out, const Med &m);


template<class T>
class MED {
    int count;
    T* M;
public:
    MED(int n = 0) : count(n), M(n ? new T[n] : nullptr) {}
    ~MED() { delete[] M; }

    MED(const MED &other) : count(other.count), M(new T[other.count]) {
        for (int i = 0; i < count; ++i) M[i] = other.M[i];
    }
    MED &operator=(const MED &other) {
        if (this != &other) {
            delete[] M;
            count = other.count;
            M = new T[count];
            for (int i = 0; i < count; ++i) M[i] = other.M[i];
        }
        return *this;
    }
    T &operator[](int i) { return M[i]; }
    const T &operator[](int i) const { return M[i]; }
    int GetCount() const { return count; }

    void ReadData(QDataStream &in) {
        in >> count;
        if (count < 0) count = 0; // защита от отрицательных значений
        delete[] M;
        M = new T[count];
        for (int i = 0; i < count; ++i) in >> M[i];
    }


    void WriteData(QDataStream &out) const {
        out << count;
        for (int i = 0; i < count; ++i) out << M[i];
    }
    void PutData(QTextStream &out) const {
        for (int i = 0; i < count; ++i) out << M[i];
    }
};

#endif // MED_TEMPLATE_CLASS_H
