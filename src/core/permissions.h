/***************************************************************************
 permissions.h - CameraPermission, MicrophonePermission

 ---------------------
 begin                : 01.12.2023
 copyright            : (C) 2023 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include <QObject>
#include <QPermission>

/**
 * \ingroup core
 */
class CameraPermission : public QObject
{
    Q_OBJECT

    Q_PROPERTY( Qt::PermissionStatus status READ status NOTIFY statusChanged )

  public:
    explicit CameraPermission( QObject *parent = nullptr );

    /**
     * Returns the permission status.
     */
    Qt::PermissionStatus status();

    /**
     * Requests the permission.
     */
    Q_INVOKABLE void request();

  signals:
    /**
     * Emitted when the status has changed.
     */
    void statusChanged();

  private:
    Qt::PermissionStatus mStatus = Qt::PermissionStatus::Undetermined;
    bool mStatusChecked = false;
};

/**
 * \ingroup core
 */
class MicrophonePermission : public QObject
{
    Q_OBJECT

    Q_PROPERTY( Qt::PermissionStatus status READ status NOTIFY statusChanged )

  public:
    explicit MicrophonePermission( QObject *parent = nullptr );

    /**
     * Returns the permission status.
     */
    Qt::PermissionStatus status();

    /**
     * Requests the permission.
     */
    Q_INVOKABLE void request();

  signals:
    /**
     * Emitted when the status has changed.
     */
    void statusChanged();

  private:
    Qt::PermissionStatus mStatus = Qt::PermissionStatus::Undetermined;
    bool mStatusChecked = false;
};

#endif // PERMISSIONS_H
