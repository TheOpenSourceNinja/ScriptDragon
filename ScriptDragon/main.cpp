#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QString>

#include "notecarddata.h"
#include "notecardmanager.h"
#include "exportmanager.h"
#include "filesaverandloader.h"
#include "namegenerator.h"
#include "reportgenerator.h"
#include "scriptformatter.h"
#include "settingsmanager.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);
	app.setOrganizationDomain( "theopensource.ninja" );
	app.setApplicationName( "ScriptDragon" );
	
	uint_fast8_t version[] = { 1, 0 };
	
	QString uri( "ninja.theopensource.scriptdragon" );
	
	qmlRegisterType<NotecardData>( uri.toStdString().c_str(), version[ 0 ], version[ 1 ], "NotecardData" );
	qmlRegisterSingletonType<ExportManager>( uri.toStdString().c_str(), version[ 0 ], version[ 1 ], "ExportManager", exportManager_provider );
	qmlRegisterSingletonType<NotecardManager>( uri.toStdString().c_str(), version[ 0 ], version[ 1 ], "NotecardManager", notecardManager_provider );
	qmlRegisterSingletonType<ScriptFormatter>( uri.toStdString().c_str(), version[ 0 ], version[ 1 ], "ScriptFormatter", scriptFormatter_provider );
	qmlRegisterSingletonType<FileSaverAndLoader>( uri.toStdString().c_str(), version[ 0 ], version[ 1 ], "FileSaverAndLoader", fileSaverAndLoader_provider );
	qmlRegisterSingletonType<ReportGenerator>( uri.toStdString().c_str(), version[ 0 ], version[ 1 ], "ReportGenerator", reportGenerator_provider );
	qmlRegisterType<NameGenerator>( uri.toStdString().c_str(), version[ 0 ], version[ 1 ], "NameGenerator" );
	qmlRegisterSingletonType<SettingsManager>( uri.toStdString().c_str(), version[ 0 ], version[ 1 ], "SettingsManager", settingsManager_provider );
	
	QQuickView view;
	view.setSource( QUrl( QStringLiteral( "qrc:///Main.qml" ) ) );
	view.setResizeMode( QQuickView::SizeRootObjectToView );
	view.show();
	return app.exec();
}

