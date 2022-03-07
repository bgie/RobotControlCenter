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
#include <QImage>
#include <QScopedPointer>

class Camera : public ICamera {
    Q_OBJECT
    Q_PROPERTY(QString deviceName READ deviceName CONSTANT)
    Q_PROPERTY(QStringList videoFormats READ videoFormats NOTIFY videoFormatsChanged)
    Q_PROPERTY(int videoFormatIndex READ videoFormatIndex WRITE setVideoFormatIndex NOTIFY videoFormatIndexChanged)
    Q_PROPERTY(int exposure READ exposure WRITE setExposure NOTIFY exposureChanged)
    Q_PROPERTY(int gain READ gain WRITE setGain NOTIFY gainChanged)

public:
    explicit Camera(QString deviceName, QObject *parent = nullptr);
    virtual ~Camera();

    virtual QString className() const override;
    virtual float framesPerSecond() const override;
    virtual bool canStream() const override;
    virtual void startStream() override;
    virtual void stopStream() override;
    virtual bool isStreaming() const override;

    QString deviceName() const;

    QStringList videoFormats() const;
    int videoFormatIndex() const;
    void setVideoFormatIndex(int i);

    int exposure() const;
    void setExposure(int val);
    int gain() const;
    void setGain(int val);

signals:
    void videoFormatsChanged(QStringList list);
    void videoFormatIndexChanged(int index);
    void exposureChanged(int val);
    void gainChanged(int val);

private:
    void updateExposure();
    void updateGain();
    void setFramesPerSecond(float newValue);

private:
    struct Data;
    QScopedPointer<Data> _d;
};
