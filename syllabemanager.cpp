#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QFileInfo>
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
    _manager.resize(in.readAll().count("&"));
    file.reset();
    QString line = in.readLine();
    int lineIndex = 0;
    int syllabeIndex = 0;
    while (!line.isNull())
    {
        QStringList list1 = line.split("&");
        if (list1.size() > 1)
        {
            list1.pop_front();
            _sentenceManager.resize(_sentenceManager.size() + 1);
            Sentence &sen = _sentenceManager[lineIndex];
            sen.sentence.resize(list1.size());
            sen.setFirst(syllabeIndex);
            for(int i = 0; i < list1.size(); ++i)
            {
                Syllabe *syl =  &_manager[syllabeIndex];
                syl->setSyllabe(list1[i]);
                syl->setRelativePosition(i);
                sen.sentence[i] = syl;
                syl->setSentence(lineIndex);
                qDebug() << sen.sentence[i]->getSyllabe();
                ++syllabeIndex;
            }
            ++lineIndex;
        }
        line = in.readLine();
    }
    _isOpen = true;
}

void  SyllabeManager::saveFrmToFile(const QString &lyrFile)
{
    QFileInfo fi(lyrFile);
    QString frmFile = fi.absolutePath()+ "/" + fi.completeBaseName()+".frm";
    qDebug() << frmFile;
    QFile file(frmFile);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    for(int i = 0; i < _manager.size(); ++i) {
        out << _manager[i].getStart() << " " << _manager[i].getEnd() << "\n";
    }
    file.close();
}


QVector<Syllabe> &SyllabeManager::manager()
{
    return (_manager);
}

QVector<Sentence> &SyllabeManager::sentenceManager()
{
    return (_sentenceManager);
}
