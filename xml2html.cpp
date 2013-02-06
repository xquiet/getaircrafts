#include "xml2html.h"

xml2html::xml2html()
{
}

QString xml2html::aircraftList(QString data, QStringList filter)
{
    QXmlStreamReader xml(data);
    QString key = "";
    QString prevKey = "";
    QString output = "";
    QString authors = "";
    QString description = "";
    QString version = "";
    QString image = "";
    int idDiag = 0;
    //output += "<h2>Available Aircrafts</h2>";

    while(!xml.atEnd())
    {
        xml.readNext();
        //qDebug("%s",xml.name().toString().toStdString().data());
        //qDebug("%s",xml.text().toString().toStdString().data());
        QString xmlName = xml.name().toString();
        if((xmlName.compare("aircraft")==0)||
                xmlName.compare("aircraft")==0)
        {
            // do nothing
        }
        if(xmlName.compare("title")==0)
        {
            key = xml.readElementText();
            if(key.compare(prevKey)!=0)
            {
                if(description.trimmed().compare("")!=0)
                {
                    output += "</div> <!-- links -->";
                    output += "</div><!-- end details -->";
                    output += "<div style=\"clear: both;\"></div>";
                    output += "<input type=\"button\" onclick=\"showDialog("+QString::number(idDiag)+")\" value=\"Details\" />";
                    output += "<div id=\"detailsdiag"+QString::number(idDiag)+"\" title=\""+prevKey+"\" class=\"mydialog\">";
                    idDiag++;
                    output += "<div class=\"thumb\"><img src=\""+image+"\" alt=\""+key+"\" /></div>";
                    output += "<div class=\"details\">";
                    output += "<div><b>Authors:</b>&nbsp;&nbsp;"+authors+"</div>";
                    output += "<div><b>Version:</b>&nbsp;&nbsp;"+version+"</div>";
                    output += "<div><b>Description:</b><br />"+description+"</div>";
                    output += "<div style=\"clear: both;\"></div>";
                    output += "</div><!-- end details into dialog -->";
                    output += "</div><!-- end dialog -->";
                    output += "</div><!-- end aircraft -->";
                }
                description = "";
                authors = "";
                version = "";
                prevKey = key;
            }
            if(filter.contains(key,Qt::CaseSensitive))
            {
                output += "<div class=\"aircraft installed\"><h3>" + key + "</h3>";
            }
            else
            {
                output += "<div class=\"aircraft\"><h3>" + key + "</h3>";
            }
        }
        else if(xmlName.compare("image")==0)
        {
            image = xml.readElementText();
            output += "<div class=\"thumb\"><img src=\"" + image + "\" alt=\""+ key + "\" /></div>";
        }
        else if(xmlName.compare("model_name")==0)
        {
            description = xml.readElementText();
            QString brief_description = description;
            if(brief_description.length()>30)
            {
                brief_description = brief_description.mid(0,30) + "...";
            }
            output += "<div class=\"details\">";
            output += "<div class=\"modelname\"><b>Description:</b>&nbsp;"+brief_description+"</div>";
        }
        else if(xmlName.compare("author")==0)
        {
            authors = xml.readElementText();
            QString brief_authors = authors;
            if(brief_authors.length()>30)
            {
                brief_authors = brief_authors.mid(0,30) + "...";
            }
            output += "<div class=\"author\"><b>Authors:</b>&nbsp;&nbsp;<em>"+brief_authors+"</em></div>";
            output += "<div class=\"links\">";
        }
        else if(xmlName.compare("version")==0)
        {
            version = xml.readElementText();
            output += "<div class=\"version\"><b>Version:</b>&nbsp;&nbsp;"+version+"</div>";
        }
        else if(xmlName.contains("mirror"))
        {
            output += "<a href=\"" + xml.readElementText() + "\" title=\""+key+"\">" + xmlName.mid(0,1).toUpper()+xmlName.mid(1,xmlName.length()) + "</a>&nbsp;";
        }
    }
    output += "</div> <!-- links -->";
    output += "</div><!-- end details -->";
    output += "<div style=\"clear: both;\"></div>";
    output += "<input type=\"button\" onclick=\"showDialog("+QString::number(idDiag)+")\" value=\"Details\" />";
    output += "<div id=\"detailsdiag"+QString::number(idDiag)+"\" title=\""+prevKey+"\" class=\"mydialog\">";
    idDiag++;
    output += "<div class=\"thumb\"><img src=\""+image+"\" alt=\""+key+"\" /></div>";
    output += "<div class=\"details\">";
    output += "<div><b>Authors:</b>&nbsp;&nbsp;"+authors+"</div>";
    output += "<div><b>Version:</b>&nbsp;&nbsp;"+version+"</div>";
    output += "<div><b>Description:</b><br />"+description+"</div>";
    output += "<div style=\"clear: both;\"></div>";
    output += "</div><!-- end details into dialog -->";
    output += "</div><!-- end dialog -->";
    output += "</div><!-- end aircraft last -->";
    if(xml.hasError())
    {
        qDebug("ERROR: %s",xml.errorString().toStdString().data());
    }
    return output;
}
