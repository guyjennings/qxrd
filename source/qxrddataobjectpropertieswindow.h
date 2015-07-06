#ifndef QXRDDATAOBJECTPROPERTIESWINDOW_H
#define QXRDDATAOBJECTPROPERTIESWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "ui_qxrddataobjectpropertieswindow.h"
#include "qxrdexperiment-ptr.h"
#include "qcepdataobject-ptr.h"

class QxrdDataObjectPropertiesWindow : public QMainWindow, public Ui::QxrdDataObjectPropertiesWindow
{
  Q_OBJECT
public:
  explicit QxrdDataObjectPropertiesWindow(
      QxrdExperimentWPtr expt,
      QcepDataObjectPtr  obj,
      QWidget *parent = 0);

signals:

public slots:

private:
  QxrdExperimentWPtr  m_Experiment;
  QcepDataObjectPtr   m_Object;
};

#endif // QXRDDATAOBJECTPROPERTIESWINDOW_H
