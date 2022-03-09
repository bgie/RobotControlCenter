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
    width: 1400
    height: 1000
    visible: true
    title: qsTr("Robot Control Center")
    color: Style.darkerGray

    Item {
        id: mainMenu
        anchors.fill: parent
        visible: !gameScreen.active

        Column {
            anchors.centerIn: parent
            spacing: Style.largeMargin

            Row {
                spacing: Style.mediumMargin
                Image {
                    width: 64
                    height: 64
                    fillMode: Image.PreserveAspectFit
                    source: "img/under-construction-white.png"
                }
                Column {
                    MyLabel {
                        text: "Game mode"
                        font.pixelSize: Style.headerFontSize
                    }
                    MyLabel {
                        text: "[under construction]"
                        font.italic: true
                        font.pixelSize: Style.subHeaderFontSize
                    }
                }
            }

            MyIconTextButton {
                sourceLight: "img/keyboard-white.png"
                sourceDark: "img/keyboard-black.png"
                text: "Python programming mode"
                onClicked: {
                    gameScreen.source = "PythonGameScreen.qml"
                    gameScreen.active = true
                }
            }
        }
    }
    Loader {
        id: gameScreen
        anchors.fill: parent
        active: false

        Connections {
            target: gameScreen.item
            onExitClicked: gameScreen.active = false
        }
    }

    Row {
        id: statusIcons
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: Style.windowMargins
        opacity: statusIconsMouseArea.containsMouse ? 1 : 0.4

        MyStatusIcon {
            source: "/img/camera-white.png"
            showSubIcon: true
            subIconSource: cameraController.connectPossible ? "/img/checkmark-white.png" : "/img/error-red.png"
        }
        MyStatusIcon {
            source: "/img/gamepad-white.png"
            subText: gamePadManager.count
            showSubIcon: gamePadManager.count === 0
        }
        MyStatusIcon {
            source: "/img/antenna-white.png"
            showSubIcon: true
            subIconSource: robotNetwork.connected ? "/img/checkmark-white.png" : "/img/error-red.png"
        }
        MyStatusIcon {
            source: "/img/tank-white.png"
            subText: robotManager.count
            showSubIcon: robotManager.count === 0
        }
    }

    MouseArea {
        id: statusIconsMouseArea
        anchors.fill: statusIcons
        hoverEnabled: true
        onClicked: {
            if (window.width < 1400)
                window.width = 1400
            if (window.height < 1000)
                window.height = 1000
            settingsDialog.active = true
        }
    }

    MyToolButton {
        id: fullscreenButton
        anchors.right: closeButton.left
        anchors.top: closeButton.top
        anchors.rightMargin: Style.mediumMargin
        width: 64
        height: 64
        sourceLight: "img/fullscreen-white.png"
        sourceDark: "img/fullscreen-black.png"

        onClicked: {
            window.visibility = Window.FullScreen
            fullscreenButton.visible = false
        }
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
