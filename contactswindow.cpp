#include "contactswindow.h"
#include "ui_contactswindow.h"
#include "preferences.h"
#include <fstream>
#include <iostream>

ContactsWindow::ContactsWindow(QWidget *parent) : QDialog(parent),ui(new Ui::ContactsWindow){
    ui->setupUi(this);
    getContacts();
    showContacts();
}

ContactsWindow::~ContactsWindow(){
    delete ui;
}

void ContactsWindow::setListener(MainWindow *listenerIn){
    listener = listenerIn;
}

void ContactsWindow::getContacts(){
    ifstream fileIn = readPreferences("contacts");
    std::string line;
    names.clear();
    numbers.clear();
    while(std::getline(fileIn, line)){
        QString contact = QString::fromUtf8(line.c_str());
        QRegExp rx("(\\:)");
        QStringList contactSplit = contact.split(rx);
        names.append(contactSplit.at(0));
        numbers.append(contactSplit.at(1));
    }
}

void ContactsWindow::showContacts(){
    int index = 0;
    QStandardItemModel* contactListModel = new QStandardItemModel();
    ui->ContactsList->connect(ui->ContactsList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_ContactsList_doubleClicked()));
    ui->ContactsList->setModel(contactListModel);
    foreach(QString name , names){
        QStandardItem* contact = new QStandardItem(name + "\t" + numbers.at(index));
        contactListModel->appendRow(contact);
        index++;
    }
}

void ContactsWindow::on_addContactButton_clicked(){
    ui->addContactButton->setVisible(false);
}

void ContactsWindow::on_ContactsList_doubleClicked(){
    QString contactText = ui->ContactsList->selectionModel()->selectedIndexes().at(0).data(Qt::DisplayRole).toString();
    QStringList contactTextList = contactText.split("\t");
    QString contactNumberText = contactTextList.at(1);
    listener->getDestinationSelect()->setEditText(contactNumberText);
    this->close();
}

void ContactsWindow::on_okayButton_clicked(){
    ui->addContactButton->setVisible(true);
    QString contact = ui->nameTextEdit->toPlainText() + ":" + ui->numberTextEdit->toPlainText();
    writePreference("contacts", contact.toStdString());
    ui->nameTextEdit->clear();
    ui->numberTextEdit->clear();
    getContacts();
    showContacts();
}
