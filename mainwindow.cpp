#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "formnotenlaengeerfragen.h"

#include <QString>
#include <QPushButton>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow( QWidget *parent )
: QMainWindow{ parent }
, dialog{ nullptr }
, notenleangeDialogOkResult{ false }
, ui{ new Ui::MainWindow }
{
    this->ui->setupUi(this);

    this->initialisiereTastenNotennamen();
    this->initialisiereTasten();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initialisiereTasten()
{
    const int anzahlWeisseTasten = 57;
    const int anzahlSchwarzeTasten = 40;

    const int yPos = 291;

    int xPos = 20;
    int notenNameIndex = 0;

    bool zweierSprung = false;
    bool reset = false;

    int sprungCounter = 0;

    QDialog::connect( this->ui->pushButton_save,
                 &QPushButton::clicked,
            [=](){
                this->speichern();
            }
                    );

    // erst weisse Tasten
    for( int i=1; i<=anzahlWeisseTasten; i++ )
    {
        if( reset )
        {
            notenNameIndex -= 1;
            reset = false;
            sprungCounter = 0;
        }

        QPushButton *button = new QPushButton(
                    this->notenName.at( notenNameIndex ),
                   // QString::number(i).toStdString().c_str(),
                    this );

        QObject::connect( button, &QPushButton::clicked,
                  [=](){
                        this->appendText( " " + this->tastenNummer(button->text()) );
                        this->frageNachNotenLaenge();
                   }
                );

        button->setProperty(
              QString::number(i).toStdString().c_str(), i );

        button->setStyleSheet( "padding: 0px; margin: 0px;" );

        button->setFixedSize( 18, 130 );
        button->setGeometry( xPos, yPos, 18, 130 );

        xPos += 18;
        notenNameIndex += 2;

        if( !zweierSprung )
        {
            if( sprungCounter == 2)
            {
                zweierSprung = true;
                reset = true;
            }
        }
        else
        {
            if( sprungCounter == 3 )
            {
                zweierSprung = false;
                reset = true;
            }
        }

        ++sprungCounter;
        this->buttons.append( button );
    }

    xPos = 27;

    int blackStep = 0;

    QList<int> blackSpruenge{ 3,2,3,2,2 };
    sprungCounter = 0;
    notenNameIndex = -2;

    // dann schwarze Tasten zum Überlagern
    for( int i=1; i<=anzahlSchwarzeTasten; i++ )
    {
        sprungCounter = sprungCounter % blackSpruenge.size();
        notenNameIndex += blackSpruenge.at(sprungCounter);

        ++blackStep;

        QPushButton *button = new QPushButton(
                    this->notenName.at(notenNameIndex),
                    this );

        QObject::connect( button, &QPushButton::clicked,
                  [=](){
                        this->appendText( " " + this->tastenNummer( button->text() ) );
                        this->frageNachNotenLaenge();
                   }
                );

        button->setProperty(
              QString::number(i+anzahlWeisseTasten).toStdString().c_str(),
                    i+anzahlWeisseTasten );


        button->setStyleSheet( "padding: 0px; "
                               "margin: 0px;"
                               "background-color: black;");

        button->setFixedSize( 16, 80 );
        button->setGeometry( xPos+2, yPos, 16, 80 );

        if( blackStep == 2  )
        {
            xPos += 18;
        }
        else if( blackStep == 5 )
        {
            xPos += 18;
            blackStep = 0;
        }

        xPos += 18;

        ++sprungCounter;
        this->buttons.append( button );
    }
}

void MainWindow::leaveDialog()
{
    if( !this->notenleangeDialogOkResult )
    {
        this->removeLastUncompletedNote();
    }

    this->dialog = nullptr;
}

void MainWindow::frageNachNotenLaenge()
{
    this->notenleangeDialogOkResult = false;

    if( this->dialog != nullptr )
    {
        this->dialog->setAttribute(
              Qt::WA_DeleteOnClose, false );

        this->dialog->close();
        delete this->dialog;

        this->dialog = nullptr;
    }

    this->dialog = new FrameNotenlaengeErfragen( this );
    this->dialog->setAttribute( Qt::WA_DeleteOnClose, true );

    this->dialog->show();
}

QString MainWindow::tastenNummer(const QString &notenName) const
{
    static QMap<QString,QString> tastenNummern;

    tastenNummern.insert( "C0", "-8" );
    tastenNummern.insert( "Db0", "-7");
    tastenNummern.insert( "D0", "-6" );
    tastenNummern.insert( "Eb0", "-5");
    tastenNummern.insert( "E0", "-4" );
    tastenNummern.insert( "F0", "-3" );
    tastenNummern.insert( "Gb0", "-2");
    tastenNummern.insert( "G0", "-1" );
    tastenNummern.insert( "Ab0", "0" );
    tastenNummern.insert( "A0", "1" );
    tastenNummern.insert( "Bb0", "2" );
    tastenNummern.insert( "B0", "3" );
    tastenNummern.insert( "C1", "4" );
    tastenNummern.insert( "Db1", "5" );
    tastenNummern.insert( "D1", "6" );
    tastenNummern.insert( "Eb1", "7" );
    tastenNummern.insert( "E1", "8" );
    tastenNummern.insert( "F1", "9" );
    tastenNummern.insert( "Gb1", "10");
    tastenNummern.insert( "G1", "11" );
    tastenNummern.insert( "Ab1", "12");
    tastenNummern.insert( "A1", "13" );
    tastenNummern.insert( "Bb1", "14");
    tastenNummern.insert( "B1", "15" );
    tastenNummern.insert( "C2",  "16");
    tastenNummern.insert( "Db2", "17" );
    tastenNummern.insert( "D2",  "18");
    tastenNummern.insert( "Eb2", "19");
    tastenNummern.insert( "E2",  "20");
    tastenNummern.insert( "F2",  "21");
    tastenNummern.insert( "Gb2", "22");
    tastenNummern.insert( "G2",  "23");
    tastenNummern.insert( "Ab2", "24");
    tastenNummern.insert( "A2",  "25");
    tastenNummern.insert( "Bb2", "26");
    tastenNummern.insert( "B2",  "27");
    tastenNummern.insert( "C3",  "28");
    tastenNummern.insert( "Db3", "29" );
    tastenNummern.insert( "D3",  "30");
    tastenNummern.insert( "Eb3", "31");
    tastenNummern.insert( "E3",  "32");
    tastenNummern.insert( "F3",  "33");
    tastenNummern.insert( "Gb3", "34");
    tastenNummern.insert( "G3",  "35");
    tastenNummern.insert( "Ab3", "36");
    tastenNummern.insert( "A3",  "37");
    tastenNummern.insert( "Bb3", "38");
    tastenNummern.insert( "B3",  "39");
    tastenNummern.insert( "C4", "40" );
    tastenNummern.insert( "Db4", "41" );
    tastenNummern.insert( "D4", "42" );
    tastenNummern.insert( "Eb4","43" );
    tastenNummern.insert( "E4", "44" );
    tastenNummern.insert( "F4", "45" );
    tastenNummern.insert( "Gb4","46" );
    tastenNummern.insert( "G4", "47" );
    tastenNummern.insert( "Ab4","48" );
    tastenNummern.insert( "A4", "49" );
    tastenNummern.insert( "Bb4","50" );
    tastenNummern.insert( "B4", "51" );
    tastenNummern.insert( "C5", "52" );
    tastenNummern.insert( "Db5", "53" );
    tastenNummern.insert( "D5", "54" );
    tastenNummern.insert( "Eb5","55" );
    tastenNummern.insert( "E5", "56" );
    tastenNummern.insert( "F5", "57" );
    tastenNummern.insert( "Gb5","58" );
    tastenNummern.insert( "G5", "59" );
    tastenNummern.insert( "Ab5","60" );
    tastenNummern.insert( "A5", "61" );
    tastenNummern.insert( "Bb5","62" );
    tastenNummern.insert( "B5", "63" );
    tastenNummern.insert( "C6", "64" );
    tastenNummern.insert( "Db6", "65" );
    tastenNummern.insert( "D6", "66" );
    tastenNummern.insert( "Eb6","67" );
    tastenNummern.insert( "E6", "68" );
    tastenNummern.insert( "F6", "69" );
    tastenNummern.insert( "Gb6","70" );
    tastenNummern.insert( "G6", "71" );
    tastenNummern.insert( "Ab6","72" );
    tastenNummern.insert( "A6", "73" );
    tastenNummern.insert( "Bb6","74" );
    tastenNummern.insert( "B6", "75" );
    tastenNummern.insert( "C7", "76" );
    tastenNummern.insert( "Db7", "77" );
    tastenNummern.insert( "D7", "78" );
    tastenNummern.insert( "Eb7","79" );
    tastenNummern.insert( "E7", "80" );
    tastenNummern.insert( "F7", "81" );
    tastenNummern.insert( "Gb7","82" );
    tastenNummern.insert( "G7", "83" );
    tastenNummern.insert( "Ab7","84" );
    tastenNummern.insert( "A7", "85" );
    tastenNummern.insert( "Bb7","86" );
    tastenNummern.insert( "B7", "87" );
    tastenNummern.insert( "C8", "88" );

    return (tastenNummern.contains(notenName)) ? tastenNummern[notenName] : "ERROR";
}

bool MainWindow::speichern() const
{

    QString titel = this->ui->lineEdit_titel->text();
    QString bpm = this->ui->lineEdit_bpm->text();
    QString notes = this->ui->textEdit->toPlainText().trimmed();
    int iBPM = bpm.toInt();

    if( titel.isEmpty() ||
        bpm.isEmpty() ||
        notes.isEmpty() ||
        iBPM < 1 )
    {
        QMessageBox::information(

            const_cast<MainWindow*>(this),
            tr("Missing Data"),
            tr("One of these attributes is missing/corrupt: TITLE, BPM, NOTES")
                   );

        return false;
    }


    QString fileName = QFileDialog::getSaveFileName(
                const_cast<MainWindow*>(this),
         tr("Save as.."), titel, tr(".txt") );

    if( fileName.isEmpty() )
    {
        return false;
    }
    else
    {
        QFile file( fileName );

        if( !file.open( QIODevice::WriteOnly) )
        {
            QMessageBox::information(

                const_cast<MainWindow*>(this),
                tr("Unable to open file"),
                file.errorString()

                    );

            return false;
        }

        QTextStream out( &file );

        out << "Titel: " << titel << endl;
        out << "Bpm: " << bpm<< endl;
        out << notes;

        file.close();
        return true;
    }
}

void MainWindow::removeLastUncompletedNote()
{  
    // Letze unvollständige Note (ohne Notenlänge) entfernen

    QString text = this->ui->textEdit->toPlainText();

    int lastWhiteSpace = text.lastIndexOf( ' ' );

    // 54:1/4 35 -> 54:1/4

    QString newText( text.mid(0,lastWhiteSpace) );

    this->ui->textEdit->clear();
    this->ui->textEdit->append( newText );
}

void MainWindow::appendText( const QString &text )
{
    QString newText = this->ui->textEdit->toPlainText();
    newText += text;
    this->ui->textEdit->clear();
    this->ui->textEdit->append( newText );
}

void MainWindow::initialisiereTastenNotennamen()
{
this->notenName.append("C0" );
this->notenName.append("Db0" );
this->notenName.append("D0" );
this->notenName.append("Eb0" );
this->notenName.append("E0" );
this->notenName.append("F0" );
this->notenName.append("Gb0" );
this->notenName.append("G0" );
this->notenName.append("Ab0" );
this->notenName.append("A0" );
this->notenName.append("Bb0" );
this->notenName.append("B0" );
this->notenName.append("C1" );
this->notenName.append("Db1" );
this->notenName.append("D1" );
this->notenName.append("Eb1" );
this->notenName.append("E1" );
this->notenName.append("F1" );
this->notenName.append("Gb1" );
this->notenName.append("G1" );
this->notenName.append("Ab1" );
this->notenName.append("A1" );
this->notenName.append("Bb1" );
this->notenName.append("B1" );
this->notenName.append("C2" );
this->notenName.append("Db2" );
this->notenName.append("D2" );
this->notenName.append("Eb2" );
this->notenName.append("E2" );
this->notenName.append("F2" );
this->notenName.append("Gb2" );
this->notenName.append("G2" );
this->notenName.append("Ab2" );
this->notenName.append("A2" );
this->notenName.append("Bb2" );
this->notenName.append("B2" );
this->notenName.append("C3" );
this->notenName.append("Db3" );
this->notenName.append("D3" );
this->notenName.append("Eb3" );
this->notenName.append("E3" );
this->notenName.append("F3" );
this->notenName.append("Gb3" );
this->notenName.append("G3" );
this->notenName.append("Ab3" );
this->notenName.append("A3" );
this->notenName.append("Bb3" );
this->notenName.append("B3" );
this->notenName.append("C4" );
this->notenName.append("Db4" );
this->notenName.append("D4" );
this->notenName.append("Eb4" );
this->notenName.append("E4" );
this->notenName.append("F4" );
this->notenName.append("Gb4" );
this->notenName.append("G4" );
this->notenName.append("Ab4" );
this->notenName.append("A4" );
this->notenName.append("Bb4" );
this->notenName.append("B4" );
this->notenName.append("C5" );
this->notenName.append("Db5" );
this->notenName.append("D5" );
this->notenName.append("Eb5" );
this->notenName.append("E5" );
this->notenName.append("F5" );
this->notenName.append("Gb5" );
this->notenName.append("G5" );
this->notenName.append("Ab5" );
this->notenName.append("A5" );
this->notenName.append("Bb5" );
this->notenName.append("B5" );
this->notenName.append("C6" );
this->notenName.append("Db6" );
this->notenName.append("D6" );
this->notenName.append("Eb6" );
this->notenName.append("E6" );
this->notenName.append("F6" );
this->notenName.append("Gb6" );
this->notenName.append("G6" );
this->notenName.append("Ab6" );
this->notenName.append("A6" );
this->notenName.append("Bb6" );
this->notenName.append("B6" );
this->notenName.append("C7" );
this->notenName.append("Db7" );
this->notenName.append("D7" );
this->notenName.append("Eb7" );
this->notenName.append("E7" );
this->notenName.append("F7" );
this->notenName.append("Gb7" );
this->notenName.append("G7" );
this->notenName.append("Ab7" );
this->notenName.append("A7" );
this->notenName.append("Bb7" );
this->notenName.append("B7" );
this->notenName.append("C8" );
}
