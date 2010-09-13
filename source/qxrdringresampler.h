/******************************************************************
*
*  $Id: qxrdringresampler.h,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#ifndef QXRDRINGRESAMPLER_H
#define QXRDRINGRESAMPLER_H

#include "qxrdresampler.h"

class QxrdRingResampler : public QxrdResampler
{
  Q_OBJECT;
public:
    QxrdRingResampler(QObject *parent=0);

private:
    HEADER_IDENT("$Id: qxrdringresampler.h,v 1.2 2010/09/13 20:00:41 jennings Exp $");
};

#endif // QXRDRINGRESAMPLER_H

/******************************************************************
*
*  $Log: qxrdringresampler.h,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/08/03 20:14:38  jennings
*  Added classes for resampling ring centroids and fitting ring parameters
*
*
*******************************************************************/

