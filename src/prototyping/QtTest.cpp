#include <iostream>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>

int main( int argc, char* argv[] )
{
    QApplication app( argc, argv );

    QLabel* label = new QLabel( "Hello World" );
    label->show();

    QApplication::exec();

    return 0;
}
