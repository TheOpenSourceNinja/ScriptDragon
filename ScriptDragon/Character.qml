import QtQuick 2.0
import Ubuntu.Components 1.3
import ninja.theopensource.scriptdragon 1.0

ScrollView {
	property alias name : nameField.text
	property alias initialName : nameField.text
	property var id //index of this character in characterListModel
	width: parent.width
	height: parent.height
	
	property color separatorColor: UbuntuColors.darkGrey
	property color separatorTextColor: "white"
	
	property var associatedNotecards: []
	
	Column {
		/*Rectangle {
			color: separatorColor
			width: parent.width
			height: children[ 0 ].height
			Label {
				text: "The basics"
				color: separatorTextColor
			}
		}*/
		
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
				
				/*onAccepted: {
					//characterListModel.get( id ).text = text
					//characterListModel.setProperty( id, "text", text )
					
					//These next two lines are a workaround because setProperty() doesn't visibly update the drop-down list.
					//characterListModel.remove( id )
					//characterListModel.insert( id, { "text":text } )
				}*/
				
				/*onTextChanged: {
					console.log( "onTextChanged() called" )
					
					if( characterListModel.count > 0 ) {
						//selector.wipeAndRecreateModel()
						characterListModel.setProperty( id, "text", text )
						//These next two lines are a workaround because setProperty() doesn't visibly update the drop-down list.
						//characterListModel.remove( id )
						//characterListModel.insert( id, { "text":text } )
					}
				}*/
			}
			Button {
				text: i18n.tr( "Apply Name" )
				onClicked: {
					characterListModel.setProperty( id, "text", nameField.text )
				}
			}
		}
		
		Flow {
			id: notecardGrid
			children: NotecardManager.getNotecardsForCharacter( id );
			
			function setChildren() {
				console.log("test")
				children = NotecardManager.getNotecardsForCharacter( id );
			}
			
			Connections {
				target: NotecardManager
				onNotecardsChanged: notecardGrid.setChildren()
			}
		}
		
		/*Grid {
			width: parent.width
			height: parent.height - nameRow.height
			id: notecardGrid
			spacing: units.gu( 1 )
			columns: 1
			//children: NotecardManager.allNotecards
			//data: NotecardManager.allNotecards
			
			function setChildren() {
				children = NotecardManager.getNotecardsForCharacter( id );
			}
			
			Connections {
				target: NotecardManager
				onNotecardsChanged: notecardGrid.setChildren()
			}
			
			function changeColumns() {
				var numChildren = children.length;
				
				if( numChildren > 0 ) {
					//console.log( "Width:", parent.parent.width )
					//console.log( "Child's width:", children[0].width )
					//console.log( parent.parent.width / children[0].width );
					columns = Math.max( 1, Math.floor( parent.parent.width / children[0].width ) );
				} else {
					columns = 1;
				}
			}
			
			onChildrenChanged: changeColumns()
			//onWidthChanged changeColumns() //Apparently Grids don't send the onWidthChanged signal or something.
		}*/
		
		//TODO: Show notecards associated with this character
		
		/*Row {
			Label {
				text: i18n.tr( "Short description: " )
			}
			TextArea {
			}
		}
		
		Rectangle {
			color: separatorColor
			width: parent.width
			height: children[ 0 ].height
			Label {
				text: "Detailed physical description"
				color: separatorTextColor
			}
		}
		Row {
			Label {
				text: i18n.tr( "Age: " )
			}
			TextField {
			}
			Label {
				text: i18n.tr( "Hair: " )
			}
			TextField {
			}
		}
		Row {
			Label {
				text: i18n.tr( "Height: " )
			}
			TextField {
				
			}
			Label {
				text: i18n.tr( "Weight: " )
			}
			TextField {
				
			}
		}
		Row {
			Label {
				text: i18n.tr( "Eyes: " )
			}
			TextField {
				
			}
			Label {
				text: i18n.tr( "Distinguishing Features" )
			}
			TextField {
				
			}
		}
		
		Rectangle {
			color: separatorColor
			width: parent.width
			height: children[ 0 ].height
			Label {
				text: "Character traits"
				color: separatorTextColor
			}
		}
		Row {
			Label {
				text: i18n.tr( "Key character traits: " )
			}
			TextArea {
				
			}
		}
		
		Rectangle {
			color: separatorColor
			width: parent.width
			height: children[ 0 ].height
			Label {
				text: "Motivation"
				color: separatorTextColor
			}
		}
		Row {
			Label {
				text: i18n.tr( "Goal: " )
			}
			TextArea {
				
			}
		}
		Row {
			Label {
				text: i18n.tr( "Plan to achieve the goal: " )
			}
			TextArea {
				
			}
		}
		
		Rectangle {
			color: separatorColor
			width: parent.width
			height: children[ 0 ].height
			Label {
				text: "Background"
				color: separatorTextColor
			}
		}
		Row {
			Label {
				text: i18n.tr( "Family history: " )
			}
			TextArea {
				
			}
		}
		Row {
			Label {
				text: i18n.tr( "Habits: " )
			}
			TextArea {
				
			}
		}
		Row {
			Label {
				text: i18n.tr( "Education: " )
			}
			TextArea {
				
			}
		}*/
	}
}

