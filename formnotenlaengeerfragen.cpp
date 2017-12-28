#include "formnotenlaengeerfragen.h"
#include "ui_framenotenlaengeerfragen.h"

#include <QRadioButton>
#include <QDebug>

#include "mainwindow.h"

QMap<QString,FrameNotenlaengeErfragen::Bruch>
            FrameNotenlaengeErfragen::notenlaengen{
    { "Ganze Note", Bruch{4,4} },
    { "Halbe Note", Bruch{2,4} },
    { "Viertel Note", Bruch{1,4} },
    { "Achtel Note", Bruch{1,8} },
    { "Sechszehntel Note", Bruch{1,16} }
};

FrameNotenlaengeErfragen::FrameNotenlaengeErfragen(MainWindow *mainWindow ,QWidget *parent)
: mainWindow{ mainWindow }
, QFrame{ parent }
, ui{ new Ui::FrameNotenlaengeErfragen }
{
    ui->setupUi(this);

    QObject::connect( this->ui->pushButton,
                      &QPushButton::clicked,
                        [=](){
        QRadioButton *checkedRadioButton = nullptr;

        for( auto& objekt : this->ui->groupBox->children() )
        {
            QRadioButton *radioButton = dynamic_cast<QRadioButton*>(objekt);

            if( radioButton != nullptr &&
                radioButton->isChecked() )
            {
                checkedRadioButton = radioButton;
                break;
            }
        }

        if( checkedRadioButton != nullptr )
        {
            QString radioText = checkedRadioButton->text();

            Bruch notenlaenge =
                FrameNotenlaengeErfragen::notenlaengen[
                        checkedRadioButton->text()];

            if( this->ui->checkBox->isChecked() )
            {
                Bruch newBruch{notenlaenge.zaehler,
                              notenlaenge.nenner * 2 };

                notenlaenge = notenlaenge + newBruch;
                notenlaenge.kuerzen();
            }

            this->appendText(
                        QString::number(notenlaenge.zaehler) +
                        "/" +
                        QString::number(notenlaenge.nenner)
                        );

            this->mainWindow->notenleangeDialogOkResult = true;
        }
        else
        {
            this->mainWindow->notenleangeDialogOkResult = false;
        }
    }
                      );
}

FrameNotenlaengeErfragen::~FrameNotenlaengeErfragen()
{
    QObject::disconnect( this->ui->pushButton,0,0,0 );

    this->mainWindow->leaveDialog();
    delete ui;
}

void FrameNotenlaengeErfragen::setAsParentForm( MainWindow *mainWindow )
{
    this->mainWindow = mainWindow;
}

void FrameNotenlaengeErfragen::appendText( const QString &text )
{
    if( this->mainWindow != nullptr )
    {
        mainWindow->appendText( ":" + text );
    }
}
