import QtQuick 2.0
import Ubuntu.Components 1.3

Page {
	id: thePage
	Column {
		width: parent.width
		height: parent.height

		Button {
			iconName: "note-new"
			text: i18n.tr( "new" )
			width: parent.width
			id: newCardButton

			property var component: Qt.createComponent("Notecard.qml");

			onClicked: {
				var status = component.status;
				if( status === Component.Ready ) {
					var obj = component.createObject( notecardGrid )

					if( obj == null ) {
						console.error( i18n.tr( "Error creating new notecard." ) )
					}
				} else if( status === Component.Error ){
					console.error( i18n.tr( "Error creating new notecard: " ) + component.errorString() );
				}
			}
		}


		ScrollView {
			width: parent.width
			height: parent.height - newCardButton.height

			Grid {
				//width: thePage.width
				//height: (1 * thePage.height) - newCardButton.height
				id: notecardGrid
				columns: 2 //todo: Instead of using a fixed number, make this dependent on the sizes of the window and the notecards OR make it user configurable
				columnSpacing: units.gu( 1 )
				rowSpacing: units.gu( 1 )
			}

			viewport.width: parent.width
			viewport.height: parent.height
		}
	}
}
