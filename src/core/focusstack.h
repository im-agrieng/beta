/***************************************************************************
  focusstack.h

 ---------------------
 begin                : 11.4.2018
 copyright            : (C) 2018 by david
 email                : david at opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef FOCUSSTACK_H
#define FOCUSSTACK_H

#include <QList>
#include <QQuickItem>

/**
 * \ingroup core
 */
class FocusStack : public QObject
{
    Q_OBJECT

  public:
    Q_INVOKABLE void addFocusTaker( QObject *object );
    Q_INVOKABLE void forceActiveFocusOnLastTaker() const;

  private slots:
    void visibleChanged();
    void popupOpened();
    void popupClosed();

  private:
    QList<QObject *> mStackList;
    void setFocused( QObject *object );
    void setUnfocused( QObject *object );
};

#endif // FOCUSSTACK_H
