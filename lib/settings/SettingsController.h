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

class Aruco;

class SettingsController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QImage arucoImage READ arucoImage NOTIFY arucoImageChanged)

public:
    explicit SettingsController(Aruco& aruco, QObject* parent = nullptr);
    virtual ~SettingsController() override;

    QImage arucoImage() const;

public slots:
    void setImage(QImage newImage);

signals:
    void arucoImageChanged(QImage arucoImage);

private:
    Aruco& _aruco;
    QImage _image;
    QImage _arucoImage;
};
