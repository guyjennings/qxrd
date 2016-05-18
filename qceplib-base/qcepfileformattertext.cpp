#include "qcepfileformattertext.h"
#include "qcepserializableobject.h"

QcepFileFormatterText::QcepFileFormatterText(QString filePath) :
  QcepFileFormatter(filePath),
  m_OutputIndent(0),
  m_OutputStep(2),
  m_File(NULL)
{
}

void QcepFileFormatterText::beginWriteFile()
{
  openForWriting();
}

void QcepFileFormatterText::endWriteFile()
{
  closeForWriting();
}

void QcepFileFormatterText::writeIndent()
{
  writeToken(tr("%1").arg("",m_OutputIndent*m_OutputStep));
}

void QcepFileFormatterText::writeToken(QString tok)
{
  fputs(qPrintable(tok), m_File);
}

void QcepFileFormatterText::writeVariant(QVariant val)
{
  QString s = toScriptLiteral(val);

  writeToken(s);
}

void QcepFileFormatterText::beginWriteObject(QString objectName, QString className)
{
  printMessage(tr("%1%2:%3 {").arg("", m_OutputIndent).arg(objectName).arg(className));

  writeIndent();
  writeToken(objectName);
  writeToken(":");
  writeToken(className);
  writeToken(" {\n");

  m_OutputIndent++;
  m_ObjectPath.append(objectName);
}

void QcepFileFormatterText::endWriteObject()
{
  m_ObjectPath.takeLast();
  m_OutputIndent--;

  printMessage(tr("%1}").arg("", m_OutputIndent));

  writeIndent();
  writeToken("}\n");
}

void QcepFileFormatterText::writeComment(QString cmt)
{
  printMessage(tr("%1// %2").arg("", m_OutputIndent).arg(cmt));

  writeIndent();
  writeToken("// ");
  writeToken(cmt);
  writeToken("\n");
}

void QcepFileFormatterText::beginWriteProperties()
{
  printMessage(tr("%1properties {").arg("", m_OutputIndent));

  writeIndent();
  writeToken("properties {\n");

  m_OutputIndent++;
}

void QcepFileFormatterText::writeProperty(QString name, QVariant val)
{
  printMessage(tr("%1%2 = %3").arg("",m_OutputIndent).arg(name).arg(toScriptLiteral(val)));

  writeIndent();
  writeToken(name);
  writeToken(" = ");
  writeVariant(val);
  writeToken("\n");
}

void QcepFileFormatterText::endWriteProperties()
{
  m_OutputIndent--;

  printMessage(tr("%1}").arg("", m_OutputIndent));

  writeIndent();
  writeToken("}\n");
}

void QcepFileFormatterText::beginWriteChildren()
{
  printMessage(tr("%1children {").arg("", m_OutputIndent));

  writeIndent();
  writeToken("children {\n");

  m_OutputIndent++;
}

void QcepFileFormatterText::endWriteChildren()
{
  m_OutputIndent--;

  printMessage(tr("%1}").arg("", m_OutputIndent));

  writeIndent();
  writeToken("}\n");
}

void QcepFileFormatterText::beginWriteData()
{
  printMessage(tr("%1data {").arg("", m_OutputIndent));

  writeIndent();
  writeToken("data {\n");

  m_OutputIndent++;
}

void QcepFileFormatterText::endWriteData()
{
  m_OutputIndent--;

  printMessage(tr("%1}").arg("", m_OutputIndent));

  writeIndent();
  writeToken("}\n");
}

void QcepFileFormatterText::openForWriting()
{
  m_File = fopen(qPrintable(m_FilePath), "w+");

  if (m_File == NULL) {
    printMessage(tr("File open of %1 failed").arg(m_FilePath));
  }
}

void QcepFileFormatterText::closeForWriting()
{
  fclose(m_File);
}

QChar QcepFileFormatterText::skipWhiteSpace()
{
  while (!feof(m_File)) {
    int ch = getc(m_File);

    if (!QChar::isSpace(ch)) {
      ungetc(ch, m_File);
      return ch;
    }
  }

  return -1;
}

QChar QcepFileFormatterText::nextChar()
{
  return getc(m_File);
}

void QcepFileFormatterText::ungetChar(QChar ch)
{
  ungetc(ch.unicode(), m_File);
}

void QcepFileFormatterText::nextToken()
{
  m_TokenType = nextToken(m_Token);
}

QcepFileFormatterText::TokenType QcepFileFormatterText::nextToken(QString &token)
{
  QcepFileFormatterText::TokenType res = EndOfFile;

  QChar ch = skipWhiteSpace();

  token = ch;

  if (ch.isLetter()) {
    res = identifierToken(token);
  } else if (ch.isNumber() || ch=='-') {
    res = numberToken(token);
  } else if (ch=='"') {
    res = stringToken(token);
  } else if (ch=='@') {
    res = variantToken(token);
  } else {
    res = operatorToken(token);
  }

  printMessage(tr("Token %1 = \"%2\"").arg(res).arg(token));

  return res;
}

QcepFileFormatterText::TokenType QcepFileFormatterText::numberToken(QString &token)
{
  token = "";

  QChar ch = nextChar();

  if (ch == '-') {
    token.append(ch);
    ch = nextChar();
  }

  while (ch.isNumber()) {
    token.append(ch);
    ch = nextChar();
  }

  if (ch == '.') {
    token.append(ch);
    ch = nextChar();
  }

  while (ch.isNumber()) {
    token.append(ch);
    ch = nextChar();
  }

  if (ch == 'e' || ch == 'E') {
    token.append(ch);
    ch = nextChar();
  }

  if (ch == '-' || ch == '+') {
    token.append(ch);
    ch = nextChar();
  }

  while (ch.isNumber()) {
    token.append(ch);
    ch = nextChar();
  }

  ungetChar(ch);

  return Number;
}

