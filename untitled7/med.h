#ifndef MED_H
#define MED_H
#include <QtCore>
#include <algorithm>
#include <iostream>

// Структура для лекарств из аптеки
struct Drug {
    QString name;
    QString myDate;
    QString section;
    int count;

    // Операторы для работы с потоками
    friend QDataStream& operator<<(QDataStream& out, const Drug& d) {
        out << d.name << d.myDate << d.section << d.count;
        return out;
    }
    friend QDataStream& operator>>(QDataStream& in, Drug& d) {
        in >> d.name >> d.myDate >> d.section >> d.count;
        return in;
    }
    friend QTextStream& operator<<(QTextStream& out, const Drug& d) {
        out << "Название: " << d.name << "\nСрок годности: " << d.myDate
            << "\nСекция: " << d.section << "\nКоличество: " << d.count << "\n\n";
        return out;
    }
    bool operator==(const Drug& other) const {
        return name == other.name;
    }
};

// Структура для лекарств из прайс-листа
struct Price {
    QString name;
    QString myDate;
    double price;

    friend QDataStream& operator<<(QDataStream& out, const Price& p) {
        out << p.name << p.myDate << p.price;
        return out;
    }
    friend QDataStream& operator>>(QDataStream& in, Price& p) {
        in >> p.name >> p.myDate >> p.price;
        return in;
    }
    friend QTextStream& operator<<(QTextStream& out, const Price& p) {
        out << "Название: " << p.name << "\nСрок годности: " << p.myDate
            << "\nЦена: " << p.price << "\n\n";
        return out;
    }
    bool operator==(const Price& other) const {
        return name == other.name;
    }
};

// Обобщенная структура для объединенных данных
struct Med {
    QString name;
    QString myDate;
    QString section;
    int count;
    double price;

    friend QDataStream& operator<<(QDataStream& out, const Med& m) {
        out << m.name << m.myDate << m.section << m.count << m.price;
        return out;
    }
    friend QDataStream& operator>>(QDataStream& in, Med& m) {
        in >> m.name >> m.myDate >> m.section >> m.count >> m.price;
        return in;
    }
    friend QTextStream& operator<<(QTextStream& out, const Med& m) {
        out << "Название: " << m.name << "\nСрок годности: " << m.myDate
            << "\nСекция: " << m.section << "\nКоличество: " << m.count
            << "\nЦена: " << m.price << "\n\n";
        return out;
    }
    bool operator==(const Med& other) const {
        return name == other.name;
    }
};

// Вспомогательная функция для сравнения дат
int dateToInt(const QString& date) {
    QStringList parts = date.split('.');
    if (parts.size() != 2) return 0;
    return parts[1].toInt() * 100 + parts[0].toInt(); // ГГГГММ
}
template <class T>
class MED {
    int count;
    T* arr;

public:
    // Конструкторы и деструктор
    MED() : count(0), arr(nullptr) {}
    MED(int size) : count(size), arr(new T[size]) {}
    MED(const MED& other) : count(other.count), arr(new T[count]) {
        for (int i = 0; i < count; ++i) arr[i] = other.arr[i];
    }
    ~MED() { delete[] arr; }

    // Оператор присваивания
    MED& operator=(const MED& other) {
        if (this != &other) {
            delete[] arr;
            count = other.count;
            arr = new T[count];
            for (int i = 0; i < count; ++i) arr[i] = other.arr[i];
        }
        return *this;
    }
    void Sort(std::function<bool(const T&, const T&)> compareFunction) {
        std::sort(arr, arr + count, compareFunction);
    }

    // Доступ к элементу
    T& operator[](int index) { return arr[index]; }
    const T& operator[](int index) const { return arr[index]; }

    // Получение количества элементов
    int GetCount() const { return count; }

    // Чтение/запись бинарных данных
    void ReadData(QDataStream& stream) {
        stream >> count;
        arr = new T[count];
        for (int i = 0; i < count; ++i) stream >> arr[i];
    }

    void WriteData(QDataStream& stream) const {
        stream << count;
        for (int i = 0; i < count; ++i) stream << arr[i];
    }

    // Вывод в текстовый поток
    friend QTextStream& operator<<(QTextStream& out, const MED& med) {
        for (int i = 0; i < med.count; ++i) out << med.arr[i];
        return out;
    }
};
#endif // MED_H
