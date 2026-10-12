#ifndef MED_TEMPLATE_CLASS_H
#define MED_TEMPLATE_CLASS_H

#include <vector>

#include <QDataStream>
#include <QList>
#include <QString>
#include <QTextStream>

struct Drug {
    QString name;
    QString myDate;
    QString section;
};

struct Price {
    QString name;
    QString myDate;
    double pr;
};

struct Med {
    QString name;
    QString myDate;
    QString section;
    double pr;
    int count;
};

inline QDataStream& operator<<(QDataStream& out, const Drug& drug) {
    out << drug.name << drug.myDate << drug.section;
    return out;
}

inline QDataStream& operator>>(QDataStream& in, Drug& drug) {
    in >> drug.name >> drug.myDate >> drug.section;
    return in;
}

inline QDataStream& operator<<(QDataStream& out, const Price& price) {
    out << price.name << price.myDate << price.pr;
    return out;
}

inline QDataStream& operator>>(QDataStream& in, Price& price) {
    in >> price.name >> price.myDate >> price.pr;
    return in;
}

inline QDataStream& operator<<(QDataStream& out, const Med& med) {
    out << med.name << med.myDate << med.section << med.pr << med.count;
    return out;
}

inline QDataStream& operator>>(QDataStream& in, Med& med) {
    in >> med.name >> med.myDate >> med.section >> med.pr >> med.count;
    return in;
}

inline QTextStream& operator<<(QTextStream& out, const Med& med) {
    out << med.name << ";" << med.myDate << ";" << med.section << ";" << med.pr << ";" << med.count;
    return out;
}

template <typename T>
class MED {
private:
    std::vector<T> m_data;

public:
    MED() = default;

    int GetCount() const { return m_data.size(); }

    T& operator[](int index) { return m_data[index]; }
    const T& operator[](int index) const { return m_data[index]; }

    auto begin() { return m_data.begin(); }
    auto end() { return m_data.end(); }
    auto begin() const { return m_data.cbegin(); }
    auto end() const { return m_data.cend(); }

    void setData(const QList<T>& list) {
        m_data.assign(list.begin(), list.end());
    }

    void WriteData(QDataStream& out) const {
        out << static_cast<qint32>(m_data.size());
        for (const T& item : m_data) {
            out << item;
        }
    }

    void ReadData(QDataStream& in) {
        m_data.clear();
        qint32 count = 0;
        in >> count;
        if (count > 0) {
            m_data.resize(count);
            for (qint32 i = 0; i < count; ++i) {
                in >> m_data[i];
            }
        }
    }
};

template <typename T>
QTextStream& operator<<(QTextStream& out, const MED<T>& medArray) {
    for (int i = 0; i < medArray.GetCount(); ++i) {
        out << medArray[i] << "\n";
    }
    return out;
}

#endif // MED_TEMPLATE_CLASS_H
