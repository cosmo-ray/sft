#ifndef SENTENCE_H
#define SENTENCE_H

#include <syllabe.h>
#include <QVector>

class Sentence
{
public:
    Sentence();
    QString toString(int, bool);
    QVector<Syllabe *> sentence;
private:
};

#endif // SENTENCE_H
