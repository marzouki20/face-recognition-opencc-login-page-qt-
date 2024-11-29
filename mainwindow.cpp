#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCryptographicHash>
//#include <curl/curl.h>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QUrlQuery>
#include <QRandomGenerator>
#include <QNetworkReply>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Initialize QNetworkAccessManager
    networkManager = new QNetworkAccessManager(this);

    // Connect the finished signal to the response handler
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onNetworkReply);
    connect(ui->test, &QPushButton::clicked, this, &MainWindow::runPythonScript);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::on_pushButton_2_clicked);
    connect(ui->create, &QPushButton::clicked, this, &MainWindow::on_pushButton_create);
    connect(ui->retour1, &QPushButton::clicked, this, &MainWindow::retour);
    connect(ui->create1, &QPushButton::clicked, this, &MainWindow::on_insertButton_clicked);
    connect(ui->face, &QPushButton::clicked, this, &MainWindow::createface);
    connect(ui->RETOUR, &QPushButton::clicked, this, &MainWindow::RETOUR);
    connect(ui->reset, &QPushButton::clicked, this, &MainWindow::reset);
    connect(ui->verif, &QPushButton::clicked, this, &MainWindow::on_send_clicked);
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onNetworkReply);

    connect(ui->verif_2, &QPushButton::clicked, this, &MainWindow::verifyCodeAndEnablePasswordReset);
    connect(ui->resettt, &QPushButton::clicked, this, &MainWindow::resetPassword);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::runPythonScript()
{
    QProcess *process = new QProcess(this);


    QStringList env = QProcess::systemEnvironment();
    env << "PYTHONHOME=C:/Users/Admin/AppData/Local/Programs/Python/Python38";
    env << "PYTHONPATH=C:/Users/Admin/AppData/Local/Programs/Python/Python38/Lib";
    process->setEnvironment(env);


    QString pythonPath = "C:/Users/Admin/AppData/Local/Programs/Python/Python38/python.exe";


    QString pythonScript = "C:/Users/Admin/Desktop/opencv-qt/opencv/READFROMDATA.py";

    process->start(pythonPath, QStringList() << pythonScript);

    connect(process, &QProcess::finished, this, [process]() {
        if (process->exitCode() != 0) {
            QMessageBox::critical(nullptr, "Error", "Python script failed.");
        }
        process->deleteLater();
    });


    connect(process, &QProcess::readyReadStandardOutput, this, [this, process]() {
        QString output = process->readAllStandardOutput().trimmed();
        qDebug() << "Python Output: " << output;

        if (output.isEmpty()) {
            QMessageBox::critical(this, "Error", "No output received from the Python script.");
            return;
        }


        QSqlQuery query;
        bool result = false;
        query.prepare("SELECT name FROM IMAGE WHERE name = :name");
        query.bindValue(":name", output);


        if (query.exec() && query.next()) {
            result = true;
        } else {
            result = false;
        }


        if (result) {

            QMessageBox::information(this, "Login Success", "bonjourrr, " + output + " ! !!!!!!!.");
        } else {
            QMessageBox::critical(this, "Error", "Name not found in the database.");
        }
    });


    connect(process, &QProcess::readyReadStandardError, this, [process]() {
        qDebug() << "Python Error: " << process->readAllStandardError();
    });
}

//------------------------------------------------------------------------//
void MainWindow::on_pushButton_2_clicked()
{

    ui->lineEdit_2->setEchoMode(QLineEdit::Password);


    QString username = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();

    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYEE WHERE LOGIN = :username AND MODPASS = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {

        QMessageBox::information(this, "Login Successful", "Welcome, " + username + "!");

    } else {
        // If credentials are incorrect
        QMessageBox::warning(this, "Login Failed", "Invalid username or password. Please try again.");
    }
}

//---------------------------------create-------------------------------------------
void MainWindow::on_pushButton_create()
{
 ui->stackedWidget->setCurrentIndex(1);
}
void MainWindow::retour(){
     ui->stackedWidget->setCurrentIndex(0);

}
void MainWindow::RETOUR(){
     ui->stackedWidget->setCurrentIndex(0);

}void MainWindow::reset(){
    ui->stackedWidget->setCurrentIndex(2);

}
//-------------------------------------------insert the login and the mod pass -----------------------------------

void MainWindow::on_insertButton_clicked()
{
    ui->pass->setEchoMode(QLineEdit::Password);

    ui->log->setMaxLength(20);
    ui->pass->setMaxLength(20);

    QRegularExpression regex("^[A-Za-z0-9]+$");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regex, this);

    ui->log->setValidator(validator);
    ui->pass->setValidator(validator);

    QString login = ui->log->text();
    QString modpass = ui->pass->text();
    int cin = ui->cin->text().toInt(); // Assuming ui->cin is a QLineEdit and the CIN is an integer

    if (login.isEmpty() || modpass.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in both the login and password.");
        return;
    }

    int pos = 0;
    if (validator->validate(login, pos) != QValidator::Acceptable || validator->validate(modpass, pos) != QValidator::Acceptable) {
        QMessageBox::warning(this, "Input Error", "Login and password must be alphanumeric.");
        return;
    }

    // Prepare the SQL query to update the login and password where the cin matches
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE employee SET login = :login, modpass = :modpass WHERE CIN_EMP = :cin");
    updateQuery.bindValue(":cin", cin); // Bind the 'cin'
    updateQuery.bindValue(":login", login); // Bind the 'login'
    updateQuery.bindValue(":modpass", modpass); // Bind the 'modpass'

    if (!updateQuery.exec()) {
        QMessageBox::critical(this, "Update Error", updateQuery.lastError().text());
    } else {
        QMessageBox::information(this, "Success", "Employee login et mot de pass a jour ! !");
    }
}

