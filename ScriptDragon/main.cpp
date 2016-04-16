#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>

#include "notecarddata.h"
#include "notecardmanager.h"
#include "exportmanager.h"
#include "filesaverandloader.h"
#include "reportgenerator.h"
#include "scriptformatter.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);
	
	uint_fast8_t version[] = { 1, 0 };
	
	qmlRegisterType<NotecardData>( "ninja.theopensource.scriptdragon", version[ 0 ], version[ 1 ], "NotecardData" );
	qmlRegisterSingletonType<ExportManager>( "ninja.theopensource.scriptdragon", version[ 0 ], version[ 1 ], "ExportManager", exportManager_provider );
	qmlRegisterSingletonType<NotecardManager>( "ninja.theopensource.scriptdragon", version[ 0 ], version[ 1 ], "NotecardManager", notecardManager_provider );
	qmlRegisterSingletonType<ScriptFormatter>( "ninja.theopensource.scriptdragon", version[ 0 ], version[ 1 ], "ScriptFormatter", scriptFormatter_provider );
	qmlRegisterSingletonType<FileSaverAndLoader>( "ninja.theopensource.scriptdragon", version[ 0 ], version[ 1 ], "FileSaverAndLoader", fileSaverAndLoader_provider );
	qmlRegisterSingletonType<ReportGenerator>( "ninja.theopensource.scriptdragon", version[ 0 ], version[ 1 ], "ReportGenerator", reportGenerator_provider );
	
	QQuickView view;
	view.setSource( QUrl( QStringLiteral( "qrc:///Main.qml" ) ) );
	view.setResizeMode( QQuickView::SizeRootObjectToView );
	view.show();
	return app.exec();
}

