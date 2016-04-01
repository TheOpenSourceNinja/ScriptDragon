#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>

#include "notecarddata.h"
#include "notecardmanager.h"
#include "exportmanager.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);
	
	qmlRegisterType<NotecardData>( "ninja.theopensource.scriptdragon", 1, 0, "NotecardData" );
	qmlRegisterSingletonType<ExportManager>( "ninja.theopensource.scriptdragon", 1, 0, "ExportManager", exportManager_provider );
	qmlRegisterSingletonType<NotecardManager>( "ninja.theopensource.scriptdragon", 1, 0, "NotecardManager", notecardManager_provider );
	
	QQuickView view;
	view.setSource( QUrl( QStringLiteral( "qrc:///Main.qml" ) ) );
	view.setResizeMode( QQuickView::SizeRootObjectToView );
	view.show();
	return app.exec();
}

