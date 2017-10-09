#ifndef QXRDTESTIMAGEGENERATORWINDOW_H
#define QXRDTESTIMAGEGENERATORWINDOW_H

#include "qxrdlib_global.h"
#include <QMainWindow>
#include "qxrdtestimagegeneratorthread-ptr.h"
#include "qxrdtestimagegenerator-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "ui_qxrdtestimagegeneratorwindow.h"

class QXRD_EXPORT QxrdTestImageGeneratorWindow : public QMainWindow, public Ui::QxrdTestImageGeneratorWindow
{
  Q_OBJECT

public:
  explicit QxrdTestImageGeneratorWindow(QxrdExperimentWPtr doc, QWidget *parent = 0);
  ~QxrdTestImageGeneratorWindow();

public slots:
  void doNewQWTWindow();

#ifdef HAVE_DATAVIS
  void doNewDataVisWindow();
#endif

private:
  QxrdTestImageGeneratorThreadPtr m_Thread;
  QxrdTestImageGeneratorWPtr      m_Generator;
};

#endif // QXRDTESTIMAGEGENERATORWINDOW_H
