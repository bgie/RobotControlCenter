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
pragma Singleton
import QtQuick 2.0

Item {
    readonly property int windowMargins: 8
    readonly property int smallMargin: 8
    readonly property int mediumMargin: 16
    readonly property int largeMargin: 32
    readonly property int extraLargeMargin: 48

    readonly property int smallRadius: 3
    readonly property int largeRadius: 12

    readonly property int fontSize: 20
    readonly property int statusIconFontSize: 24
    readonly property int headerFontSize: 28

    readonly property int toolButtonSize: 64
    readonly property int statusIconSize: 64
    readonly property int navigationButtonSize: 96
    readonly property int navigationButtonIconMargin: 16

    readonly property color black: "#000000"
    readonly property color darkerGray: "#101010"
    readonly property color darkGray: "#303030"
    readonly property color mediumGray: "#808080"
    readonly property color lightGray: "#D0D0D0"
    readonly property color white: "#FFFFFF"
}
