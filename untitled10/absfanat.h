#ifndef ABSFANAT_H
#define ABSFANAT_H

#include <QTextStream>
#include <QString>

class ABS_Fanat {
public:
    virtual ~ABS_Fanat() {}
    virtual void GetFanats(QTextStream& in) = 0;
    virtual void PutFanats(QTextStream& out) = 0;
    virtual void SortUp() = 0;
    virtual void SortDown() = 0;
    virtual bool containsName(const QString& name) const = 0;
    virtual void printFanInfo(const QString& name, QTextStream& out) const = 0;
};

#endif // ABSFANAT_H
