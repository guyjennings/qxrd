#ifndef QXRDCENTERFINDER_H
#define QXRDCENTERFINDER_H

#include <QObject>

class QxrdImagePlot;
class QxrdCenterFinderPlot;
class QxrdCenterFinderDialog;

class QxrdCenterFinder : public QObject
{
  Q_OBJECT;

public:
  QxrdCenterFinder(QxrdImagePlot *imgplot, QxrdCenterFinderPlot *cntplot, QxrdCenterFinderDialog *cfdialog, QObject *parent=0);

public slots:
  void moveCenter(int dx, int dy);
  void centerMoveUpLeft();
  void centerMoveUp();
  void centerMoveUpRight();
  void centerMoveRight();
  void centerMoveDownRight();
  void centerMoveDown();
  void centerMoveDownLeft();
  void centerMoveLeft();
  void onCenterXChanged(double cx);
  void onCenterYChanged(double cy);

signals:
  void centerChanged(double cx, double cy);

private:
  QxrdImagePlot             *m_Imageplot;
  QxrdCenterFinderPlot      *m_CenterFinderPlot;
  QxrdCenterFinderDialog    *m_CenterFinderDialog;

  double                     m_CenterX;
  double                     m_CenterY;
  double                     m_StepSize;
};

#endif // QXRDCENTERFINDER_H
