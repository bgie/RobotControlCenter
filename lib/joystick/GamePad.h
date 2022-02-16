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

class IJoystick;

// A classic 8bit era gamepad, with 2 axes and 8 buttons
class GamePad : public QObject {
    Q_OBJECT
    Q_PROPERTY(int xAxis READ xAxis NOTIFY xAxisChanged)
    Q_PROPERTY(int yAxis READ yAxis NOTIFY yAxisChanged)
    Q_PROPERTY(bool aButton READ aButton NOTIFY aButtonChanged)
    Q_PROPERTY(bool bButton READ bButton NOTIFY bButtonChanged)
    Q_PROPERTY(bool xButton READ xButton NOTIFY xButtonChanged)
    Q_PROPERTY(bool yButton READ yButton NOTIFY yButtonChanged)
    Q_PROPERTY(bool lButton READ lButton NOTIFY lButtonChanged)
    Q_PROPERTY(bool rButton READ rButton NOTIFY rButtonChanged)
    Q_PROPERTY(bool selectButton READ selectButton NOTIFY selectButtonChanged)
    Q_PROPERTY(bool startButton READ startButton NOTIFY startButtonChanged)

public:
    explicit GamePad(IJoystick* joystick = nullptr);

    IJoystick* joystick() const;

    int xAxis() const;
    int yAxis() const;
    bool aButton() const;
    bool bButton() const;
    bool xButton() const;
    bool yButton() const;
    bool lButton() const;
    bool rButton() const;
    bool selectButton() const;
    bool startButton() const;

signals:
    void xAxisChanged(int position);
    void yAxisChanged(int position);
    void aButtonChanged(bool pressed);
    void bButtonChanged(bool pressed);
    void xButtonChanged(bool pressed);
    void yButtonChanged(bool pressed);
    void lButtonChanged(bool pressed);
    void rButtonChanged(bool pressed);
    void selectButtonChanged(bool pressed);
    void startButtonChanged(bool pressed);

private:
    void onButtonChanged(uint8_t button, bool pressed);
    void onAxisChanged(uint8_t axis, int16_t position);

private:
    IJoystick* const _joystick;
};
