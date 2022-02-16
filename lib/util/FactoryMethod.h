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
#include <functional>

// Register a factory method to QML from a lambda
// allows dependency injection for types created from QML.
class FactoryMethod : public QObject
{
     Q_OBJECT
public:
    explicit FactoryMethod(std::function<QObject*()> factory, QObject* parent = nullptr);

    Q_INVOKABLE QObject* create() const;

private:
    std::function<QObject*()> _factory;
};
