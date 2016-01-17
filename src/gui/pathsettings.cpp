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

#include "gui/pathsettings.h"
#include "settings/settings.h"

#include <QFileInfo>
#include <QSettings>

const char *PathSettings::SETTINGS_LOGBOOK_PATHS[NUM_SIMULATOR_TYPES] =
{
  "Paths/FileLogbookFsx", "Paths/FileLogbookFsxSe",
  "Paths/FileLogbookP3dV2", "Paths/FileLogbookP3dV3"
};

const char *PathSettings::SETTINGS_RUNWAY_PATHS[NUM_SIMULATOR_TYPES] =
{
  "Paths/FileRunwaysFsx", "Paths/FileRunwaysFsxSe",
  "Paths/FileRunwaysP3dV2", "Paths/FileRunwaysP3dV3"
};

const char *PathSettings::SETTINGS_LOGBOOK_TIMESTAMPS[NUM_SIMULATOR_TYPES] =
{
  "Paths/TimestampLogbookFsx", "Paths/TimestampLogbookFsxSe",
  "Paths/TimestampLogbookP3dV2", "Paths/TimestampLogbookP3dV3"
};

const char *PathSettings::SETTINGS_RUNWAY_TIMESTAMPS[NUM_SIMULATOR_TYPES] =
{
  "Paths/TimestampRunwaysFsx", "Paths/TimestampRunwaysFsxSe",
  "Paths/TimestampRunwaysP3dV2", "Paths/TimestampRunwaysP3dV3"
};

using atools::settings::Settings;
using atools::fs::SimulatorType;

PathSettings::PathSettings()
{
  nullTime.setMSecsSinceEpoch(0L);
  for(int i = 0; i < NUM_SIMULATOR_TYPES; i++)
  {
    simulators[i] = false;

    logbookPaths.append(QString());
    logbookTimestamps.append(nullTime);

    runwayPaths.append(QString());
    runwayTimestamps.append(nullTime);
  }
}

QString PathSettings::getSimulatorName(atools::fs::SimulatorType type)
{
  switch(type)
  {
    case atools::fs::FSX:
      return QObject::tr("FSX Boxed");

    case atools::fs::FSX_SE:
      return QObject::tr("FSX Steam Edition");

    case atools::fs::P3D_V2:
      return QObject::tr("Prepar3d V2");

    case atools::fs::P3D_V3:
      return QObject::tr("Prepar3d V3");

    case atools::fs::ALL_SIMULATORS:
      return QString();
  }
  return QString();
}

bool PathSettings::hasSimulator(SimulatorType type) const
{
  return simulators[type];
}

QString PathSettings::getLogbookFile(SimulatorType type) const
{
  return logbookPaths.at(type);
}

QString PathSettings::getRunwaysFile(SimulatorType type) const
{
  return runwayPaths.at(type);
}

bool PathSettings::isLogbookFileValid(SimulatorType type) const
{
  QFileInfo fi = logbookPaths.at(type);
  return fi.exists() && fi.isReadable() && fi.isFile();
}

bool PathSettings::isRunwaysFileValid(SimulatorType type) const
{
  QFileInfo fi = runwayPaths.at(type);
  return fi.exists() && fi.isReadable() && fi.isFile();
}

bool PathSettings::isAnyRunwaysFileValid() const
{
  for(SimulatorType type : atools::fs::ALL_SIMULATOR_TYPES)
    if(isRunwaysFileValid(type))
      return true;

  return false;
}

void PathSettings::setLogbookFileLoaded(SimulatorType type)
{
  QFileInfo fi = logbookPaths.at(type);
  QDateTime time;
  if(fi.exists() && fi.isReadable() && fi.isFile())
    time = fi.lastModified();
  else
    time.setMSecsSinceEpoch(0L);

  logbookTimestamps[type] = time;
  Settings::instance()->setValue(SETTINGS_LOGBOOK_TIMESTAMPS[type], time.toMSecsSinceEpoch());
  Settings::instance().syncSettings();
}

void PathSettings::setRunwaysFileLoaded(SimulatorType type)
{
  QFileInfo fi = runwayPaths.at(type);
  QDateTime time;
  if(fi.exists() && fi.isReadable() && fi.isFile())
    time = fi.lastModified();
  else
    time.setMSecsSinceEpoch(0L);

  runwayTimestamps[type] = time;
  Settings::instance()->setValue(SETTINGS_RUNWAY_TIMESTAMPS[type], time.toMSecsSinceEpoch());
  Settings::instance().syncSettings();
}

