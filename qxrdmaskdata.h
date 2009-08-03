/******************************************************************
*
*  $Id: qxrdmaskdata.h,v 1.1 2009/08/03 20:58:59 jennings Exp $
*
*******************************************************************/

#ifndef QXRDMASKDATA_H
#define QXRDMASKDATA_H

#include "qcepmacros.h"
#include "qcepimagedata.h"

class QxrdMaskData : public QcepImageData<bool>
{
  Q_OBJECT;

public:
  QxrdMaskData(int width=0, int height=0);

private:
  HEADER_IDENT("$Id: qxrdmaskdata.h,v 1.1 2009/08/03 20:58:59 jennings Exp $");
};

#endif // QXRDMASKDATA_H

/******************************************************************
*
*  $Log: qxrdmaskdata.h,v $
*  Revision 1.1  2009/08/03 20:58:59  jennings
*  Minor fixups
*
*
*******************************************************************/
