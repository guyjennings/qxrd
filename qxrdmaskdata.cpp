/******************************************************************
*
*  $Id: qxrdmaskdata.cpp,v 1.1 2009/08/03 20:58:59 jennings Exp $
*
*******************************************************************/

#include "qxrdmaskdata.h"

QxrdMaskData::QxrdMaskData(int width, int height)
  : QcepImageData<bool>(width, height),
    SOURCE_IDENT("$Id: qxrdmaskdata.cpp,v 1.1 2009/08/03 20:58:59 jennings Exp $")
{
}

/******************************************************************
*
*  $Log: qxrdmaskdata.cpp,v $
*  Revision 1.1  2009/08/03 20:58:59  jennings
*  Minor fixups
*
*
*******************************************************************/
