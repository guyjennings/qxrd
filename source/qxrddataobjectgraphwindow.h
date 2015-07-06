#ifndef QXRDDATAOBJECTGRAPHWINDOW_H
#define QXRDDATAOBJECTGRAPHWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "ui_qxrddataobjectgraphwindow.h"
#include "qxrdexperiment-ptr.h"
#include "qcepdataobject-ptr.h"

class QxrdDataObjectGraphWindow : public QMainWindow, public Ui::QxrdDataObjectGraphWindow
{
  Q_OBJECT
public:
  explicit QxrdDataObjectGraphWindow(
      QxrdExperimentWPtr expt,
      QcepDataObjectPtr  obj,
      QWidget *parent = 0);

signals:

public slots:

private:
  QxrdExperimentWPtr  m_Experiment;
  QcepDataObjectPtr   m_Object;
};

#endif // QXRDDATAOBJECTGRAPHWINDOW_H
