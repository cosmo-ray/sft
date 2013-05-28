#ifndef SYLLABE_H
#define SYLLABE_H

#include <QtGlobal>
#include <QString>

class Syllabe
{
public:
    Syllabe();

    void setStart(qint64);
    void setEnd(qint64);
    void setSentence(int);
    void setSyllabe(const QString &str);
    void setRelativePosition(int);

    qint64 getStart(void);
    qint64 getEnd(void);
    int getSentence(void);
    const QString &getSyllabe(void);
    int getRelativePosition(void);

private:
    qint64 _start;
    qint64 _end;
    QString _syllabe;
    int _sentence;
    int _relativePosition;
};

#endif // SYLLABE_H
