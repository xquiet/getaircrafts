#ifndef XML2HTML_H
#define XML2HTML_H

#include <QXmlStreamReader>
#include <QStringList>

class xml2html
{
public:
    xml2html();
    QString aircraftList(QString data, QStringList filter);
};

#endif // XML2HTML_H
