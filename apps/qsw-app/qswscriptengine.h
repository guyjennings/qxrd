#ifndef QSWSCRIPTENGINE_H
#define QSWSCRIPTENGINE_H

#include "qswlib_global.h"
#include "qcepscriptengine.h"

class /*QSW_EXPORT*/ QswScriptEngine : public QcepScriptEngine
{
  Q_OBJECT

private:
  typedef QcepScriptEngine inherited;

public:
  Q_INVOKABLE QswScriptEngine(QString name);
  ~QswScriptEngine();

  void initialize(QcepObjectWPtr parent);
};

#endif // QSWSCRIPTENGINE_H
