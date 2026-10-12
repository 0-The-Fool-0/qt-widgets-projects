#include "mainviolators.h"
#include <algorithm>

QTextStream& operator<<(QTextStream& out, const MainViolator& fan) {
    out << fan.Name << "\t"
        << fan.Country << "\t"
        << fan.BirthYear << "\t"
        << fan.Code << "\t"
        << fan.TotalDetentions << "\n";
    return out;
}

void MainViolatorsManager::createFrom(const FanDatabaseManager& db, const FanWorldCupManager& wc) {
    violators.clear();

    for (const auto& dbFan : db.getFans()) {
        for (const auto& wcFan : wc.getFans()) {
            if (dbFan.Name == wcFan.Name) {
                MainViolator violator;
                violator.Name = dbFan.Name;
                violator.Country = dbFan.Country;
                violator.BirthYear = dbFan.BirthYear;
                violator.Code = wcFan.Code;
                violator.TotalDetentions = dbFan.Detentions + wcFan.Detentions;
                violators.append(violator);
                break;
            }
        }
    }
}

void MainViolatorsManager::PutFanats(QTextStream& out) {
    out << "Main Violators:\n";
    out << "Name\tCountry\tBirthYear\tCode\tTotalDetentions\n";
    for (const auto& violator : violators) {
        out << violator;
    }
}

void MainViolatorsManager::SortUp() {
    std::sort(violators.begin(), violators.end(),
              [](const MainViolator& a, const MainViolator& b) { return a.Name < b.Name; });
}

void MainViolatorsManager::SortDown() {
    std::sort(violators.begin(), violators.end(),
              [](const MainViolator& a, const MainViolator& b) { return a.Name > b.Name; });
}

bool MainViolatorsManager::containsName(const QString& name) const {
    return std::any_of(violators.begin(), violators.end(),
                       [&name](const MainViolator& violator) { return violator.Name == name; });
}

void MainViolatorsManager::printFanInfo(const QString& name, QTextStream& out) const {
    for (const auto& violator : violators) {
        if (violator.Name == name) {
            out << "Found in Main Violators:\n";
            out << "Name: " << violator.Name << "\n";
            out << "Country: " << violator.Country << "\n";
            out << "Birth Year: " << violator.BirthYear << "\n";
            out << "Code: " << violator.Code << "\n";
            out << "Total Detentions: " << violator.TotalDetentions << "\n\n";
            return;
        }
    }
    out << "Fan " << name << " not found in Main Violators.\n\n";
}
