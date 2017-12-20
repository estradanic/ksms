#ifndef CONTACTSWINDOW_H
#define CONTACTSWINDOW_HMainWindow

#include <QDialog>
#include <QStandardItem>
#include "mainwindow.h"

namespace Ui {
class ContactsWindow;
}

class ContactsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ContactsWindow(QWidget *parent = 0);
    ~ContactsWindow();
    void setListener(MainWindow *listenerIn);


private slots:
    void on_addContactButton_clicked();
    void on_ContactsList_doubleClicked();
    void on_okayButton_clicked();
    void removeContact();
    void editContact();
    void on_ContactsList_customContextMenuRequested(const QPoint &pos);
    void onOkayButtonClickedEdit();

private:
    Ui::ContactsWindow *ui;
    MainWindow *listener;
    QStringList names;
    QStringList numbers;
    void getContacts();
    void showContacts();
};


#endif // CONTACTSWINDOW_H
