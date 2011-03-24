#include <QtGui>

#include "qxrdhighlighter.h"

QxrdHighlighter::QxrdHighlighter(QTextDocument *parent)
  : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\break\\b" << "\\bcase\\b" << "\\bcatch\\b"
                    << "\\bcontinue\\b" << "\\bdefault\\b" << "\\bdelete\\b"
                    << "\\bdo\\b" << "\\belse\\b" << "\\bfinally\\b"
                    << "\\bfor\\b" << "\\bfunction\\b" << "\\bif\\b"
                    << "\\bin\\b" << "\\binstanceof\\b" << "\\bnew\\b"
                    << "\\breturn\\b" << "\\bswitch\\b" << "\\bthis\\b"
                    << "\\bthrow\\b" << "\\btry\\b" << "\\btypeof\\b"
                    << "\\bvar\\b" << "\\btypedef\\b" << "\\btypename\\b"
                    << "\\bunion\\b" << "\\bvoid\\b" << "\\bwhile\\b"
                    << "\\bwith\\b"
		    << "\\babstract\\b" << "\\benum\\b"       << "\\bint\\b"       << "\\bshort\\b"
		    << "\\bboolean\\b"  << "\\bexport\\b"     << "\\binterface\\b" << "\\bstatic\\b"
		    << "\\bbyte\\b"     << "\\bextends\\b"    << "\\blong\\b"      << "\\bsuper\\b"
		    << "\\bchar\\b"     << "\\bfinal\\b"      << "\\bnative\\b"    << "\\bsynchronized\\b"
		    << "\\bclass\\b"    << "\\bfloat\\b"      << "\\bpackage\\b"   << "\\bthrows\\b"
		    << "\\bconst\\b"    << "\\bgoto\\b"       << "\\bprivate\\b"   << "\\btransient\\b"
		    << "\\bdebugger\\b" << "\\bimplements\\b" << "\\bprotected\\b" << "\\bvolatile\\b"
		    << "\\bdouble\\b"   << "\\bimport\\b"     << "\\bpublic\\b";

    foreach (QString pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}

void QxrdHighlighter::highlightBlock(const QString &text)
{
    foreach (HighlightingRule rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = text.indexOf(expression);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = text.indexOf(expression, index + length);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        int endIndex = text.indexOf(commentEndExpression, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression,
                                                startIndex + commentLength);
    }
}
