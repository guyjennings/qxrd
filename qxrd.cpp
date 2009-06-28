/******************************************************************
*
*  $Id: qxrd.cpp,v 1.3 2009/06/28 04:04:52 jennings Exp $
*
*******************************************************************/

#include <QApplication>

#include "qxrdapplication.h"

int main(int argc, char *argv[])
{
  printf("App starts\n");

  QxrdApplication app(argc, argv);

  printf("App Constructed\n");

  return app.exec();
}

/******************************************************************
*
*  $Log: qxrd.cpp,v $
*  Revision 1.3  2009/06/28 04:04:52  jennings
*  Partial implementation of separate thread for script engine
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

