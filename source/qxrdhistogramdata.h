/******************************************************************
*
*  $Id: qxrdhistogramdata.h,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#ifndef QXRDHISTOGRAMDATA_H
#define QXRDHISTOGRAMDATA_H

#include "qcepmacros.h"
#include <QObject>
#include <QSharedPointer>

class QxrdHistogramData : public QObject
{
  Q_OBJECT
public:
  explicit QxrdHistogramData(QObject *parent = 0);

signals:

public slots:

private:
  HEADER_IDENT("$Id: qxrdhistogramdata.h,v 1.2 2010/09/13 20:00:40 jennings Exp $");
};

typedef QSharedPointer<QxrdHistogramData> QxrdHistogramDataPtr;

#endif // QXRDHISTOGRAMDATA_H

/******************************************************************
*
*  $Log: qxrdhistogramdata.h,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:39  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.1  2010/06/17 16:02:36  jennings
*  Skeleton code to support result serialization during crucial points in processing
*  Separate QxrdDoubleImageData class which can hold a reference to a mask
*
*
*******************************************************************/