QcepFileFormatterText::TokenType QcepFileFormatterText::stringToken(QString &token)
{
  QChar ch = nextChar();

  if (ch == '"') {
    token = ch;

    while (!(ch = nextChar()).isNull() && ch != '"') {
      token.append(ch);

      if (ch == '\\') {
        token.append(nextChar());
      }
    }

    token.append("\"");
  } else {
    printMessage("Error in string");
  }

  return String;
}

QcepFileFormatterText::TokenType QcepFileFormatterText::operatorToken(QString &token)
{
  token = nextChar();

  return Operator;
}

QcepFileFormatterText::TokenType QcepFileFormatterText::identifierToken(QString &token)
{
  token = "";

  QChar ch = nextChar();

  while (ch.isLetterOrNumber() || ch == '_') {
    token.append(ch);
    ch = nextChar();
  }

  ungetChar(ch);

  return Identifier;
}

QcepFileFormatterText::TokenType QcepFileFormatterText::variantToken(QString &token)
{
  token = nextChar();

  QString id;

  if (identifierToken(id) == Identifier) {
    token.append(id);

    QString obr;

    if (operatorToken(obr) == Operator && obr == "(") {
      token.append(obr);

      QString tok;
      do {
        int id = nextToken(tok);

        token.append(tok);
      } while (tok.length() > 0 && tok != ")");
    }
  }

  return Variant;
}

void QcepFileFormatterText::beginReadFile()
{
  openForReading();
}

void QcepFileFormatterText::endReadFile()
{
  closeForReading();
}

void QcepFileFormatterText::openForReading()
{
  m_File = fopen(qPrintable(m_FilePath), "r+");

  if (m_File == NULL) {
    printMessage(tr("File open of %1 failed").arg(m_FilePath));
  }
}

void QcepFileFormatterText::closeForReading()
{
  fclose(m_File);
}

QcepSerializableObjectPtr QcepFileFormatterText::nextObject()
{
  QString s1, s2, s3, s4;
  int     i1, i2, i3, i4;

  i1 = nextToken(s1);
  i2 = nextToken(s2);
  i3 = nextToken(s3);
  i4 = nextToken(s4);

  QcepSerializableObjectPtr obj;

  if (i1 == Identifier && s2 == ":" && i3 == Identifier && s4 == "{") {
    obj = construct(s3);

    if (obj) {
      obj->set_Name(s1);
      obj->readObject(qSharedPointerDynamicCast<QcepFileFormatter>(sharedFromThis()));
    } else {
      printMessage(tr("Failed to construct %1:%2").arg(s1).arg(s3));
    }
  }

  return obj;
}

void QcepFileFormatterText::beginReadObject(QcepSerializableObjectPtr obj)
{
  nextToken();
}

void QcepFileFormatterText::endReadObject()
{
}

bool QcepFileFormatterText::beginReadProperties()
{
  bool res = false;

  if (m_TokenType == Identifier && m_Token == "properties") {
    nextToken();

    if (m_TokenType == Operator && m_Token == "{") {
      nextToken();

      res = true;
    }
  }

  return res;
}

QString QcepFileFormatterText::nextPropertyName()
{
  QString res = "";

  if (m_TokenType == Identifier) {
    res = m_Token;
    nextToken();
  }

  return res;
}

QVariant QcepFileFormatterText::nextPropertyValue()
{
  QVariant res = QVariant();

  if (m_TokenType == Operator && m_Token == "=") {
    nextToken();

    if (m_TokenType == Number) {
      res = QVariant(m_Token.toDouble());
    } else if (m_TokenType == String) {
      printMessage("Need to parse string literal to string value");
      res = QVariant(m_Token);
    } else if (m_TokenType == Variant) {
      printMessage("Need to parse token to QVariant");
      res = m_Token;
    } else {
      printMessage(tr("nextPropertyValue : unexpected token %1").arg(m_Token));
    }

    nextToken();
  }

  return res;
}

void QcepFileFormatterText::endReadProperties()
{
  if (m_TokenType == Operator && m_Token == "}") {
    nextToken();
  } else {
    printMessage(tr("endReadProperties : unexpected token \"%1\"").arg(m_Token));
  }
}

bool QcepFileFormatterText::beginReadChildren()
{
  bool res = false;

  if (m_TokenType == Identifier && m_Token == "children") {
    nextToken();

    if (m_TokenType == Operator && m_Token == "{") {
//      nextToken();

      res = true;
    }
  }

  return res;
}

QcepSerializableObjectPtr QcepFileFormatterText::nextChild()
{
  return nextObject();
}

void QcepFileFormatterText::endReadChildren()
{
  if (m_TokenType == Operator && m_Token == "}") {
    nextToken();
  } else {
    printMessage(tr("endReadChildren : unexpected token \"%1\"").arg(m_Token));
  }
}

bool QcepFileFormatterText::beginReadData()
{
  bool res = false;

  if (m_TokenType == Identifier && m_Token == "data") {
    nextToken();

    if (m_TokenType == Operator && m_Token == "{") {
      nextToken();

      res = true;
    }
  }

  return res;
}

void QcepFileFormatterText::endReadData()
{
  if (m_TokenType == Operator && m_Token == "}") {
    nextToken();
  } else {
    printMessage(tr("endReadData : unexpected token \"%1\"").arg(m_Token));
  }
}
