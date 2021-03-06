/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <qmediaplayer.h>

#include <QtGui/QMovie>
#include <QtWidgets/QWidget>
#include <syllabemanager.h>
#include <QPushButton>
#include <QLabel>
#include <parolestextedit.h>

QT_BEGIN_NAMESPACE
class QAbstractButton;
class QSlider;
class QLabel;
QT_END_NAMESPACE

class VideoPlayer : public QWidget
{
    Q_OBJECT
public:
    VideoPlayer(QWidget *parent = 0);
    ~VideoPlayer();

public slots:
    void openFile();
    void openLyr();
    void reloadLyr();
    void play();



private slots:
    void mediaStateChanged(QMediaPlayer::State state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void setPosition(int position);
    void handleError();
    void setFrameRate(int);
    void keyPressEvent(QKeyEvent * e);
    void keyReleaseEvent(QKeyEvent * e);
    void endSyllabe();
    void startSyllabe();
    void prevSyllabe();
    void nextSyllabe();
    void nextPhrase();
    void prevPhrase();
    void genereASS(QString);
    void dropEvent(QDropEvent * e);
    void dragEnterEvent(QDragEnterEvent * e);
    void loadMedia(const QString &vidFile);
    void setVolume(int vol);
    void affichelaphrase(bool);
    void genereAll(void);
    void wheelEvent(QWheelEvent * e);
    void autodecoupe(const QString &txt);
    void sauverLyrics();
    void fermerLyrics();

private:
    bool isLyr(const QString &);
    bool isTxt(const QString &);
    void openLyrOrTxt(const QString &fileEnCours);
    void mousePressEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent * e);
    const QString &changeBaseNameToLyr(QString &originalStr);


    QMediaPlayer mediaPlayer;
    QAbstractButton *playButton;
    QPushButton addLyrButton;
    QPushButton addReloadLyr;
    QPushButton convert;
    QSlider *positionSlider;
    SyllabeManager syllabes;
    QLabel *errorLabel;
    QLabel theSentence;
    int currentSyllabe;
    QSlider *rateSlider;
    QSlider *volumeSlider;
    QString lyrFile;
    QString vidFile;
    QLabel _volume;
    QLabel _playrate;
    QLabel informationVideo;
    QLabel informationLyr;
    parolestextedit paroles;
    QPushButton saveLyr;
    QPushButton fermeLyr;
    QString	fileName;
};

#endif
