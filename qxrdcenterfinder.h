/******************************************************************
*
*  $Id: qxrdcenterfinder.h,v 1.5 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#ifndef QXRDCENTERFINDER_H
#define QXRDCENTERFINDER_H

#include "qcepmacros.h"

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
  HEADER_IDENT("$Id: qxrdcenterfinder.h,v 1.5 2009/06/27 22:50:32 jennings Exp $");
};

#endif // QXRDCENTERFINDER_H

/******************************************************************
*
*  $Log: qxrdcenterfinder.h,v $
*  Revision 1.5  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

