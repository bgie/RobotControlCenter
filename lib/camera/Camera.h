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
#include <QImage>
#include <QObject>
#include <QScopedPointer>

class Camera : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString deviceName READ deviceName CONSTANT)
    Q_PROPERTY(QStringList videoFormats READ videoFormats NOTIFY videoFormatsChanged)
    Q_PROPERTY(int videoFormatIndex READ videoFormatIndex WRITE setVideoFormatIndex NOTIFY videoFormatIndexChanged)
    Q_PROPERTY(float framesPerSecond READ framesPerSecond NOTIFY framesPerSecondChanged)
    Q_PROPERTY(int exposure READ exposure WRITE setExposure NOTIFY exposureChanged)
    Q_PROPERTY(int gain READ gain WRITE setGain NOTIFY gainChanged)
    Q_PROPERTY(bool canStream READ canStream NOTIFY canStreamChanged)
    Q_PROPERTY(bool isStreaming READ isStreaming NOTIFY isStreamingChanged)

public:
    explicit Camera(QString deviceName, QObject *parent = nullptr);
    virtual ~Camera();

    QString deviceName() const;

    QStringList videoFormats() const;
    int videoFormatIndex() const;
    void setVideoFormatIndex(int i);
    float framesPerSecond() const;

    int exposure() const;
    void setExposure(int val);
    int gain() const;
    void setGain(int val);

    bool canStream() const;
    void startStream();
    void stopStream();
    bool isStreaming() const;

signals:
    void videoFormatsChanged(QStringList list);
    void videoFormatIndexChanged(int index);
    void framesPerSecondChanged(float value);
    void exposureChanged(int val);
    void gainChanged(int val);
    void canStreamChanged(bool val);
    void isStreamingChanged(bool val);

    void frameRead(const QImage image);

private:
    void updateExposure();
    void updateGain();
    void setFramesPerSecond(float newValue);

private:
    struct Data;
    QScopedPointer<Data> _d;
};
