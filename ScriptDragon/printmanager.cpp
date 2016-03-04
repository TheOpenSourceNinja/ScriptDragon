#include "printmanager.h"
#include <QtPrintSupport/QPrintDialog>
#include <iostream>

Q_INVOKABLE void PrintManager::printTextDocument( QQuickTextDocument* document ) {
	std::cout << "printTextDocument() called" << std::endl;
	std::cout << document->textDocument()->toPlainText().toStdString() << std::endl;
	//bool shouldPrint = showPrintDialog( "Print script" );
	bool shouldPrint = true;
	if( shouldPrint ) {
		document->textDocument()->print( &printer );
	}
}

bool PrintManager::showPrintDialog( const QString& windowTitle ) {
	QPrintDialog dialog(&printer);
	dialog.setWindowTitle( windowTitle );
	
	if( dialog.exec() == QDialog::Accepted ) {
		return true;
	} else {
		return false;
	}
}
