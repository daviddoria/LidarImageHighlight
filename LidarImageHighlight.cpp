#include <QApplication>
#include <QCleanlooksStyle>
#include "SimpleView.h"

int main( int argc, char** argv )
{
  QApplication app( argc, argv );

  QApplication::setStyle(new QCleanlooksStyle);

  SimpleView mySimpleView(argc,argv);
  mySimpleView.show();

  return app.exec();
}