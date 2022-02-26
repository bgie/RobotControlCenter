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
#pragma once

// Simple filter helper for a rotating angle: will keep track of the number of rotation
// This allows to retrieve a continuous value (angleWithRotations) that does not jump at the 2pi (360deg) mark
// Limitation is that angles must not change more than pi (180deg) per frame
class RotationCounter
{
public:
    RotationCounter(float angle = 0.0f, int rotations = 0);

    void updateAngle(float angle);
    float angleWithRotations() const;

private:
    float _angle;
    int _rotations;
};
