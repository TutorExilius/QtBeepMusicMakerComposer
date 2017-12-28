#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QTextEdit>
#include <QPushButton>

#include "formnotenlaengeerfragen.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget *parent = nullptr );
    ~MainWindow();

    void appendText( const QString &text );
    void initialisiereTastenNotennamen();
    void initialisiereTasten();
    void frageNachNotenLaenge();
    bool speichern() const;

    void leaveDialog();

    bool notenleangeDialogOkResult;

public slots:
    void removeLastUncompletedNote();

private:
    FrameNotenlaengeErfragen *dialog;
    Ui::MainWindow *ui;

    QList<QPushButton*> buttons;
    QList<QString> notenName;
};

#endif // MAINWINDOW_H
