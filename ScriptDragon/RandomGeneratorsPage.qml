import QtQuick 2.0
import Ubuntu.Components 1.3

Page {
	head {
		sections {
			model: [ "characters", "locations", "events", "names" ]
		}
	}

	Label {
		text: i18n.tr( "Click the links at the top to access the various generators." )
	}
}
