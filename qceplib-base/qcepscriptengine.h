#ifndef QCEPSCRIPTENGINE_H
#define QCEPSCRIPTENGINE_H

#include <QScriptEngine>
#include "qcepobject-ptr.h"

class QcepScriptEngine : public QScriptEngine
{
public:
  QcepScriptEngine(QObject *parent = 0);

public slots:
  QString documentationLink(QString base, QString subItem);
  QString objectLink(QcepObject *obj);
  QByteArray helpText(QString item);
  QByteArray helpText(QcepObject *obj);
  QString documentationText(QcepObject *qobj);
  QString documentationText(QString item);
  QString documentationText(QRegExp pattern);

private:
  QString tableHeader();
  QString tableFooter();
};

#endif // QCEPSCRIPTENGINE_H
