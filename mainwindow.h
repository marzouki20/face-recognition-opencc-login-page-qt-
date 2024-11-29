#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "create.h"  // Include the create header to use the create dialog
#include <QNetworkAccessManager>
#include <QNetworkReply>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void runPythonScript();
    void on_pushButton_2_clicked();
   void on_pushButton_create();
   void retour();
   void on_insertButton_clicked();
   void createface();// Slot for the button to go to create window
   void RETOUR();
   void reset();
   void on_send_clicked();
   void onNetworkReply(QNetworkReply *reply); // Slot for handling network responses


   void verifyCodeAndEnablePasswordReset();
   void resetPassword();
   QString hashPassword(const QString &password);


private:
    Ui::MainWindow *ui;
private:
    QString verificationCode;
    QNetworkAccessManager *networkManager;

    // Helper function to send an SMS
    void sendSMS(QString num);

};

#endif // MAINWINDOW_H
