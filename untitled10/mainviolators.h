#ifndef MAINVIOLATORS_H
#define MAINVIOLATORS_H

#include "absfanat.h"
#include "fanatdatabase.h"
#include "fanatsworldcup.h"
#include <QVector>

struct MainViolator {
    QString Name;
    QString Country;
    int BirthYear;
    int Code;
    int TotalDetentions;

    friend QTextStream& operator<<(QTextStream& out, const MainViolator& fan);
};

class MainViolatorsManager : public ABS_Fanat {
    QVector<MainViolator> violators;

public:
    void createFrom(const FanDatabaseManager& db, const FanWorldCupManager& wc);
    void GetFanats(QTextStream& in) override { Q_UNUSED(in); }  // Добавлено Q_UNUSED
    void PutFanats(QTextStream& out) override;
    void SortUp() override;
    void SortDown() override;
    bool containsName(const QString& name) const override;
    void printFanInfo(const QString& name, QTextStream& out) const override;

    const QVector<MainViolator>& getViolators() const { return violators; }
};

#endif // MAINVIOLATORS_H
