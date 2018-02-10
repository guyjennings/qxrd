#ifndef QNEWAPPLICATION_H
#define QNEWAPPLICATION_H

#include "qceplib_global.h"
#include <QApplication>

class QCEP_EXPORT QNewApplication : public QApplication
{
  Q_OBJECT

private:
  typedef QApplication inherited;

public:
  QNewApplication(int &argc, char **argv);

  bool event(QEvent *e);
};

#endif // QNEWAPPLICATION_H
