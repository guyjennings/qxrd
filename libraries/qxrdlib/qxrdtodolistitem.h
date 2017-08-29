#ifndef QXRDTODOLISTITEM_H
#define QXRDTODOLISTITEM_H

#include "qxrdlib_global.h"
#include "qcepobject.h"

class QXRD_EXPORT QxrdToDoListItem : public QObject
{
public:
  QxrdToDoListItem(QObject *parent, int id, bool active, QString d1, QString d2, QString s);

  int identifier();
  bool isActive();
  QDateTime insertedDate();
  QDateTime completedDate();
  QString description();

signals:

public slots:

private:
  int       m_Identifier;
  bool      m_Active;
  QDateTime m_InsertedDate;
  QDateTime m_CompletedDate;
  QString   m_Description;
};

#endif // QXRDTODOLISTITEM_H
