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
    dialogWidth: 1200
    dialogHeight: 1000
    title: "Settings"

    signal exitClicked

    property SettingsController controller: settingsControllerFactory.create()

    onCloseClicked: exitClicked()

    MyNavigationBar {
        id: navigationBar
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        selectedButton: cameraButton

        MyNavigationButton {
            id: cameraButton
            navigationBar: navigationBar
            sourceLight: "img/camera-white.png"
            sourceDark: "img/camera-black.png"
        }

        MyNavigationButton {
            id: markerButton
            navigationBar: navigationBar
            sourceLight: "img/marker.png"
            sourceDark: sourceLight
        }

        MyNavigationButton {
            id: gamepadButton
            navigationBar: navigationBar
            sourceLight: "img/gamepad-white.png"
            sourceDark: "img/gamepad-black.png"
        }

        MyNavigationButton {
            id: tankButton
            navigationBar: navigationBar
            sourceLight: "img/tank-white.png"
            sourceDark: "img/tank-black.png"
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

        ColumnLayout { // Camera panel
            anchors.fill: parent
            visible: cameraButton.selected
            spacing: 0

            Rectangle {
                color: Style.black
                Layout.fillWidth: true
                height: 160

                ListView {
                    anchors.fill: parent
                    anchors.margins: 16
                    clip: true
                    orientation: Qt.Horizontal
                    model: cameraManager.availableDevices

                    delegate: MyToolButton {
                        selected: modelData === cameraController.videoDevice
                        width: 160
                        height: 128

                        sourceLight: "img/camera-white.png"
                        sourceDark: "img/camera-black.png"
                        imageMargins: 20
                        imageBottomMargin: 48
                        backgroundColor: Style.black

                        MyLabel {
                            anchors.fill: parent
                            anchors.margins: Style.mediumMargin
                            verticalAlignment: Qt.AlignBottom
                            horizontalAlignment: Qt.AlignHCenter
                            text: modelData
                            color: parent.foregroundInverted ? Style.black : Style.lightGray
                        }

                        onClicked: cameraController.videoDevice = modelData
                    }
                }
            }

            GridLayout {
                rows: 2
                flow: GridLayout.TopToBottom
                Layout.margins: Style.largeMargin
                columnSpacing: Style.largeMargin
                rowSpacing: Style.smallMargin

                MyLabel {
                    text: "Format"
                }
                MyComboBox {
                    enabled: !cameraController.isCameraStreaming
                    model: cameraController.videoFormats
                    Layout.preferredWidth: 380
                    Layout.leftMargin: Style.mediumMargin
                    currentIndex: cameraController.currentVideoFormatIndex
                    onCurrentIndexChanged: cameraController.setCurrentVideoFormatIndex(currentIndex)
                }
                MyLabel {
                    text: "Gain"
                }
                MyTextEdit {
                    Layout.leftMargin: Style.mediumMargin
                    Layout.preferredWidth: 60
                    text: cameraController.gain
                    onTextChanged: cameraController.gain = parseInt(text)
                }
                MyLabel {
                    text: "Exposure"
                }
                MyTextEdit {
                    Layout.leftMargin: Style.mediumMargin
                    Layout.preferredWidth: 60
                    text: cameraController.exposure
                    onTextChanged: cameraController.exposure = parseInt(text)
                }

                MyLabel {
                    text: "Streaming"
                }
                Row {
                    Layout.leftMargin: Style.mediumMargin
                    MyButton {
                        id: startCameraButton
                        text: "Start"
                        backgroundColor: Style.darkGray
                        enabled: cameraController.canCameraStream && !cameraController.isCameraStreaming
                        visible: !cameraController.isCameraStreaming
                        onClicked: cameraController.startCameraStream()
                    }
                    MyButton {
                        text: "Stop"
                        backgroundColor: Style.darkGray
                        visible: cameraController.isCameraStreaming
                        onClicked: cameraController.stopCameraStream()
                        width: startCameraButton.width
                    }
                }
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.margins: Style.largeMargin

                ImageItem {
                    anchors.fill: parent
                    image: cameraController.image
                    visible: hasImage
                }
            }
        }

        ColumnLayout { // Marker panel
            anchors.fill: parent
            visible: markerButton.selected
            spacing: 0

            GridLayout {
                rows: 2
                flow: GridLayout.TopToBottom
                Layout.margins: Style.largeMargin
                columnSpacing: Style.largeMargin
                rowSpacing: Style.smallMargin

                MyLabel {
                    text: "Calibration File"
                }
                Row {
                    Layout.leftMargin: Style.mediumMargin
                    spacing: Style.smallMargin
                    MyTextEdit {
                        width: 800
                        text: calibrationController.calibrationFile
                        onTextChanged: calibrationController.calibrationFile = text
                    }
                    Image {
                        width: 32
                        height: 32
                        source: "/img/error-red.png"
                        visible: !calibrationController.loaded
                    }
                    Image {
                        width: 32
                        height: 32
                        source: "/img/checkmark-white.png"
                        visible: calibrationController.loaded
                    }
                }

                MyLabel {
                    text: "Camera"
                }
                Row {
                    Layout.leftMargin: Style.mediumMargin
                    MyButton {
                        id: startMarkerCameraButton
                        text: "Start"
                        backgroundColor: Style.darkGray
                        enabled: cameraController.canCameraStream && !cameraController.isCameraStreaming
                        visible: !cameraController.isCameraStreaming
                        onClicked: cameraController.startCameraStream()
                    }
                    MyButton {
                        text: "Stop"
                        backgroundColor: Style.darkGray
                        visible: cameraController.isCameraStreaming
                        onClicked: cameraController.stopCameraStream()
                        width: startMarkerCameraButton.width
                    }
                }
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.margins: Style.largeMargin

                ImageItem {
                    anchors.fill: parent
                    image: controller.arucoImage
                    visible: hasImage
                }
            }
        }

        Grid {  // GamePad panel
            anchors.fill: parent
            anchors.margins: Style.largeMargin
            visible: gamepadButton.selected
            columns: 2

            MyLabel {
                text: "No gamepads connected"
                visible: gamePadManager.count === 0
            }

            Repeater {
                model: gamePadManager.gamePads

                delegate:  Column {
                    property GamePad gamePad: modelData

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
                        wrapMode: Text.WordWrap
                        text: gamePad.debugString
                    }
                }
            }
        }

        Grid {  // Tanks panel
            anchors.fill: parent
            anchors.margins: Style.largeMargin
            visible: tankButton.selected
            columns: 2

            Row {
                visible: !robotNetwork.connected
                spacing: Style.smallMargin

                Image {
                    width: 48
                    height: 48
                    source: "/img/error-red.png"
                }

                MyLabel {
                    text: "Network connection error\n" + robotNetwork.connectionError
                }
            }

            MyLabel {
                text: "No robots connected"
                visible: robotNetwork.connected && robotNetwork.count === 0
            }

            Repeater {
                model: robotNetwork.robots

                delegate:  Row {
                    property Robot robot: modelData

                    spacing: Style.mediumMargin
                    width: 500

                    Image {
                        width: 128
                        height: 110
                        source: "/img/tank-white.png"
                    }

                    Column {
                        spacing: Style.mediumMargin

                        MyLabel {
                            text: "Robot " + robot.id
                            font.pixelSize: Style.subHeaderFontSize
                        }

                        Row {
                            spacing: Style.smallMargin
                            Image {
                                width: 32
                                height: 32
                                fillMode: Image.PreserveAspectFit
                                source: "/img/battery-high-white.png"
                            }
                            MyLabel {
                                height: 32
                                verticalAlignment: Qt.AlignVCenter
                                text: robot.batteryVoltage.toFixed(2) + "V"
                            }
                            Item {
                                width: Style.mediumMargin
                                height: 1
                            }
                            Image {
                                width: 32
                                height: 32
                                fillMode: Image.PreserveAspectFit
                                source: "/img/antenna-white.png"
                            }
                            MyLabel {
                                height: 32
                                verticalAlignment: Qt.AlignVCenter
                                text: robot.url
                            }
                        }

                        Row {
                            spacing: Style.smallMargin
                            MyLabel {
                                height: commandInput.height
                                verticalAlignment: Qt.AlignVCenter
                                text: "Command"
                            }
                            MyTextEdit {
                                id: commandInput
                                width: 400
                            }
                            MyButton {
                                text: "Send"
                                backgroundColor: Style.darkGray
                                enabled: commandInput.text
                                onClicked: robot.sendCommand(commandInput.text)
                            }
                            MyToolButton {
                                width: commandInput.height
                                height: commandInput.height
                                sourceLight: "img/help-white.png"
                                sourceDark: "img/help-black.png"
                                onClicked: commandsHelpPanel.visible = true
                            }
                        }
                    }
                }
            }
        }
    }

    MyDialog {
        id: commandsHelpPanel
        anchors.fill: parent
        visible: false
        dialogWidth: 600
        dialogHeight: 240
        title: "Robot Commands"

        onCloseClicked: visible = false

        Column {
            anchors.fill: parent
            anchors.margins: Style.largeMargin
            spacing: Style.mediumMargin

            MyLabel {
                text: "MOTOR left_speed right_speed\n  Where speed is a value in range [-1024, 1024]"
            }
            MyLabel {
                text: "SERVO angle_1 angle_2\n  Where angle is a value in range [0, 180]"
            }
        }
    }
}
