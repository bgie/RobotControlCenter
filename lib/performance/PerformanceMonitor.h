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

// Monitors camera pipeline performance - for development and debugging
class PerformanceMonitor : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList mostRecentBatch READ mostRecentBatch)
public:
    explicit PerformanceMonitor(QObject* parent = nullptr);
    virtual ~PerformanceMonitor();

    void log(QString entry);

    QStringList mostRecentBatch();

private:
    QStringList _entries;
    QStringList _mostRecentBatch;
};
