#ifndef QXRDROIEDITORDIALOG_H
#define QXRDROIEDITORDIALOG_H

#include "qxrdlib_global.h"
#include <QDialog>
#include "qxrdroi-ptr.h"
#include "ui_qxrdroieditordialog.h"

class QXRD_EXPORT QxrdROIEditorDialog : public QDialog, public Ui::QxrdROIEditorDialog
{
  Q_OBJECT

public:
  explicit QxrdROIEditorDialog(QxrdROIWPtr roi, QWidget *parent = 0);
  ~QxrdROIEditorDialog();

public slots:
  void accept();

public:
  QxrdROIWPtr roi();

private slots:
  void newInnerType(int n);
  void newOuterType(int n);

  void innerAddPoint();
  void innerDelPoint();
  void outerAddPoint();
  void outerDelPoint();

private:
  QxrdROIWPtr m_ROI;
};

#endif // QXRDROIEDITORDIALOG_H
