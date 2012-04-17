#ifndef QXRDDOCUMENTATIONDICTIONARY_H
#define QXRDDOCUMENTATIONDICTIONARY_H

#include <QString>
#include <QHash>

class QxrdDocumentationForObject
{
public:
  QxrdDocumentationForObject(QString objectName, QString objectDoc);
};

class QxrdLongDocumentationForObject
{
public:
  QxrdLongDocumentationForObject(QString objectName, QString longDoc);
};

class QxrdDocumentationForFunction : public QxrdDocumentationForObject
{
public:
  QxrdDocumentationForFunction(QString functionName,
                               QString functionProto,
                               QString functionDoc,
                               QString functionLogDoc);
};

class QxrdDocumentationDictionary
{
public:
  QxrdDocumentationDictionary();

  void setProto(QString name, QString proto);
  void setDoc(QString name, QString doc);
  void setLongDoc(QString name, QString doc);

  QString proto(QString name);
  QString doc(QString name);
  QString longDoc(QString name);

  static QString get_Proto(QString name);
  static QString get_Doc(QString name);
  static QString get_LongDoc(QString name);

  static void set_Proto(QString name, QString proto);
  static void set_Doc(QString name, QString doc);
  static void set_LongDoc(QString name, QString longDoc);

private:
  QHash<QString,QString> m_Protos;
  QHash<QString,QString> m_Docs;
  QHash<QString,QString> m_LongDocs;
};

extern QxrdDocumentationDictionary *gDocumentationDirectory;

#define _TOKENPASTE(x, y) x ## y
#define TOKENPASTE(x, y) _TOKENPASTE(x, y)

#define QXRD_DOC_OBJECT(name,doc) \
  static QxrdDocumentationForObject TOKENPASTE(a__, __LINE__) (name, doc);

#define QXRD_DOC_LONG(name, longDoc) \
  static QxrdLongDocumentationForObject TOKENPASTE(a__, __LINE__) (name, longDoc);

#define QXRD_DOC_FUNCTION(name, proto, doc, longDoc) \
  static QxrdDocumentationForFunction TOKENPASTE(a__, __LINE__) (name, proto, doc, longDoc);

#endif // QXRDDOCUMENTATIONDICTIONARY_H
