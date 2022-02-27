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
#include "NonBlockingReceiverPipe.h"
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

NonBlockingReceiverPipe::NonBlockingReceiverPipe(QString path, QObject* parent)
    : NonBlockingPipe(path, parent)
{
}

NonBlockingReceiverPipe::~NonBlockingReceiverPipe()
{
}

void NonBlockingReceiverPipe::receive()
{
    if (openFile(false)) {
        int bytesRead = 0;
        char c = 0;
        do {
            bytesRead = read(fileHandle(), &c, sizeof(c));
            if (bytesRead > 0) {
                _receiveBuffer.append(c);
                if (c == '\n') {
                    emit received(_receiveBuffer);
                    _receiveBuffer.clear();
                }
            } else if (bytesRead == -1 && errno != EAGAIN) {
                setError(errno);
            }
        } while (bytesRead > 0);
    }
}
