/*  RobotControlCenter
    Copyright (C) 2021-2022 Kuppens Brecht

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "ReplayThread.h"
#include <QDebug>
#include <QDir>
#include <QElapsedTimer>
#include <QtConcurrent/QtConcurrentRun>
#include <math.h>

const QStringList filter = QStringList() << (QStringLiteral("????????.JPG"));

ReplayThread::ReplayThread(QString path, QObject* parent)
    : QThread(parent)
    , _path(path)
    , _stopReading(false)
    , _framesPerSecond(30)
{
    this->start(); //QThread::TimeCriticalPriority);
}

ReplayThread::~ReplayThread()
{
    _stopReading = true;
    this->wait();
}

void ReplayThread::setFramesPerSecond(float framesPerSecond)
{
    _framesPerSecond = framesPerSecond;
}

void ReplayThread::run()
{
    qDebug() << "ReplayThread run started";

    QDir path(_path);
    auto filenames = path.entryList(filter, QDir::Files | QDir::Readable, QDir::Name);
    if (filenames.empty()) {
        qDebug() << "ReplayThread: no frames";
        return;
    }
    QList<QFuture<QImage>> images;
    int prefetch = qMin(4, filenames.count());
    for (int i = 0; i < prefetch; ++i) {
        QString filename = path.filePath(filenames.at(i));
        images << QtConcurrent::run([filename]() -> QImage {
            return QImage(filename, "jpg").convertToFormat(QImage::Format_RGB888);
        });
    }

    QElapsedTimer timer;
    timer.start();
    int sentFrames = 0;
    int frameIndex = 0;
    float framesPerSecond = _framesPerSecond;

    while (!_stopReading) {
        int shouldHaveSent = static_cast<int>(floor(timer.elapsed() * framesPerSecond / 1000));
        if (shouldHaveSent > sentFrames) {
            emit frameRead(images.at(frameIndex).result());

            if (prefetch < filenames.count()) {
                QString filename = path.filePath(filenames.at(prefetch));
                images << QtConcurrent::run([filename]() -> QImage {
                    return QImage(filename, "jpg").convertToFormat(QImage::Format_RGB888);
                });
                prefetch++;
            }

            frameIndex = (frameIndex + 1) % filenames.size();
            sentFrames++;
        } else {
            this->yieldCurrentThread();
        }

        if (framesPerSecond != _framesPerSecond) {
            framesPerSecond = _framesPerSecond;
            timer.restart();
            sentFrames = 0;
        }
    }

    qDebug() << "ReplayThread run finished";
}
