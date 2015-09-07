#include <iostream>
#include <QtWidgets/QApplication>

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

int main( int argc, char* argv[] )
{
    QApplication app( argc, argv );

    QLabel* label = new QLabel( "Hello World" );
    label->show();

    QApplication::exec();

    return 0;
}
