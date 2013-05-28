#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include "syllabemanager.h"

SyllabeManager::SyllabeManager()
    : _isOpen(false)
{
}

bool SyllabeManager::isOpen()
{
    return (_isOpen);
}

void SyllabeManager::open(const QString &lyrFile)
{
    QFile file(lyrFile);

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString line = in.readLine();
    int lineIndex = 0;
    int syllabeIndex = 0;
    while (!line.isNull())
    {
        QStringList list1 = line.split("&");
        if (list1.size() > 1)
        {
            list1.pop_front();
            _manager.resize(_manager.size() + list1.size());
            _sentenceManager.resize(_sentenceManager.size() + 1);
            Sentence &sen = _sentenceManager[lineIndex];
            sen.sentence.resize(list1.size());
            for(int i = 0; i < list1.size(); ++i)
            {
                Syllabe &syl =  _manager[syllabeIndex];
                syl.setSyllabe(list1[i]);
                sen.sentence[i] = &syl;
                syl.setSentence(lineIndex);
                qDebug() << sen.sentence[i]->getSyllabe();
                ++syllabeIndex;
            }
            ++lineIndex;
        }
        line = in.readLine();
    }
    _isOpen = true;
}

QVector<Syllabe> &SyllabeManager::manager()
{
    return (_manager);
}

QVector<Sentence> &SyllabeManager::sentenceManager()
{
    return (_sentenceManager);
}
