#ifndef FANATDATABASE_H
#define FANATDATABASE_H

#include "absfanat.h"
#include <QVector>

struct FanDatabase {
    QString Name;
    QString Country;
    int BirthYear;
    int Detentions;

    friend QTextStream& operator>>(QTextStream& in, FanDatabase& fan);
    friend QTextStream& operator<<(QTextStream& out, const FanDatabase& fan);
};

class FanDatabaseManager : public ABS_Fanat {
    QVector<FanDatabase> fans;

public:
    void GetFanats(QTextStream& in) override;
    void PutFanats(QTextStream& out) override;
    void SortUp() override;
    void SortDown() override;
    bool containsName(const QString& name) const override;
    void printFanInfo(const QString& name, QTextStream& out) const override;

    const QVector<FanDatabase>& getFans() const { return fans; }
};

#endif // FANATDATABASE_H
