#ifndef QXRDDOCUMENTATIONDICTIONARY_H
#define QXRDDOCUMENTATIONDICTIONARY_H

#include <QString>
#include <QHash>

class QxrdDocumentationDictionary
{
public:
  QxrdDocumentationDictionary();

  void setDoc(QString name, QString doc);
  void setProto(QString name, QString proto);

  QString doc(QString name);
  QString proto(QString name);

  static QString get_Doc(QString name);
  static QString get_Proto(QString name);

private:
  QHash<QString,QString> m_Docs;
  QHash<QString,QString> m_Protos;
};

extern QxrdDocumentationDictionary *gDocumentationDirectory;

#endif // QXRDDOCUMENTATIONDICTIONARY_H
