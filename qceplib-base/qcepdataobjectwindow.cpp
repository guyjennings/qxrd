#include "qcepdataobjectwindow.h"
#include <stdio.h>

QcepDataObjectWindow::QcepDataObjectWindow(QWidget *parent) : QcepMainWindow(parent)
{

}

void QcepDataObjectWindow::printMessage(QString msg, QDateTime ts)
{
  printf("%s\n", qPrintable(msg));
}
