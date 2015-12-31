/*****************************************************************************
* Copyright 2015 Alexander Barthel albar965@mailbox.org
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "export/htmlexporter.h"

#include "gui/constants.h"

#include "gui/errorhandler.h"
#include "table/sqlmodel.h"
#include "gui/dialog.h"
#include "settings/settings.h"
#include "table/controller.h"
#include "sql/sqlquery.h"
#include "logging/loggingdefs.h"

#include <QUrl>
#include <QFile>
#include <QApplication>
#include <QDateTime>
#include <QXmlStreamReader>
#include <QDesktopServices>

using atools::gui::ErrorHandler;
using atools::gui::Dialog;
using atools::sql::SqlQuery;

HtmlExporter::HtmlExporter(QWidget *parent, Controller *controller) :
  Exporter(parent, controller)
{
}

HtmlExporter::~HtmlExporter()
{
}

QString HtmlExporter::saveHtmlFileDialog()
{
  return dialog->saveFileDialog(tr("Export HTML Document"),
                                tr("HTML Documents (*.htm *.html);;All Files (*)"),
                                ll::constants::SETTINGS_EXPORT_FILE_DIALOG, "html");
}

int HtmlExporter::exportAll(bool open)
{
  int exported = 0;
  QString filename = saveHtmlFileDialog();

  if(!filename.isEmpty())
  {
    qDebug() << "exportAllHtml" << filename;

    QFile file(filename);
    if(file.open(QIODevice::WriteOnly))
    {
      // Run the current query to get all results - not only the visible
      atools::sql::SqlDatabase *db = controller->getSqlDatabase();
      SqlQuery query(db);
      query.exec(controller->getCurrentSqlQuery());

      QXmlStreamWriter stream(&file);
      stream.setAutoFormatting(true);
      stream.setAutoFormattingIndent(2);
      stream.setCodec(ll::constants::EXPORT_HTML_CODEC);

      // Get the CSS either from the resources or from the settings directory
      writeHtmlStart(stream,
                     atools::settings::Settings::getOverloadedPath(ll::constants::EXPORT_HTML_CSS_FILE));
      writeHtmlHeader(stream);

      stream.writeStartElement("table");
      stream.writeStartElement("tbody");
      QVector<int> visualColumnIndex;
      int row = 0;
      while(query.next())
      {
        QSqlRecord rec = query.record();

        if(row == 0)
        {
          // Create an index that maps the (probably reordered) columns of the
          // view to the model
          createVisualColumnIndex(rec.count(), visualColumnIndex);
          writeHtmlTableHeader(stream, headerNames(rec.count(), visualColumnIndex));
        }

        stream.writeStartElement("tr");
        if((row % 2) == 1)
          // Use alternating color CSS class to row
          stream.writeAttribute("class", "alt");

        for(int col = 0; col < rec.count(); ++col)
        {
          // Get data formatted as shown in the table
          int physIndex = visualColumnIndex[col];
          if(physIndex != -1)
            writeHtmlTableCellFormatted(stream, rec.fieldName(physIndex), rec.value(physIndex), row);
        }
        stream.writeEndElement(); // tr
        row++;
        exported++;
      }

      stream.writeEndElement(); // tbody
      stream.writeEndElement(); // table

      writeHtmlFooter(stream);
      writeHtmlEnd(stream);

      file.close();

      if(open)
        openDocument(filename);
    }
    else
      errorHandler->handleIOError(file);
  }
  return exported;
}

int HtmlExporter::exportSelected(bool open)
{
  int exported = 0;
  QString filename = saveHtmlFileDialog();

  if(!filename.isEmpty())
  {
    qDebug() << "exportSelectedHtml" << filename;

    QFile file(filename);
    if(file.open(QIODevice::WriteOnly))
    {
      const QItemSelection sel = controller->getSelection();

      QXmlStreamWriter stream(&file);
      stream.setAutoFormatting(true);
      stream.setAutoFormattingIndent(2);
      stream.setCodec(ll::constants::EXPORT_HTML_CODEC);

      // Get the CSS either from the resources or from the settings directory
      writeHtmlStart(stream,
                     atools::settings::Settings::getOverloadedPath(ll::constants::EXPORT_HTML_CSS_FILE));
      writeHtmlHeader(stream);

      stream.writeStartElement("table");
      stream.writeStartElement("tbody");

      QVector<const Column *> columnList = controller->getCurrentColumns();

      // Create an index that maps the (probably reordered) columns of the
      // view to the model
      QVector<int> visualColumnIndex;
      createVisualColumnIndex(columnList.size(), visualColumnIndex);
      writeHtmlTableHeader(stream, headerNames(columnList.size(), visualColumnIndex));

      for(QItemSelectionRange rng : sel)
        for(int row = rng.top(); row <= rng.bottom(); ++row)
        {
          stream.writeStartElement("tr");
          if((row % 2) == 1)
            // Write alternating color class
            stream.writeAttribute("class", "alt");

          QVariantList values = controller->getFormattedModelData(row);

          for(int col = 0; col < values.size(); ++col)
          {
            int physIndex = visualColumnIndex[col];
            if(physIndex != -1)
              writeHtmlTableCellRaw(stream, columnList.at(physIndex)->getColumnName(),
                                    values.at(physIndex).toString(), row);
          }

          stream.writeEndElement(); // tr
          exported++;
        }

      stream.writeEndElement(); // tbody
      stream.writeEndElement(); // table

      writeHtmlFooter(stream);
      writeHtmlEnd(stream);
      file.close();

      if(open)
        openDocument(filename);
    }
    else
      errorHandler->handleIOError(file);
  }
  return exported;
}

void HtmlExporter::writeHtmlStart(QXmlStreamWriter& stream, const QString& cssFilename)
{
  // Read CSS into a string
  QString css;
  QFile cssFile(cssFilename);
  if(cssFile.open(QIODevice::ReadOnly))
  {
    QTextStream is(&cssFile);
    css = "\n" + is.readAll();
    cssFile.close();
  }
  else
    errorHandler->handleIOError(cssFile);

  stream.writeStartDocument();
  stream.writeStartElement("html");
  stream.writeStartElement("head");

  stream.writeStartElement("meta");
  stream.writeAttribute("http-equiv", "content-type");
  stream.writeAttribute("content", QString("text/html; charset=%1").arg(ll::constants::EXPORT_HTML_CODEC));
  stream.writeEndElement(); // meta

  stream.writeTextElement("title", QApplication::applicationName());

  stream.writeTextElement("style", css);
  stream.writeEndElement(); // head

  stream.writeStartElement("body");
}

void HtmlExporter::writeHtmlEnd(QXmlStreamWriter& stream)
{
  stream.writeEndElement(); // body
  stream.writeEndElement(); // html
  stream.writeEndDocument();
}

void HtmlExporter::writeHtmlFooter(QXmlStreamWriter& stream)
{
  stream.writeStartElement("p");
  stream.writeAttribute("class", "footer");
  stream.writeCharacters(QString(tr("%1 Version %2 (revision %3) by Alexander Barthel. Exported on %4.")).
                         arg(QApplication::applicationName()).
                         arg(QApplication::applicationVersion()).
                         arg(GIT_REVISION).
                         arg(QDateTime::currentDateTime().toString(Qt::DefaultLocaleLongDate)));
  stream.writeEndElement(); // p
}

void HtmlExporter::writeHtmlHeader(QXmlStreamWriter& stream)
{
  stream.writeTextElement("h1", tr("FSX Logbook"));
}

void HtmlExporter::writeHtmlTableHeader(QXmlStreamWriter& stream, const QStringList& names)
{
  stream.writeStartElement("tr");
  for(QString name : names)
    stream.writeTextElement("th", name.replace("\n", " "));
  stream.writeEndElement(); // tr
}

void HtmlExporter::writeHtmlTableCellFormatted(QXmlStreamWriter& stream,
                                               const QString& fieldName,
                                               QVariant value,
                                               int row)
{
  QString fmtVal = controller->formatModelData(fieldName, value);
  writeHtmlTableCellRaw(stream, fieldName, fmtVal, row);
}

void HtmlExporter::writeHtmlTableCellRaw(QXmlStreamWriter& stream,
                                         const QString& fieldName,
                                         const QString& value,
                                         int row)
{
  stream.writeStartElement("td");
  if(fieldName == controller->getSortColumn())
  {
    // Change table field background color to darker if it is the sorting
    // columns
    if((row % 2) == 1)
      stream.writeAttribute("class", "sort");
    else
      stream.writeAttribute("class", "sortalt");
  }
  stream.writeCharacters(value);
  stream.writeEndElement();
}
