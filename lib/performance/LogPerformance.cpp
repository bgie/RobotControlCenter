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
#include "LogPerformance.h"
#include "PerformanceMonitor.h"
#include <QElapsedTimer>
#include <QHash>
#include <QPair>

namespace {
static PerformanceMonitor* monitor = nullptr;
static QHash<int, QPair<QElapsedTimer, qint64>> timer;

void logTimeEntry(int time, int delta, QString description, int frameIndex)
{
    if (monitor) {
        monitor->log(QStringLiteral("#%4 %1ms (%2ms) - %3")
                         .arg(time, 4)
                         .arg(delta, 3)
                         .arg(description)
                         .arg(frameIndex, 4, 10, QLatin1Char('0')));
    }
}
}

void logRelativeTime(int frameIndex, QString description)
{
    if (!timer.contains(frameIndex)) {
        timer[frameIndex] = QPair<QElapsedTimer, qint64>(QElapsedTimer(), 0);
        timer[frameIndex].first.start();
    }
    qint64 elapsed = timer[frameIndex].first.elapsed();
    logTimeEntry(static_cast<int>(elapsed), static_cast<int>(elapsed - timer[frameIndex].second), description, frameIndex);
    timer[frameIndex].second = elapsed;
}

GlobalPerformanceMonitor::GlobalPerformanceMonitor(PerformanceMonitor* instance)
{
    monitor = instance;
}

GlobalPerformanceMonitor::~GlobalPerformanceMonitor()
{
    monitor = nullptr;
}

void logFrameComplete(int frameIndex)
{
    timer.remove(frameIndex);
}
