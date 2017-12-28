#ifndef FRAMENOTENLAENGEERFRAGEN_H
#define FRAMENOTENLAENGEERFRAGEN_H

#include <QFrame>
#include <QString>
#include <QMap>


namespace Ui {
    class FrameNotenlaengeErfragen;
}


// Vorwärtsdeklaration
class MainWindow;

class FrameNotenlaengeErfragen : public QFrame
{
    Q_OBJECT

public:

    struct Bruch
    {
        Bruch()
        : Bruch{ 0,0 }
        {
        }

        Bruch( const int &zaehler, int const &nenner )
        : zaehler{ zaehler }
        , nenner{ nenner }
        {
        }

        Bruch operator+( const Bruch &bruch )
        {
            Bruch ergebnis;

            // Gemeinsamen Nenner bilden (einfache Version)
            ergebnis.nenner = this->nenner * bruch.nenner;

            // Zähler der einzelnen Brüche erweitern
            // und anschließend addieren
            ergebnis.zaehler = (this->zaehler * bruch.nenner)
                                   + (bruch.zaehler * this->nenner);

            return ergebnis;
        }

        void kuerzen()
        {
            // PQ-Formel um den ggT zu finden
            int q = this->zaehler;
            int p = this->nenner;
            int r = 0;

            while( q != 0 )
            {
                r = p%q;
                p = q;
                q = r;
            }
            // p = ggT

            this->zaehler /= p;
            this->nenner /= p;
        }

        int zaehler;
        int nenner;
    };

    explicit FrameNotenlaengeErfragen(
            MainWindow *maionWindow = nullptr,
            QWidget *parent = nullptr
            );
    ~FrameNotenlaengeErfragen();

    void appendText( const QString &text );
    void setAsParentForm( MainWindow *mainWindow );

private:
    static QMap<QString, Bruch> notenlaengen;

    MainWindow *mainWindow;
    Ui::FrameNotenlaengeErfragen *ui;
};

#endif // FRAMENOTENLAENGEERFRAGEN_H