//------------------------------------stock imaagessss to dataaabaseeeee ----------------------------------------
void MainWindow::createface()
{
    QProcess *process = new QProcess(this);


    QStringList env = QProcess::systemEnvironment();
    env << "PYTHONHOME=C:/Users/Admin/AppData/Local/Programs/Python/Python38";
    env << "PYTHONPATH=C:/Users/Admin/AppData/Local/Programs/Python/Python38/Lib";
    process->setEnvironment(env);


    QString pythonPath = "C:/Users/Admin/AppData/Local/Programs/Python/Python38/python.exe";


    QString pythonScript = "C:/Users/Admin/Desktop/opencv-qt/opencv/STOCKIMAGES.py";

    process->start(pythonPath, QStringList() << pythonScript);

    connect(process, &QProcess::finished, this, [process]() {
        if (process->exitCode() != 0) {
            QMessageBox::critical(nullptr, "Error", "Python script failed.");
        }
        process->deleteLater();
    });


    connect(process, &QProcess::readyReadStandardOutput, this, [process]() {
        qDebug() << "Python Output: " << process->readAllStandardOutput();
    });


    connect(process, &QProcess::readyReadStandardError, this, [process]() {
        qDebug() << "Python Error: " << process->readAllStandardError();
    });
}

//--------------------------------------------send pass ---------------------------------------------


void MainWindow::on_send_clicked()
{
    QString num = ui->NUM->text();  // Phone number from the line edit
   // QString text = ui->text->text();  // Additional text from the line edit

    if (num.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Phone number must be filled!");
        return;
    }

    sendSMS(num);  // Call the helper function to send SMS
}

void MainWindow::sendSMS(QString num)
{
    int verificationKey = QRandomGenerator::global()->bounded(100000, 999999);  // Generate random key
    QString messageBody = "Your FIX-IT verification code is: " + QString::number(verificationKey);

    QString recipient = "+216" + num;  // Format the recipient phone number
    QUrl url("https://api.twilio.com/2010-04-01/Accounts/AC434590747fc4d471bc4d370415c2104b/Messages.json");  // Replace with your new SID
    QNetworkRequest request(url);

    QString accountSid = "AC434590747fc4d471bc4d370415c2104b";  // Replace with your new Account SID
    QString authToken = "265ebce5cff465ac381778a483ae127b";   // Replace with your new Auth Token
    QString credentials = accountSid + ":" + authToken;
    QString authHeader = "Basic " + credentials.toUtf8().toBase64();

    request.setRawHeader("Authorization", authHeader.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
    params.addQueryItem("To", recipient);
    params.addQueryItem("From", "+18178544334");  // Replace with your new Twilio number
    params.addQueryItem("Body", messageBody);

    networkManager->post(request, params.toString(QUrl::FullyEncoded).toUtf8());
}


void MainWindow::onNetworkReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QMessageBox::information(this, "Success", "SMS sent successfully!");
    } else {
        QMessageBox::critical(this, "Error", "Failed to send SMS: " + reply->errorString());
    }
    reply->deleteLater();
}


//----------------------------------------------------------


void MainWindow::verifyCodeAndEnablePasswordReset()
{
    QString enteredCode = ui->code->text().trimmed();
    if (enteredCode.isEmpty()) {
        QMessageBox::warning(this, "Invalid Code", "Verification code cannot be empty.");
        return;
    }

    if (enteredCode == verificationCode) {
        QMessageBox::information(this, "Verified", "Code correct. Set a new password.");
        ui->lineEdit_6->setEnabled(true);
    } else {
        QMessageBox::warning(this, "Invalid Code", "The verification code is incorrect. Please try again.");
    }
}


QString MainWindow::hashPassword(const QString &password)
{
    QByteArray byteArray = password.toUtf8();
    QByteArray hashed = QCryptographicHash::hash(byteArray, QCryptographicHash::Sha256);
    return QString(hashed.toHex());
}
void MainWindow::resetPassword()
{
    QString newPassword = ui->lineEdit_6->text().trimmed();
    QString cin = ui->forgetuser->text();

    if (newPassword.isEmpty()) {
        QMessageBox::warning(this, "Error", "New password cannot be empty.");
        return;
    }

    QString hashedPassword = hashPassword(newPassword);
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYEE SET PASS = :password WHERE LOGIN = :cin");
    query.bindValue(":password", hashedPassword);
    query.bindValue(":cin", cin);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Password updated successfully.");

    } else {
        QMessageBox::warning(this, "Error", "Failed to update password. Please check your input and try again.");
    }
}
