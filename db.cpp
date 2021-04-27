#include "db.h"
#include <QtDebug>

DB::DB()
{


}
//============================================
DB::~DB()
{
    delete querly;
    delete model;
    delete  tmodel;
    delete  file;
    delete  word;
    delete  doc;
}
//==============================================
bool DB::GreateDb()
{
    bool fl{false};
    QString path{Path.currentPath()+"/"+"test.db"};
    db = QSqlDatabase::addDatabase("QSQLITE");

    if(!QFile(path).exists()){
        db.setDatabaseName(path);
       if (db.open()) {
         querly = new QSqlQuery(db);
        fl = true;
        querly->exec("CREATE TABLE product ""(id integer primary key, ""name varchar(30), ""col integer,"
                      """date varchar(10), ""price double)");
        querly->exec("CREATE TABLE productsales ""(id integer, ""name varchar(30), ""col integer,"
                      """date varchar(10), ""price double, ""pricesales double )");

       }

       }

    else {     
        db.setDatabaseName(path);
       if (db.open()) {fl = true; querly = new QSqlQuery(db);}

       }

    return fl;
}
//==========================================================================
QSqlQueryModel* DB::GetProduct()
{

    model = new QSqlQueryModel();

    model->setQuery("SELECT * FROM product");
    model->setHeaderData(0,Qt::Horizontal,"Номер позиции");
    model->setHeaderData(1,Qt::Horizontal,"Наименование товара");
    model->setHeaderData(2,Qt::Horizontal,"В наличии");
    model->setHeaderData(3,Qt::Horizontal,"Дата производства");
    model->setHeaderData(4,Qt::Horizontal,"Цена на реализацию за ед. товара");

    return model;

}

//=========================================================================
void DB::addProduct(QVector<QString> const &v)
{

    querly->prepare("INSERT INTO product(name, col, date, price) VALUES(:name, :col, :date, :price)");

     auto it = v.begin();

     querly->bindValue(":name",*it);

     std::advance(it,1);
     querly->bindValue(":col",it->toInt());

     std::advance(it,1);
     querly->bindValue(":date",*it);

     std::advance(it,1);
     querly->bindValue(":price",it->toDouble());

     querly->exec();    

    }
//=====================================================
bool DB::delProduct(int const & id)
{

    querly->prepare("DELETE FROM product WHERE id=?");

    querly->bindValue(0,id);
    return querly->exec();
}
//===========================================================
QVector<QString>& DB::getPoroductInfo(int const & id)
{
      querly->prepare("SELECT name, col, date, price FROM product WHERE id=?");
      querly->bindValue(0,id);

        vec.clear();

        if(querly->exec()){
            querly->next();
            vec.push_back(querly->value(0).toString());
            vec.push_back(querly->value(1).toString());
            vec.push_back(querly->value(2).toString());
            vec.push_back(querly->value(3).toString());
            vec.push_back(QString::number(id));
        }

        return vec;
}
//=======================================================================
bool DB::updateProduct(QVector<QString> const & vec)
{

     querly->prepare("UPDATE product SET name = ?, col = ?, date = ?, price = ?  WHERE id = ?");
     auto it = vec.begin();

     querly->bindValue(0,*it);
     std::advance(it,1);

     querly->bindValue(1,it->toInt());
     std::advance(it,1);

     querly->bindValue(2,*it);
     std::advance(it,1);

     querly->bindValue(3,it->toDouble());
     std::advance(it,1);

     querly->bindValue(4,it->toInt());

     return querly->exec();;

}
//===========================================================================
void DB::addProductSales(QVector<QString> const & vec)
{

    querly->prepare("INSERT INTO productsales(name, col, date, price, pricesales, id)"
                   " VALUES(:name, :col, :date, :price, :pricesales, :id)");

    auto it = vec.begin();

    querly->bindValue(":name",*it);

    std::advance(it,1);
    querly->bindValue(":col",it->toInt());

    std::advance(it,1);
    querly->bindValue(":date",*it);

    std::advance(it,1);
    querly->bindValue(":price",it->toDouble());

    std::advance(it,1);
    querly->bindValue(":pricesales",it->toDouble());

    std::advance(it,1);
    querly->bindValue(":id",it->toInt());

    querly->exec();

}
//==========================================================================================
QSqlTableModel* DB::GetProductSales()
{
    tmodel = new QSqlTableModel();
    tmodel->setTable("productsales");
    tmodel->select();

    tmodel->setHeaderData(0,Qt::Horizontal,"Номер позиции");
    tmodel->setHeaderData(1,Qt::Horizontal,"Наименование товара");
    tmodel->setHeaderData(2,Qt::Horizontal,"Кол. реализованного товара");
    tmodel->setHeaderData(3,Qt::Horizontal,"Дата реализации");
    tmodel->setHeaderData(4,Qt::Horizontal,"Цена на реализацию за ед. товара");
    tmodel->setHeaderData(5,Qt::Horizontal,"Сумма за реализацию");

    if(querly->exec("SELECT * FROM productsales")){

        while (querly->next()) {

           total += querly->value(5).toDouble();
        }

    }

    return tmodel;
}

double total{};
//============================================================================
void DB::printData(const QString &str, QObject *obj)
{

    if(str == "SALES")
    {

        file = new QFile(Path.currentPath()+"/"+"sales.html");
        file->open(QIODevice::WriteOnly);

        QTextStream in(file);
        in << "<html><head><head><body><center>" + QString("Отчет о продажах");
        in << "<table bolder-1><tr>";
        in << "<td>" + QString("Номер позиции") + "<td>";
        in << "<td>" + QString("Наименование товара") + "<td>";
        in << "<td>" + QString("Кол. реализованного товара") + "<td>";
        in << "<td>" + QString("Дата реализации") + "<td>";
        in << "<td>" + QString("Цена на реализацию за ед. товара") + "<td>";
        in << "<td>" + QString("Сумма за реализацию  товара") + "<td><tr>";

        querly->exec("SELECT * FROM productsales");


        while (querly->next()) {

            in << "<td>";
            in << querly->value(0).toString();
            in << "<td><td>";
            in << querly->value(1).toString();
            in << "<td><td>";
            in << querly->value(2).toString();
            in << "<td><td>";
            in << querly->value(3).toString();
            in << "<td><td>";
            in << querly->value(4).toString();
            in << "<td><td>";
            in << querly->value(5).toString();
            in << "<td><tr>";
        }

        in << QString("Итоговая сумма :");
        in << "<td>";
        in << QString::number(total);
        in <<"<table><center><body><html>";
        file->close();

        word= new QAxObject("Word.Application",obj);
        word->setProperty("DisplayAlerts", false);
        word->setProperty("Visible", true);
        doc = word->querySubObject("Documents");
        doc->dynamicCall("Open(QVariant)",Path.currentPath()+"/"+"sales.html");


    }


}


