/*****************************************************************************
* Copyright 2015-2016 Alexander Barthel albar965@mailbox.org
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

#ifndef LITTLELOGBOOK_HTMLEXPORTER_H
#define LITTLELOGBOOK_HTMLEXPORTER_H

#include "export/exporter.h"

#include <QObject>

class Controller;
class QWidget;
class QXmlStreamWriter;

/*
 * Allows to export the table content or the selected table content from the
 * given controller into HTML files. Uses a CSS file from the resources to
 * format file.
 */
class HtmlExporter :
  public Exporter
{
  Q_OBJECT

public:
  HtmlExporter(QWidget *parentWidget, Controller *controller);
  virtual ~HtmlExporter();

  /* Export all rows.
   *
   * @param open Open file in default browser after export.
   * @return number of rows exported.
   */
  virtual int exportAll(bool open);

  /* Export only selected rows.
   *
   * @param open Open file in default browser after export.
   * @return number of rows exported.
   */
  virtual int exportSelected(bool open);

private:
  /* Get filename from save dialog */
  QString saveHtmlFileDialog();

  /* Write head and start of body */
  void writeHtmlStart(QXmlStreamWriter& stream, const QString& cssFilename);

  /* End body and html */
  void writeHtmlEnd(QXmlStreamWriter& stream);

  /* Write heading */
  void writeHtmlHeader(QXmlStreamWriter& stream);

  /* Write table header tr and th */
  void writeHtmlTableHeader(QXmlStreamWriter& stream, const QStringList& names);

  /* Write footer (exported on, etc.) */
  void writeHtmlFooter(QXmlStreamWriter& stream);

  /* Write td as is */
  void writeHtmlTableCellRaw(QXmlStreamWriter& stream,
                             const QString& fieldName,
                             const QString& value,
                             int row);

  /* Write td and format it using the controller */
  void writeHtmlTableCellFormatted(QXmlStreamWriter& stream,
                                   const QString& fieldName,
                                   QVariant value,
                                   int row);

};

#endif // LITTLELOGBOOK_HTMLEXPORTER_H
