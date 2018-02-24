#ifndef QXRDANALYSISWINDOW_H
#define QXRDANALYSISWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdanalysiswindow.h"

//TODO: implement
class QXRD_EXPORT QxrdAnalysisWindow : public QxrdMainWindow, public Ui::QxrdAnalysisWindow
{
  Q_OBJECT

private:
  typedef QxrdMainWindow inherited;

public:
  explicit QxrdAnalysisWindow(QString name,
                              QxrdAppCommonWPtr app,
                              QxrdExperimentWPtr expt,
                              QxrdAcqCommonWPtr acqw,
                              QxrdProcessorWPtr procw);
  void initialize(QcepObjectWPtr parent);
  ~QxrdAnalysisWindow();

protected:
  void changeEvent(QEvent *e);

private:
};

#endif // QXRDANALYSISWINDOW_H
