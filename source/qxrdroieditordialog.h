#ifndef QXRDROIEDITORDIALOG_H
#define QXRDROIEDITORDIALOG_H

#include <QDialog>
#include "qxrdroicoordinates-ptr.h"
#include "ui_qxrdroieditordialog.h"

class QxrdROIEditorDialog : public QDialog, public Ui::QxrdROIEditorDialog
{
  Q_OBJECT

public:
  explicit QxrdROIEditorDialog(QxrdROICoordinatesWPtr roi, QWidget *parent = 0);
  ~QxrdROIEditorDialog();

public slots:
  void accept();

public:
  QxrdROICoordinatesWPtr roi();

private slots:
  void newInnerType(int n);
  void newOuterType(int n);

private:
  QxrdROICoordinatesWPtr m_ROI;
};

#endif // QXRDROIEDITORDIALOG_H
