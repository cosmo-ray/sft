#include "syllabe.h"
#include <QDebug>

Syllabe::Syllabe()
{
}

void Syllabe::setSentence(int sentence)
{
    _sentence = sentence;
}

void Syllabe::setStart(qint64 start)
{
    _start = start;
}

void Syllabe::setEnd(qint64 end)
{
    _end = end;
}

qint64 Syllabe::getStart(void)
{
    return (_start);
}

qint64 Syllabe::getEnd(void)
{
    return (_end);
}

int Syllabe::getSentence(void)
{
    return (_sentence);
}

void Syllabe::setSyllabe(const QString &str)
{
    _syllabe = str;
}

const QString &Syllabe::getSyllabe(void)
{
    return (_syllabe);
}

void Syllabe::setRelativePosition(int pos)
{
    _relativePosition = pos;
}

int Syllabe::getRelativePosition(void)
{
    return (_relativePosition);
}
