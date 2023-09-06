// qt_library.h
#ifndef QT_LIBRARY_H
#define QT_LIBRARY_H

#include <QObject>

class Qt_library : public QObject
{
    Q_OBJECT

public:
    Qt_library();

    // Définition de la méthode myMethod
    void myMethod();

private:
         // Autres membres de la classe
};

#endif // QT_LIBRARY_H
