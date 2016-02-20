import QtQuick 2.4
import Ubuntu.Components 1.3
//import U1db 1.0 as U1db

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

	/*U1db.Database {
		id: database;
		path: "test_database.u1db"
	}*/

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
			
			property alias notecards: notecardsPage.notecards
			
			NotecardsPage {
				id: notecardsPage
			}
		}

		Tab {
			id: charactersTab
			title: i18n.tr( "Characters" )

			CharactersPage {}
		}

		Tab {
			id: locationsTab
			title: i18n.tr( "Locations" )

			LocationsPage {}
		}

		Tab {
			id: randomGeneratorsTab
			title: i18n.tr( "Random generators" )

			RandomGeneratorsPage {}
		}

		Tab {
			id: scriptTab
			title: i18n.tr( "Script" )

			property alias text: scriptPage.text

			ScriptPage {
				id: scriptPage
			}
		}

		Tab {
			id: reportsTab
			title: i18n.tr( "Reports" )

			ReportsPage {}
		}
	}
}

