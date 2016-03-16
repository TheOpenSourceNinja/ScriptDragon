import QtQuick 2.0
import Ubuntu.Components 1.3
import ninja.theopensource.scriptdragon 1.0

Page {
	id: thePage
	property alias notecards: notecardGrid.children;
	
	property var component: Qt.createComponent( "TextNotecard.qml" );
	
	function addNotecard( /*string*/newCardText, /*string*/newCardTitle ) {
		NotecardManager.addNotecard( newCardText, newCardTitle );
		
		notecardGrid.setChildren();
		/*var status = component.status;
		if( status === Component.Ready ) {
			var obj = component.createObject( notecardGrid )
			
			if( obj == null ) {
				console.error( i18n.tr( "Error creating new notecard." ) )
			} else {
				if( newCardText != null ) {
					obj.text = newCardText
				}
				
				if( newCardTitle != null ) {
					obj.title = newCardTitle
				}
			}
		} else if( status === Component.Error ){
			console.error( i18n.tr( "Error creating new notecard: " ) + component.errorString() );
		}*/
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
				addNotecard()
			}
		}
		
		
		ScrollView {
			width: parent.width
			height: parent.height - newCardButton.height
			
			Grid {
				id: notecardGrid
				spacing: units.gu( 1 )
				//children: NotecardManager.allNotecards
				//data: NotecardManager.allNotecards
				
				function setChildren() {
					children = NotecardManager.getAllNotecards();
					console.log( children.length );
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
			}
			
			viewport.width: parent.width
			viewport.height: parent.height
		}
		
		onWidthChanged: notecardGrid.changeColumns()
	}
}
