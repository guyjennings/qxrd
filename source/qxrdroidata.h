/******************************************************************
*
*  $Id: qxrdroidata.h,v 1.2 2010/09/13 20:00:42 jennings Exp $
*
*******************************************************************/

#ifndef QXRDROIDATA_H
#define QXRDROIDATA_H

#include "qcepmacros.h"
#include <QObject>
#include <QSharedPointer>

class QxrdROIData : public QObject
{
Q_OBJECT
public:
    explicit QxrdROIData(QObject *parent = 0);

signals:

public slots:

private:
  HEADER_IDENT("$Id: qxrdroidata.h,v 1.2 2010/09/13 20:00:42 jennings Exp $");
};

typedef QSharedPointer<QxrdROIData> QxrdROIDataPtr;

#endif // QXRDROIDATA_H

/******************************************************************
*
*  $Log: qxrdroidata.h,v $
*  Revision 1.2  2010/09/13 20:00:42  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:41  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.1  2010/06/17 16:02:36  jennings
*  Skeleton code to support result serialization during crucial points in processing
*  Separate QxrdDoubleImageData class which can hold a reference to a mask
*
*
*******************************************************************/

