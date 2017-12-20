#include "mainwindow.h"
#include "ui_dialog.h"
#include "contactswindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog){
        QWidget::setFixedSize(331, 281);
        ui->setupUi(this);
        getDevices(ui);
        readRecents(ui);
    }

MainWindow::~MainWindow(){
    delete ui;
}

void readRecents(const Ui::Dialog *ui){
    ifstream fileIn = readPreferences("recents");
    QString recent;
    std::string line;
    while(std::getline(fileIn, line)){
        recent = QString::fromUtf8(line.c_str());
        if(ui->destinationSelect->findText(recent) == -1){
            ui->destinationSelect->addItem(recent);
        }
    }
    ui->destinationSelect->clearEditText();
}

int getDevices(const Ui::Dialog* ui){
    ui->refreshButton->setVisible(false);
    ui->refreshButton->setEnabled(false);
    ui->sendButton->setVisible(true);
    ui->sendButton->setEnabled(true);
    QString availableOutput = QString::fromStdString(execute("kdeconnect-cli -a"));
    if(availableOutput.at(0) == '0'){
        ui->sendButton->setVisible(false);
        ui->sendButton->setEnabled(false);
        ui->refreshButton->setVisible(true);
        ui->refreshButton->setEnabled(true);
    }
    QStringList lines = availableOutput.split("\n");
    QStringList devices = QStringList();
    QStringList halfLines = QStringList();
    foreach(QString line, lines.filter(":")){
        QStringList temp = line.split(":");
        halfLines.append(temp.at(1));
    }
    foreach(QString line, halfLines){
        QStringList temp = line.split("(");
        devices.append(temp.at(0));
    }
    foreach(QString device, devices){
        ui->deviceSelect->addItem(device);
    }
    return 0;
}

void MainWindow::on_sendButton_clicked(){
    QRegExp re("\\d*");
    if (!re.exactMatch(ui->destinationSelect->currentText()) || ui->destinationSelect->currentText().isEmpty()){
        ui->sendButton->setText("Please enter a valid phone number");
    }
    else if(ui->message->toPlainText().isEmpty()){
        ui->sendButton->setText("Please enter a message");
    }
    else{
        QString command = "kdeconnect-cli -d " + ui->deviceSelect->currentText() + " --destination " + ui->destinationSelect->currentText() + " --send-sms \"" + ui->message->toPlainText() + "\"";
        QByteArray ba = command.toLatin1();
        const char* cmd = ba.data();
        execute(cmd);
        if(ui->destinationSelect->findText(ui->destinationSelect->currentText()) == -1){
            ui->destinationSelect->addItem(ui->destinationSelect->currentText());
            writePreference("recents", ui->destinationSelect->currentText().toStdString());
        }
        ui->destinationSelect->clearEditText();
        ui->message->clear();
        ui->sendButton->setText("Send Message");
    }
}

void MainWindow::on_refreshButton_clicked(){
    getDevices(ui);
}

QComboBox* MainWindow::getDestinationSelect(){
    return ui->destinationSelect;
}

void MainWindow::on_destinationButton_clicked(){
    ContactsWindow* cw = new ContactsWindow();
    cw->setListener(this);
    cw->show();
}

void MainWindow::on_message_textChanged(){
    if(ui->message->toPlainText().isEmpty()){
        ui->sendButton->setText("Please enter a message");
    }
    else{
        ui->sendButton->setText("Send Message");
    }
}

void MainWindow::on_destinationSelect_currentTextChanged(const QString &arg1){
    QRegExp re("\\d*");
    if (!re.exactMatch(ui->destinationSelect->currentText()) || ui->destinationSelect->currentText().isEmpty()){
        ui->sendButton->setText("Please enter a valid phone number");
    }
    else{
        ui->sendButton->setText("Send Message");
    }
}
