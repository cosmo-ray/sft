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
    , saveLyr("Sauver")
    , fermeLyr("Cacher")
{
    QVideoWidget *videoWidget = new QVideoWidget;

    QAbstractButton *openButton = new QPushButton(tr("Open Video"));

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

    connect(&saveLyr, SIGNAL(clicked()),this,SLOT(sauverLyrics()));
    connect(&fermeLyr, SIGNAL(clicked()),this,SLOT(fermerLyrics()));


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

    QBoxLayout *information1 = new QHBoxLayout;
    information1->setMargin(0);
    information1->addWidget(&informationVideo);
    information1->addWidget(&informationLyr);

    informationVideo.setText("<font color=\"red\">No Video</font>");
    informationVideo.setFont(QFont("Arial",12,QFont::Bold));
    informationLyr.setText("<font  color=\"red\">No Lyrics</font>");
    informationLyr.setFont(QFont("Arial",12,QFont::Bold));
    theSentence.setFont(QFont("Arial",12));

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
    controlLayout->addWidget(playButton);
    controlLayout->addWidget(positionSlider);

    QBoxLayout *controlLayout2 = new QHBoxLayout;
    controlLayout2->setMargin(0);
    controlLayout2->addWidget(openButton);
    controlLayout2->addWidget(&addLyrButton);
    controlLayout2->addWidget(&convert);
    connect(&convert, SIGNAL(clicked()), this, SLOT(genereAll()));

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
    layout->addLayout(information1);
    layout->addLayout(test);
    layout->addLayout(controlLayout);
    layout->addLayout(controlLayout2);
    layout->addWidget(&theSentence);
    layout->addWidget(errorLabel);
    QBoxLayout * parolesbox = new QVBoxLayout;
    QBoxLayout * parolesboutonbox = new QHBoxLayout;
    parolesbox->addWidget(&paroles);
    parolesboutonbox->addWidget(&saveLyr);
    parolesboutonbox->addWidget(&fermeLyr);
    parolesbox->addLayout(parolesboutonbox);
    layout->addLayout(parolesbox);
    paroles.setVisible(false);
    saveLyr.setVisible(false);
    fermeLyr.setVisible(false);
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
    openLyrOrTxt(fileName);
}

void VideoPlayer::openLyrOrTxt(const QString &fileName)
{
  if (isLyr(fileName)) {
    lyrFile = fileName;
    syllabes.open(lyrFile);
    QFileInfo fi(lyrFile);
    informationLyr.setText(fi.fileName());
    affichelaphrase(false);
  } else if (isTxt(fileName)) {
    autodecoupe(fileName);
  }
}

void VideoPlayer::genereAll()
{
  syllabes.saveFrmToFile(lyrFile);
  genereASS(lyrFile);  
}

