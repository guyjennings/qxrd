#ifndef QXRDTESTIMAGEGENERATORWINDOW_H
#define QXRDTESTIMAGEGENERATORWINDOW_H

#include <QMainWindow>
#include "qxrdtestimagegeneratorthread-ptr.h"
#include "qxrdtestimagegenerator-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "ui_qxrdtestimagegeneratorwindow.h"

class QxrdTestImageGeneratorWindow : public QMainWindow, public Ui::QxrdTestImageGeneratorWindow
{
  Q_OBJECT

public:
  explicit QxrdTestImageGeneratorWindow(QxrdExperimentWPtr doc, QWidget *parent = 0);
  ~QxrdTestImageGeneratorWindow();

public slots:
  void doNewQWTWindow();
  void doNewDataVisWindow();

private:
  QxrdTestImageGeneratorThreadPtr m_Thread;
  QxrdTestImageGeneratorWPtr      m_Generator;
};

#endif // QXRDTESTIMAGEGENERATORWINDOW_H
