#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <QFile>
#include <QList>
#include <algorithm>
using namespace std;

struct Drug {
    QString name;
    QString date;
    QString section;
    int count;

    ~Drug() {}
    Drug() : name(""), date(""), section(""), count(0) {}
    Drug(QString &a, QString &b, QString &c, int &d) {
        name = a;
        date = b;
        section = c;
        count = d;
    }
    friend QTextStream &operator<<(QTextStream &out, const Drug &a) {
        out << a.name << " " << a.date << " " << a.section << " " << a.count;
        return out;
    }
    friend QTextStream &operator>>(QTextStream &in, Drug &a) {
        in >> a.name >> a.date >> a.section >> a.count;
        return in;
    }
};

struct Price {
    QString name;
    QString date;
    double price;

    ~Price() {}
    Price() : name(""), price(0), date("") {}
    Price(QString &a,QString &c, double b) {
        name = a;
        date=c;
        price = b;
    }
    friend QTextStream &operator<<(QTextStream &out, const Price &a) {
        out << a.name << " " <<a.date<<" "<< a.price;
        return out;
    }
    friend QTextStream &operator>>(QTextStream &in, Price &a) {
        in >> a.name >>a.date>> a.price;
        return in;
    }
};

struct Med {
    QString name;
    QString date;
    QString section;
    double price;
    int count;

    Med() : name(""), date(""), section(""), price(0), count(0) {}
    Med(QString &a, QString &b, QString &c, double &d, int &e) {
        name = a;
        date = b;
        section = c;
        price = d;
        count = e;
    }
    ~Med() {}
    friend QTextStream &operator<<(QTextStream &out, const Med &a) {
        out << a.name << " " << a.date << " " << a.section << " " << a.price << " " << a.count;
        return out;
    }
    friend QTextStream &operator>>(QTextStream &in, Med &a) {
        in >> a.name >> a.date >> a.section >> a.price >> a.count;
        return in;
    }
};

class ABS_Med {
public:
    virtual ~ABS_Med() {}
    virtual void ReadMedsText(QTextStream &in) = 0;
    virtual void WriteMedsText(QTextStream &out) const = 0;
    virtual void SortUp()=0;
};

class MB1 : public ABS_Med {
private:
    int count;
    Drug *M;
public:
    int getCount() const { return count; }
    const Drug* getArray() const { return M; }
    MB1() : count(0), M(nullptr) {}
    virtual ~MB1() { delete[] M; }
    friend class MB3;
    int GetAmount() const {
        return count;
    }
    virtual void ReadMedsText(QTextStream &in) override {
        QList<Drug> drugs;
        while (!in.atEnd()) {
            Drug d;
            in >> d;
            drugs.append(d);
        }
        count = drugs.size();
        delete[] M;
        if (count > 0) {
            M = new Drug[count];
            for (int i = 0; i < count; ++i)
                M[i] = drugs[i];
        } else {
            M = nullptr;
        }
    }
    virtual void SortUp() override {
        if (count > 1 && M != nullptr) {
            sort(M, M + count, [](const Drug &a, const Drug &b) {
                return a.name < b.name;
            });
        }
    }
    virtual void WriteMedsText(QTextStream &out) const override {
        for (int i = 0; i < count; ++i)
            out << M[i] << "\n";
        out.flush();
    }
    Drug& operator[](int index) {
        return M[index];
    }
    const Drug& operator[](int index) const {
        return M[index];
    }
};

class MB2 : public ABS_Med {
private:
    int count;
    Price *M;
public:
    int GetAmount() const {
        return count;
    }
    MB2() : count(0), M(nullptr) {}
    virtual ~MB2() { delete[] M; }
    friend class MB3;
    int getCount() const { return count; }
    const Price* getArray() const { return M; }

    virtual void ReadMedsText(QTextStream &in) override {
        QList<Price> prices;
        while (!in.atEnd()) {
            Price p;
            in >> p;
            prices.append(p);
        }
        count = prices.size();
        delete[] M;
        if (count > 0) {
            M = new Price[count];
            for (int i = 0; i < count; ++i)
                M[i] = prices[i];
        } else {
            M = nullptr;
        }
    }
    Price& operator[](int index) {
        return M[index];
    }
    const Price& operator[](int index) const {
        return M[index];
    }
    virtual void WriteMedsText(QTextStream &out) const override {
        for (int i = 0; i < count; ++i)
            out << M[i] << "\n";
        out.flush();
    }
    virtual void SortUp() override {
        if (count > 1 && M != nullptr) {
            sort(M, M + count, [](const Price &a, const Price &b) {
                return a.name < b.name;
            });
        }
    }
};

class MB3 : public ABS_Med {
private:
    int count;
    Med *M;
public:
    MB3() : count(0), M(nullptr) {}
    virtual ~MB3() { delete[] M; }
    void IntersectionFrom(const MB1 &mb1, const MB2 &mb2) {
        QList<Med> medList;
        for (int i = 0; i < mb1.getCount(); ++i) {
            const Drug &drug = mb1.getArray()[i];
            for (int j = 0; j < mb2.getCount(); ++j) {
                const Price &price = mb2.getArray()[j];
                if (drug.name == price.name) {
                    Med med;
                    med.name = drug.name;
                    med.date = drug.date;
                    med.section = drug.section;
                    med.count = drug.count;
                    med.price = price.price;
                    medList.append(med);
                    break;
                }
            }
        }
        delete[] M;
        count = medList.size();
        if (count > 0) {
            M = new Med[count];
            for (int i = 0; i < count; ++i)
                M[i] = medList[i];
        } else {
            M = nullptr;
        }
    }
    int getCount() const { return count; }
    const Med* getArray() const { return M; }
    Med& operator[](int index) {
        return M[index];
    }
    const Med& operator[](int index) const {
        return M[index];
    }
    virtual void ReadMedsText(QTextStream &in) override {}

