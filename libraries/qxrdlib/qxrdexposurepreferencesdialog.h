#ifndef QXRDEXPOSUREPREFERENCESDIALOG_H
#define QXRDEXPOSUREPREFERENCESDIALOG_H

#include "qxrdlib_global.h"
#include <QDialog>
#include "ui_qxrdexposurepreferencesdialog.h"
#include "qxrdacqcommon-ptr.h"
#include <QVector>
#include <QCheckBox>
#include <QSpinBox>

class QXRD_EXPORT QxrdExposurePreferencesDialog : public QDialog, public Ui::QxrdExposurePreferencesDialog
{
  Q_OBJECT

private:
  typedef QDialog inherited;

public:
  explicit QxrdExposurePreferencesDialog(QxrdAcqCommonWPtr acq, QWidget *parent = 0);
  ~QxrdExposurePreferencesDialog();

public slots:
  void accept();

private:
  QxrdAcqCommonWPtr m_Acquisition;
  QVector<QCheckBox*> m_CheckBoxes;
  QVector<QSpinBox*> m_SpinBoxes;
};

#endif // QXRDEXPOSUREPREFERENCESDIALOG_H
