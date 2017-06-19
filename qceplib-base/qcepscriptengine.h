#ifndef QCEPSCRIPTENGINE_H
#define QCEPSCRIPTENGINE_H

#include <QScriptEngine>

class QcepScriptEngine : public QScriptEngine
{
public:
  QcepScriptEngine(QObject *parent = 0);

public slots:
  QString documentationLink(QString base, QString subItem);
  QByteArray helpText(QString item);

  QString documentationText(QString item);
  QString documentationText(QRegExp pattern);

private:
  QString tableHeader();
  QString tableFooter();
};

#endif // QCEPSCRIPTENGINE_H
