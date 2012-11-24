#ifndef QXRDCENTERFINDERDIALOG_H
#define QXRDCENTERFINDERDIALOG_H

#include <QDockWidget>
#include "ui_qxrdcenterfinderdialog.h"
#include "qxrdcenterfinder-ptr.h"

class QxrdCenterFinderDialog : public QDockWidget, public Ui::QxrdCenterFinderDialog
{
  Q_OBJECT

public:
  QxrdCenterFinderDialog(QxrdCenterFinderPtr cen, QWidget *parent=0);
  virtual ~QxrdCenterFinderDialog();

public slots:
  void centerMoveUpLeft();
  void centerMoveUp();
  void centerMoveUpRight();
  void centerMoveRight();
  void centerMoveDownRight();
  void centerMoveDown();
  void centerMoveDownLeft();
  void centerMoveLeft();

  void onCenterChanged(double cx, double cy);
  void onImplementTiltChanged(bool imp);
  void onEnablePolarizationChanged(bool pol);
  void onEnableAbsorptionChanged(bool absn);

private:
  void moveCenter(int dx, int dy);

private:
  QxrdCenterFinderPtr m_CenterFinder;
};

#endif // QXRDCENTERFINDERDIALOG_H
