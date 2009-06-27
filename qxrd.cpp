/******************************************************************
*
*  $Id: qxrd.cpp,v 1.2 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#include <QApplication>

#include "qxrdapplication.h"

int main(int argc, char *argv[])
{
  QxrdApplication app(argc, argv);

  return app.exec();
}

/******************************************************************
*
*  $Log: qxrd.cpp,v $
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

