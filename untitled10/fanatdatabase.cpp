#include "fanatdatabase.h"
#include <algorithm>

QTextStream& operator>>(QTextStream& in, FanDatabase& fan) {
    fan.Name = in.readLine();
    fan.Country = in.readLine();
    in >> fan.BirthYear;
    in >> fan.Detentions;
    in.readLine(); // consume newline
    return in;
}

QTextStream& operator<<(QTextStream& out, const FanDatabase& fan) {
    out << fan.Name << "\t"
        << fan.Country << "\t"
        << fan.BirthYear << "\t"
        << fan.Detentions << "\n";
    return out;
}

void FanDatabaseManager::GetFanats(QTextStream& in) {
    int count;
    in >> count;
    in.readLine(); // consume newline

    fans.resize(count);
    for (int i = 0; i < count; ++i) {
        in >> fans[i];
    }
}

void FanDatabaseManager::PutFanats(QTextStream& out) {
    for (const auto& fan : fans) {
        out << fan;
    }
}

void FanDatabaseManager::SortUp() {
    std::sort(fans.begin(), fans.end(),
              [](const FanDatabase& a, const FanDatabase& b) { return a.Name < b.Name; });
}

void FanDatabaseManager::SortDown() {
    std::sort(fans.begin(), fans.end(),
              [](const FanDatabase& a, const FanDatabase& b) { return a.Name > b.Name; });
}

bool FanDatabaseManager::containsName(const QString& name) const {
    return std::any_of(fans.begin(), fans.end(),
                       [&name](const FanDatabase& fan) { return fan.Name == name; });
}

void FanDatabaseManager::printFanInfo(const QString& name, QTextStream& out) const {
    for (const auto& fan : fans) {
        if (fan.Name == name) {
            out << "Found in Database:\n";
            out << "Name: " << fan.Name << "\n";
            out << "Country: " << fan.Country << "\n";
            out << "Birth Year: " << fan.BirthYear << "\n";
            out << "Detentions: " << fan.Detentions << "\n\n";
            return;
        }
    }
    out << "Fan " << name << " not found in Database.\n\n";
}
