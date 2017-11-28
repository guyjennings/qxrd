#include "qcepscriptengine.h"
#include "qcepdocumentationdictionary.h"
#include <QMetaProperty>
#include <QScriptValueIterator>
#include "qcepserializableobject.h"

QcepScriptEngine::QcepScriptEngine(QObject *parent) :
  QScriptEngine(parent)
{
}

QString QcepScriptEngine::documentationLink(QString base, QString subItem)
{
  QString item = base.isEmpty() ? subItem : base+"."+subItem;
  QString res = tr("<a href=\"qrc:/help/autohelp?%1\">%2</a>").arg(item).arg(item);

  QString proto = QcepDocumentationDictionary::get_Proto(item);
  QString doc   = QcepDocumentationDictionary::get_Doc(item);

  res.append("</td>\n<td width=\"75%\">\n");

  if (proto.length()) {
    res.append(tr("<p><i>%1</i></p>\n").arg(proto));
  }

  if (doc.length()) {
    res.append(tr("<p>%1</p>\n").arg(doc));
  }

  return res;
}

QString QcepScriptEngine::objectLink(QcepObject *obj)
{
  QString res = tr("<a href=\"qrc:/help/objecthelp?%1\">%2</a>")
      .arg((quint64) obj)
      .arg(obj->get_Name());

  return res;
}

QString QcepScriptEngine::tableHeader()
{
  return
      "<table border=\"0\" cellpadding=\"0\" cellspacing=\"1\" bgcolor=\"black\" width=\"100%\">\n"
      "<tr>\n"
      "<td bgcolor=\"black\">\n"
      "<table border=\"0\" cellpadding=\"1\" cellspacing=\"1\" bgcolor=\"black\" width=\"100%\">\n";
}

QString QcepScriptEngine::tableFooter()
{
  return
      "</table>\n"
      "</td>\n"
      "</tr>\n"
      "</table>\n";
}

