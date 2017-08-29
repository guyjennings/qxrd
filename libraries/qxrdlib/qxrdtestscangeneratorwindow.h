#ifndef QXRDTESTSCANGENERATORWINDOW_H
#define QXRDTESTSCANGENERATORWINDOW_H

#include "qxrdlib_global.h"
#include <QMainWindow>
#include "qxrdtestscangeneratorthread-ptr.h"
#include "qxrdtestscangenerator-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "ui_qxrdtestimagegeneratorwindow.h"

class QXRD_EXPORT QxrdTestScanGeneratorWindow : public QMainWindow, public Ui::QxrdTestImageGeneratorWindow
{
  Q_OBJECT

public:
  explicit QxrdTestScanGeneratorWindow(QxrdExperimentWPtr doc, QWidget *parent = 0);
  ~QxrdTestScanGeneratorWindow();

public slots:
  void doNewQWTWindow();
  void doNewDataVisWindow();

private:
  QxrdTestScanGeneratorThreadPtr m_Thread;
  QxrdTestScanGeneratorWPtr      m_Generator;
};

#endif // QXRDTESTSCANGENERATORWINDOW_H
