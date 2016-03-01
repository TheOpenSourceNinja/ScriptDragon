import QtQuick 2.0
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.3 as ListItem

Page {
	Column {
		anchors.fill: parent
		Button {
			text: i18n.tr( "New character" )
			onClicked: {
				theModel.append( { "text":"test" } )
				selector.selectedIndex = theModel.count - 1
			}
		}
		
		ListItem.ItemSelector {
			text: "Choose character:"
			id: selector
			model: theModel
			expanded: false
			onDelegateClicked: {
				console.log( index )
			}
			
			onSelectedIndexChanged: {
				console.log( selectedIndex )
			}
			
			ListModel {
				id: theModel
			}
		}
	}
}
