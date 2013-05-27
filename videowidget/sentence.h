#ifndef SENTENCE_H
#define SENTENCE_H

#include <syllabe.h>
#include <QVector>

class Sentence
{
public:
    Sentence();
    QVector<Syllabe *> sentence;
private:
};

#endif // SENTENCE_H
