import QtQuick 2.0
import Ubuntu.Components 1.3
import ninja.theopensource.scriptdragon 1.0

Item {
	property var name: ""
	property var locationID //index of this location in locationListModel
	width: parent.width
	height: parent.height
	id: theItem
	
	property color separatorColor: UbuntuColors.darkGrey
	property color separatorTextColor: "white"
	
	property var associatedNotecards: []
	
	Connections {
		target: NotecardManager
		onNotecardsChanged: notecardGrid.setChildren();
	}
	
	Column {
		
		Row {
			id: nameRow
			height: nameField.height
			width: parent.width
			
			Label {
				text: i18n.tr( "Name: " )
			}
			TextField {
				id: nameField
				inputMethodHints: Qt.ImhUppercaseOnly
				text: name
			}
			Button {
				text: i18n.tr( "Apply Name" )
				onClicked: {
					locationListModel.setProperty( locationID, "text", nameField.text )
					name = nameField.text;
				}
			}
		}
		
		ScrollView {
			width: theItem.width
			height: theItem.height - nameRow.height
			id: scrollView
			
			Flow {
				id: notecardGrid
				children: NotecardManager.getNotecardsForLocation( locationID );
				spacing: units.gu( 1 )
				width: scrollView.width
				
				function setChildren() {
					children = NotecardManager.getNotecardsForLocation( locationID );
				}
				
				Connections {
					target: NotecardManager
					onNotecardsChanged: notecardGrid.setChildren()
				}
				
				add: Transition {
					NumberAnimation {
						properties: "x,y"
					}
				}
				
				move: Transition {
					NumberAnimation {
						properties: "x,y"
					}
				}
			}
		}
	}
}

