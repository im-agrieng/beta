/***************************************************************************
 sensorlistmodel.cpp
 ---------------------
 begin                : 25.03.2023
 copyright            : (C) 2023 by Mathieu Pellerin
 email                : mathieu at opengisch dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "sensorlistmodel.h"

#include <qgis.h>
#include <qgsabstractsensor.h>
#include <qgsproject.h>
#include <qgssensormanager.h>

SensorListModel::SensorListModel( QObject *parent )
  : QSortFilterProxyModel( parent )
{
}

QHash<int, QByteArray> SensorListModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
#if _QGIS_VERSION_INT >= 33500
  roles[static_cast<int>( QgsSensorModel::CustomRole::SensorId )] = "SensorId";
  roles[static_cast<int>( QgsSensorModel::CustomRole::SensorType )] = "SensorType";
  roles[static_cast<int>( QgsSensorModel::CustomRole::SensorName )] = "SensorName";
  roles[static_cast<int>( QgsSensorModel::CustomRole::SensorStatus )] = "SensorStatus";
  roles[static_cast<int>( QgsSensorModel::CustomRole::SensorLastValue )] = "SensorLastValue";
  roles[static_cast<int>( QgsSensorModel::CustomRole::SensorLastTimestamp )] = "SensorLastTimestamp";
#else
  roles[QgsSensorModel::SensorId] = "SensorId";
  roles[QgsSensorModel::SensorType] = "SensorType";
  roles[QgsSensorModel::SensorName] = "SensorName";
  roles[QgsSensorModel::SensorStatus] = "SensorStatus";
  roles[QgsSensorModel::SensorLastValue] = "SensorLastValue";
  roles[QgsSensorModel::SensorLastTimestamp] = "SensorLastTimestamp";
#endif
  return roles;
}

QgsProject *SensorListModel::project() const
{
  return mProject;
}

void SensorListModel::setProject( QgsProject *project )
{
  if ( mProject == project )
    return;

  if ( mProject )
  {
    disconnect( mProject->sensorManager(), &QgsSensorManager::sensorErrorOccurred, this, &SensorListModel::handleSensorError );
  }

  mProject = project;
  emit projectChanged();

  if ( mProject )
  {
    connect( mProject->sensorManager(), &QgsSensorManager::sensorErrorOccurred, this, &SensorListModel::handleSensorError );
    mSensorModel.reset( new QgsSensorModel( mProject->sensorManager() ) );
  }
  else
  {
    mSensorModel.reset( nullptr );
  }

  setSourceModel( mSensorModel.get() );
}

void SensorListModel::setShowConnectedOnly( bool showConnectedOnly )
{
  if ( mShowConnectedOnly == showConnectedOnly )
    return;

  mShowConnectedOnly = showConnectedOnly;
  emit showConnectedOnlyChanged();

  invalidateFilter();
}

bool SensorListModel::filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const
{
  if ( mShowConnectedOnly )
  {
#if _QGIS_VERSION_INT >= 33500
    return mSensorModel->data( mSensorModel->index( source_row, 0 ), static_cast<int>( QgsSensorModel::CustomRole::SensorStatus ) ).value<Qgis::DeviceConnectionStatus>() == Qgis::DeviceConnectionStatus::Connected;
#else
    return mSensorModel->data( mSensorModel->index( source_row, 0 ), QgsSensorModel::SensorStatus ).value<Qgis::DeviceConnectionStatus>() == Qgis::DeviceConnectionStatus::Connected;
#endif
  }

  return true;
}

void SensorListModel::connectSensorId( const QString &id ) const
{
  if ( mProject && mProject->sensorManager()->sensor( id ) )
  {
    mProject->sensorManager()->sensor( id )->connectSensor();
  }
}

void SensorListModel::disconnectSensorId( const QString &id ) const
{
  if ( mProject && mProject->sensorManager()->sensor( id ) )
  {
    mProject->sensorManager()->sensor( id )->disconnectSensor();
  }
}

void SensorListModel::handleSensorError( const QString &id )
{
  if ( mProject )
  {
    if ( QgsAbstractSensor *sensor = mProject->sensorManager()->sensor( id ) )
    {
      emit sensorErrorOccurred( QStringLiteral( "%1: %2" ).arg( sensor->name(), sensor->errorString() ) );
    }
  }
}
