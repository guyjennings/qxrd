/******************************************************************
*
*  $Id: qxrdsharedpointer.h,v 1.2 2010/09/13 20:00:42 jennings Exp $
*
*******************************************************************/

#ifndef QXRDSHAREDPOINTER_H
#define QXRDSHAREDPOINTER_H

template <class T>
class QxrdSharedPointer
{
public:
    QxrdSharedPointer();
};

template <class T>
QxrdSharedPointer<T>::QxrdSharedPointer()
{
}


#endif // QXRDSHAREDPOINTER_H

/******************************************************************
*
*  $Log: qxrdsharedpointer.h,v $
*  Revision 1.2  2010/09/13 20:00:42  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:42  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.1  2010/06/11 21:20:40  jennings
*  Added QxrdSharedPointer, QxrdIntegratedData and QxrdIntegratedDataQueue
*
*
*******************************************************************/
