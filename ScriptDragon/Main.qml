import QtQuick 2.4
import Ubuntu.Components 1.3

/*!
	\brief MainView has tabs, tabs link to other files.
*/

MainView {
	// objectName for functional testing purposes (autopilot-qt5)
	objectName: "mainView"
	
	// Note! applicationName needs to match the "name" field of the click manifest
	applicationName: "scriptdragon.dearingj"
	
	/* 
	 This property enables the application to change orientation 
	 when the device is rotated. The default is false.
	*/
	automaticOrientation: true
	
	
	width: units.gu(75)
	height: units.gu(100)
	
	Tabs {
		id: tabs
		
		Tab {
			id: welcomeTab
			title: i18n.tr( "Welcome" )
			
			WelcomePage {}
		}
		
		Tab {
			id: notecardsTab
			title: i18n.tr( "Notecards" )
			
			NotecardsPage {}
		}
		
		Tab {
			id: scriptTab
			title: i18n.tr( "Script" )
			
			ScriptPage {}
		}
		
		Tab {
			id: reportsTab
			title: i18n.tr( "Reports" )
			
			ReportsPage {}
		}
	}
}

