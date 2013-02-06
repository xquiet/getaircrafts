/**
    Copyright (C) 2012-2013  Matteo Pasotti <matteo.pasotti@gmail.com>

    This file is part of fgam - FlightGear Aircraft Manager

    fgam is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    fgam is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#ifndef INSTALLER_H
#define INSTALLER_H

#include <QString>
#include <QDir>
#include <zip.h>
#include <errno.h>

class Installer
{
public:
    Installer(QString destpath, QString zipped);
    bool extract();

private:
    const char *archive;
    QString destinationPath;
};

#endif // INSTALLER_H
