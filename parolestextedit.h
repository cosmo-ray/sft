#ifndef PAROLESTEXTEDIT_H
#define PAROLESTEXTEDIT_H

#include <QTextEdit>
#include <QMouseEvent>
#include <QAbstractTextDocumentLayout>
#include <QDebug>

class parolestextedit : public QTextEdit
{
public:
    parolestextedit();
private:
    void mousePressEvent(QMouseEvent *e);
};

#endif // PAROLESTEXTEDIT_H
