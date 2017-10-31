#ifndef QXRDANALYSISWINDOW_H
#define QXRDANALYSISWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdanalysiswindow.h"

class QXRD_EXPORT QxrdAnalysisWindow : public QxrdMainWindow, public Ui::QxrdAnalysisWindow
{
  Q_OBJECT

public:
  explicit QxrdAnalysisWindow(QWidget *parent = 0);
  ~QxrdAnalysisWindow();

protected:
  void changeEvent(QEvent *e);

private:
};

#endif // QXRDANALYSISWINDOW_H
