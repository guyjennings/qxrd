#ifndef QXRDCENTERFINDER_H
#define QXRDCENTERFINDER_H

#include <QObject>
#include <QPen>

class QxrdWindow;
class QxrdImagePlot;
class QxrdCenterFinderPlot;
class QxrdCenterFinderDialog;
class QxrdCenterFinderPicker;
class QwtPlotMarker;
#include <qwt_double_rect.h>

class QxrdCenterFinder : public QObject
{
  Q_OBJECT;

public:
  QxrdCenterFinder(QxrdWindow *win, QxrdImagePlot *imgplot, QxrdCenterFinderPlot *cntplot, QxrdCenterFinderDialog *cfdialog, QObject *parent=0);

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
  void onCenterChanged(double cx, double cy);
  void onCenterChanged(QwtDoublePoint pt);
  void onCenterStepChanged(double stp);

public:
  void setEnabled(bool imgenabled, bool cntrenabled);
  void setPen(const QPen &pen);

signals:
  void centerChanged(double cx, double cy);

private:
  QxrdWindow                *m_Window;
  QxrdImagePlot             *m_ImagePlot;
  QxrdCenterFinderPlot      *m_CenterFinderPlot;
  QxrdCenterFinderDialog    *m_CenterFinderDialog;

  double                     m_CenterX;
  double                     m_CenterY;
  double                     m_StepSize;

  QxrdCenterFinderPicker    *m_CenterFinderPicker;
  QwtPlotMarker             *m_CenterMarker;

  QPen                       m_Pen;
};

#endif // QXRDCENTERFINDER_H
