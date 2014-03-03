#ifndef QCEPDOCUMENTATIONDICTIONARY_H
#define QCEPDOCUMENTATIONDICTIONARY_H

#include <QString>
#include <QHash>

class QcepDocumentationForObject
{
public:
  QcepDocumentationForObject(QString objectName, QString objectDoc);
};

class QcepLongDocumentationForObject
{
public:
  QcepLongDocumentationForObject(QString objectName, QString longDoc);
};

class QcepDocumentationForFunction : public QcepDocumentationForObject
{
public:
  QcepDocumentationForFunction(QString functionName,
                               QString functionProto,
                               QString functionDoc,
                               QString functionLogDoc);
};

class QcepDocumentationDictionary
{
public:
  QcepDocumentationDictionary();

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

  static QHash<QString,QString> docs();
  static QHash<QString,QString> protos();
  static QHash<QString,QString> longDocs();

private:
  QHash<QString,QString> m_Protos;
  QHash<QString,QString> m_Docs;
  QHash<QString,QString> m_LongDocs;
};

extern QcepDocumentationDictionary *gDocumentationDirectory;

#define _TOKENPASTE(x, y) x ## y
#define TOKENPASTE(x, y) _TOKENPASTE(x, y)

#define QCEP_DOC_OBJECT(name,doc) \
  static QcepDocumentationForObject TOKENPASTE(a__, __LINE__) (name, doc);

#define QCEP_DOC_LONG(name, longDoc) \
  static QcepLongDocumentationForObject TOKENPASTE(a__, __LINE__) (name, longDoc);

#define QCEP_DOC_FUNCTION(name, proto, doc, longDoc) \
  static QcepDocumentationForFunction TOKENPASTE(a__, __LINE__) (name, proto, doc, longDoc);

#endif // QXRDDOCUMENTATIONDICTIONARY_H
