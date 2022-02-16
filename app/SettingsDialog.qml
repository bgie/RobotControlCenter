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
    width: 800
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
}
