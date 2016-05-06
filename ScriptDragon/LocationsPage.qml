import QtQuick 2.0
//import QtQuick.Controls 1.4
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.3 as ListItem
import ninja.theopensource.scriptdragon 1.0

Page {
	
	property alias locations: locationView.children
	property alias locationListModel: locationListModel
	
	property var component: Qt.createComponent( "Location.qml" );
	
	function addLocation( name ) {
		var status = component.status;
		if( status === Component.Ready ) {
			var obj = component.createObject( locationView )
			
			if( obj === null ) {
				console.error( i18n.tr( "Error creating new location." ) )
			} else {
				if( name !== null ) {
					obj.name = name
				}
				
				obj.locationID = locationListModel.count
				
				NotecardManager.addNotecard( i18n.tr( "Physical Locationistics" ), "", NotecardManager.LOCATION, obj.locationID );
				
				console.log( "test" );
				
				locationListModel.append( { text:name } )
				
				selector.selectedIndex = locationListModel.count - 1
			}
		} else if( status === Component.Error ){
			console.error( i18n.tr( "Error creating new location: " ) + component.errorString() );
		}
	}
	
	function deleteLocation( indexToRemove ) {
		//console.log( NotecardManager.getNotecardsForLocation( indexToRemove ) );
		var array = NotecardManager.getNotecardsForLocation( indexToRemove );
		for( var i = 0; i < array.length; ++i ) {
			console.log( array[ i ] );
			NotecardManager.removeAssociation( array[ i ] );
		}
		
		locationListModel.remove( indexToRemove )
		locationView.removeChild( indexToRemove )
		selector.selectedIndex -= 1
	}
	
	
	Column {
		anchors.fill: parent
		
		Column {
			id: header
			width: parent.width
			Row {
				Button {
					text: i18n.tr( "New location" )
					
					onClicked: {
						var name = "Unnamed Location";
						var age = "";
						var gender = "";
						var archetype = "";
						var job = "";
						
						addLocation( name, age, gender, archetype, job )
					}
				}
				
				Button {
					text: i18n.tr( "Delete location" )
					
					onClicked: deleteLocation( selector.selectedIndex );
				}
			}
			
			ListItem.ItemSelector {
				text: "Choose location:"
				id: selector
				model: locationListModel
				expanded: false
				
				onDelegateClicked: {
					//console.log( index )
					locationView.setVisibleChild( index )
				}
				
				onSelectedIndexChanged: {
					//console.log( selectedIndex )
					locationView.setVisibleChild( selectedIndex )
				}
				
				ListModel {
					id: locationListModel
					onDataChanged: {
						//console.log("onDataChanged() called");
						
						//These next four lines are a workaround: the selector's text only gets updated if we change models
						var selected = selector.selectedIndex
						selector.model = fakeModel
						selector.model = this
						selector.selectedIndex = selected
					}
				}
				
				ListModel {
					id: fakeModel
					//This is a workaround: the selector's text only gets updated if we change models
				}
			}
			
		}
		
		
		Item {
			width: parent.width
			height: parent.height - header.height
			id: locationView
			
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
