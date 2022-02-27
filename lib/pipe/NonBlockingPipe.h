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
#include <QObject>

class NonBlockingPipe : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString path READ path CONSTANT)
    Q_PROPERTY(bool hasError READ hasError NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorChanged)

public:
    explicit NonBlockingPipe(QString path, QObject* parent = nullptr);
    virtual ~NonBlockingPipe() override;

    QString path() const;
    const char* pathUtf8() const;
    bool hasError() const;
    QString errorString() const;

signals:
    void errorChanged();

protected:
    bool openFile(bool writeOnly);
    bool isFileOpen() const;
    int fileHandle() const;
    void closeFile();

    void clearError();
    void setError(int err);
    void setErrorString(QString newValue);

private:
    const QString _path;
    const QByteArray _pathUtf8;
    QString _errorString;
    bool _fifoCreated;
    int _fileHandle;
};
