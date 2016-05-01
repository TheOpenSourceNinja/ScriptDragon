#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QSettings>

class SettingsManager : public QObject
{
		Q_OBJECT
	public:
		explicit SettingsManager(QObject *parent = 0);
		Q_INVOKABLE void setValue( const QString& key, const QVariant& value );
		Q_INVOKABLE QVariant getValue( const QString& key, const QVariant& defaultValue = QVariant() );
		
	signals:
		
	public slots:
		
	private:
		QSettings settings;
};

//Define the singleton type provider function
static QObject* settingsManager_provider( QQmlEngine* newEngine, QJSEngine* scriptEngine ) {
	Q_UNUSED( scriptEngine )
	
	SettingsManager* sm = new SettingsManager( newEngine );
	return sm;
}

#endif // SETTINGSMANAGER_H
