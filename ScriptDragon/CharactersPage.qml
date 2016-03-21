import QtQuick 2.0
//import QtQuick.Controls 1.4
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.3 as ListItem

Page {
	
	property alias characters: characterView.children
	property alias characterListModel: characterListModel
	
	Column {
		anchors.fill: parent
		
		Column {
			id: header
			width: parent.width
			Row {
				Button {
					text: i18n.tr( "New character" )
					
					property var component: Qt.createComponent( "Character.qml" );
					
					function addCharacter( /*string*/newCharacterName ) {
						var status = component.status;
						if( status === Component.Ready ) {
							var obj = component.createObject( characterView )
							
							if( obj == null ) {
								console.error( i18n.tr( "Error creating new character." ) )
							} else {
								if( newCharacterName != null ) {
									obj.initialName = newCharacterName
								}
								
								obj.id = characterListModel.count
							}
						} else if( status === Component.Error ){
							console.error( i18n.tr( "Error creating new character: " ) + component.errorString() );
						}
					}
					
					onClicked: {
						addCharacter( "Unnamed Character" )
						//characterListModel.append( { text:characterView.children[ characterView.children.length - 1 ].name } )
						characterListModel.append( { text:"Character #" + characterListModel.count } )
						
						selector.selectedIndex = characterListModel.count - 1
						
						characterView.setVisibleChild( selector.selectedIndex )
					}
				}
				
				Button {
					text: i18n.tr( "Delete character" )
					
					onClicked: {
						characterListModel.remove( selector.selectedIndex )
						characterView.removeChild( selector.selectedIndex )
						selector.selectedIndex -= 1
					}
				}
			}
			
			ListItem.ItemSelector {
				text: "Choose character:"
				id: selector
				model: characterListModel
				expanded: false
				
				onDelegateClicked: {
					console.log( index )
					characterView.setVisibleChild( index )
				}
				
				onSelectedIndexChanged: {
					console.log( selectedIndex )
					characterView.setVisibleChild( selectedIndex )
				}
				
				ListModel {
					id: characterListModel
				}
			}
			
			/*ComboBox {
				id: selector
				model: characterListModel
				
				ListModel {
					id: characterListModel
				}
				
				onCurrentIndexChanged: {
					console.log( selectedIndex )
					characterView.setVisibleChild( selectedIndex )
				}
			}*/
		}
		
		Item {
			width: parent.width
			height: parent.height - header.height
			id: characterView
			
			function setVisibleChild( childNumber ) {
				for( var i = 0; i < children.length; ++i ) {
					children[ i ].visible = false
				}
				
				children[ childNumber ].visible = true
			}
			
			function removeChild( childNumber ) {
				children[ childNumber ].destroy();
			}
		}
	}
}
