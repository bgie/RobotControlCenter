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
import QtQuick.Controls 2.12
import RobotControlCenter 1.0

Rectangle {
    property alias source: image.source
    property alias subText: subTextLabel.text
    property int imageMargins: Style.smallMargin
    property bool showSubIcon: false
    property alias subIconSource: subIcon.source

    color: Style.darkerGray

    implicitHeight: image.sourceSize.height
    implicitWidth: image.sourceSize.width
    width: Style.statusIconSize
    height: Style.statusIconSize

    Item {
        anchors.fill: parent
        anchors.rightMargin: Style.mediumMargin
        anchors.bottomMargin: Style.mediumMargin

        Image {
            id: image
            anchors.fill: parent
            anchors.margins: imageMargins
            fillMode: Image.PreserveAspectFit
            opacity: enabled ? 1.0 : 0.3
        }
    }
    MyLabel {
        id: subTextLabel
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: Style.smallMargin
        visible: !showSubIcon
        text: ""
        font.pixelSize: Style.statusIconFontSize
    }
    Image {
        id: subIcon
        visible: showSubIcon
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: Style.smallMargin
        width: 24
        height: 24
        source: "/img/error-red.png"
    }
}
