import QtQuick 2.0
import Ubuntu.Components 1.3
import ninja.theopensource.scriptdragon 1.0

Item {
	property alias name : nameField.text
	property alias initialName : nameField.text
	property var characterID //index of this character in characterListModel
	width: parent.width
	height: parent.height
	id: theItem
	
	property color separatorColor: UbuntuColors.darkGrey
	property color separatorTextColor: "white"
	
	property var associatedNotecards: []
	
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
			}
			Button {
				text: i18n.tr( "Apply Name" )
				onClicked: {
					characterListModel.setProperty( characterID, "text", nameField.text )
					for( var i = 0; i < notecardGrid.children.length; ++i ) {
						notecardGrid.children[ i ].setAssociatedText( nameField.text );
					}
				}
			}
		}
		
		ScrollView {
			width: theItem.width
			height: theItem.height - nameRow.height
			id: scrollView
			
			Flow {
				id: notecardGrid
				children: NotecardManager.getNotecardsForCharacter( characterID );
				spacing: units.gu( 1 )
				width: scrollView.width
				
				function setChildren() {
					children = NotecardManager.getNotecardsForCharacter( characterID );
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

