#ifndef PRINTMANAGER_H
#define PRINTMANAGER_H

#include <QObject>
#include <QtPrintSupport/QPrinter>
#include <QQmlEngine>
#include <QQuickTextDocument>

class PrintManager : public QObject {
		Q_OBJECT
	public:
		PrintManager( QObject *parent = 0 ) : QObject( parent ) {}
		
		Q_INVOKABLE void printTextDocument( QQuickTextDocument* document );
	signals:
		
	public slots:
		
	private:
		QPrinter printer;
		
		bool showPrintDialog( const QString& windowTitle );
};

//Define the singleton type provider function
static QObject* printManager_provider( QQmlEngine* engine, QJSEngine* scriptEngine ) {
	Q_UNUSED( engine )
	Q_UNUSED( scriptEngine )
	
	PrintManager* pm = new PrintManager();
	return pm;
}

#endif // PRINTMANAGER_H
