import QtQuick 2.0
import Ubuntu.Components 1.3

Page {
	head {
		sections {
			//model: [ "characters", "locations", "events", "names" ]
			actions: [
				Action {
					text: i18n.tr( "&characters" )
					onTriggered: {
						console.log( "characters section triggered" );
					}
				},
				Action {
					text: i18n.tr( "&locations" )
					onTriggered: {
						console.log( "locations section triggered" );
					}
				},
				Action {
					text: i18n.tr( "&events" )
					onTriggered: {
						console.log( "events section triggered" );
					}
				},
				Action {
					text: i18n.tr( "&names" )
					onTriggered: {
						console.log( "names section triggered" );
					}
				}
			]
		}
	}

	Label {
		text: i18n.tr( "Click the links at the top to access the various generators." )
	}
}
