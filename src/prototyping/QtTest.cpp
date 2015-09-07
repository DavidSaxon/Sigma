#include <iostream>
#include <QtWidgets/QApplication>

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

int main( int argc, char* argv[] )
{
    QApplication app( argc, argv );

    QWidget* window = new QWidget();

    QVBoxLayout* layout = new QVBoxLayout();
    window->setLayout( layout );

    QLabel* label = new QLabel( "SIGMA" );
    layout->addWidget( label );

    QPushButton* button = new QPushButton( "START" );
    layout->addWidget( button );

    window->show();

    QApplication::exec();

    return 0;
}
