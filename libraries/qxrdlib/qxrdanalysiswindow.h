#ifndef QXRDANALYSISWINDOW_H
#define QXRDANALYSISWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdanalysiswindow.h"

//TODO: implement
class QXRD_EXPORT QxrdAnalysisWindow : public QxrdMainWindow, public Ui::QxrdAnalysisWindow
{
  Q_OBJECT

public:
  explicit QxrdAnalysisWindow(QString name,
                              QxrdAppCommonWPtr app,
                              QxrdExperimentWPtr expt,
                              QxrdAcquisitionWPtr acqw,
                              QxrdProcessorWPtr procw);
  ~QxrdAnalysisWindow();

protected:
  void changeEvent(QEvent *e);

private:
};

#endif // QXRDANALYSISWINDOW_H
