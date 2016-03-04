#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>

#include "notecarddata.h"
#include "printmanager.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);
	
	qmlRegisterType<NotecardData>( "ninja.theopensource.scriptdragon", 1, 0, "NotecardData" );
	qmlRegisterSingletonType<PrintManager>( "ninja.theopensource.scriptdragon", 1, 0, "PrintManager", printManager_provider );
	
	QQuickView view;
	view.setSource(QUrl(QStringLiteral("qrc:///Main.qml")));
	view.setResizeMode(QQuickView::SizeRootObjectToView);
	view.show();
	return app.exec();
}

