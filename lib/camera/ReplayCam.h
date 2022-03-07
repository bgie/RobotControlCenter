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
#pragma once
#include "ICamera.h"

class ReplayCam : public ICamera {
    Q_OBJECT
    Q_PROPERTY(float framesPerSecond READ framesPerSecond WRITE setFramesPerSecond NOTIFY framesPerSecondChangedNOPARAM)
    Q_PROPERTY(QString videosPath READ videosPath WRITE setVideosPath NOTIFY videosPathChanged)
    Q_PROPERTY(bool videosPathValid READ videosPathValid NOTIFY videosChanged)
    Q_PROPERTY(QStringList videos READ videos NOTIFY videosChanged)
    Q_PROPERTY(int videoIndex READ videoIndex WRITE setVideoIndex NOTIFY videoIndexChanged)

public:
    explicit ReplayCam(QObject* parent = nullptr);
    virtual ~ReplayCam();

    virtual QString className() const override;
    virtual float framesPerSecond() const override;
    virtual bool canStream() const override;
    virtual void startStream() override;
    virtual void stopStream() override;
    virtual bool isStreaming() const override;

    void setFramesPerSecond(float newValue);

    QString videosPath() const;
    void setVideosPath(QString videosPath);
    bool videosPathValid() const;

    QStringList videos() const;
    QString video() const;
    int videoIndex() const;
    void setVideo(QString newVideo);
    void setVideoIndex(int newIndex);

signals:
    void framesPerSecondChangedNOPARAM(); // because of MOC limitation
    void videosPathChanged(QString path);
    void videosChanged();
    void videoChanged(QString newVideo);
    void videoIndexChanged(int newIndex);

private:
    void updateVideos();

private:
    struct Data;
    QScopedPointer<Data> _d;
};
