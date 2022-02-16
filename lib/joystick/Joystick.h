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
#include "IJoystick.h"
#include <QObject>
#include <QScopedPointer>

struct _SDL_Joystick;
typedef struct _SDL_Joystick SDL_Joystick;

class Joystick : public IJoystick {
    Q_OBJECT
public:
    explicit Joystick(SDL_Joystick* joy, QObject* parent = nullptr);
    virtual ~Joystick();

    virtual bool isButtonPressed(uint8_t button) const override;
    virtual int16_t axisPosition(uint8_t axis) const override;

    void setButtonChanged(uint8_t button, bool pressed);
    void setAxisChanged(uint8_t axis, int16_t position);

private:
    struct Data;
    QScopedPointer<Data> _d;
};
