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

			property var component: Qt.createComponent("Notecard.qml"); //Note to self: Experiment with creating the component only once

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
			height: parent.height

			Grid {
				//width: thePage.width
				//height: (1 * thePage.height) - newCardButton.height
				id: notecardGrid
				columns: 2
				columnSpacing: units.gu( 1 )
				rowSpacing: units.gu( 1 )
			}

			viewport.width: parent.width
			viewport.height: parent.height
		}
	}
}
