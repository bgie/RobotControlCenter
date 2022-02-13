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
import QtQuick 2.12
import RobotControlCenter 1.0

Rectangle {
    property alias title: titleLabel.text

    radius: 4
    color: Style.darkGray

    MyLabel {
        id: titleLabel
        anchors.top: parent.top
        anchors.margins: 6
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Rectangle {
        anchors.fill: parent
        anchors.margins: 6
        anchors.topMargin: 36
        color: Style.darkerGray
    }
}
