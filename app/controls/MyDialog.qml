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
    default property alias content: insideDialog.data
    property alias dialogWidth: dialog.width
    property alias dialogHeight: dialog.height

    signal closeClicked

    id: dialogShadow
    color: "#B0000000"

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.AllButtons
    }

    Rectangle {
        id: dialog
        anchors.centerIn: parent

        width: 200
        height: 200

        radius: 4
        color: Style.darkGray

        MyLabel {
            id: titleLabel
            anchors.top: parent.top
            anchors.margins: 10
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: Style.headerFontSize
        }

        Rectangle {
            id: insideDialog

            anchors.fill: parent
            anchors.margins: 8
            anchors.topMargin: 28 + Style.headerFontSize
            color: Style.darkerGray
        }

        MyToolButton {
            id: closeButton
            width: 40
            height: 40
            anchors.margins: 8
            anchors.right: parent.right
            anchors.top: parent.top
            sourceLight: "/img/x-white.png"
            sourceDark: "/img/x-red.png"
            imageMargins: 4
            backgroundColor: "#600000"
            radius: 20
            onClicked: closeClicked()
        }
    }
}
