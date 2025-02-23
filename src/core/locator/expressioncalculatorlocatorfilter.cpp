/***************************************************************************
  expressioncalculatorlocatorfilter.cpp

 ---------------------
 begin                : 11.04.2023
 copyright            : (C) 2023 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "expressioncalculatorlocatorfilter.h"
#include "locatormodelsuperbridge.h"
#include "platformutilities.h"

#include <QAction>
#include <QRegularExpression>
#include <qgscoordinatereferencesystemutils.h>
#include <qgscoordinateutils.h>
#include <qgsexpressioncontextutils.h>
#include <qgsfeedback.h>
#include <qgspoint.h>
#include <qgsproject.h>


ExpressionCalculatorLocatorFilter::ExpressionCalculatorLocatorFilter( LocatorModelSuperBridge *locatorBridge, QObject *parent )
  : QgsLocatorFilter( parent )
  , mLocatorBridge( locatorBridge )
{
  setUseWithoutPrefix( false );
}

ExpressionCalculatorLocatorFilter *ExpressionCalculatorLocatorFilter::clone() const
{
  return new ExpressionCalculatorLocatorFilter( mLocatorBridge );
}

void ExpressionCalculatorLocatorFilter::fetchResults( const QString &string, const QgsLocatorContext &, QgsFeedback * )
{
  QgsExpressionContext context;
  context << QgsExpressionContextUtils::globalScope()
          << QgsExpressionContextUtils::projectScope( QgsProject::instance() );

  QString error;
  if ( QgsExpression::checkExpression( string, &context, error ) )
  {
    QgsExpression exp( string );
    const QString resultString = exp.evaluate( &context ).toString();
    if ( !resultString.isEmpty() )
    {
      QgsLocatorResult result;
      result.filter = this;
      result.displayString = tr( "Copy “%1” to clipboard" ).arg( resultString );
#if _QGIS_VERSION_INT >= 33300
      result.setUserData( resultString );
#else
      result.userData = resultString;
#endif
      result.score = 1;
      emit resultFetched( result );
    }
  }

  return;
}

void ExpressionCalculatorLocatorFilter::triggerResult( const QgsLocatorResult &result )
{
  triggerResultFromAction( result, Normal );
}

void ExpressionCalculatorLocatorFilter::triggerResultFromAction( const QgsLocatorResult &result, const int actionId )
{
#if _QGIS_VERSION_INT >= 33601
  QString resultString = result.userData().toString();
#else
  QString resultString = result.getUserData().toString();
#endif

  switch ( actionId )
  {
    case Normal:
    {
      PlatformUtilities::instance()->copyTextToClipboard( resultString );
    }
  }

  return;
}
