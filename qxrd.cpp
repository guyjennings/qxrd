/******************************************************************
*
*  $Id: qxrd.cpp,v 1.5 2009/09/20 21:18:53 jennings Exp $
*
*******************************************************************/

#include <QApplication>

#include "qxrdapplication.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
//  printf("App starts\n");

  QxrdApplication app(argc, argv);

//  printf("App Constructed\n");

  return app.exec();
}

/******************************************************************
*
*  $Log: qxrd.cpp,v $
*  Revision 1.5  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
*
*  Revision 1.4  2009/07/08 00:35:33  jennings
*  *** empty log message ***
*
*  Revision 1.3  2009/06/28 04:04:52  jennings
*  Partial implementation of separate thread for script engine
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

