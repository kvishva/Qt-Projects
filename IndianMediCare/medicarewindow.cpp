#include "medicarewindow.h"
#include "ui_medicarewindow.h"
#include "medicaredatabase.h"

//
//initalize the view members
//connect signals with MediCareData instance methods
MediCareWindow::MediCareWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MediCareWindow),mModel(new medi::MediCareData(this))
{
    ui->setupUi(this);

    //set default page is Patient Registratoin Form
    ui->stackedWidget->setCurrentIndex(0);
    ui->lineEdit_Patient_Name->setFocus();
    ui->lcdnumber_Patient_Id->setSegmentStyle(QLCDNumber::Filled);
    ui->lcdnumber_Patient_Id->setMode(QLCDNumber::Dec);
    ui->lcdnumber_Patient_Id->setMinimumWidth(ui->lcdnumber_Patient_Id->width()+1);

    ui->comboBox_Gender->addItem("Male");
    ui->comboBox_Gender->addItem("Female");
    ui->comboBox_Gender->addItem("Transgender");

    ui->dateEdit_Patient_dob->setDisplayFormat("dd.MM.yyyy");
    ui->dateEdit_Patient_dob->setMinimumDate(QDate::currentDate().addYears(-100));
    ui->dateEdit_Patient_dob->setMaximumDate(QDate::currentDate());
    ui->dateEdit_Patient_dob->setDate(QDate::currentDate());
    ui->dateEdit_Patient_dob->setCalendarPopup(true);

    ui->lcdnumber_Exam_Id->setSegmentStyle(QLCDNumber::Filled);
    ui->lcdnumber_Exam_Id->setMode(QLCDNumber::Dec);
    ui->lcdnumber_Exam_Id->setMinimumWidth(ui->lcdnumber_Exam_Id->width()+1);

    ui->dateTimeEdit_Exam_date->setDisplayFormat("dd.MM.yyyy hh.mm.ss.zzz AP");
    ui->dateTimeEdit_Exam_date->setCalendarPopup(false);


    qDebug()<<" inst list in constructor";

    examTime.reset(new QTimer(this));

    connect(this,SIGNAL(setPatientId(int)),ui->lcdnumber_Patient_Id,SLOT(display(int)));

    connect(this,SIGNAL(saveInstitution(QStringList)),mModel.get(),SLOT(saveInstitutionDetails(QStringList)));
    connect(this,SIGNAL(getInstList()),mModel.get(),SLOT(getHospitalList()));
    connect(this,SIGNAL(savePatient(QStringList)),mModel.get(),SLOT(savePatientDetails(QStringList)));
    connect(this,SIGNAL(getNextPatientId()),mModel.get(),SLOT(getNextPatientId()));
    connect(this,SIGNAL(genPatientList()),mModel.get(),SLOT(generatePatientList()));
    connect(this,SIGNAL(getNextExamId()),mModel.get(),SLOT(generateNextExamId()));
    connect(examTime.get(),SIGNAL(timeout()),this,SLOT(updateExamDateTime()));
    connect(this,SIGNAL(saveExamination(QStringList)),mModel.get(),SLOT(savexamination(QStringList)));
    connect(this,SIGNAL(getPatientInst(int)),mModel.get(),SLOT(generatePatientInstId(int)));
    connect(this,SIGNAL(getExamPatient(QString)),mModel.get(),SLOT(generateExamListPatientId(QString)));
    connect(this,SIGNAL(getPatientExamDetails(int)),mModel.get(),SLOT(generatePatientExamDetails(int)));

    examTime->start(1000);

    emit getInstList();
    emit getNextPatientId();
    emit genPatientList();
    emit getNextExamId();
}


//delete the ui heap memory
MediCareWindow::~MediCareWindow()
{

    delete ui;
}

//navigate to patient view
void MediCareWindow::on_cmdLinkButton_Patients_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->lineEdit_Patient_Name->setFocus();
}

//navigate to institution view
void MediCareWindow::on_cmdLinkButton_Inst_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->lineEdit_Inst_Name->setFocus();
}

//navigate to reports view
void MediCareWindow::on_cmdLinkButton_Reports_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

//navigate to examination view
void MediCareWindow::on_cmdLinkButton_Examination_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    // ui->comboBox_Exam_Patient_Name->setFocus();
    updateExamDateTime();

}

//navigate to institution view
void MediCareWindow::on_cmdLinkButton_Inst_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->lineEdit_Inst_Name->setFocus();
}

//navigate to reports view
void MediCareWindow::on_cmdLinkButton_Reports_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

//navigate to patient view
void MediCareWindow::on_cmdLinkButton_Patient_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->lineEdit_Patient_Name->setFocus();

}

//navigate to examination view
void MediCareWindow::on_cmdLinkButton_Examination_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    // ui->comboBox_Exam_Patient_Name->setFocus();
    updateExamDateTime();
}

//navigate to institution view
void MediCareWindow::on_cmdLinkButton_Inst_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->lineEdit_Inst_Name->setFocus();

}

//navigate to patient view
void MediCareWindow::on_cmdLinkButton_Patients_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->lineEdit_Patient_Name->setFocus();
}

//navigate to examination view
void MediCareWindow::on_cmdLinkButton_Examination_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    // ui->comboBox_Exam_Patient_Name->setFocus();
    updateExamDateTime();
}

