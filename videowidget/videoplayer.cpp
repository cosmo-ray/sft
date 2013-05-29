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

#include "videoplayer.h"

#include <QDebug>
#include <QtWidgets>
#include <qvideowidget.h>
#include <qvideosurfaceformat.h>

VideoPlayer::VideoPlayer(QWidget *parent)
    : QWidget(parent)
    , mediaPlayer(0, QMediaPlayer::VideoSurface)
    , playButton(0)
    , addLyrButton("Open lyrics")
    , convert("convert")
    , positionSlider(0)
    , errorLabel(0)
    , theSentence("open a lyr file and a video")
    , currentSyllabe(0)
{
    QVideoWidget *videoWidget = new QVideoWidget;

    QAbstractButton *openButton = new QPushButton(tr("Open..."));

    setWindowTitle("Sailor Fuku Timer");
    setWindowIcon(QIcon("resources/sfhd.jpg"));
    setAcceptDrops(true);

    connect(openButton, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(&addLyrButton, SIGNAL(clicked()), this, SLOT(openLyr()));

    playButton = new QPushButton;
    playButton->setEnabled(false);
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(playButton, SIGNAL(clicked()),
            this, SLOT(play()));

    positionSlider = new QSlider(Qt::Horizontal);
    positionSlider->setRange(0, 0);

    connect(positionSlider, SIGNAL(sliderMoved(int)),
            this, SLOT(setPosition(int)));

    rateSlider = new QSlider(Qt::Vertical);
    rateSlider->setRange(0,40);
    setFrameRate(20);
    rateSlider->setValue(20);

    connect(rateSlider, SIGNAL(sliderMoved(int)),this, SLOT(setFrameRate(int)));

    volumeSlider = new QSlider(Qt::Vertical);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(100);
   setVolume(100);

    connect(volumeSlider, SIGNAL(sliderMoved(int)),this, SLOT(setVolume(int)));




    errorLabel = new QLabel;
    errorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
    controlLayout->addWidget(playButton);
    controlLayout->addWidget(positionSlider);

    QBoxLayout *controlLayout2 = new QHBoxLayout;
    controlLayout2->setMargin(0);
    controlLayout2->addWidget(openButton);
    controlLayout2->addWidget(&addLyrButton);
    controlLayout2->addWidget(&convert);

    QBoxLayout *layout = new QVBoxLayout;
    QBoxLayout *test = new QHBoxLayout;
    test->addWidget(videoWidget);
    QBoxLayout *volbox = new QVBoxLayout;
    volbox->addWidget(&_volume);
    volbox->addWidget(volumeSlider);
    test->addLayout(volbox);
    QBoxLayout *ratebox = new QVBoxLayout;
    ratebox->addWidget(&_playrate);
    ratebox->addWidget(rateSlider);
    test->addLayout(ratebox);
    layout->addLayout(test);
    layout->addLayout(controlLayout);
    layout->addLayout(controlLayout2);
    layout->addWidget(&theSentence);
    layout->addWidget(errorLabel);

    setLayout(layout);

    mediaPlayer.setVideoOutput(videoWidget);
    connect(&mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(mediaStateChanged(QMediaPlayer::State)));
    connect(&mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(&mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
    connect(&mediaPlayer, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(handleError()));
}

VideoPlayer::~VideoPlayer()
{
    delete positionSlider;
    delete errorLabel;
    delete rateSlider;
}

void VideoPlayer::openLyr()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());
    if (!fileName.isEmpty()) {
        syllabes.open(fileName);
         lyrFile = fileName;
    }
}

void VideoPlayer::openFile()
{
    errorLabel->setText("");

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());

    if (!fileName.isEmpty()) {
        vidFile=fileName;
        loadMedia(fileName);
    }
}

void VideoPlayer::loadMedia(const QString &vidFile)
{
    mediaPlayer.setMedia(QUrl::fromLocalFile(vidFile));
    playButton->setEnabled(true);
}


void VideoPlayer::play()
{
    switch(mediaPlayer.state()) {
    case QMediaPlayer::PlayingState:
        mediaPlayer.pause();
        break;
    default:
        mediaPlayer.play();
        break;
    }
}

void VideoPlayer::mediaStateChanged(QMediaPlayer::State state)
{
    switch(state) {
    case QMediaPlayer::PlayingState:
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    default:
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }
}

void VideoPlayer::positionChanged(qint64 position)
{
    positionSlider->setValue(position);
}

void VideoPlayer::durationChanged(qint64 duration)
{
    positionSlider->setRange(0, duration);
}

void VideoPlayer::setPosition(int position)
{
    mediaPlayer.setPosition(position);
}

void VideoPlayer::setFrameRate(int fr)
{
    mediaPlayer.setPlaybackRate(0.05*fr);
    _playrate.setText(QString::number(fr*0.05));
}

void VideoPlayer::setVolume(int vol)
{
    mediaPlayer.setVolume(vol);
    _volume.setText(QString::number(vol));
}

void VideoPlayer::handleError()
{
    playButton->setEnabled(false);
    errorLabel->setText("Error: " + mediaPlayer.errorString());
}

void VideoPlayer::mousePressEvent(QMouseEvent *)
{
    startSyllabe();
    qDebug() << "not release" << mediaPlayer.position();
}

