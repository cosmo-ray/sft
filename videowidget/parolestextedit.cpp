#include "parolestextedit.h"

parolestextedit::parolestextedit()
{
}

void parolestextedit::mousePressEvent(QMouseEvent *e) {
    QString s = toPlainText();
    int i = document()->documentLayout()->hitTest(QPointF( e->localPos().x(), e->localPos().y() ), Qt::ExactHit );
    qDebug() << i << " " << s;
    if(e->button()==Qt::LeftButton && e->modifiers()!=Qt::ControlModifier){
        if (s.at(i)=='&' && i>0 && s.at(i-1)!='\n') {
            s.remove(i,1);
        }
    } else if(e->button()==Qt::LeftButton && e->modifiers()==Qt::ControlModifier){
        if (s.at(i)!='&' && i>0 && s.at(i-1)!='&') {
        s.insert(i,'&');
        }
    }
    setText(s);
}