void VideoPlayer::openFile()
{
    errorLabel->setText("");

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());

    if (!fileName.isEmpty()) {
        vidFile=fileName;
        loadMedia(fileName);
        QFileInfo fi(vidFile);
        informationVideo.setText(fi.completeBaseName());
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

void VideoPlayer::wheelEvent(QWheelEvent * e) {
    if(e->orientation() == Qt::Vertical) {
        int numDegrees = e->delta() / 8;
             int numSteps = numDegrees / 15;
             if (numSteps==1) {
                 nextSyllabe();
             } else if (numSteps==-1) {
                 prevSyllabe();
             }
    }
}

void VideoPlayer::mousePressEvent(QMouseEvent * e)
{
    if (e->button()==Qt::LeftButton) {
    startSyllabe();
    qDebug() << "not release" << mediaPlayer.position();
    } else if (e->button()==Qt::RightButton) {
         mediaPlayer.setPosition(mediaPlayer.position()-2000);
    }
}

void VideoPlayer::mouseReleaseEvent(QMouseEvent * e)
{
if (e->button()==Qt::LeftButton) {
    qDebug() << "release" << mediaPlayer.position();
    endSyllabe();
}
}

void VideoPlayer::keyPressEvent(QKeyEvent * e)
{
    if (e->key()==Qt::Key_Backspace) {
        mediaPlayer.setPosition(mediaPlayer.position()-2000);
    }
    if (e->key()==Qt::Key_T && !e->isAutoRepeat()) {
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
      genereAll();
    }
    if (e->key()==Qt::Key_Space) {
        play();
    }
    if (e->key()==Qt::Key_U) {
        mediaPlayer.setPosition(mediaPlayer.position()+2000);
    }
    if (e->key()==Qt::Key_H && e->modifiers()==Qt::ControlModifier) {
        fermeLyr.setVisible(!fermeLyr.isVisible());
        saveLyr.setVisible(!saveLyr.isVisible());
        paroles.setVisible(!paroles.isVisible());
    }
}

void VideoPlayer::keyReleaseEvent(QKeyEvent * e)
{
    if (e->key()==Qt::Key_T && !e->isAutoRepeat()) {
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
            affichelaphrase(false);
}

void VideoPlayer::startSyllabe() {
    if (!syllabes.isOpen())
        return ;
            affichelaphrase(true);
    syllabes.manager()[currentSyllabe].setStart(mediaPlayer.position());
}

void VideoPlayer::prevSyllabe()
{
    --currentSyllabe;
    if (currentSyllabe<0)
        currentSyllabe=0;
            affichelaphrase(false);
}

void VideoPlayer::nextSyllabe()
{
    ++currentSyllabe;
    if (currentSyllabe>=syllabes.manager().size())
        --currentSyllabe;
            affichelaphrase(false);
}

void VideoPlayer::nextPhrase()
{
    int i = syllabes.manager()[currentSyllabe].getSentence();
    qDebug() << i;
    if (i<syllabes.sentenceManager().size()-1) { currentSyllabe = syllabes.sentenceManager()[i+1].getFirst(); }
            affichelaphrase(false);
}

void VideoPlayer::prevPhrase()
{
    int i =syllabes.sentenceManager()[syllabes.manager()[currentSyllabe].getSentence()-1].getFirst();
    if (syllabes.manager()[currentSyllabe].getSentence()>0) { currentSyllabe = i; }
            affichelaphrase(false);
}

void VideoPlayer::genereASS(QString fileLyr)
{
  QProcess *p = new QProcess();
  QStringList args;
  args << fileLyr;
  args << vidFile;
  args << QString::number(1000.0, 'f', 6);
  qDebug() << args;
  p->startDetached("./toy2ass",args);
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
	openLyrOrTxt(fileEnCours);
        if (fileEnCours.endsWith(".mp4",Qt::CaseInsensitive) || fileEnCours.endsWith(".avi",Qt::CaseInsensitive)
                || fileEnCours.endsWith(".mkv",Qt::CaseInsensitive) || fileEnCours.endsWith(".flv",Qt::CaseInsensitive)
                || fileEnCours.endsWith(".ogg",Qt::CaseInsensitive)) {
            vidFile = fileEnCours;
            loadMedia(vidFile);
            QFileInfo fi(vidFile);
            informationVideo.setText(fi.fileName());
        }
    }
}

bool VideoPlayer::isLyr(const QString &s)
{
  return (s.endsWith(".lyr",Qt::CaseInsensitive));
}

bool VideoPlayer::isTxt(const QString &s)
{
  return (s.endsWith(".txt",Qt::CaseInsensitive));
}


void VideoPlayer::dragEnterEvent(QDragEnterEvent * e)
{
    e->accept();
 }

void VideoPlayer::affichelaphrase(bool b) {
    theSentence.setText(
                syllabes.sentenceManager()[syllabes.manager()[currentSyllabe].getSentence()].toString(
                    syllabes.manager()[currentSyllabe].getRelativePosition()
                    , b)
    );
}

void VideoPlayer::autodecoupe(const QString &txt) {
    fermeLyr.setVisible(true);
    saveLyr.setVisible(true);
    QFileInfo fi(txt);
    QProcess *p = new QProcess();
    QStringList args;
    QByteArray ba;
    args << txt;
    p->start("./txt2lyr",args);
    p->waitForFinished();
    ba = p->readAllStandardOutput();
    //lyrFile = fi.completeBaseName()+".lyr";
     //   QFileInfo fi2(lyrFile);
   //     qDebug() << fi2.exists();
     //     syllabes.open(lyrFile);

    //qDebug() << "ficher lyr converti" << ba;

   // informationLyr.setText(fi2.fileName());
   // affichelaphrase(false);
        paroles.setText(QString(ba));
        paroles.setVisible(true);
}

void VideoPlayer::sauverLyrics() {
  if (!lyrFile.endsWith(".lyr")) {
    lyrFile = QFileDialog::getSaveFileName(this, tr("Save Lyrics"),
					   QDir::homePath(),
					   tr("Playlist (*.lyr)"));
  }
  QFile file(lyrFile);
  file.open(QIODevice::WriteOnly | QIODevice::Text);
  QTextStream out(&file);
  out << paroles.toPlainText();
  //    for(int i = 0; i < _manager.size(); ++i) {
  //        out << _manager[i].getStart() << " " << _manager[i].getEnd() << "\n";
  //    }
  file.close();
  syllabes.open(lyrFile);
  QFileInfo fi(lyrFile);
  informationLyr.setText(fi.fileName());
  affichelaphrase(false);
  fermerLyrics();
}

void VideoPlayer::fermerLyrics() {
    fermeLyr.setVisible(false);
    saveLyr.setVisible(false);
    paroles.setVisible(false);
}
