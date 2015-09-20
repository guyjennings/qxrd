#ifndef QXRDTODOLISTITEM_H
#define QXRDTODOLISTITEM_H

#include "qcepobject.h"

class QxrdToDoListItem : public QcepObject
{
public:
  QxrdToDoListItem(bool active, QString d1, QString d2, QString s);

  bool isActive();
  QDateTime insertedDate();
  QDateTime completedDate();
  QString description();

signals:

public slots:

private:
  bool m_Active;
  QDateTime m_InsertedDate;
  QDateTime m_CompletedDate;
  QString   m_Description;
};

#endif // QXRDTODOLISTITEM_H
