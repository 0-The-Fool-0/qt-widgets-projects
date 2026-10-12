#include "med.h"

QTextStream &operator<<(QTextStream &out, const Med &m) {
    out << m.name << ", "
        << QString("%1.%2").arg(m.myDate.month, 2, 10, QChar('0')).arg(m.myDate.year) << ", "
        << m.section << ", "
        << m.count << ", "
        << QString::number(m.pr, 'f', 2);
    return out;
}
