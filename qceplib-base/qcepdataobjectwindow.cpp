#include "qcepdataobjectwindow.h"
#include <stdio.h>

QcepDataObjectWindow::QcepDataObjectWindow(QString name)
  : inherited(name)
{

}

void QcepDataObjectWindow::printLine(QString line)
{
  printf("%s\n", qPrintable(line));
}

void QcepDataObjectWindow::printMessage(QString msg, QDateTime /*ts*/)
{
  printf("MESSAGE: %s\n", qPrintable(msg));
}

void QcepDataObjectWindow::criticalMessage(QString msg, QDateTime /*ts*/)
{
  printf("CRIT: %s\n", qPrintable(msg));
}

void QcepDataObjectWindow::statusMessage(QString msg, QDateTime /*ts*/)
{
  printf("STATUS: %s\n", qPrintable(msg));
}
