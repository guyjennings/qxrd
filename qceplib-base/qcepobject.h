#ifndef QCEPOBJECT_H
#define QCEPOBJECT_H

#include <QObject>
#include <QSettings>
#include "qcepobjectnamer.h"
#include <QDateTime>
#include <QSet>

class QcepObject : public QObject
{
  Q_OBJECT
public:
  explicit QcepObject(QString name, QcepObject *parent);
  virtual ~QcepObject();

  static int allocatedObjects();
  static int deletedObjects();

#ifndef QT_NO_DEBUG
  static QSet<QcepObject*> allocatedObjectsSet();
#endif

signals:

public slots:
  virtual void printLine(QString line);
  virtual void printMessage(QString msg, QDateTime dt=QDateTime::currentDateTime()) const;
  virtual void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;
  virtual void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;

  virtual QString settingsScript();
  QString scriptValueLiteral(QVariant v);

  void readObjectSettings(QSettings *set, QString section);

public:
  virtual void writeSettings(QSettings *set, QString section);
  virtual void readSettings(QSettings *set, QString section);

  QString get_Name() const;
  void    set_Name(QString name);

  static QString addSlashes(QString str);

private:
  QcepObject                         *m_Parent;
  QcepObjectNamer                     m_ObjectNamer;

public:
  Q_PROPERTY(QString name READ get_Name WRITE set_Name STORED false)
//  QCEP_STRING_PROPERTY(Name)
};

#endif // QCEPOBJECT_H
