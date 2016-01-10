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

using atools::settings::Settings;
using atools::fs::SimulatorType;

PathSettings::PathSettings()
{
  nullTime.setMSecsSinceEpoch(0L);
  for(int i = 0; i < 4; i++)
  {
    simulators.append(false);

    logbookPaths.append(QString());
    logbookTimestamps.append(nullTime);

    runwayPaths.append(QString());
    runwayTimestamps.append(nullTime);
  }
}

bool PathSettings::hasSimulator(SimulatorType type)
{
  return simulators.at(type);
}

QString PathSettings::getLogbookFile(SimulatorType type)
{
  return logbookPaths.at(type);
}

QString PathSettings::getRunwaysFile(SimulatorType type)
{
  return runwayPaths.at(type);
}

bool PathSettings::isLogbookFileValid(SimulatorType type)
{
  QFileInfo fi = logbookPaths.at(type);
  return fi.exists() && fi.isReadable() && fi.isFile();
}

bool PathSettings::isRunwaysFileValid(SimulatorType type)
{
  QFileInfo fi = runwayPaths.at(type);
  return fi.exists() && fi.isReadable() && fi.isFile();
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
  Settings::instance()->setValue(settingsLogbookTimestamps.at(type), time.toMSecsSinceEpoch());
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
  Settings::instance()->setValue(settingsRunwayTimestamps.at(type), time.toMSecsSinceEpoch());
  Settings::instance().syncSettings();
}

bool PathSettings::hasLogbookFileChanged(SimulatorType type)
{
  QFileInfo fi = logbookPaths.at(type);
  if(fi.exists() && fi.isReadable() && fi.isFile())
    return logbookTimestamps.at(type) != fi.lastModified();
  else
    return false;
}

bool PathSettings::hasRunwaysFileChanged(SimulatorType type)
{
  QFileInfo fi = runwayPaths.at(type);
  if(fi.exists() && fi.isReadable() && fi.isFile())
    return runwayTimestamps.at(type) != fi.lastModified();
  else
    return false;
}

void PathSettings::invalidateLogbookFile(SimulatorType type)
{
  logbookTimestamps[type] = nullTime;
  Settings::instance()->setValue(settingsLogbookTimestamps.at(type), nullTime.toMSecsSinceEpoch());
  Settings::instance().syncSettings();

}

void PathSettings::invalidateRunwaysFile(SimulatorType type)
{
  runwayTimestamps[type] = nullTime;
  Settings::instance()->setValue(settingsRunwayTimestamps.at(type), nullTime.toMSecsSinceEpoch());
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

    logbookPaths[type] = s->value(settingsLogbookFilePaths.at(type)).toString();
    logbookTimestamps[type].setMSecsSinceEpoch(s->value(settingsLogbookTimestamps.at(type)).toLongLong());

    runwayPaths[type] = s->value(settingsRunwayFilePaths.at(type)).toString();
    runwayTimestamps[type].setMSecsSinceEpoch(s->value(settingsRunwayTimestamps.at(type)).toLongLong());
  }
  else
    simulators[type] = false;
}

void PathSettings::storeSim(SimulatorType type)
{
  Settings& s = Settings::instance();

  if(simulators.at(type))
  {
    s->setValue(settingsLogbookFilePaths.at(type), logbookPaths.at(type));
    s->setValue(settingsLogbookTimestamps.at(type), logbookTimestamps.at(type).toMSecsSinceEpoch());

    s->setValue(settingsRunwayFilePaths.at(type), runwayPaths.at(type));
    s->setValue(settingsRunwayTimestamps.at(type), runwayTimestamps.at(type).toMSecsSinceEpoch());
  }
}
