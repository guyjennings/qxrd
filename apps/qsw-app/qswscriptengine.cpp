#include "qswscriptengine.h"

QswScriptEngine::QswScriptEngine(QString name)
  : inherited(name)
{

}

QswScriptEngine::~QswScriptEngine()
{
}

void QswScriptEngine::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}
