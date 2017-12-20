#include "contactswindow.h"
#include "ui_contactswindow.h"
#include "preferences.h"
#include <fstream>
#include <iostream>
#include <QTableWidget>
#include <QMenu>

ContactsWindow::ContactsWindow(QWidget *parent) : QDialog(parent),ui(new Ui::ContactsWindow){
    ui->setupUi(this);
    QWidget::setFixedSize(401, 511);
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
    QMap<QString, QString> contactsMap;
    while(std::getline(fileIn, line)){
        QString contact = QString::fromUtf8(line.c_str());
        QRegExp rx("(\\:)");
        QStringList contactSplit = contact.split(rx);
        contactsMap.insert(contactSplit.at(0), contactSplit.at(1));
    }
    names = contactsMap.keys();
    numbers = contactsMap.values();
}

void ContactsWindow::showContacts(){
    QStandardItemModel* contactListModel = new QStandardItemModel(1, 2);
    ui->ContactsList->connect(ui->ContactsList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_ContactsList_doubleClicked()));
    ui->ContactsList->setModel(contactListModel);
    for(int index = 0; index < numbers.length(); index++){
        QStandardItem* nameItem = new QStandardItem(names.at(index));
        QStandardItem* numberItem = new QStandardItem("\t"+numbers.at(index));
        QList<QStandardItem*> contact;
        contact.append(nameItem);
        contact.append(numberItem);
        contactListModel->appendRow(contact);
        ui->ContactsList->setColumnWidth(index, 175);
    }
    contactListModel->removeRow(0);
    ui->ContactsList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->ContactsList->setContextMenuPolicy(Qt::CustomContextMenu);
}

void ContactsWindow::on_addContactButton_clicked(){
    ui->addContactButton->setVisible(false);
}

void ContactsWindow::on_ContactsList_doubleClicked(){
    int rowIndex = ui->ContactsList->selectionModel()->selection().indexes().at(0).row();
    QString numberText = ui->ContactsList->selectionModel()->selection().indexes().at(0).sibling(rowIndex, 1).data().toString();
    numberText.remove(0, 1);
    listener->getDestinationSelect()->setEditText(numberText);
    this->close();
}

void ContactsWindow::editContact(){
    ui->addContactButton->setVisible(false);
    ui->okayButton->disconnect(ui->okayButton, SIGNAL(clicked()), this, SLOT(on_okayButton_clicked()));
    ui->okayButton->connect(ui->okayButton, SIGNAL(clicked()), this, SLOT(onOkayButtonClickedEdit()));
}

void ContactsWindow::onOkayButtonClickedEdit(){
    QString newContact = ui->nameTextEdit->text() + ":" + ui->numberTextEdit->text();
    int rowIndex = ui->ContactsList->selectionModel()->selection().indexes().at(0).row();
    QString numberText = ui->ContactsList->selectionModel()->selection().indexes().at(0).sibling(rowIndex, 1).data().toString().remove(0, 1);
    QString nameText = ui->ContactsList->selectionModel()->selection().indexes().at(0).data().toString();
    QString contactToEdit = nameText + ":" + numberText;
    editPreference("contacts", contactToEdit.toStdString(), newContact.toStdString());
    ui->okayButton->disconnect(ui->okayButton, SIGNAL(clicked()), this, SLOT(onOkayButtonClickedEdit()));
    ui->okayButton->connect(ui->okayButton, SIGNAL(clicked()), this, SLOT(on_okayButton_clicked()));
    ui->addContactButton->setVisible(true);
    getContacts();
    showContacts();
}

void ContactsWindow::removeContact(){
    int rowIndex = ui->ContactsList->selectionModel()->selection().indexes().at(0).row();
    QString numberText = ui->ContactsList->selectionModel()->selection().indexes().at(0).sibling(rowIndex, 1).data().toString().remove(0, 1);
    QString nameText = ui->ContactsList->selectionModel()->selection().indexes().at(0).data().toString();
    QString contactToRemove = nameText + ":" + numberText;
    removePreference("contacts", contactToRemove.toStdString());
    getContacts();
    showContacts();
}

void ContactsWindow::on_okayButton_clicked(){
    ui->addContactButton->setVisible(true);
    QString contact = ui->nameTextEdit->text() + ":" + ui->numberTextEdit->text();
    writePreference("contacts", contact.toStdString());
    ui->nameTextEdit->clear();
    ui->numberTextEdit->clear();
    getContacts();
    showContacts();
}

void ContactsWindow::on_ContactsList_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    QAction *editAction = new QAction("Edit Contact", this);
    connect(editAction, SIGNAL(triggered()), this, SLOT(editContact()));

    QAction *removeAction = new QAction("Remove Contact", this);
    connect(removeAction, SIGNAL(triggered()), this, SLOT(removeContact()));

    QMenu* menu = new QMenu(this);
    menu->addAction(editAction);
    menu->addAction(removeAction);
    menu->exec(QCursor::pos());
}
