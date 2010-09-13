/******************************************************************
*
*  $Id: qxrdresultserializer.cpp,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#include "qxrdresultserializer.h"

QxrdResultSerializerBase::QxrdResultSerializerBase(QObject *parent) :
    QObject(parent),
    SOURCE_IDENT("$Id: qxrdresultserializer.cpp,v 1.2 2010/09/13 20:00:41 jennings Exp $")
{
}

QxrdResultSerializerBase::~QxrdResultSerializerBase()
{
}

/******************************************************************
*
*  $Log: qxrdresultserializer.cpp,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
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

