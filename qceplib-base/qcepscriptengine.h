#ifndef QCEPSCRIPTENGINE_H
#define QCEPSCRIPTENGINE_H

#include <QScriptEngine>

class QcepScriptEngine : public QScriptEngine
{
public:
  QcepScriptEngine(QObject *parent = 0);
};

#endif // QCEPSCRIPTENGINE_H
