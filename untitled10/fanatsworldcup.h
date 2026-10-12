#ifndef FANATSWORLDCUP_H
#define FANATSWORLDCUP_H

#include "absfanat.h"
#include <QVector>

struct FanWorldCup {
    QString Name;
    QString Country;
    int Code;
    int Detentions;

    friend QTextStream& operator>>(QTextStream& in, FanWorldCup& fan);
    friend QTextStream& operator<<(QTextStream& out, const FanWorldCup& fan);
};

class FanWorldCupManager : public ABS_Fanat {
    QVector<FanWorldCup> fans;

public:
    void GetFanats(QTextStream& in) override;
    void PutFanats(QTextStream& out) override;
    void SortUp() override;
    void SortDown() override;
    bool containsName(const QString& name) const override;
    void printFanInfo(const QString& name, QTextStream& out) const override;

    const QVector<FanWorldCup>& getFans() const { return fans; }
};

#endif // FANATSWORLDCUP_H
