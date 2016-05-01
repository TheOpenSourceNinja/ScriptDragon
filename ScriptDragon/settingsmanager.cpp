#include "settingsmanager.h"

SettingsManager::SettingsManager( QObject *parent ) : QObject( parent ) {
	
}

void SettingsManager::setValue( const QString& key, const QVariant& value ) {
	settings.setValue( key, value );
}

QVariant SettingsManager::getValue( const QString& key, const QVariant& defaultValue ) {
	return settings.value( key, defaultValue );
}

