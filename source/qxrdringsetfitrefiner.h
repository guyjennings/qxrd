/******************************************************************
*
*  $Id: qxrdringsetfitrefiner.h,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#ifndef QXRDRINGSETFITREFINER_H
#define QXRDRINGSETFITREFINER_H

#include "qxrdfitrefiner.h"

class QxrdRingSetFitRefiner : public QxrdFitRefiner
{
Q_OBJECT
public:
    explicit QxrdRingSetFitRefiner(QObject *parent = 0);

signals:

public slots:

private:
    HEADER_IDENT("$Id: qxrdringsetfitrefiner.h,v 1.2 2010/09/13 20:00:41 jennings Exp $");
};

#endif // QXRDRINGSETFITREFINER_H

/******************************************************************
*
*  $Log: qxrdringsetfitrefiner.h,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/08/03 20:14:38  jennings
*  Added classes for resampling ring centroids and fitting ring parameters
*
*
*******************************************************************/

