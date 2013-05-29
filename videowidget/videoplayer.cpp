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
    rateSlider->setValue(20);

    connect(rateSlider, SIGNAL(sliderMoved(int)),this, SLOT(setFrameRate(int)));

    volumeSlider = new QSlider(Qt::Vertical);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(100);

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
    test->addWidget(volumeSlider);
    test->addWidget(rateSlider);
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
        mediaPlayer.setMedia(QUrl::fromLocalFile(fileName));
        playButton->setEnabled(true);
    }
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
}

void VideoPlayer::setVolume(int vol)
{
    mediaPlayer.setVolume(vol);
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
    if (e->key()==Qt::Key_P) {
        startSyllabe();
    }
    if (e->key()==Qt::Key_O) {
        prevSyllabe();
    }
    if (e->key()==Qt::Key_I) {
        syllabes.saveFrmToFile(lyrFile);
	genereASS(lyrFile);
    }

}

void VideoPlayer::keyReleaseEvent(QKeyEvent * e)
{
    if (e->key()==Qt::Key_P) {
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

void VideoPlayer::genereASS(QString fileLyr)
{
  QProcess *p = new QProcess();
  QStringList args;
  args << fileLyr;
  args << QString::number(1000.0, 'f', 7);
  qDebug() << args;;
  p->execute("./toy2assConverter.ml",args);
}
