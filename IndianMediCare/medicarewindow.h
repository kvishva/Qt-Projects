/**********************************************************
 * Author : Vishvanathan Kuppusamy
 * Date: 28-May-2018
 *
 * It deals the business logic and
 * communcate with view and model
 *
 *
 * ********************************************************/

#ifndef MEDICAREWINDOW_H
#define MEDICAREWINDOW_H

#include <QWidget>
#include <QScopedPointer>
#include <QTimer>

#include <QStringListModel>

namespace medi  {

class MediCareData;

}


namespace Ui {
class MediCareWindow;
}

class MediCareWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MediCareWindow(QWidget *parent = 0);
    ~MediCareWindow();
public slots:
    void nextPatientId(int patientId);
    void institutionList(QStringList instList);
    void updatePatientList(QStringList list);
    void nextExamId(int examId);
    void updateExamDateTime();
    void updatePatientListInstId(QStringList list);
    void updateExamListPatientId(QStringList list);
    void updatePatientExamDetails(QStringList list);


private slots:
    void on_cmdLinkButton_Patients_clicked();

    void on_cmdLinkButton_Inst_2_clicked();

    void on_cmdLinkButton_Reports_2_clicked();

    void on_cmdLinkButton_Examination_clicked();

    void on_cmdLinkButton_Inst_clicked();

    void on_cmdLinkButton_Reports_clicked();

    void on_cmdLinkButton_Patient_clicked();

    void on_cmdLinkButton_Examination_3_clicked();

    void on_cmdLinkButton_Inst_3_clicked();

    void on_cmdLinkButton_Patients_2_clicked();

    void on_cmdLinkButton_Examination_2_clicked();

    void on_cmdLinkButton_Reports_3_clicked();

    void on_pushButton_Patient_Submit_clicked();
    void on_pushButton_Inst_Submit_clicked();

    void on_comboBox_Inst_Name_activated(const QString &arg1);

    void on_pushButton_Exam_submit_clicked();

    void on_listView_Inst_indexesMoved(const QModelIndexList &indexes);

    void on_listView_Inst_clicked(const QModelIndex &index);

    void on_listView_Patient_clicked(const QModelIndex &index);

    void on_listView_Exam_clicked(const QModelIndex &index);

signals:
    void setPatientId(int patientId);
    void saveInstitution(QStringList list);
    void getInstList();
    void savePatient(QStringList list);
    void getNextPatientId();
    void genPatientList();
    void getNextExamId();
    void saveExamination(QStringList list);
    void getPatientInst(int instId);
    void getExamPatient(QString);
    void getPatientExamDetails(int examId);

private:
    Ui::MediCareWindow *ui;
    QScopedPointer<medi::MediCareData> mModel;
    QScopedPointer<QTimer> examTime;
};

#endif // MEDICAREWINDOW_H
