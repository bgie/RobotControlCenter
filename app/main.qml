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
import QtQml 2.3
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0
import RobotControlCenter 1.0
import './controls/'

Window {
    id: window
    width: 800
    height: 600
    visible: true
    visibility: Window.FullScreen
    title: qsTr("Robot Control Center")
    color: Style.darkerGray

    Item {
        id: content
        anchors.fill: parent

        MyLabel {
            anchors.centerIn: parent
            text: "Under construction"
        }

        Row {
            id: statusIcons
            anchors.right: closeButton.left
            anchors.top: parent.top
            anchors.topMargin: Style.windowMargins
            anchors.rightMargin: Style.largeMargin

            MyStatusIcon {
                source: "/img/gamepad-white.png"
                subText: gamePadManager.count
                showSubIcon: gamePadManager.count === 0
                highlighted: statusIconsMouseArea.containsMouse
            }
            MyStatusIcon {
                source: "/img/camera-white.png"
                showSubIcon: true
                subIconSource: cameraController.connectPossible ? "/img/checkmark-white.png" : "/img/error-red.png"
                highlighted: statusIconsMouseArea.containsMouse
            }
            MyStatusIcon {
                source: "/img/antenna-white.png"
                showSubIcon: true
                subIconSource: robotNetwork.connected ? "/img/checkmark-white.png" : "/img/error-red.png"
                highlighted: statusIconsMouseArea.containsMouse
            }
            MyStatusIcon {
                source: "/img/tank-white.png"
                subText: robotNetwork.count
                showSubIcon: robotNetwork.count === 0
                highlighted: statusIconsMouseArea.containsMouse
            }
        }

        MouseArea {
            id: statusIconsMouseArea
            anchors.fill: statusIcons
            hoverEnabled: true
            onClicked: settingsDialog.active = true
        }

        Item {
            id: closeButton
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: Style.windowMargins

            width: Style.toolButtonSize
            height: Style.toolButtonSize

            MyToolButton {
                anchors.centerIn: parent
                width: 48
                height: 48
                sourceLight: "img/x-white.png"
                sourceDark: "img/x-red.png"
                imageMargins: 4
                backgroundColor: "#600000"
                radius: 32
                onClicked: window.close()
            }
        }
    }

    Loader {
        id: settingsDialog
        anchors.fill: parent
        source: "SettingsDialog.qml"
        active: false

        Connections {
            target: settingsDialog.item
            onExitClicked: settingsDialog.active = false
        }
    }
}
