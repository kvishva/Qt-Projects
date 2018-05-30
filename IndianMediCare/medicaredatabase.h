/************************************************************
 * Author : Vishvanathan Kuppusamy
 * Date :28-May-2018
 *
 * The MediCareData is model class and connect with Sqlite
 * database.
 *
 * **********************************************************/



#ifndef MEDICAREDATABASE_H
#define MEDICAREDATABASE_H

#include <QObject>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QMessageBox>

class MediCareWindow;

namespace medi {

class MediCareData:public QObject
{
    Q_OBJECT

public:
    explicit MediCareData(QObject *parent = 0);
    MediCareData(MediCareWindow *window);
   virtual ~MediCareData();

public slots:
    void savePatientDetails(QStringList list);
    void generatePatientList();
    void generateExaminations(int patientId);
    void saveInstitutionDetails(QStringList list);
    void getHospitalList();
    int getNextPatientId();
    void generateNextExamId();
    void savexamination(QStringList list);
    void generatePatientInstId(int instId);
    void generateExamListPatientId(QString name);
    void generatePatientExamDetails(int examId);

signals:
    void nextPatientId(int patientId);
    void populateHospitalList(QStringList);
    void populatePatientList(QStringList);
    void nextExamId(int examId);
    void patientListInstId(QStringList);
    void examListPatientId(QStringList);
    void patientExamDetails(QStringList);


private:
    int initDb();
    void showError();

    QSqlDatabase mDb;
    QSqlError mError;
    MediCareWindow *mWindow;



};


}

#endif // MEDICAREDATABASE_H
