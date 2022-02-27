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
#include "NonBlockingPipe.h"
#include <QFile>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace {
int disableBrokenPipeSignal()
{
    struct sigaction action;
    action.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &action, NULL);
    return 0;
}
static const int helper = disableBrokenPipeSignal();
}

NonBlockingPipe::NonBlockingPipe(QString path, QObject* parent)
    : QObject(parent)
    , _path(path)
    , _pathUtf8(path.toUtf8())
    , _fifoCreated(false)
    , _fileHandle(-1)
{
    if (mkfifo(_pathUtf8, 0666) == -1) {
        if (errno != EEXIST) {
            setError(errno);
        }
    } else {
        _fifoCreated = true;

        if (chmod(_pathUtf8, 0666) == -1) {
            setError(errno);
        }
    }
}

NonBlockingPipe::~NonBlockingPipe()
{
    closeFile();
    if (_fifoCreated) {
        unlink(_pathUtf8);
    }
}

QString NonBlockingPipe::path() const
{
    return _path;
}

const char* NonBlockingPipe::pathUtf8() const
{
    return _pathUtf8;
}

bool NonBlockingPipe::hasError() const
{
    return !_errorString.isEmpty();
}

QString NonBlockingPipe::errorString() const
{
    return _errorString;
}

bool NonBlockingPipe::openFile(bool writeOnly)
{
    if (isFileOpen())
        return true;

    _fileHandle = open(_pathUtf8, (writeOnly ? O_WRONLY : O_RDONLY) | O_NONBLOCK);
    if (isFileOpen()) {
        return true;
    } else if (ENXIO != errno) { // ENXIO indicates no reader = no real error, try again next time
        setError(errno);
    }
    return false;
}

bool NonBlockingPipe::isFileOpen() const
{
    return _fileHandle != -1;
}

int NonBlockingPipe::fileHandle() const
{
    return _fileHandle;
}

void NonBlockingPipe::closeFile()
{
    if (isFileOpen()) {
        close(_fileHandle);
        _fileHandle = -1;
    }
}

void NonBlockingPipe::clearError()
{
    setErrorString(QString());
}

void NonBlockingPipe::setError(int err)
{
    setErrorString(QStringLiteral("%2 (%1)").arg(err).arg(QString(strerror(err))));
}

void NonBlockingPipe::setErrorString(QString newValue)
{
    if (_errorString == newValue)
        return;

    _errorString = newValue;
    emit errorChanged();
}
