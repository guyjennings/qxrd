/******************************************************************
*
*  $Id: qxrd.cpp,v 1.2 2010/09/13 20:00:38 jennings Exp $
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

  int res = app.exec();

  QCEP_DEBUG(DEBUG_EXITWAIT,
             while(1) {}
  );

  return res;
}

/******************************************************************
*
*  $Log: qxrd.cpp,v $
*  Revision 1.2  2010/09/13 20:00:38  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:36  jennings
*  Moving files into source subdirectory
*
*  Revision 1.5.4.5  2010/05/20 20:14:44  jennings
*  Added EXITWAIT debug option to stop app from quitting after QxrdApplication finishes
*
*  Revision 1.5.4.4  2010/05/19 20:30:20  jennings
*  *** empty log message ***
*
*  Revision 1.5.4.3  2010/04/26 23:46:12  jennings
*  *** empty log message ***
*
*  Revision 1.5.4.2  2010/04/26 20:53:25  jennings
*  More attempts to get QSharedPointers to work...
*
*  Revision 1.5.4.1  2010/04/26 00:37:09  jennings
*  Attempting to convert to using QSharedPointers
*
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

