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

namespace cv {
class Mat;
}

class CalibrationController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString calibrationFile READ calibrationFile WRITE setCalibrationFile NOTIFY calibrationFileChanged)
    Q_PROPERTY(bool loaded READ loaded NOTIFY calibrationFileChanged)

public:
    explicit CalibrationController(QObject* parent = nullptr);
    virtual ~CalibrationController();

    QString calibrationFile() const;
    bool loaded() const;

public slots:
    void setCalibrationFile(QString calibrationFile);

signals:
    void calibrationFileChanged(QString calibrationFile);
    void calibrationChanged(const cv::Mat& cameraMatrix, const cv::Mat& distCoefs);

private:
    void load();

private:
    struct Data;
    QScopedPointer<Data> _d;
};
