import QtQuick 2.4
import Ubuntu.Components 1.3
import ninja.theopensource.scriptdragon 1.0

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

	/*
	Ubuntu discourages the use of pixels. Instead, they recommend "graphical units",
	the advantage being that graphical units can be different pixel sizes based on
	a screen's pixel density.

	Feel free to try different numbers here. I put in 54 and 96 because that
	reflects the 9:16 aspect ratio of my phone's screen (9:16 x 6 = 54:96). The x6
	multiplier makes it big enough to use on my laptop's screens.

	The window is user-resizable on normal computers. On my phone it seems to just
	fill the screen.
	*/
	width: units.gu(54)
	height: units.gu(96)

	/*"The LayoutMirroring attached property is used to horizontally mirror Item anchors, positioner types (such as Row and Grid) and views (such as GridView and horizontal ListView). Mirroring is a visual change: left anchors become right anchors, and positioner types like Grid and Row reverse the horizontal layout of child items." -https://doc.qt.io/qt-5/qml-qtquick-layoutmirroring.html */
	//LayoutMirroring.enabled: true
	LayoutMirroring.childrenInherit: true
	
	Connections {
		target: FileSaverAndLoader
		onFileLoaded: {
			SettingsManager.setValue( "lastFileLoaded", fileURL );
		}
	}
	
	property alias characters: charactersPage.characters
	property alias characterListModel: charactersPage.characterListModel
	property alias locations: locationsPage.locations
	property alias script: scriptPage.text
	property alias report: reportsPage.report
	
	Component.onCompleted: {
		NotecardManager.setCharactersPage( charactersPage );
		NotecardManager.setLocationsPage( locationsPage );
		NotecardManager.setNotecardsPage( notecardsPage );
		FileSaverAndLoader.setNotecardManager( NotecardManager );
		FileSaverAndLoader.setScriptPage( scriptPage );
		FileSaverAndLoader.setCharactersPage( charactersPage );
		FileSaverAndLoader.setLocationsPage( locationsPage );
	}
	
	Keys.onPressed: { //todo: If we decide to implement keyboard shortcuts, this is a good place to start
		//https://doc.qt.io/qt-5/qml-qtquick-keys.html
		//https://doc.qt.io/qt-5/qkeysequence.html#StandardKey-enum
		switch( event.key ) {
			case Qt.Key_S:
				if( event.modifiers & Qt.ControlModifier ) {
					console.log( "Ctrl+S was pressed" );
				}
				break;
			default: break;
		}
		
		if( event.matches( StandardKey.Save ) ) {
			console.log( "Standard save key sequence hit" );
		} else if( event.matches( StandardKey.SaveAs ) ) {
			console.log( "Standard Save As key sequence hit" );
		}
	}

	Tabs {
		id: tabs
		
		selectedTabIndex: welcomeTab.index
		
		Tab {
			id: notecardsTab
			title: i18n.tr( "Notecards" )
			
			property alias notecards: notecardsPage.notecards
			
			NotecardsPage {
				id: notecardsPage
			}
		}

		Tab {
			id: charactersTab
			title: i18n.tr( "Characters" )
			
			CharactersPage {
				id: charactersPage
			}
		}

		Tab {
			id: locationsTab
			title: i18n.tr( "Locations" )
			
			LocationsPage {
				id: locationsPage
			}
		}

		Tab {
			id: randomGeneratorsTab
			title: i18n.tr( "Random generators" )
			
			RandomGeneratorsPage {
				id: randomGeneratorsPage
			}
		}

		Tab {
			id: scriptTab
			title: i18n.tr( "Script" )
			
			//property alias textDocument: scriptPage.textDocument
			
			ScriptPage {
				id: scriptPage
			}
		}

		Tab {
			id: reportsTab
			title: i18n.tr( "Reports" )

			ReportsPage {
				id: reportsPage
			}
		}
		
		Tab {
			id: welcomeTab
			title: i18n.tr( "Welcome" )

			WelcomePage {
				id: welcomePage
			}
		}
	}
}

