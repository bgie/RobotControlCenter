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

MyDialog {
    width: 1024
    height: 600
    title: "Settings"

    signal exitClicked

    property SettingsController controller: settingsControllerFactory.create()

    onCloseClicked: exitClicked()

    MyNavigationBar {
        id: navigationBar
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        selectedButton: gamepadButton

        MyNavigationButton {
            id: gamepadButton
            navigationBar: navigationBar
            sourceLight: "img/gamepad-white.png"
            sourceDark: "img/gamepad-black.png"
        }

        MyNavigationButton {
            id: antennaButton
            navigationBar: navigationBar
            sourceLight: "img/antenna-white.png"
            sourceDark: "img/antenna-black.png"
        }

        MyNavigationButton {
            id: tankButton
            navigationBar: navigationBar
            sourceLight: "img/tank-white.png"
            sourceDark: "img/tank-black.png"
        }

        MyNavigationButton {
            id: cameraButton
            navigationBar: navigationBar
            sourceLight: "img/camera-white.png"
            sourceDark: "img/camera-black.png"
        }

        bottomButtons: [
            MyNavigationButton {
                id: exitButton
                navigationBar: navigationBar
                sourceLight: "img/exit-white.png"
                sourceDark: "img/exit-black.png"

                onClicked: exitClicked()
            }
        ]
    }

    Item {
        id: content
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: navigationBar.right
        anchors.margins: Style.largeMargin

        Grid {  // GamePad panel
            anchors.fill: parent
            visible: gamepadButton.selected
            columns: 2

            Repeater {
                model: gamePadManager.gamePads

                delegate:  Column {
                    spacing: Style.mediumMargin
                    width: 256

                    MyLabel {
                        width: parent.width
                        horizontalAlignment: Qt.AlignCenter
                        text: "Player " + (index+1)
                        font.pixelSize: Style.subHeaderFontSize
                    }

                    Image {
                        id: gamePadImage
                        width: parent.width
                        source: "/img/gamepad.png"
                    }

                    MyLabel {
                        width: parent.width
                        horizontalAlignment: Qt.AlignHCenter
                        text: modelData.debugString
                    }
                }
            }
        }

        // Camera panel
        ColumnLayout {
            anchors.fill: parent
            visible: cameraButton.selected
            spacing: Style.smallMargin

            Row {
                Repeater {
                    model: cameraManager.availableDevices

                    delegate: MyToolButton {
                        selected: modelData === cameraManager.currentDevice
                        width: 160
                        height: 128

                        sourceLight: "img/camera-white.png"
                        sourceDark: "img/camera-black.png"
                        imageMargins: 20
                        imageBottomMargin: 48

                        MyLabel {
                            anchors.fill: parent
                            anchors.margins: Style.smallMargin
                            verticalAlignment: Qt.AlignBottom
                            horizontalAlignment: Qt.AlignHCenter
                            text: modelData
                            color: parent.foregroundInverted ? Style.black : Style.lightGray
                        }

                        onClicked: cameraManager.currentDevice = modelData
                    }
                }
            }
            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }
    }
}
