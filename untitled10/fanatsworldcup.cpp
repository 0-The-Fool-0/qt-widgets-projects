#include "fanatsworldcup.h"
#include <algorithm>

QTextStream& operator>>(QTextStream& in, FanWorldCup& fan) {
    fan.Name = in.readLine();
    fan.Country = in.readLine();
    in >> fan.Code;
    in >> fan.Detentions;
    in.readLine(); // consume newline
    return in;
}

QTextStream& operator<<(QTextStream& out, const FanWorldCup& fan) {
    out << fan.Name << "\t"
        << fan.Country << "\t"
        << fan.Code << "\t"
        << fan.Detentions << "\n";
    return out;
}

void FanWorldCupManager::GetFanats(QTextStream& in) {
    int count;
    in >> count;
    in.readLine(); // consume newline

    fans.resize(count);
    for (int i = 0; i < count; ++i) {
        in >> fans[i];
    }
}

void FanWorldCupManager::PutFanats(QTextStream& out) {
    for (const auto& fan : fans) {
        out << fan;
    }
}

void FanWorldCupManager::SortUp() {
    std::sort(fans.begin(), fans.end(),
              [](const FanWorldCup& a, const FanWorldCup& b) { return a.Name < b.Name; });
}

void FanWorldCupManager::SortDown() {
    std::sort(fans.begin(), fans.end(),
              [](const FanWorldCup& a, const FanWorldCup& b) { return a.Name > b.Name; });
}

bool FanWorldCupManager::containsName(const QString& name) const {
    return std::any_of(fans.begin(), fans.end(),
                       [&name](const FanWorldCup& fan) { return fan.Name == name; });
}

void FanWorldCupManager::printFanInfo(const QString& name, QTextStream& out) const {
    for (const auto& fan : fans) {
        if (fan.Name == name) {
            out << "Found in World Cup Fans:\n";
            out << "Name: " << fan.Name << "\n";
            out << "Country: " << fan.Country << "\n";
            out << "Code: " << fan.Code << "\n";
            out << "Detentions: " << fan.Detentions << "\n\n";
            return;
        }
    }
    out << "Fan " << name << " not found in World Cup Fans.\n\n";
}
