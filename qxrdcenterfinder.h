/******************************************************************
*
*  $Id: qxrdcenterfinder.h,v 1.7 2009/07/21 22:55:48 jennings Exp $
*
*******************************************************************/

#ifndef QXRDCENTERFINDER_H
#define QXRDCENTERFINDER_H

#include "qcepmacros.h"

#include <QObject>
#include <QPen>
#include "qcepproperty.h"
#include "qxrdsettings.h"

class QxrdCenterFinder : public QObject
{
  Q_OBJECT;

public:
  QxrdCenterFinder(QObject *parent=0);

public:
  Q_PROPERTY(double centerX READ get_CenterX WRITE set_CenterX);
  QCEP_DOUBLE_PROPERTY(CenterX);

  Q_PROPERTY(double centerY READ get_CenterY WRITE set_CenterY);
  QCEP_DOUBLE_PROPERTY(CenterY);

  Q_PROPERTY(double centerStep READ get_CenterStep WRITE set_CenterStep);
  QCEP_DOUBLE_PROPERTY(CenterStep);

//public slots:
//  void onCenterXChanged(double cx);
//  void onCenterYChanged(double cy);
//  void onCenterChanged(double cx, double cy);
//  void onCenterChanged(QwtDoublePoint pt);
//  void onCenterStepChanged(double stp);

public:
//  void setEnabled(bool imgenabled, bool cntrenabled);
//  void setPen(const QPen &pen);

  void readSettings(QxrdSettings *settings, QString section);
  void writeSettings(QxrdSettings *settings, QString section);

//signals:
////  void centerChanged(double cx, double cy);
//
//private:
////
//
private:
  mutable QMutex             m_Mutex;

  HEADER_IDENT("$Id: qxrdcenterfinder.h,v 1.7 2009/07/21 22:55:48 jennings Exp $");
};

#endif // QXRDCENTERFINDER_H

/******************************************************************
*
*  $Log: qxrdcenterfinder.h,v $
*  Revision 1.7  2009/07/21 22:55:48  jennings
*  Rearranged center finder and integrator code so that the center finder and integrator objects go into the data processor thread, and the GUI stuff goes in the GUI thread
*
*  Revision 1.6  2009/07/08 19:06:27  jennings
*  Made centering parameters into Q_PROPERTYs
*  Saved centering, integrator and data processor settings
*
*  Revision 1.5  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

