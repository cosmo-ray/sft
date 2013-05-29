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
    void setFirst(int i);
    int getFirst();
private:
    int _firtlSyllabe;
};

#endif // SENTENCE_H