QString QcepScriptEngine::documentationText(QcepObject *qobj)
{
  QString res;

  if (qobj) {
    QString itemName = qobj->get_Name();
    QString prefix = "" /*itemName + "."*/;

    res.append(tr("<h2>Documentation for %1</h2>\n").arg(qobj->get_Name()));

    res.append(tr("<h3>QObject class %1 name %2</h3>\n")
               .arg(qobj->metaObject()->className())
               .arg(qobj->objectName()));

    const QMetaObject *meta = qobj->metaObject();

    if (meta->propertyCount() > QObject::staticMetaObject.propertyCount()) {
      res.append(tr("<h3>%1 Properties</h3>\n").arg(itemName));
      res.append(tableHeader());

      for (int i=QObject::staticMetaObject.propertyCount();
           i<meta->propertyCount(); i++) {
        const char* propName = meta->property(i).name();
        QVariant val = qobj->property(propName);

        if ((i%2)) {
          res.append(tr("<tr bgcolor=\"#e0e0e0\">\n"));
        } else {
          res.append(tr("<tr bgcolor=\"white\">\n"));
        }
        res.append(tr("<td>%1</td><td>%2</td><td>%3</td>\n")
                   .arg(prefix+propName)
                   .arg(val.typeName())
                   .arg(val.toString()));

        QString doc = QcepDocumentationDictionary::get_Doc(prefix+propName);

        res.append(tr("<td width=\"75%\">%1</td>\n").arg(doc));

        res.append("</tr>\n");
      }

      res.append(tableFooter());
    }

    if (qobj->dynamicPropertyNames().count()) {
      res.append(tr("<h3>%1 User Defined Properties</h3>\n").arg(itemName));
      res.append(tableHeader());

      QList<QByteArray> props = qobj->dynamicPropertyNames();

      for (int i=0; i<props.count(); i++) {
        QString propName = props[i];
        QVariant val = qobj->property(qPrintable(propName));

        if ((i%2)) {
          res.append(tr("<tr bgcolor=\"#e0e0e0\">\n"));
        } else {
          res.append(tr("<tr bgcolor=\"white\">\n"));
        }

        res.append(tr("<td>%1</td><td>%2</td><td>%3</td>\n")
                   .arg(prefix+propName)
                   .arg(val.typeName())
                   .arg(val.toString()));

        QString doc = QcepDocumentationDictionary::get_Doc(prefix+propName);

        res.append(tr("<td width=\"75%\">%1</td>\n").arg(doc));

        res.append("</tr>\n");
      }

      res.append(tableFooter());
    }

    if (meta->methodCount() > QObject::staticMetaObject.methodCount()) {
      res.append(tr("<h3>%1 Functions</h3>\n").arg(itemName));
      res.append(tableHeader());

      for (int i=QObject::staticMetaObject.methodCount();
           i<meta->methodCount(); i++) {
        QByteArray methodSig = meta->method(i).methodSignature();
        const char* returnType = meta->method(i).typeName();
        QByteArray  methodName = meta->method(i).name();

        QMetaMethod::MethodType methodType = meta->method(i).methodType();

        if ((i%2)) {
          res.append(tr("<tr bgcolor=\"#e0e0e0\">\n"));
        } else {
          res.append(tr("<tr bgcolor=\"white\">\n"));
        }

        QList<QByteArray> parameterTypes = meta->method(i).parameterTypes();
        QList<QByteArray> parameterNames = meta->method(i).parameterNames();

        QString parameterPack = "";

        for (int i=0; i<parameterTypes.count(); i++) {
          if (i!=0) {
            parameterPack += ", ";
          }
          parameterPack += parameterTypes.value(i) + " " + parameterNames.value(i);
        }

        QString prototype = tr("%1 %2%3(%4)")
            .arg(returnType)
            .arg(QString(prefix))
            .arg(QString(methodName))
            .arg(parameterPack);

        prototype.replace(QString("&"), QString("&amp;"));
        prototype.replace(QString("\""), QString("&quot;"));
        prototype.replace(QString("'"), QString("&#039;"));
        prototype.replace(QString("<"), QString("&lt;"));
        prototype.replace(QString(">"), QString("&gt;"));

        res.append(tr("<td><i>%1</i></td>\n").arg(prototype));

        QString proto = QcepDocumentationDictionary::get_Proto(methodSig);
        QString doc   = QcepDocumentationDictionary::get_Doc(methodSig);

        res.append("<td>");
        switch (methodType) {
        case QMetaMethod::Signal:
          res.append("<i>Signal</i><br/>\n");
          break;
        case QMetaMethod::Slot:
          res.append("<i>Slot</i><br/>\n");
          break;
        default:
          break;
        }

        res.append(tr("%1<br/>\n").arg(proto));
        res.append(tr("%1</td>\n").arg(doc));

        res.append(tr("</tr>\n"));
      }

      res.append(tableFooter());
    }

    if (qobj && (qobj->childCount() > 0)) {
      res.append(tr("<h3>%1 Children</h3>\n").arg(itemName));
      res.append(tableHeader());

      for (int i=0; i<qobj->childCount(); i++) {
        QcepObjectPtr obj = qobj->childPtr(i);

        if (obj) {
          if ((i%2)) {
            res.append(tr("<tr bgcolor=\"#e0e0e0\">\n"));
          } else {
            res.append(tr("<tr bgcolor=\"white\">\n"));
          }

          res.append(tr("<td>%1</td><td>%2</td><td>%3</td>\n")
                     .arg(i)
                     .arg(objectLink(obj.data()))
                     .arg(obj->get_Type()));

          res.append("</tr>\n");
        }
      }

      res.append(tableFooter());
    }
  } else {
    res.append(tr("<h2>Documentation for NULL</h2>\n"));
  }

  return res;
}

