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
