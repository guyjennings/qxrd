/******************************************************************
*
*  $Id: qxrdresampler.h,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#ifndef QXRDRESAMPLER_H
#define QXRDRESAMPLER_H

#include "qcepmacros.h"
#include <QObject>

class QxrdResampler : public QObject
{
  Q_OBJECT
public:
  explicit QxrdResampler(QObject *parent = 0);

signals:

public slots:

private:
  HEADER_IDENT("$Id: qxrdresampler.h,v 1.2 2010/09/13 20:00:41 jennings Exp $");
};

#endif // QXRDRESAMPLER_H

/******************************************************************
*
*  $Log: qxrdresampler.h,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/08/03 20:14:38  jennings
*  Added classes for resampling ring centroids and fitting ring parameters
*
*
*******************************************************************/

