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
    id: main
    dialogWidth: 1400
    dialogHeight: 1000
    title: "Settings"

    signal exitClicked

    property SettingsController controller: settingsControllerFactory.create()
    property ICamera camera: cameraController.camera

    onCloseClicked: exitClicked()

    MyNavigationBar {
        id: navigationBar
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        selectedButton: cameraButton

        MyNavigationButton {
            id: exitButton
            navigationBar: navigationBar
            sourceLight: "img/exit-white.png"
            sourceDark: "img/exit-black.png"

            onClicked: exitClicked()
        }

        MyNavigationButton {
            id: cameraButton
            navigationBar: navigationBar
            sourceLight: "img/camera-white.png"
            sourceDark: "img/camera-black.png"
        }

        Row {
            Layout.leftMargin: Style.mediumMargin
            MyButton {
                text: "Start"
                backgroundColor: Style.darkGray
                radius: 0
                enabled: cameraController.canStart
                visible: !cameraController.isStreaming
                onClicked: cameraController.startCameraStream()
                width: cameraButton.width
            }
            MyButton {
                text: "Stop"
                backgroundColor: Style.darkGray
                radius: 0
                visible: cameraController.isStreaming
                onClicked: cameraController.stopCameraStream()
                width: cameraButton.width
            }
        }

        MyNavigationButton {
            id: markerButton
            navigationBar: navigationBar
            sourceLight: "img/marker.png"
            sourceDark: sourceLight
        }

        MyNavigationButton {
            id: worldButton
            navigationBar: navigationBar
            sourceLight: "img/world-white.png"
            sourceDark: "img/world-black.png"
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

        MyNavigationButton {
            id: pipeButton
            navigationBar: navigationBar
            sourceLight: "img/pipe-white.png"
            sourceDark: "img/pipe-black.png"
        }

        MyNavigationButton {
            id: monitorButton
            navigationBar: navigationBar
            sourceLight: "img/stethoscope-white.png"
            sourceDark: "img/stethoscope-black.png"
        }
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

            Loader {
                Layout.margins: Style.largeMargin

                active: camera && camera.className == "Camera"
                visible: active
                sourceComponent: GridLayout {
                    rows: 2
                    flow: GridLayout.TopToBottom
                    columnSpacing: Style.largeMargin
                    rowSpacing: Style.smallMargin

                    MyLabel {
                        text: "Format"
                    }
                    MyComboBox {
                        enabled: !camera.isStreaming
                        model: camera.videoFormats
                        Layout.preferredWidth: 600
                        Layout.leftMargin: Style.mediumMargin
                        currentIndex: camera.videoFormatIndex
                        onCurrentIndexChanged: camera.videoFormatIndex = currentIndex
                    }
                    MyLabel {
                        text: "Gain"
                    }
                    MyTextEdit {
                        Layout.leftMargin: Style.mediumMargin
                        Layout.preferredWidth: 80
                        text: camera.gain
                        onTextChanged: camera.gain = parseInt(text)
                    }
                    MyLabel {
                        text: "Exposure"
                    }
                    MyTextEdit {
                        Layout.leftMargin: Style.mediumMargin
                        Layout.preferredWidth: 80
                        text: camera.exposure
                        onTextChanged: camera.exposure = parseInt(text)
                    }
                }
            }

            Loader {
                Layout.margins: Style.largeMargin

                active: camera && camera.className == "ReplayCam"
                visible: active
                sourceComponent: GridLayout {
                    rows: 2
                    flow: GridLayout.TopToBottom
                    columnSpacing: Style.largeMargin
                    rowSpacing: Style.smallMargin

                    MyLabel {
                        text: "Path"
                    }
                    Row {
                        Layout.leftMargin: Style.mediumMargin
                        spacing: Style.mediumMargin
                        MyTextEdit {
                            id: videosPath
                            width: 700
                            text: camera.videosPath
                            onTextChanged: camera.videosPath = text
                        }
                        Image {
                            width: 32
                            height: videosPath.height
                            fillMode: Image.PreserveAspectFit
                            source: "/img/error-red.png"
                            visible: !camera.videosPathValid
                        }
                        Image {
                            width: 32
                            height: videosPath.height
                            fillMode: Image.PreserveAspectFit
                            source: "/img/checkmark-white.png"
                            visible: camera.videosPathValid
                        }
                    }

                    MyLabel {
                        text: "Videos"
                    }
                    MyComboBox {
                        enabled: camera.videosPathValid
                        model: camera.videos
                        Layout.preferredWidth: 250
                        Layout.leftMargin: Style.mediumMargin
                        currentIndex: camera.videoIndex
                        onCurrentIndexChanged: camera.videoIndex = currentIndex
                    }

                    MyLabel {
                        text: "FPS"
                    }
                    MyTextEdit {
                        Layout.leftMargin: Style.mediumMargin
                        Layout.preferredWidth: 60
                        text: camera.framesPerSecond
                        onTextChanged: camera.framesPerSecond = parseInt(text)
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
            anchors.margins: Style.largeMargin
            visible: markerButton.selected
            spacing: Style.largeMargin

            GridLayout {
                rows: 2
                flow: GridLayout.TopToBottom

                columnSpacing: Style.largeMargin
                rowSpacing: Style.smallMargin

                MyLabel {
                    text: "Calibration File"
                }
                Row {
                    Layout.leftMargin: Style.mediumMargin
                    spacing: Style.mediumMargin
                    MyTextEdit {
                        id: calibrationFile
                        width: 1000
                        text: calibrationController.calibrationFile
                        onTextChanged: calibrationController.calibrationFile = text
                    }
                    Image {
                        width: 32
                        height: calibrationFile.height
                        fillMode: Image.PreserveAspectFit
                        source: "/img/error-red.png"
                        visible: !calibrationController.loaded
                    }
                    Image {
                        width: 32
                        height: calibrationFile.height
                        fillMode: Image.PreserveAspectFit
                        source: "/img/checkmark-white.png"
                        visible: calibrationController.loaded
                    }
                }
            }

            GridLayout {
                rows: 2
                flow: GridLayout.TopToBottom

                columnSpacing: Style.largeMargin
                rowSpacing: Style.smallMargin

                MyLabel {
                    text: "Screen Rotation"
                }
                MyTextEdit {
                    Layout.leftMargin: Style.mediumMargin
                    Layout.preferredWidth: 60
                    text: controller.screenRotation
                    onTextChanged: controller.screenRotation = parseInt(text)
                }

                MyLabel {
                    text: "Reference Plane"
                }
                MyLabel {
                    Layout.leftMargin: Style.mediumMargin
                    Layout.preferredWidth: 260
                    text: controller.markerIds
                }
                Row {
                    Layout.rowSpan: 2
                    visible: controller.hasPlane
                    spacing: Style.mediumMargin

                    MyLabel {
                        text: "\u03B1"
                    }
                    MyLabel {
                        text: controller.planeAlpha.toFixed(1)
                        font.pixelSize: 48
                    }
                    Item {
                        width: Style.largeMargin
                        height: 1
                    }

                    MyLabel {
                        text: "\u03B2"
                    }
                    MyLabel {
                        text: controller.planeBeta.toFixed(1)
                        font.pixelSize: 48
                    }
                }
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true

                ImageItem {
                    id: markerImage
                    anchors.fill: parent
                    image: controller.arucoImage
                    visible: hasImage
                    rotation: controller.screenRotation
                }

                Rectangle {
                    anchors.fill: markerIdsLabel
                    color: Style.black
                    opacity: 0.3
                }

                MyLabel {
                    id: markerIdsLabel
                    x: markerImage.offset.x + Style.smallMargin
                    y: markerImage.offset.y + Style.smallMargin
                    font.pixelSize: Style.tinyFontSize
                    text: controller.serializedMarkers
                    color: Style.white
                }
            }
        }

        ColumnLayout { // World panel
            anchors.fill: parent
            anchors.margins: Style.largeMargin
            visible: worldButton.selected
            spacing: Style.largeMargin

            GridLayout {
                rows: 2
                flow: GridLayout.TopToBottom

                columnSpacing: Style.largeMargin
                rowSpacing: Style.smallMargin

                MyLabel {
                    text: "Edge Points"
                    Layout.preferredWidth: 160
                }
                Row {
                    Layout.leftMargin: Style.mediumMargin
                    spacing: Style.smallMargin
                    MyLabel {
                        height: resetWorldEdgeButton.height
                        verticalAlignment: Qt.AlignVCenter
                        text: worldEdge.count
                    }
                    MyButton {
                        id: resetWorldEdgeButton
                        text: "Reset"
                        backgroundColor: Style.darkGray
                        visible: worldEdge.count > 0
                        onClicked: worldEdge.reset()
                    }
                }
                MyLabel {
                    text: "Z height"
                    Layout.preferredWidth: 80
                }
                MyLabel {
                    Layout.leftMargin: Style.mediumMargin
                    text: worldEdge.z.toFixed(1)
                }
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
                rotation: controller.screenRotation

                ImageItem {
                    id: worldImage
                    anchors.fill: parent
                    image: controller.arucoImage
                    visible: hasImage
                }

                Repeater {
                    model: controller.markerScreenPoints

                    delegate:  MyToolButton {
                        x: (worldImage.zoom * modelData.screenX) + worldImage.offset.x - 24
                        y: (worldImage.zoom * modelData.screenY) + worldImage.offset.y - 24
                        width: 48
                        height: 48
                        imageMargins: 0
                        radius: 32
                        sourceDark: "/img/plus-black.png"
                        sourceLight: "/img/plus-yellow.png"

                        onClicked: controller.addPointToWorldEdge(modelData.x, modelData.y, modelData.z)
                    }
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

        Item {  // Robots panel
            anchors.fill: parent
            anchors.margins: Style.largeMargin
            visible: tankButton.selected
            ColumnLayout {
                anchors.fill: parent
                spacing: Style.largeMargin

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
                    visible: robotManager.count === 0
                }

                ScrollView {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    clip: true
                    GridLayout {
                        columns: 2
                        Repeater {
                            model: robotManager.robots

                            delegate:  Row {
                                property Robot robot: modelData

                                spacing: Style.mediumMargin

                                Image {
                                    width: 96
                                    height: 96
                                    fillMode: Image.PreserveAspectFit
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
                                            id: lowBatteryIcon
                                            visible: robot.batteryCharge < 25
                                            width: 32
                                            height: 32
                                            fillMode: Image.PreserveAspectFit
                                            source: "/img/battery-low-red.png"
                                            SequentialAnimation on opacity {
                                                loops: Animation.Infinite
                                                running: lowBatteryIcon.visible
                                                OpacityAnimator { from: 0.2; to: 1.0; duration: 150 }
                                                OpacityAnimator { from: 1.0; to: 0.2; duration: 350 }
                                            }
                                        }
                                        Image {
                                            visible: robot.batteryCharge >= 25 && robot.batteryCharge < 50
                                            width: 32
                                            height: 32
                                            fillMode: Image.PreserveAspectFit
                                            source: "/img/battery-mid-white.png"
                                        }
                                        Image {
                                            visible: robot.batteryCharge >= 50 && robot.batteryCharge < 95
                                            width: 32
                                            height: 32
                                            fillMode: Image.PreserveAspectFit
                                            source: "/img/battery-high-white.png"
                                        }
                                        Image {
                                            visible: robot.batteryCharge >= 95
                                            width: 32
                                            height: 32
                                            fillMode: Image.PreserveAspectFit
                                            source: "/img/battery-full-white.png"
                                        }
                                        MyLabel {
                                            height: 32
                                            verticalAlignment: Qt.AlignVCenter
                                            text: robot.batteryCharge + "%"
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
                                        height: markerIdInput.height
                                        MyLabel {
                                            height: parent.height
                                            width: commandLabel.width
                                            verticalAlignment: Qt.AlignVCenter
                                            text: "Marker id"
                                        }
                                        MyTextEdit {
                                            id: markerIdInput
                                            width: 60
                                            text: robot.markerId
                                            onTextChanged: robot.markerId = parseInt(text)
                                        }
                                        MyLabel {
                                            height: parent.height
                                            verticalAlignment: Qt.AlignVCenter
                                            text: "Gamepad index"
                                        }
                                        MyTextEdit {
                                            id: gamePadIndexInput
                                            width: 60
                                            text: robot.gamepadIndex
                                            onTextChanged: robot.gamepadIndex = parseInt(text)
                                        }
                                    }

                                    Row {
                                        spacing: Style.smallMargin
                                        MyLabel {
                                            id: commandLabel
                                            height: commandInput.height
                                            verticalAlignment: Qt.AlignVCenter
                                            text: "Command"
                                        }
                                        MyTextEdit {
                                            id: commandInput
                                            width: 300

                                            MyToolButton {
                                                anchors.right: parent.right
                                                width: commandInput.height
                                                height: commandInput.height
                                                radius: commandInput.height / 2
                                                sourceLight: "img/help-black.png" // inverted on purpose
                                                sourceDark: "img/help-white.png"
                                                backgroundColor: "transparent"
                                                onClicked: commandsHelpPanel.visible = true
                                            }
                                            onAccepted: sendButton.onClicked()
                                        }
                                        MyButton {
                                            id: sendButton
                                            text: "Send"
                                            backgroundColor: Style.darkGray
                                            enabled: commandInput.text
                                            onClicked: robot.sendCommand(commandInput.text)
                                        }
                                    }

                                    MyLabel {
                                        id: lastCommand
                                        text: robot.lastCommand
                                    }

                                    Item {
                                        height: Style.largeMargin
                                        width: 1
                                    }
                                }
                            }
                        }
                    }
                }
            }
            MyToolButton {
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.rightMargin: Style.mediumMargin
                width: 64
                height: 64
                sourceLight: "img/plus-white.png"
                sourceDark: "img/plus-black.png"

                onClicked: fakeRobotManager.addFakeRobot()
            }
        }

        ColumnLayout { // Pipe panel
            anchors.fill: parent
            anchors.margins: Style.largeMargin
            visible: pipeButton.selected
            spacing: Style.largeMargin

            MyLabel {
                text: "No robots connected"
                visible: !robotSocketManager.robotsConnected
            }

            ColumnLayout {
                visible: robotSocketManager.robotsConnected
                spacing: Style.largeMargin

                Repeater {
                    model: robotSocketManager.robotSocket

                    delegate:  Row {
                        property RobotSocket socket: modelData

                        spacing: Style.mediumMargin
                        Item {
                            width: Style.mediumMargin
                            height: 1
                        }
                        MyLabel {
                            id: pipePath
                            text: socket.path
                        }
                        Image {
                            width: pipePath.height
                            height: pipePath.height
                            fillMode: Image.PreserveAspectFit
                            source: socket.hasError ? "/img/error-red.png" : "/img/checkmark-white.png"
                        }
                        MyLabel {
                            text: socket.errorString
                        }
                    }
                }
            }
            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }

        ColumnLayout { // Performance monitor panel
            anchors.fill: parent
            visible: monitorButton.selected
            anchors.margins: Style.largeMargin
            spacing: Style.smallMargin

            MyButton {
                text: "Update"
                backgroundColor: Style.darkGray
                onClicked: performanceLogView.model = performanceMonitor.mostRecentBatch
            }

            Rectangle {
                Layout.fillHeight: true
                Layout.fillWidth: true
                color: Style.black

                ListView{
                    id: performanceLogView
                    anchors.fill: parent
                    anchors.margins: 16
                    clip: true
                    orientation: Qt.Vertical

                    delegate: MyLabel {
                        text: modelData
                        font.family: "Freemono"
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
