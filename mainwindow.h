#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QComboBox>
#include "utils.h"
#include "preferences.h"

namespace Ui {
    class Dialog;
}

class MainWindow : public QDialog{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();
        void contactsWindowHandle(QString contact);
        QComboBox* getDestinationSelect();

    private slots:
        void on_sendButton_clicked();

        void on_refreshButton_clicked();

        void on_destinationButton_clicked();

        void on_message_textChanged();

        void on_destinationSelect_currentTextChanged(const QString &arg1);

    private:
        Ui::Dialog *ui;
};

int getDevices(const Ui::Dialog* ui);

void readRecents(const Ui::Dialog* ui);

#endif // DIALOG_H
