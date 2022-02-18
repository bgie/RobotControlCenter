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
#include <QObject>

class AppSettings : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString cameraDevice READ cameraDevice WRITE setCameraDevice NOTIFY cameraDeviceChanged)
    Q_PROPERTY(int exposure READ exposure WRITE setExposure NOTIFY exposureChanged)
    Q_PROPERTY(int gain READ gain WRITE setGain NOTIFY gainChanged)
    Q_PROPERTY(int videoFormatIndex READ videoFormatIndex WRITE setVideoFormatIndex NOTIFY videoFormatIndexChanged)
    Q_PROPERTY(QString calibrationFile READ calibrationFile WRITE setCalibrationFile NOTIFY calibrationFileChanged)

public:
    explicit AppSettings(QObject* parent = nullptr);

    QString cameraDevice() const;
    int exposure() const;
    int gain() const;
    int videoFormatIndex() const;
    QString calibrationFile() const;

public slots:
    void setCameraDevice(QString cameraDevice);
    void setExposure(int exposure);
    void setGain(int gain);
    void setVideoFormatIndex(int videoFormatIndex);
    void setCalibrationFile(QString calibrationFile);

signals:
    void cameraDeviceChanged(QString cameraDevice);
    void exposureChanged(int exposure);
    void gainChanged(int gain);
    void videoFormatIndexChanged(int videoFormatIndex);
    void calibrationFileChanged(QString calibrationFile);

private:
    QString _cameraDevice;
    int _exposure;
    int _gain;
    int _videoFormatIndex;
    QString _calibrationFile;
};
