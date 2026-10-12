// // generate_bin_files.cpp
// #include "med.h"
// #include <QCoreApplication>
// #include <QFile>
// #include <QDataStream>

// int main() {
//     QFile f1("F1.bin");
//     QFile f2("F2.bin");
//     f1.open(QIODevice::WriteOnly);
//     f2.open(QIODevice::WriteOnly);
//     QDataStream out1(&f1);
//     QDataStream out2(&f2);

//     QVector<Drug> drugs = {
//         {"Coldrex", {10, 2022}, "Medicine", 10},
//         {"Doxiciclin", {12, 2023}, "Medicine", 20},
//         {"Alphabet", {5, 2008}, "Vitamin", 20}
//     };

//     QVector<Price> prices = {
//         {"Coldrex", {5, 2023}, 100.20},
//         {"Alphabet", {11, 2019}, 20.30},
//         {"Multivitamin", {5, 2024}, 200.00}
//     };

//     out1 << drugs.size();
//     for (const auto &d : drugs) out1 << d;

//     out2 << prices.size();
//     for (const auto &p : prices) out2 << p;

//     return 0;
// }