    virtual void WriteMedsText(QTextStream &out) const override {
        for (int i = 0; i < count; ++i)
            out << M[i] << "\n";
        out.flush();
    }
    virtual void SortUp() override {
        if (count > 1 && M != nullptr) {
            sort(M, M + count, [](const Med &a, const Med &b) {
                return a.name < b.name;
            });
        }
    }
};

MB1 M1;
MB2 M2;
MB3 M3;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mb1count->hide();
    ui->mb1date->hide();
    ui->mb1name->hide();
    ui->mb1count->hide();
    ui->mb2name->hide();
    ui->mb2date->hide();
    ui->mb2price->hide();
    ui->mb3name->hide();
    ui->mb3date->hide();
    ui->mb3section->hide();
    ui->mb3price->hide();
    ui->mb3count->hide();
    ui->pokaz->hide();
    ui->sort->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOPEN_PROGRAMM_triggered()
{
    ui->sort->show();
    ui->mb1count->show();
    ui->mb1date->show();
    ui->mb1name->show();
    ui->mb1count->show();
    ui->mb2name->show();
    ui->mb2date->show();
    ui->mb2price->show();
    ui->mb3name->show();
    ui->mb3date->show();
    ui->mb3section->show();
    ui->mb3price->show();
    ui->mb3count->show();
    ui->pokaz->show();
}

void MainWindow::on_actionCLOSE_PROGRAMM_triggered()
{
    close();
}


void MainWindow::on_actionREAD_FROM_FILE_triggered()
{
    QFile fin1("C:/Users/Redmi/source/QT projects/untitled8/mb1.txt");
    if (!fin1.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "Cannot open mb1.txt for reading");
        return;
    }
    QTextStream in1(&fin1);
    M1.ReadMedsText(in1);
    fin1.close();
    QFile fin2("C:/Users/Redmi/source/QT projects/untitled8/mb2.txt");
    if (!fin2.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "Cannot open mb2.txt for reading");
        return;
    }
    QTextStream in2(&fin2);
    M2.ReadMedsText(in2);
    fin2.close();
    M3.IntersectionFrom(M1, M2);
    QFile fout3("C:/Users/Redmi/source/QT projects/untitled8/.txt");
    if (!fout3.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Error", "Cannot open output3.txt for writing");
        return;
    }
    QTextStream out3(&fout3);
    M3.WriteMedsText(out3);
    fout3.close();
}

void MainWindow::on_pokaz_clicked()
{
    ui->mb1count->clear();
    ui->mb1date->clear();
    ui->mb1name->clear();
    ui->mb1count->clear();
    ui->mb2name->clear();
    ui->mb2date->clear();
    ui->mb2price->clear();
    ui->mb3name->clear();
    ui->mb3date->clear();
    ui->mb3section->clear();
    ui->mb3price->clear();
    ui->mb3count->clear();
    ui->mb1name->addItem("NAME");
    ui->mb1date->addItem("DATE");
    ui->mb1section->addItem("SECTION");
    ui->mb1count->addItem("COUNT");
    for(int i=0; i<M1.getCount();i++)
    {
        ui->mb1name->addItem(M1[i].name);
        ui->mb1date->addItem(M1[i].date);
        ui->mb1section->addItem(M1[i].section);
        ui->mb1count->addItem(QString::number(M1[i].count));
    }
    ui->mb2name->addItem("NAME");
    ui->mb2date->addItem("DATE");
    ui->mb2price->addItem("PRICE");
    for(int i=0; i<M2.getCount();i++)
    {
        ui->mb2name->addItem(M2[i].name);
        ui->mb2date->addItem(M2[i].date);
        ui->mb2price->addItem(QString::number(M2[i].price));
    }
    ui->mb3name->addItem("NAME");
    ui->mb3date->addItem("DATE");
    ui->mb3section->addItem("SECTION");
    ui->mb3price->addItem("PRICE");
    ui->mb3count->addItem("COUNT");
    for(int i=0; i<M3.getCount();i++)
    {
        ui->mb3name->addItem(M3[i].name);
        ui->mb3date->addItem(M3[i].date);
        ui->mb3section->addItem(M3[i].section);
        ui->mb3price->addItem(QString::number(M3[i].price));
        ui->mb3count->addItem(QString::number(M3[i].count));
    }
}


void MainWindow::on_sort_clicked()
{
    M1.SortUp();
    M2.SortUp();
    M3.SortUp();
}


void MainWindow::on_pushButton_clicked()
{
    M1.SortUp();
    M2.SortUp();
    M3.SortUp();
    QFile fout3("C:/Users/Redmi/source/QT projects/untitled8/output3.txt");
    if (!fout3.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Error", "Cannot open output3.txt for writing");
        return;
    }
    QTextStream out3(&fout3);
    M3.WriteMedsText(out3);
    fout3.close();
    QFile fout2("C:/Users/Redmi/source/QT projects/untitled8/output2.txt");
    if (!fout2.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Error", "Cannot open output2.txt for writing");
        return;
    }
    QTextStream out2(&fout2);
    M2.WriteMedsText(out2);
    fout2.close();
    QFile fout1("C:/Users/Redmi/source/QT projects/untitled8/output1.txt");
    if (!fout1.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Error", "Cannot open output1.txt for writing");
        return;
    }
    QTextStream out1(&fout1);
    M1.WriteMedsText(out1);
    fout1.close();
}

