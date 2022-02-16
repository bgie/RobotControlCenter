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

Button {
    property alias radius: backgroundRect.radius
    property alias sourceDark: darkImage.source
    property alias sourceLight: lightImage.source
    property int imageMargins: 0
    property int imageBottomMargin
    property int buttonDownDistance: 3
    property color backgroundColor: Style.darkerGray
    property bool selected: false
    property bool foregroundInverted: control.down || control.hovered || control.selected

    id: control
    hoverEnabled: true
    implicitHeight: darkImage.sourceSize.height
    implicitWidth: darkImage.sourceSize.width
    width: Style.toolButtonSize
    height: Style.toolButtonSize

    contentItem: Item {
        id: imageContainer
        Image {
            id: darkImage
            anchors.fill: parent
            anchors.margins: imageMargins
            anchors.bottomMargin: imageBottomMargin
            fillMode: Image.PreserveAspectFit
            visible: foregroundInverted
            opacity: enabled ? 1.0 : 0.3
            transform: Translate { y: control.down ? buttonDownDistance : 0 }
        }
        Image {
            id: lightImage
            anchors.fill: parent
            anchors.margins: imageMargins
            anchors.bottomMargin: imageBottomMargin
            fillMode: Image.PreserveAspectFit
            visible: ! foregroundInverted
            opacity: enabled ? 1.0 : 0.3
            transform: Translate { y: control.down ? buttonDownDistance : 0 }
        }
    }
    background: Rectangle {
        id: backgroundRect
        radius: 0
        color: control.down ? Style.white :
               control.hovered ? Style.lightGray :
               control.selected ? Style.mediumGray : control.backgroundColor
    }
}
