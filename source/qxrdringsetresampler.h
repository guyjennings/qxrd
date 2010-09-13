/******************************************************************
*
*  $Id: qxrdringsetresampler.h,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#ifndef QXRDRINGSETRESAMPLER_H
#define QXRDRINGSETRESAMPLER_H

#include "qxrdresampler.h"
#include <QObject>

class QxrdRingSetResampler : public QxrdResampler
{
  Q_OBJECT;
public:
  explicit QxrdRingSetResampler(QObject *parent = 0);

signals:

public slots:

private:
  HEADER_IDENT("$Id: qxrdringsetresampler.h,v 1.2 2010/09/13 20:00:41 jennings Exp $");
};

#endif // QXRDRINGSETRESAMPLER_H

/******************************************************************
*
*  $Log: qxrdringsetresampler.h,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/08/03 20:14:38  jennings
*  Added classes for resampling ring centroids and fitting ring parameters
*
*
*******************************************************************/