bool PathSettings::hasAnyLogbookFileChanged() const
{
  for(SimulatorType type : atools::fs::ALL_SIMULATOR_TYPES)
    if(hasLogbookFileChanged(type))
      return true;

  return false;
}

bool PathSettings::hasAnyRunwaysFileChanged() const
{
  for(SimulatorType type : atools::fs::ALL_SIMULATOR_TYPES)
    if(hasRunwaysFileChanged(type))
      return true;

  return false;
}

bool PathSettings::hasLogbookFileChanged(SimulatorType type) const
{
  QFileInfo fi = logbookPaths.at(type);
  if(fi.exists() && fi.isReadable() && fi.isFile())
    return logbookTimestamps.at(type) != fi.lastModified();
  else
    return false;
}

bool PathSettings::hasRunwaysFileChanged(SimulatorType type) const
{
  QFileInfo fi = runwayPaths.at(type);
  if(fi.exists() && fi.isReadable() && fi.isFile())
    return runwayTimestamps.at(type) != fi.lastModified();
  else
    return false;
}

void PathSettings::invalidateAllLogbookFiles()
{
  for(SimulatorType type : atools::fs::ALL_SIMULATOR_TYPES)
    invalidateLogbookFile(type);
}

void PathSettings::invalidateAllRunwayFiles()
{
  for(SimulatorType type : atools::fs::ALL_SIMULATOR_TYPES)
    invalidateRunwaysFile(type);
}

void PathSettings::invalidateLogbookFile(SimulatorType type)
{
  logbookTimestamps[type] = nullTime;
  Settings::instance()->setValue(SETTINGS_LOGBOOK_TIMESTAMPS[type], nullTime.toMSecsSinceEpoch());
  Settings::instance().syncSettings();
}

void PathSettings::invalidateRunwaysFile(SimulatorType type)
{
  runwayTimestamps[type] = nullTime;
  Settings::instance()->setValue(SETTINGS_RUNWAY_TIMESTAMPS[type], nullTime.toMSecsSinceEpoch());
  Settings::instance().syncSettings();
}

void PathSettings::writeSettings()
{
  storeSim(atools::fs::FSX);
  storeSim(atools::fs::FSX_SE);
  storeSim(atools::fs::P3D_V2);
  storeSim(atools::fs::P3D_V3);

  Settings::instance().syncSettings();
}

void PathSettings::readSettings()
{
  loadSim(atools::fs::FSX);
  loadSim(atools::fs::FSX_SE);
  loadSim(atools::fs::P3D_V2);
  loadSim(atools::fs::P3D_V3);
}

void PathSettings::loadSim(SimulatorType type)
{
  Settings& s = Settings::instance();

  if(!atools::fs::FsPaths::getBasePath(type).isEmpty())
  {
    simulators[type] = true;

    logbookPaths[type] = s->value(SETTINGS_LOGBOOK_PATHS[type]).toString();
    logbookTimestamps[type].setMSecsSinceEpoch(s->value(SETTINGS_LOGBOOK_TIMESTAMPS[type]).toLongLong());

    runwayPaths[type] = s->value(SETTINGS_RUNWAY_PATHS[type]).toString();
    runwayTimestamps[type].setMSecsSinceEpoch(s->value(SETTINGS_RUNWAY_TIMESTAMPS[type]).toLongLong());
  }
  else
    simulators[type] = false;
}

void PathSettings::storeSim(SimulatorType type)
{
  Settings& s = Settings::instance();

  if(simulators[type])
  {
    s->setValue(SETTINGS_LOGBOOK_PATHS[type], logbookPaths.at(type));
    s->setValue(SETTINGS_LOGBOOK_TIMESTAMPS[type], logbookTimestamps.at(type).toMSecsSinceEpoch());

    s->setValue(SETTINGS_RUNWAY_PATHS[type], runwayPaths.at(type));
    s->setValue(SETTINGS_RUNWAY_TIMESTAMPS[type], runwayTimestamps.at(type).toMSecsSinceEpoch());
  }
}
