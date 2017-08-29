#ifndef QXRDSPLASHSCREEN_H
#define QXRDSPLASHSCREEN_H

#include "qxrdlib_global.h"
#include <QSplashScreen>

class QXRD_EXPORT QxrdSplashScreen : public QSplashScreen
{
  Q_OBJECT
public:
  explicit QxrdSplashScreen(QWidget *parent);
  virtual ~QxrdSplashScreen();

signals:

public slots:

};

#endif // QXRDSPLASHSCREEN_H