//navigate to report view
void MediCareWindow::on_cmdLinkButton_Reports_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

//submit the patient details
void MediCareWindow::on_pushButton_Patient_Submit_clicked()
{
    QStringList patientDetails;
    QString msg;

    patientDetails.append(ui->lineEdit_Patient_Name->text());
    patientDetails.append(ui->dateEdit_Patient_dob->date().toString());
    patientDetails.append(ui->comboBox_Gender->currentText());

    msg.sprintf("%d",ui->comboBox_Inst_Name->currentIndex()+1);
    patientDetails.append(msg);

    emit savePatient(patientDetails);
    ui->lineEdit_Patient_Name->clear();
    ui->lineEdit_Patient_Name->setFocus();
    emit genPatientList();


}

//set next patient id
void MediCareWindow::nextPatientId(int patientId)
{

    emit setPatientId(patientId);

}

//update institution list to institution view and report view
void MediCareWindow::institutionList(QStringList instList)
{

    QStringListModel *model(new QStringListModel(instList));
    ui->comboBox_Inst_Name->setModel(model);
    ui->listView_Inst->setModel(model);


}

//submit the institution details
void MediCareWindow::on_pushButton_Inst_Submit_clicked()
{
    QStringList list;
    list.append(ui->lineEdit_Inst_Name->text());
    list.append(ui->textEdit_Inst_Desc->toPlainText());
    emit saveInstitution(list);

    ui->lineEdit_Inst_Name->clear();
    ui->textEdit_Inst_Desc->clear();
    ui->lineEdit_Inst_Name->setFocus();

}

//un used
void MediCareWindow::on_comboBox_Inst_Name_activated(const QString &arg1)
{
    //ui->comboBox_Inst_Name->addItem(arg1);
}

//update the patient list
void MediCareWindow::updatePatientList(QStringList list)
{
    ui->comboBox_Exam_Patient_Name->clear();
    ui->comboBox_Exam_Patient_Name->addItems(list);
    /*QStringListModel *model = new QStringListModel(list);
    ui->listView_Patient->setModel(model);*/
}

//set next examination id
void MediCareWindow::nextExamId(int examId)
{
    ui->lcdnumber_Exam_Id->display(examId);
}

//submit the examination details
void MediCareWindow::on_pushButton_Exam_submit_clicked()
{
    QStringList list;
    QString msg;
    list.append(ui->dateTimeEdit_Exam_date->dateTime().toString());
    list.append(ui->comboBox_Exam_Patient_Name->currentText());
    list.append(ui->textBrowser_Exam_Desc->toPlainText());
    list.append(ui->lineEdit_Weight->text());
    list.append(ui->lineEdit_Height->text());
    msg.sprintf("%d",ui->comboBox_Exam_Patient_Name->currentIndex()+1);
    list.append(msg);

    emit saveExamination(list);

    updateExamDateTime();
    ui->textBrowser_Exam_Desc->clear();
    ui->lineEdit_Height->clear();
    ui->lineEdit_Weight->clear();
    ui->comboBox_Exam_Patient_Name->clear();

    emit getNextExamId();
    emit genPatientList();


}

//update the exam date and time for every second
void MediCareWindow::updateExamDateTime()
{

    ui->dateTimeEdit_Exam_date->clear();
    ui->dateTimeEdit_Exam_date->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit_Exam_date->update();

}

//un used
void MediCareWindow::on_listView_Inst_indexesMoved(const QModelIndexList &indexes)
{
    int nInstId = ui->listView_Inst->currentIndex().row();
    nInstId++;
}

//get the patient name based on institution id
void MediCareWindow::on_listView_Inst_clicked(const QModelIndex &index)
{
    int nInstId = ui->listView_Inst->currentIndex().row();
    emit getPatientInst(++nInstId);
}

//update the patient list in report view
void MediCareWindow::updatePatientListInstId(QStringList list)
{
    QStringListModel *model = new QStringListModel(list);
    ui->listView_Patient->setModel(model);
}

//get the patient's examination id
void MediCareWindow::on_listView_Patient_clicked(const QModelIndex &index)
{

    QString name = index.data().toString();
    emit getExamPatient(name);
}

//update the examination ids for paritcular patient
void MediCareWindow::updateExamListPatientId(QStringList list)
{
    QStringListModel *model = new QStringListModel(list);
    ui->listView_Exam->setModel(model);
}

//get the particular exam details for the select patient
void MediCareWindow::on_listView_Exam_clicked(const QModelIndex &index)
{

    int nId = index.data().toInt();
    emit getPatientExamDetails(nId);
}

//to show the examination details of patient
void MediCareWindow::updatePatientExamDetails(QStringList list)
{
    ui->textBrowser->clear();
    QString msg;
    QString examDate(list.at(0));
    QString patient(list.at(1));
    QString desc(list.at(2));
    QString weight(list.at(3));
    QString height(list.at(4));
    msg = "Patient Name : "+ patient;
    msg +="\n Exam Date : "+ examDate;
    msg +="\n Weight(kg): "+ weight;
    msg +="\n Height(cm): "+ height;
    msg +="\n Description:"+ desc;

    ui->textBrowser->setText(msg);


}
