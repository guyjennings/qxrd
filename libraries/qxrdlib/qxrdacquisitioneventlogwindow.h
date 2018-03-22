#ifndef QXRDACQUISITIONEVENTLOGWINDOW_H
#define QXRDACQUISITIONEVENTLOGWINDOW_H

#include "qxrdlib_global.h"
#include <QMainWindow>
#include "ui_qxrdacquisitioneventlogwindow.h"
#include "qxrdacqcommon-ptr.h"
#include "qxrdacquisitioneventlogmodel-ptr.h"

class QXRD_EXPORT QxrdAcquisitionEventLogWindow :
    public QMainWindow,
    public Ui::QxrdAcquisitionEventLogWindow
{
  Q_OBJECT

public:
  explicit QxrdAcquisitionEventLogWindow(QxrdAcqCommonWPtr a);
  ~QxrdAcquisitionEventLogWindow();

  void doPauseResume();
  void doClearEventLog();
  void updateButtonTitles();

private:
  QxrdAcqCommonWPtr               m_Acquisition;
  QxrdAcquisitionEventLogModelPtr m_AcquisitionEventLogModel;
};

#endif // QXRDACQUISITIONEVENTLOGWINDOW_H