void VideoPlayer::mouseReleaseEvent(QMouseEvent *)
{

    qDebug() << "release" << mediaPlayer.position();
    endSyllabe();
}

void VideoPlayer::keyPressEvent(QKeyEvent * e)
{
    if (e->key()==Qt::Key_Backspace) {
        mediaPlayer.setPosition(mediaPlayer.position()-2000);
    }
    if (e->key()==Qt::Key_T) {
        startSyllabe();
    }
    if (e->key()==Qt::Key_O) {
        prevSyllabe();
    }
    if (e->key()==Qt::Key_P) {
        nextSyllabe();
    }
    if (e->key()==Qt::Key_M) {
        nextPhrase();
    }
    if (e->key()==Qt::Key_L) {
        prevPhrase();
    }
    if (e->key()==Qt::Key_I) {
        syllabes.saveFrmToFile(lyrFile);
	genereASS(lyrFile);
    }

}

void VideoPlayer::keyReleaseEvent(QKeyEvent * e)
{
    if (e->key()==Qt::Key_T) {
        endSyllabe();
    }

}

void VideoPlayer::endSyllabe()
{
    if (!syllabes.isOpen())
        return ;
    syllabes.manager()[currentSyllabe].setEnd(mediaPlayer.position());
    ++currentSyllabe;
    if (currentSyllabe>=syllabes.manager().size())
        --currentSyllabe;
    theSentence.setText(
                syllabes.sentenceManager()[syllabes.manager()[currentSyllabe].getSentence()].toString(
                    syllabes.manager()[currentSyllabe].getRelativePosition()
                    , false)
    );
}

void VideoPlayer::startSyllabe() {
    if (!syllabes.isOpen())
        return ;
    theSentence.setText(
                syllabes.sentenceManager()[syllabes.manager()[currentSyllabe].getSentence()].toString(
                    syllabes.manager()[currentSyllabe].getRelativePosition()
                    , true)
    );
    syllabes.manager()[currentSyllabe].setStart(mediaPlayer.position());
}

void VideoPlayer::prevSyllabe()
{
    --currentSyllabe;
    if (currentSyllabe<0)
        currentSyllabe=0;
    theSentence.setText(
                syllabes.sentenceManager()[syllabes.manager()[currentSyllabe].getSentence()].toString(
                    syllabes.manager()[currentSyllabe].getRelativePosition()
                    , false)
    );
}

void VideoPlayer::nextSyllabe()
{
    ++currentSyllabe;
    if (currentSyllabe>=syllabes.manager().size())
        --currentSyllabe;
    theSentence.setText(
                syllabes.sentenceManager()[syllabes.manager()[currentSyllabe].getSentence()].toString(
                    syllabes.manager()[currentSyllabe].getRelativePosition()
                    , false)
    );
}

void VideoPlayer::nextPhrase()
{
    int i = syllabes.manager()[currentSyllabe].getSentence();
    qDebug() << i;
    if (i<syllabes.sentenceManager().size()-1) { currentSyllabe = syllabes.sentenceManager()[i+1].getFirst(); }
    theSentence.setText(
                syllabes.sentenceManager()[syllabes.manager()[currentSyllabe].getSentence()].toString(
                    syllabes.manager()[currentSyllabe].getRelativePosition()
                    , false)
    );
}

void VideoPlayer::prevPhrase()
{
    int i =syllabes.sentenceManager()[syllabes.manager()[currentSyllabe].getSentence()-1].getFirst();
    if (i>0) { currentSyllabe = i; }
    theSentence.setText(
                syllabes.sentenceManager()[syllabes.manager()[currentSyllabe].getSentence()].toString(
                    syllabes.manager()[currentSyllabe].getRelativePosition()
                    , false)
    );
}


void VideoPlayer::genereASS(QString fileLyr)
{
  QProcess *p = new QProcess();
  QStringList args;
  args << fileLyr;
  args << QString::number(1000.0, 'f', 7);
  qDebug() << args;;
  p->execute("./toy2assConverter.ml",args);
}

void VideoPlayer::dropEvent(QDropEvent * event)
{
    const QMimeData* mimeData = event->mimeData();

    // check for our needed mime type, here a file or a list of files
    if (mimeData->hasUrls())
    {
        QStringList pathList;
        QList<QUrl> urlList = mimeData->urls();

        // extract the local paths of the files
        for (int i = 0; i < urlList.size() && i < 32; ++i)
        {
            pathList.append(urlList.at(i).toLocalFile());
        }

        // call a function to open the files
        //openFiles(pathList);
        qDebug()<<pathList;
        QString fileEnCours = pathList[0];
        if (fileEnCours.endsWith(".lyr",Qt::CaseInsensitive)) {
            lyrFile = fileEnCours;
            syllabes.open(lyrFile);
            qDebug() << "ficher lyr recu";
        }
        if (fileEnCours.endsWith(".mp4",Qt::CaseInsensitive) || fileEnCours.endsWith(".avi",Qt::CaseInsensitive)
                || fileEnCours.endsWith(".mkv",Qt::CaseInsensitive) || fileEnCours.endsWith(".flv",Qt::CaseInsensitive)
                || fileEnCours.endsWith(".ogg",Qt::CaseInsensitive)) {
            vidFile = fileEnCours;
            loadMedia(vidFile);
        }
    }
}

void VideoPlayer::dragEnterEvent(QDragEnterEvent * e)
{
    e->accept();
 }
