#include "sentence.h"
#include <QDebug>

Sentence::Sentence()
{
}

QString Sentence::toString(int pos, bool b)
{
    QString res;

    for(int i = 0; i < sentence.size(); ++i)
    {
        if (i < pos)
        {
            res += "<font color=\"black\">";
            res += sentence[i]->getSyllabe();
            res += "</font>";
        }
        else if (i == pos && b)
        {
            res += "<font color=\"green\">";
            res += sentence[i]->getSyllabe();
            res += "</font>";
        }
        else if (i % 2)
        {
            res += "<font color=\"blue\">";
            res += sentence[i]->getSyllabe();
            res += "</font>";
        }
        else
        {
            res += "<font color=\"red\">";
            res += sentence[i]->getSyllabe();
            res += "</font>";
        }
    }
    return (res);
}
