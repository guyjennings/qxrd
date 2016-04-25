#include "qxrdtodolistitem.h"

QxrdToDoListItem::QxrdToDoListItem(QObject *parent, int id, bool active, QString d1, QString d2, QString s)
  : QObject(parent),
    m_Identifier(id),
    m_Active(active),
    m_InsertedDate(QDateTime::fromString(d1, Qt::ISODate)),
    m_CompletedDate(QDateTime::fromString(d2, Qt::ISODate)),
    m_Description(s)
{
}

int QxrdToDoListItem::identifier()
{
  return m_Identifier;
}

bool QxrdToDoListItem::isActive()
{
  return m_Active;
}

QDateTime QxrdToDoListItem::insertedDate()
{
  return m_InsertedDate;
}

QDateTime QxrdToDoListItem::completedDate()
{
  return m_CompletedDate;
}

QString QxrdToDoListItem::description()
{
  return m_Description;
}
