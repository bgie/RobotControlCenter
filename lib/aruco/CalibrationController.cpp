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
#include "CalibrationController.h"
#include <QFile>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

struct CalibrationController::Data {
    QString calibrationFile;
    cv::Mat cameraMatrix;
    cv::Mat distCoeffs;
};

CalibrationController::CalibrationController(QObject* parent)
    : QObject(parent)
    , _d(new Data())
{
}

CalibrationController::~CalibrationController()
{
}

QString CalibrationController::calibrationFile() const
{
    return _d->calibrationFile;
}

bool CalibrationController::loaded() const
{
    return !_d->cameraMatrix.empty() && !_d->distCoeffs.empty();
}

void CalibrationController::setCalibrationFile(QString calibrationFile)
{
    if (_d->calibrationFile == calibrationFile)
        return;

    _d->calibrationFile = calibrationFile;
    load();

    emit calibrationFileChanged(_d->calibrationFile);
}

void CalibrationController::load()
{
    if (QFile::exists(_d->calibrationFile)) {
        cv::FileStorage file(_d->calibrationFile.toStdString(), cv::FileStorage::READ | cv::FileStorage::FORMAT_JSON);
        file["cameraMatrix"] >> _d->cameraMatrix;
        file["distCoeffs"] >> _d->distCoeffs;
    } else {
        _d->cameraMatrix = cv::Mat();
        _d->distCoeffs = cv::Mat();
    }
    emit calibrationChanged(_d->cameraMatrix, _d->distCoeffs);
}
