#include "medicaredatabase.h"
#include "medicarewindow.h"

using namespace medi;

//Default constructor
MediCareData::MediCareData(QObject *parent):QObject(parent)
{
    mWindow = nullptr;

    qDebug() << "default constructor";

    if(!QSqlDatabase::drivers().contains("QSQLITE"))
        qDebug() <<"Unable to load database, the app needs SQLITE driver";

    // intialize the database
    initDb();

}

//
// Initalize the Medicarewindow class
// Initialize the database
// connect the signals to main window class
//
MediCareData::MediCareData(MediCareWindow* window):mWindow(window)
{
    qDebug() << "copy constructor";

    if(!QSqlDatabase::drivers().contains("QSQLITE"))
        qDebug() <<"Unable to load database, the app needs SQLITE driver";

    // intialize the database
    initDb();

    QObject::connect(this,&MediCareData::nextPatientId,mWindow,&MediCareWindow::nextPatientId);
    connect(this,SIGNAL(populateHospitalList(QStringList)),mWindow,SLOT(institutionList(QStringList)));
    connect(this,SIGNAL(populatePatientList(QStringList)),mWindow,SLOT(updatePatientList(QStringList)));
    connect(this,SIGNAL(nextExamId(int)),mWindow,SLOT(nextExamId(int)));
    connect(this,SIGNAL(patientListInstId(QStringList)),mWindow,SLOT(updatePatientListInstId(QStringList)));
    connect(this,SIGNAL(examListPatientId(QStringList)),mWindow,SLOT(updateExamListPatientId(QStringList)));
    connect(this,SIGNAL(patientExamDetails(QStringList)),mWindow,SLOT(updatePatientExamDetails(QStringList)));
}

//close the database
MediCareData::~MediCareData()
{
    if(mDb.isOpen())
        mDb.close();

}

//
// connect sqlite database wit QSQLITE driver
//create medicare database if not exists
// create tables (patient,institution,examination)
//
int MediCareData::initDb()
{
    //
    mDb = QSqlDatabase::addDatabase("QSQLITE");
    mDb.setDatabaseName("medicare");

    if(!mDb.open()){
        mError = mDb.lastError();
        qDebug()<<"Unable to open database : "<< mError.text();
        return 0;
    }

    QSqlQuery mQuery(mDb);

    QStringList tables = mDb.tables();

    // to check institution table is present or not
    if(!tables.contains("institution",Qt::CaseInsensitive))
    {
        // create institution table here
        mQuery.clear();
        if(!mQuery.exec("CREATE TABLE institution(id INTEGER PRIMARY KEY AUTOINCREMENT,name varchar(30),description varchar(30))"))
        {
            mError = mQuery.lastError();
            qDebug()<<"Unable to create institution table: "<< mError.text();
            return 0;

        }

        //insert default data
        mQuery.clear();
        if(!mQuery.prepare(QLatin1String("insert into institution(name,description) values(?,?)")))

        {
            mError = mQuery.lastError();
            qDebug()<<"unable to insert into institution table Error: "<< mError.text();
            return 0;

        }
        else
        {


            QString name("Indian Medi Care");
            QString description("Electornic City Phase I,Bangalore,India");

            mQuery.addBindValue(name);
            mQuery.addBindValue(description);
            mQuery.exec();
        }


    }

    // to check patient table is present or not
    if(!tables.contains("patient",Qt::CaseInsensitive))
    {
        // create patients table here
        mQuery.clear();
        if(!mQuery.exec("CREATE TABLE patient(id INTEGER PRIMARY KEY AUTOINCREMENT,name varchar(30),DoB date,gender int,InstId int, FOREIGN KEY(InstId) REFERENCES  institution(id))"))
        {
            mError = mQuery.lastError();
            qDebug()<<"ubable to create patients table Error: "<< mError.text();
            return 0;

        }

    }


    // to check examination table is present or not
    if(!tables.contains("examination",Qt::CaseInsensitive))
    {
        // create examination table here
        mQuery.clear();
        if(!mQuery.exec("CREATE TABLE examination(id INTEGER PRIMARY KEY AUTOINCREMENT,examdate date,name varchar(30),description varchar(30),weight int,height int,patientId int, FOREIGN KEY(patientId) REFERENCES patient(id))"))
        {
            mError = mQuery.lastError();
            qDebug()<<"unable to create examination table Error: "<< mError.text();
            return 0;

        }
    }


    return 0;


}

//unused method
void MediCareData::showError()
{
    qDebug()<<"Database Error: "<< mError.text();
}

//store the patient details in patient table
void MediCareData::savePatientDetails(QStringList list)
{
    QSqlQuery mQuery(mDb);

    if(mQuery.prepare(QLatin1String("insert into patient(name,DoB,gender,InstId) values(?,?,?,?)")))

    {


        QString name(list.at(0));
        QString DoB(list.at(1));
        QString temp(list.at(2));
        QString tempInst(list.at(3));

        mQuery.addBindValue(name);
        mQuery.addBindValue(DoB);
        mQuery.addBindValue(temp.toInt());
        mQuery.addBindValue(tempInst.toInt());

        if(!mQuery.exec())
            qDebug() << "issue while inserting patient details"<<mQuery.lastError().text();

    }
    int id = mQuery.lastInsertId().toInt();

    emit nextPatientId(id);
}

