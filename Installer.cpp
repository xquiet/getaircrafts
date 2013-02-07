#include "Installer.h"

Installer::Installer(QString destpath, QString zipped)
{
    archive = zipped.toStdString().data();
    destinationPath = destpath.trimmed();
}

Installer::Installer(QString destpath)
{
    destinationPath = destpath.trimmed();
}

bool Installer::uninstall(QString model)
{
    QDir dir(destinationPath+model);
    if(dir.exists())
    {
        qDebug("dir to del: %s", (dir.absolutePath()).toStdString().data());
        if(dir.rmpath(dir.absolutePath()))
        {
            return true;
        }
    }
    else
    {
        qDebug("The directory %s do not exists or you require more privileges\nTry running yaai as administrator",
               QString(destinationPath+model).toStdString().data());
    }
    return false;
}

bool safe_create_dir(QString dir)
{
    QDir qdir;
    if(!qdir.mkdir(dir))
    {
        if (errno != EEXIST) {
            qDebug("Error creating dir %s",dir.toStdString().data());
            perror(dir.toStdString().data());
            return false;
        }
    }
    return true;
}

bool Installer::extract()
{
    struct zip *za;
    struct zip_file *zf;
    struct zip_stat sb;
    char buf[100];
    int err;
    int i;
    qint64 len;
    long long sum;
    QString fname = "";

    if ((za = zip_open(archive, 0, &err)) == NULL) {
        zip_error_to_str(buf, sizeof(buf), err, errno);
        qDebug("Extractor: can't open zip archive `%s': %s/n",
            archive, buf);
        return false;
    }

    for (i = 0; i < zip_get_num_entries(za, 0); i++) {
        if (zip_stat_index(za, i, 0, &sb) == 0) {
            qDebug("==================/n");
            len = strlen(sb.name);
            qDebug("Name: [%s], ", sb.name);
            qDebug("Size: [%lu], ", sb.size);
            qDebug("mtime: [%u]/n", (unsigned int)sb.mtime);
            if(destinationPath.trimmed().compare("")!=0)
            {
                fname = destinationPath + QDir::separator() + sb.name;
            }
            else
            {
                fname = sb.name;
            }
            if (sb.name[len - 1] == '/') {
                if(!safe_create_dir(fname))
                    return false;
            } else {
                zf = zip_fopen_index(za, i, 0);
                if (!zf) {
                    qDebug("boese, boese/n");
                    return false;
                }   
                QFile file(fname);
                if(!file.open(QFile::ReadWrite|QFile::Truncate))
                {
                    qDebug("boese, boese/n");
                    return false;
                }

                sum = 0;
                while (sum != sb.size) {
                    len = zip_fread(zf, buf, 100);
                    if (len < 0) {
                        qDebug("boese, boese/n");
                        exit(102);
                    }
                    file.write(buf,len);
                    sum += len;
                }
                file.close();
                zip_fclose(zf);
            }
        } else {
            qDebug("File[%s] Line[%d]/n", __FILE__, __LINE__);
        }
    }

    if (zip_close(za) == -1) {
        fprintf(stderr, "Extractor: can't close zip archive `%s'/n", archive);
        return false;
    }

    return true;
}

/*
https://gist.github.com/1759816
static void safe_create_dir(const char *dir)
{
    if (mkdir(dir, 0755) < 0) {
        if (errno != EEXIST) {
            perror(dir);
            exit(1);
        }
    }
}

int main(int argc, char *argv[])
{
    const char *archive;
    struct zip *za;
    struct zip_file *zf;
    struct zip_stat sb;
    char buf[100];
    int err;
    int i, len;
    int fd;
    long long sum;
    prg = argv[0];
    if (argc != 2) {
        fprintf(stderr, "usage: %s archive/n", prg);
        return 1;
    }

    archive = argv[1];
    if ((za = zip_open(archive, 0, &err)) == NULL) {
        zip_error_to_str(buf, sizeof(buf), err, errno);
        fprintf(stderr, "%s: can't open zip archive `%s': %s/n", prg,
            archive, buf);
        return 1;
    }

    for (i = 0; i < zip_get_num_entries(za, 0); i++) {
        if (zip_stat_index(za, i, 0, &sb) == 0) {
            printf("==================/n");
            len = strlen(sb.name);
            printf("Name: [%s], ", sb.name);
            printf("Size: [%llu], ", sb.size);
            printf("mtime: [%u]/n", (unsigned int)sb.mtime);
            if (sb.name[len - 1] == '/') {
                safe_create_dir(sb.name);
            } else {
                zf = zip_fopen_index(za, i, 0);
                if (!zf) {
                    fprintf(stderr, "boese, boese/n");
                    exit(100);
                }

                fd = open(sb.name, O_RDWR | O_TRUNC | O_CREAT, 0644);
                if (fd < 0) {
                    fprintf(stderr, "boese, boese/n");
                    exit(101);
                }

                sum = 0;
                while (sum != sb.size) {
                    len = zip_fread(zf, buf, 100);
                    if (len < 0) {
                        fprintf(stderr, "boese, boese/n");
                        exit(102);
                    }
                    write(fd, buf, len);
                    sum += len;
                }
                close(fd);
                zip_fclose(zf);
            }
        } else {
            printf("File[%s] Line[%d]/n", __FILE__, __LINE__);
        }
    }

    if (zip_close(za) == -1) {
        fprintf(stderr, "%s: can't close zip archive `%s'/n", prg, archive);
        return 1;
    }

    return 0;
}
*/
