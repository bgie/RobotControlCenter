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
#include "GamePad.h"
#include "IJoystick.h"

// hardcoded list of indices, should be dynamic if other joysticks/gamepads are to be supported
#define X_AXIS_INDEX 0
#define Y_AXIS_INDEX 1
#define A_BUTTON_INDEX 0
#define B_BUTTON_INDEX 1
#define X_BUTTON_INDEX 2
#define Y_BUTTON_INDEX 3
#define L_BUTTON_INDEX 4
#define R_BUTTON_INDEX 5
#define SELECT_BUTTON_INDEX 6
#define START_BUTTON_INDEX 7

namespace {
QString axisToString(int position)
{
    if (position > 0) {
        return QStringLiteral("+1");
    } else if (position < 0) {
        return QStringLiteral("-1");
    }
    return QStringLiteral("0");
}
}

GamePad::GamePad(IJoystick* joystick)
    : QObject(joystick)
    , _joystick(joystick)
{
    connect(_joystick, &IJoystick::axisChanged, this, &GamePad::onAxisChanged);
    connect(_joystick, &IJoystick::buttonChanged, this, &GamePad::onButtonChanged);
}

IJoystick* GamePad::joystick() const
{
    return _joystick;
}

int GamePad::xAxis() const
{
    return _joystick ? _joystick->axisPosition(X_AXIS_INDEX) : 0;
}

int GamePad::yAxis() const
{
    return _joystick ? _joystick->axisPosition(Y_AXIS_INDEX) : 0;
}

bool GamePad::aButton() const
{
    return _joystick ? _joystick->isButtonPressed(A_BUTTON_INDEX) : 0;
}

bool GamePad::bButton() const
{
    return _joystick ? _joystick->isButtonPressed(B_BUTTON_INDEX) : 0;
}

bool GamePad::xButton() const
{
    return _joystick ? _joystick->isButtonPressed(X_BUTTON_INDEX) : 0;
}

bool GamePad::yButton() const
{
    return _joystick ? _joystick->isButtonPressed(Y_BUTTON_INDEX) : 0;
}

bool GamePad::lButton() const
{
    return _joystick ? _joystick->isButtonPressed(L_BUTTON_INDEX) : 0;
}

bool GamePad::rButton() const
{
    return _joystick ? _joystick->isButtonPressed(R_BUTTON_INDEX) : 0;
}

bool GamePad::selectButton() const
{
    return _joystick ? _joystick->isButtonPressed(SELECT_BUTTON_INDEX) : 0;
}

bool GamePad::startButton() const
{
    return _joystick ? _joystick->isButtonPressed(START_BUTTON_INDEX) : 0;
}

QString GamePad::debugString() const
{
    QStringList pressedButtons;
    if (aButton())
        pressedButtons << QStringLiteral("A");
    if (bButton())
        pressedButtons << QStringLiteral("B");
    if (xButton())
        pressedButtons << QStringLiteral("X");
    if (yButton())
        pressedButtons << QStringLiteral("Y");
    if (lButton())
        pressedButtons << QStringLiteral("L");
    if (rButton())
        pressedButtons << QStringLiteral("R");
    if (startButton())
        pressedButtons << QStringLiteral("START");
    if (selectButton())
        pressedButtons << QStringLiteral("SELECT");
    if (pressedButtons.empty())
        pressedButtons << QStringLiteral("-");
    return QStringLiteral("X: %1 Y: %2 Buttons: %3").arg(axisToString(xAxis()), axisToString(yAxis()), pressedButtons.join(QStringLiteral(" ")));
}

void GamePad::onButtonChanged(uint8_t button, bool pressed)
{
    switch (button) {
    case A_BUTTON_INDEX:
        emit aButtonChanged(pressed);
        break;
    case B_BUTTON_INDEX:
        emit bButtonChanged(pressed);
        break;
    case X_BUTTON_INDEX:
        emit xButtonChanged(pressed);
        break;
    case Y_BUTTON_INDEX:
        emit yButtonChanged(pressed);
        break;
    case L_BUTTON_INDEX:
        emit lButtonChanged(pressed);
        break;
    case R_BUTTON_INDEX:
        emit rButtonChanged(pressed);
        break;
    case SELECT_BUTTON_INDEX:
        emit selectButtonChanged(pressed);
        break;
    case START_BUTTON_INDEX:
        emit startButtonChanged(pressed);
        break;
    }
    emit debugStringChanged();
}

void GamePad::onAxisChanged(uint8_t axis, int16_t position)
{
    switch (axis) {
    case X_AXIS_INDEX:
        emit xAxisChanged(position);
        break;
    case Y_AXIS_INDEX:
        emit yAxisChanged(position);
        break;
    }
    emit debugStringChanged();
}
