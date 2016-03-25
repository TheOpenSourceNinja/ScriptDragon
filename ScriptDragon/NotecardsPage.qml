import QtQuick 2.0
import Ubuntu.Components 1.3
import ninja.theopensource.scriptdragon 1.0

Page {
	id: thePage
	property alias notecards: notecardFlow.children;
	
	property var component: Qt.createComponent( "TextNotecard.qml" );
	
	function updateGrid() {
		notecardFlow.setChildren();
	}
	
	/*function addNotecard( newCardText, newCardTitle ) {
		NotecardManager.addNotecard( newCardText, newCardTitle, thePage );
	}*/
	
	Connections {
		target: NotecardManager
		onNotecardsChanged: updateGrid()
	}
	
	Column {
		width: parent.width
		height: parent.height
		
		Button {
			iconName: "note-new"
			text: i18n.tr( "new" )
			width: parent.width
			id: newCardButton
			
			onClicked: {
				NotecardManager.addNotecard();
			}
		}
		
		
		ScrollView {
			width: parent.width
			height: parent.height - newCardButton.height
			
			Flow {
				id: notecardFlow
				spacing: units.gu( 1 )
				width: thePage.width
				children: NotecardManager.getAllNotecards();
				//children: NotecardManager.allNotecards
				//data: NotecardManager.allNotecards
				
				function setChildren() {
					children = NotecardManager.getAllNotecards();
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
			
			viewport.width: parent.width
			viewport.height: parent.height
		}
	}
}
