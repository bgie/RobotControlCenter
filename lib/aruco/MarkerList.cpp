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
#include "MarkerList.h"
#include <QBuffer>
#include <QTextStream>
#include <math.h>

MarkerList::MarkerList()
{
}

int MarkerList::indexOf(int id) const
{
    for (int i = 0; i < size(); ++i) {
        if (at(i).id() == id) {
            return i;
        }
    }
    return -1;
}

QByteArray MarkerList::serialize() const
{
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    QTextStream out(&buffer);
    bool firstItem = true;

    for (auto it = this->cbegin(); it != this->cend(); ++it) {
        if (it->isDetectedFiltered()) {
            if (firstItem) {
                firstItem = false;
            } else {
                out << ";";
            }
            out << "id:" << it->id()
                << " x:" << static_cast<int>(qRound(it->filteredPos().x()))
                << " y:" << static_cast<int>(qRound(it->filteredPos().y()))
                << " a:" << static_cast<int>(qRound(it->filteredAngle() * 180 / M_PI));
        }
    }
    out << "\n";
    out.flush();
    return buffer.buffer();
}