//query the list of patient
void MediCareData::generatePatientList()
{
    if(!mDb.isOpen()){
        qDebug()<<"Unable to open database : "<< mError.text();
        initDb();
    }

    QSqlQuery mQuery(mDb);
    if(!mQuery.exec("select name from patient"))
        qDebug()<<"issue in patient table"<<mQuery.lastError().text();

    QStringList list;
    while(mQuery.next())
        list.append(mQuery.value("name").toString());

    emit populatePatientList(list);


}

//un used
void MediCareData::generateExaminations(int patientId)
{
    patientId = 0;
}

//get next patient id from record count
int MediCareData::getNextPatientId()
{
    if(!mDb.isOpen()){
        qDebug()<<"Unable to open database : "<< mError.text();
        initDb();
    }

    QSqlQuery mQuery(mDb);
    if(!mQuery.exec("select name from patient"))
        qDebug()<<"issue in count patient "<<mQuery.lastError().text();
    qDebug()<<"Query " <<mQuery.executedQuery();

    mQuery.last();

    int nCount = mQuery.at()+1;
    emit nextPatientId(++nCount);
    return nCount;


}

//get institutions list
void MediCareData::getHospitalList()
{
    QSqlQuery mQuery(mDb);
    QStringList InstList;
    mQuery.exec("select name from institution");
    while(mQuery.next())
    {
        QString name = mQuery.value("name").toString();

        InstList.append(name);
    }
    emit populateHospitalList(InstList);
}

//to store the institution details in institution table
void MediCareData::saveInstitutionDetails(QStringList list)
{
    QSqlQuery mQuery(mDb);

    if(mQuery.prepare(QLatin1String("insert into institution(name,description) values(?,?)")))

    {


        QString name(list.at(0));
        QString description(list.at(1));

        mQuery.addBindValue(name);
        mQuery.addBindValue(description);
        if(!mQuery.exec())
            qDebug() << "issue while inserting institution details"<<mQuery.lastError().text();

    }

    getHospitalList();
}

// get next examination id from record count
void MediCareData::generateNextExamId()
{
    if(!mDb.isOpen()){
        qDebug()<<"Unable to open database : "<< mError.text();
        initDb();
    }

    QSqlQuery mQuery(mDb);
    if(!mQuery.exec("select id from examination"))
        qDebug()<<"issue in count examination "<<mQuery.lastError().text();


    mQuery.last();

    int nCount = mQuery.at()+1;
    emit nextExamId(++nCount);

}


//to store the examination details in examination table
void MediCareData::savexamination(QStringList list)
{
    QSqlQuery mQuery(mDb);
    //id,examdate,name,description,weight,height,patientId
    if(mQuery.prepare(QLatin1String("insert into examination(examdate,name,description,weight,height,patientId) values(?,?,?,?,?,?)")))

    {


        QString examdate(list.at(0));
        QString name(list.at(1));
        QString desc(list.at(2));
        QString weight(list.at(3));
        QString height(list.at(4));
        QString patId(list.at(5));

        mQuery.addBindValue(examdate);
        mQuery.addBindValue(name);
        mQuery.addBindValue(desc);
        mQuery.addBindValue(weight.toInt());
        mQuery.addBindValue(height.toInt());
        mQuery.addBindValue(patId.toInt());

        if(!mQuery.exec())
            qDebug() << "issue while inserting examination details"<<mQuery.lastError().text();

    }
}

//get patient name based on institution id
void MediCareData::generatePatientInstId(int instId)
{
    if(!mDb.isOpen()){
        qDebug()<<"Unable to open database : "<< mError.text();
        initDb();
    }

    QString msg;
    msg.sprintf("select name from patient where InstId=%d",instId);
    QSqlQuery mQuery(mDb);
    if(!mQuery.exec(msg))
        qDebug()<<"issue in patient table"<<mQuery.lastError().text();

    QStringList list;
    while(mQuery.next())
        list.append(mQuery.value("name").toString());
    emit patientListInstId(list);
}

//get examination id for patient
void MediCareData::generateExamListPatientId(QString name)
{
    if(!mDb.isOpen()){
        qDebug()<<"Unable to open database : "<< mError.text();
        initDb();
    }

    QString msg;
    msg.sprintf("select id from examination where "
                "patientId IN (select id from patient "
                "where name LIKE '%s')",name.toStdString().c_str());
    qDebug()<<msg;
    QSqlQuery mQuery(mDb);
    if(!mQuery.exec(msg))
        qDebug()<<"issue in patient table"<<mQuery.lastError().text();

    QStringList list;
    while(mQuery.next())
        list.append(mQuery.value("id").toString());

    emit examListPatientId(list);
}

//get the patient's examination details based on examination id
void MediCareData::generatePatientExamDetails(int examId)
{
    if(!mDb.isOpen()){
        qDebug()<<"Unable to open database : "<< mError.text();
        initDb();
    }

    QString msg;
    msg.sprintf("select examdate,name,description,weight,"
                "height from examination where id=%d",examId);
    QSqlQuery mQuery(mDb);
    if(!mQuery.exec(msg))
        qDebug()<<"issue in examination table"<<mQuery.lastError().text();

    QStringList list;
    while(mQuery.next()){
        list.append(mQuery.value("examdate").toString());
        list.append(mQuery.value("name").toString());
        list.append(mQuery.value("description").toString());
        list.append(mQuery.value("weight").toString());
        list.append(mQuery.value("height").toString());
    }

    emit patientExamDetails(list);

}






























































































































































