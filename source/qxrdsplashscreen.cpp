#include "qxrdsplashscreen.h"
#include <stdio.h>

QxrdSplashScreen::QxrdSplashScreen(QWidget *parent) :
  QSplashScreen(parent, QPixmap(":images/qxrd-splash-screen.png"))
{
}

QxrdSplashScreen::~QxrdSplashScreen()
{
#ifndef QT_NO_DEBUG
  printf("Deleting splash screen\n");
#endif
}
