#ifndef QXRDTESTSCANGENERATORWINDOW_H
#define QXRDTESTSCANGENERATORWINDOW_H

#include <QMainWindow>
#include "qxrdtestscangeneratorthread-ptr.h"
#include "qxrdtestscangenerator-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "ui_qxrdtestimagegeneratorwindow.h"

class QxrdTestScanGeneratorWindow : public QMainWindow, public Ui::QxrdTestImageGeneratorWindow
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
