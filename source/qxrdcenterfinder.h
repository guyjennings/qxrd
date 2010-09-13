/******************************************************************
*
*  $Id: qxrdcenterfinder.h,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#ifndef QXRDCENTERFINDER_H
#define QXRDCENTERFINDER_H

#include "qcepmacros.h"

#include <QObject>
#include <QPen>
#include "qcepproperty.h"
#include "qxrdsettings.h"
#include <qwt_double_rect.h>
#include "qxrddetectorgeometry.h"

class QxrdCenterFinder : public QxrdDetectorGeometry
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

  Q_PROPERTY(double detectorXPixelSize READ get_DetectorXPixelSize WRITE set_DetectorXPixelSize);
  QCEP_DOUBLE_PROPERTY(DetectorXPixelSize);

  Q_PROPERTY(double detectorYPixelSize READ get_DetectorYPixelSize WRITE set_DetectorYPixelSize);
  QCEP_DOUBLE_PROPERTY(DetectorYPixelSize);

  Q_PROPERTY(double detectorDistance READ get_DetectorDistance WRITE set_DetectorDistance);
  QCEP_DOUBLE_PROPERTY(DetectorDistance);

  Q_PROPERTY(bool    implementTilt    READ get_ImplementTilt WRITE set_ImplementTilt);
  QCEP_BOOLEAN_PROPERTY(ImplementTilt);

  Q_PROPERTY(double detectorTilt READ get_DetectorTilt WRITE set_DetectorTilt);
  QCEP_DOUBLE_PROPERTY(DetectorTilt);

  Q_PROPERTY(double tiltPlaneRotation READ get_TiltPlaneRotation WRITE set_TiltPlaneRotation);
  QCEP_DOUBLE_PROPERTY(TiltPlaneRotation);

//public slots:
//  void onCenterXChanged(double cx);
//  void onCenterYChanged(double cy);
//  void onCenterChanged(QwtDoublePoint pt);
//  void onCenterChanged(QwtDoublePoint pt);
//  void onCenterStepChanged(double stp);

signals:
  void parameterChanged();

public slots:
  void onCenterChanged(QwtDoublePoint pt);

public:
//  void setEnabled(bool imgenabled, bool cntrenabled);
//  void setPen(const QPen &pen);

  void readSettings(QxrdSettings &settings, QString section);
  void writeSettings(QxrdSettings &settings, QString section);

  double getTTH(QwtDoublePoint pt);
  double get2th(QwtDoublePoint pt);

//signals:
////  void centerChanged(double cx, double cy);
//
//private:
////
//
private:
  mutable QMutex             m_Mutex;

  HEADER_IDENT("$Id: qxrdcenterfinder.h,v 1.2 2010/09/13 20:00:39 jennings Exp $");
};

#endif // QXRDCENTERFINDER_H

/******************************************************************
*
*  $Log: qxrdcenterfinder.h,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.5  2010/09/10 21:09:52  jennings
*  Tilt and powder fitting modifications
*
*  Revision 1.1.2.4  2010/09/10 18:54:10  jennings
*  Partial implement tilt in center finder dialog
*
*  Revision 1.1.2.3  2010/09/09 21:36:31  jennings
*  Made QxrdCenterFinder descend from QxrdDetectorGeometry
*
*  Revision 1.1.2.2  2010/09/08 19:40:57  jennings
*  Added tilt controls to center finder
*  Disabled fit refine command (for now)
*  Added percentile display mode
*
*  Revision 1.1.2.1  2010/07/22 18:39:37  jennings
*  Moving files into source subdirectory
*
*  Revision 1.8.4.2  2010/04/26 23:46:12  jennings
*  *** empty log message ***
*
*  Revision 1.8.4.1  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.8  2009/07/22 11:55:34  jennings
*  Center finder modifications
*
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

