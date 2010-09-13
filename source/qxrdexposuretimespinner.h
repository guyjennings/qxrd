/******************************************************************
*
*  $Id: qxrdexposuretimespinner.h,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#ifndef QXRDEXPOSURETIMESPINNER_H
#define QXRDEXPOSURETIMESPINNER_H

#include "qcepmacros.h"
#include <QDoubleSpinBox>
#include <QVector>

class QxrdExposureTimeSpinner : public QDoubleSpinBox
{
  Q_OBJECT;
public:
  explicit QxrdExposureTimeSpinner(QWidget *parent = 0);

  virtual void stepBy(int steps);
signals:

public slots:

private:
  QVector<double> m_PreferredExposures;

  HEADER_IDENT("$Id: qxrdexposuretimespinner.h,v 1.2 2010/09/13 20:00:39 jennings Exp $");
};

#endif // QXRDEXPOSURETIMESPINNER_H

/******************************************************************
*
*  $Log: qxrdexposuretimespinner.h,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:38  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.1  2010/06/18 18:36:59  jennings
*  Implemented custom spin box for exposure time
*
*
*******************************************************************/

