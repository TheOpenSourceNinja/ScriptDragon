#include "printmanager.h"
#include <QPagedPaintDevice>
#include <QtPrintSupport/QPrintDialog>
#include <iostream>

void PrintManager::common( const QString& docName ) {
	printer.setCreator( "ScriptDragon" );
	printer.setDocName( docName );
}

Q_INVOKABLE void PrintManager::textDocumentToPrintout( QQuickTextDocument* document ) {
	printer.setOutputFileName( "" ); //Disable printing to file because printing to file causes printerName() to be blank.
	
	common( "Script" );
	
	//bool shouldPrint = showPrintDialog( "Print script" ); //FIXME: Uncomment this line to cause an error.
	bool shouldPrint = true;
	
	if( shouldPrint ) {
		document->textDocument()->print( &printer );
	}
}

Q_INVOKABLE void PrintManager::textDocumentToPDF( QQuickTextDocument* document ) {
	//-----begin page size workaround part 1-----
	printer.setOutputFileName( "" ); //Disable printing to file so we can get the default printer's default page size.
	QPagedPaintDevice::PageSize pageSize = printer.pageSize();
	//-----end page size workaround part 1-----
	printer.setOutputFileName( "testfilename.pdf" ); //Enable printing to file. This should be the first line of the function.
	//-----begin page size workaround part 2-----
	printer.setPageSize( pageSize );
	//-----end page size workaround part 2-----
	
	common( "Script" );
	
	document->textDocument()->print( &printer );
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
