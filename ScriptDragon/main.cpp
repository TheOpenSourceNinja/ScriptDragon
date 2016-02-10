#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>

#include "notecarddata.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);
	
	qmlRegisterType<NotecardData>( "ninja.theopensource.scriptdragon", 1, 0, "NotecardData" );
	
	
	QQuickView view;
	view.setSource(QUrl(QStringLiteral("qrc:///Main.qml")));
	view.setResizeMode(QQuickView::SizeRootObjectToView);
	view.show();
	return app.exec();
}

