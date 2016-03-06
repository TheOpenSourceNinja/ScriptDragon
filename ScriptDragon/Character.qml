import QtQuick 2.0
import Ubuntu.Components 1.3

Rectangle {
	property alias name : nameField.text
	property alias initialName : nameField.text
	property var id //index of this character in characterListModel
	width: parent.width
	height: parent.height
	Column {
		Row {
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
				
				onTextChanged: {
					console.log( "onTextChanged() called" )
					
					if( characterListModel.count > 0 ) {
						//selector.wipeAndRecreateModel()
						characterListModel.setProperty( id, "text", text )
						//These next two lines are a workaround because setProperty() doesn't visibly update the drop-down list.
						//characterListModel.remove( id )
						//characterListModel.insert( id, { "text":text } )
					}
				}
			}
		}
		Row {
			Label {
				text: i18n.tr( "Age: " )
			}
			TextField {
				id: ageField
				inputMethodHints: Qt.ImhPreferNumbers
			}
		}
	}
}

