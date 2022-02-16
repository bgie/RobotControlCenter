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

class IJoystick : public QObject
{
     Q_OBJECT
public:
    explicit IJoystick(QObject* parent = nullptr);

    virtual bool isButtonPressed(uint8_t button) const = 0;
    virtual int16_t axisPosition(uint8_t axis) const = 0;

signals:
    void buttonDown(uint8_t button);
    void buttonUp(uint8_t button);
    void buttonChanged(uint8_t button, bool pressed);
    void axisChanged(uint8_t axis, int16_t position);
};
