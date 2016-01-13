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

#ifndef PATHSETTINGS_H
#define PATHSETTINGS_H

#include "fs/fspaths.h"

#include <QDateTime>
#include <QList>

class PathSettings
{
public:
  PathSettings();

  static QString getSimulatorName(atools::fs::SimulatorType type);

  bool hasSimulator(atools::fs::SimulatorType type) const;
  QString getLogbookFile(atools::fs::SimulatorType type) const;
  QString getRunwaysFile(atools::fs::SimulatorType type) const;

  void setLogbookFileLoaded(atools::fs::SimulatorType type);
  void setRunwaysFileLoaded(atools::fs::SimulatorType type);

  bool hasLogbookFileChanged(atools::fs::SimulatorType type) const;
  bool hasRunwaysFileChanged(atools::fs::SimulatorType type) const;

  bool isLogbookFileValid(atools::fs::SimulatorType type) const;
  bool isRunwaysFileValid(atools::fs::SimulatorType type) const;

  void invalidateLogbookFile(atools::fs::SimulatorType type);
  void invalidateRunwaysFile(atools::fs::SimulatorType type);

  bool isOneLogbookFileValid() const;

  // bool hasSomeLogbookFileChanged() const;
  // bool hasSomeRunwaysFileChanged() const;

  // bool areAllLogbookFilesValid() const;
  // bool areAllRunwaysFilesValid() const;

  void writeSettings();
  void readSettings();

  bool hasAnyLogbookFileChanged() const;
  bool hasAnyRunwaysFileChanged() const;

  bool isAnyLogbookFileValid() const;
private:
  friend class PathDialog;

  const char *SETTINGS_LOGBOOK_PATHS[NUM_SIMULATOR_TYPES] =
  {
    "Paths/FileLogbookFsx", "Paths/FileLogbookFsxSe",
    "Paths/FileLogbookP3dV2", "Paths/FileLogbookP3dV3"
  };

  const char *SETTINGS_RUNWAY_PATHS[NUM_SIMULATOR_TYPES] =
  {
    "Paths/FileRunwaysFsx", "Paths/FileRunwaysFsxSe",
    "Paths/FileRunwaysP3dV2", "Paths/FileRunwaysP3dV3"
  };

  const char *SETTINGS_LOGBOOK_TIMESTAMPS[NUM_SIMULATOR_TYPES] =
  {
    "Paths/TimestampLogbookFsx", "Paths/TimestampLogbookFsxSe",
    "Paths/TimestampLogbookP3dV2", "Paths/TimestampLogbookP3dV3"
  };

  const char *SETTINGS_RUNWAY_TIMESTAMPS[NUM_SIMULATOR_TYPES] =
  {
    "Paths/TimestampRunwaysFsx", "Paths/TimestampRunwaysFsxSe",
    "Paths/TimestampRunwaysP3dV2", "Paths/TimestampRunwaysP3dV3"
  };

  bool simulators[NUM_SIMULATOR_TYPES] = {false, false, false, false};

  QStringList logbookPaths;
  QStringList runwayPaths;

  QList<QDateTime> logbookTimestamps;
  QList<QDateTime> runwayTimestamps;
  QDateTime nullTime;

  void storeSim(atools::fs::SimulatorType type);
  void loadSim(atools::fs::SimulatorType type);

};

#endif // PATHSETTINGS_H
