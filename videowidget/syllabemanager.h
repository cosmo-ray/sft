#ifndef SYLLABEMANAGER_H
#define SYLLABEMANAGER_H

#include <QVector>
#include <syllabe.h>
#include <sentence.h>

class SyllabeManager
{
public:
    SyllabeManager();
    bool isOpen();

    void open(const QString &lyrFile);
private:
    bool _isOpen;
    QVector<Syllabe> _manager;
    QVector<Sentence> _sentenceManager;
};

#endif // SYLLABEMANAGER_H