QString QcepScriptEngine::documentationText(QString item)
{
  //  QScriptValue val=QScriptEngine::evaluate("global." + item, QString(), 1);

  QScriptValue val;
  QString prefix  = "";
  QString itemName="Global";

  if (item.isEmpty()) {
    val = globalObject();
  } else {
    val = globalObject().property(item);
    prefix = item+".";
    itemName = item;
  }

  QString res;
  //  QString name  = val.toString();
  QString doc   = QcepDocumentationDictionary::get_Doc(item);
  QString proto = QcepDocumentationDictionary::get_Proto(item);
  QString longDoc = QcepDocumentationDictionary::get_LongDoc(item);

  if (val.isFunction()) {
    if (proto.length()) {
      res.append(tr("<p><i>%1</i></p>\n").arg(proto));
    }

    if (doc.length()) {
      res.append(tr("%1\n").arg(doc));
    }

    if (longDoc.length()) {
      res.append(tr("%1\n").arg(longDoc));
    }
  } else if (val.isObject()) {
    res.append(tr("%1\n").arg(doc));
    res.append(tr("%1\n").arg(longDoc));

    QObject *qobj = val.toQObject();

    if (qobj) {
      res.append(tr("<h3>QObject class %1 name %2</h3>\n")
                 .arg(qobj->metaObject()->className())
                 .arg(qobj->objectName()));

      const QMetaObject *meta = qobj->metaObject();

      if (meta->propertyCount() > QObject::staticMetaObject.propertyCount()) {
        res.append(tr("<h3>%1 Properties</h3>\n").arg(itemName));
        res.append(tableHeader());

        for (int i=QObject::staticMetaObject.propertyCount();
             i<meta->propertyCount(); i++) {
          const char* propName = meta->property(i).name();
          QVariant val = qobj->property(propName);

          if ((i%2)) {
            res.append(tr("<tr bgcolor=\"#e0e0e0\">\n"));
          } else {
            res.append(tr("<tr bgcolor=\"white\">\n"));
          }
          res.append(tr("<td>%1</td><td>%2</td><td>%3</td>\n")
                     .arg(prefix+propName)
                     .arg(val.typeName())
                     .arg(val.toString()));

          QString doc = QcepDocumentationDictionary::get_Doc(prefix+propName);

          res.append(tr("<td width=\"75%\">%1</td>\n").arg(doc));

          res.append("</tr>\n");
        }

        res.append(tableFooter());
      }

      if (qobj->dynamicPropertyNames().count()) {
        res.append(tr("<h3>%1 User Defined Properties</h3>\n").arg(itemName));
        res.append(tableHeader());

        QList<QByteArray> props = qobj->dynamicPropertyNames();

        for (int i=0; i<props.count(); i++) {
          QString propName = props[i];
          QVariant val = qobj->property(qPrintable(propName));

          if ((i%2)) {
            res.append(tr("<tr bgcolor=\"#e0e0e0\">\n"));
          } else {
            res.append(tr("<tr bgcolor=\"white\">\n"));
          }

          res.append(tr("<td>%1</td><td>%2</td><td>%3</td>\n")
                     .arg(prefix+propName)
                     .arg(val.typeName())
                     .arg(val.toString()));

          QString doc = QcepDocumentationDictionary::get_Doc(prefix+propName);

          res.append(tr("<td width=\"75%\">%1</td>\n").arg(doc));

          res.append("</tr>\n");
        }

        res.append(tableFooter());
      }

      if (meta->methodCount() > QObject::staticMetaObject.methodCount()) {
        res.append(tr("<h3>%1 Functions</h3>\n").arg(itemName));
        res.append(tableHeader());

        for (int i=QObject::staticMetaObject.methodCount();
             i<meta->methodCount(); i++) {
          QByteArray methodSig = meta->method(i).methodSignature();
          const char* returnType = meta->method(i).typeName();
          QByteArray  methodName = meta->method(i).name();

          QMetaMethod::MethodType methodType = meta->method(i).methodType();

          if ((i%2)) {
            res.append(tr("<tr bgcolor=\"#e0e0e0\">\n"));
          } else {
            res.append(tr("<tr bgcolor=\"white\">\n"));
          }

          QList<QByteArray> parameterTypes = meta->method(i).parameterTypes();
          QList<QByteArray> parameterNames = meta->method(i).parameterNames();

          QString parameterPack = "";

          for (int i=0; i<parameterTypes.count(); i++) {
            if (i!=0) {
              parameterPack += ", ";
            }
            parameterPack += parameterTypes.value(i) + " " + parameterNames.value(i);
          }

          QString prototype = tr("%1 %2%3(%4)")
              .arg(returnType)
              .arg(QString(prefix))
              .arg(QString(methodName))
              .arg(parameterPack);

          prototype.replace(QString("&"), QString("&amp;"));
          prototype.replace(QString("\""), QString("&quot;"));
          prototype.replace(QString("'"), QString("&#039;"));
          prototype.replace(QString("<"), QString("&lt;"));
          prototype.replace(QString(">"), QString("&gt;"));

          res.append(tr("<td><i>%1</i></td>\n").arg(prototype));

          QString proto = QcepDocumentationDictionary::get_Proto(methodSig);
          QString doc   = QcepDocumentationDictionary::get_Doc(methodSig);

          res.append("<td>");
          switch (methodType) {
          case QMetaMethod::Signal:
            res.append("<i>Signal</i><br/>\n");
            break;
          case QMetaMethod::Slot:
            res.append("<i>Slot</i><br/>\n");
            break;
          default:
            break;
          }

          res.append(tr("%1<br/>\n").arg(proto));
          res.append(tr("%1</td>\n").arg(doc));

          res.append(tr("</tr>\n"));
        }

        res.append(tableFooter());
      }

      QcepObject *qcobj =
          qobject_cast<QcepObject*>(qobj);

      if (qcobj && (qcobj->childCount() > 0)) {
        res.append(tr("<h3>%1 Children</h3>\n").arg(itemName));
        res.append(tableHeader());

        for (int i=0; i<qcobj->childCount(); i++) {
          QcepObjectPtr obj = qcobj->childPtr(i);

          if (obj) {
            if ((i%2)) {
              res.append(tr("<tr bgcolor=\"#e0e0e0\">\n"));
            } else {
              res.append(tr("<tr bgcolor=\"white\">\n"));
            }

            res.append(tr("<td>%1</td><td>%2</td><td>%3</td>\n")
                       .arg(i)
                       .arg(objectLink(obj.data()))
                       .arg(obj->get_Type()));

            res.append("</tr>\n");
          }
        }

        res.append(tableFooter());
      }
    } else {
      QScriptValueIterator iter(val);

      QMap<QString, QScriptValue> subObjects, properties, functions;

      while (iter.hasNext()) {
        iter.next();

        QScriptValue v = iter.value();
        QString      n = iter.name();

        if (v.isFunction()) {
          functions.insert(n, v);
        } else if (v.isObject()) {
          subObjects.insert(n, v);
        } else {
          properties.insert(n, v);
        }
      }

      if (subObjects.count()) {
        QMapIterator<QString,QScriptValue> obj_iter(subObjects);

        res.append(tr("<h3>%1 Objects</h3>\n").arg(itemName));
        res.append(tableHeader());

        int i=0;

        while (obj_iter.hasNext()) {
          obj_iter.next();

          if ((i++%2)) {
            res.append(tr("<tr bgcolor=\"#e0e0e0\">\n"));
          } else {
            res.append(tr("<tr bgcolor=\"white\">\n"));
          }
          res.append(tr("<td>%1</td>").arg(documentationLink(item, obj_iter.key())));
          res.append(tr("</tr>\n"));
        }

        res.append(tableFooter());
      }

      if (properties.count()) {
        QMapIterator<QString,QScriptValue> prop_iter(properties);

        res.append(tr("<h3>%1 Properties</h3>\n").arg(itemName));
        res.append(tableHeader());

        int i=0;

        while (prop_iter.hasNext()) {
          prop_iter.next();

          if ((i++%2)) {
            res.append(tr("<tr bgcolor=\"#e0e0e0\">\n"));
          } else {
            res.append(tr("<tr bgcolor=\"white\">\n"));
          }
          res.append(tr("<td>%1</td>").arg(documentationLink(item, prop_iter.key())));
          res.append(tr("</tr>\n"));
        }

        res.append(tableFooter());
      }

      if (functions.count()) {
        QMapIterator<QString,QScriptValue> func_iter(functions);

        res.append(tr("<h3>%1 Functions</h3>\n").arg(itemName));
        res.append(tableHeader());

        int i=0;

        while (func_iter.hasNext()) {
          func_iter.next();

          if ((i++%2)) {
            res.append(tr("<tr bgcolor=\"#e0e0e0\">\n"));
          } else {
            res.append(tr("<tr bgcolor=\"white\">\n"));
          }
          res.append(tr("<td>%1</td>").arg(documentationLink(item, func_iter.key())));
          res.append(tr("</tr>\n"));
        }

        res.append(tableFooter());
      }
    }
  }

  return res;
}

QString QcepScriptEngine::documentationText(QRegExp pattern)
{
  QString res="";

  QHashIterator<QString, QString> iter(QcepDocumentationDictionary::docs());

  while (iter.hasNext()) {
    iter.next();
    if (pattern.exactMatch(iter.key())) {
      res.append(documentationText(iter.key()));
    }
  }

  return res;
}

QByteArray QcepScriptEngine::helpText(QString item)
{
  QString scriptItem;

  scriptItem = item;

  QByteArray res;

  res.append(
        "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n"
        "<html>\n"
        "  <head>\n"
        "    <title>qxrd help</title>\n"
        "  </head>\n"
        "\n"
        "  <body>\n");

  res.append(documentationText(scriptItem));

  res.append(
        "  </body>\n"
        "</html>\n");

  return res;
}

QByteArray QcepScriptEngine::helpText(QcepObject *obj)
{
  QByteArray res;

  res.append(
        "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n"
        "<html>\n"
        "  <head>\n"
        "    <title>qxrd help</title>\n"
        "  </head>\n"
        "\n"
        "  <body>\n");

  res.append(documentationText(obj));

  res.append(
        "  </body>\n"
        "</html>\n");

  return res;
}
