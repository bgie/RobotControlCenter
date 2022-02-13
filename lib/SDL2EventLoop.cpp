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
#include "SDL2EventLoop.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_joystick.h>

struct SDL2EventLoop::Data {
    Data()
        : stopRequested(false)
    {
    }
    bool stopRequested;
};

SDL2EventLoop::SDL2EventLoop(QObject* parent)
    : QObject(parent)
    , _d(new Data())
{
    SDL_InitSubSystem(SDL_INIT_EVENTS);
}

SDL2EventLoop::~SDL2EventLoop()
{
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

void SDL2EventLoop::processEvents()
{
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_JOYDEVICEADDED:
            emit joystickAdded(event.jdevice.which);
            break;
        case SDL_JOYDEVICEREMOVED:
            emit joystickRemoved(event.jdevice.which);
            break;
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            emit joystickButtonChanged(event.jbutton.which, event.jbutton.button, event.jbutton.state == SDL_PRESSED);
            break;
        case SDL_JOYAXISMOTION:
            emit joystickAxisChanged(event.jaxis.which, event.jaxis.axis, event.jaxis.value);
            break;
        }
    }
}

void SDL2EventLoop::exec()
{
    while (!_d->stopRequested) {
        processEvents();
    }
    _d->stopRequested = false; // reset flag after stopping
}

void SDL2EventLoop::stop()
{
    _d->stopRequested = true;
}
