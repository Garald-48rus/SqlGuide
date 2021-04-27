#ifndef DB_H
#define DB_H

#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QDir>
#include <QString>
#include <QFile>
#include <QVector>
#include <QAxObject>


class DB
{
public:
    DB();
    ~DB();

public:
    bool GreateDb();
    QSqlQueryModel* GetProduct();
    QSqlTableModel* GetProductSales();
    void addProduct(QVector<QString> const &);
    bool delProduct(int const &);
    QVector<QString>& getPoroductInfo(int const &);
    bool updateProduct(QVector<QString> const &);
    void addProductSales(QVector<QString> const &);
    void printData(const QString &, QObject *);

private:
    QSqlDatabase db;
    QSqlQueryModel* model;
    QSqlTableModel* tmodel;
    QSqlQuery* querly;
    QFile* file;
    QAxObject* word;
    QAxObject* doc;
    QVector<QString> vec;
    QDir Path;

};
extern double total;

#endif // DB_H
