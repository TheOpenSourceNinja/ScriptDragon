import QtQuick 2.0
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.3 as ListItem
import Ubuntu.Components.Pickers 1.3
import ninja.theopensource.scriptdragon 1.0

Page {
	width: parent.width
	
	Column {
		
		Row {
			id: selectors
			
			ListModel {
				id: shared
				
				//["characters", "locations", "scenes", "pages"]
				/*ListElement {
					name: i18n.tr( "characters" )
				}
				ListElement {
					name: "locations"
				}
				ListElement {
					name: "scenes"
				}
				ListElement {
					name: "pages"
				}*/
				Component.onCompleted: {
					append( { "name": i18n.tr( "characters" ) } )
					append( { "name": i18n.tr( "locations" ) } )
					append( { "name": i18n.tr( "scenes" ) } )
					append( { "name": i18n.tr( "pages" ) } )
				}
			}
			
			function generateReport() {
				console.log( "generateReport() called" );
				reportTA.text = "";
				
				switch( reportTypeSelector.selectedIndex ) {
					case 0: {
						//reportTA.text += i18n.tr( "Number of" );
						break;
					}
					default: {
						console.error( "generateReport(): reportTypeSelector.selectedIndex " + reportTypeSelector.selectedIndex + " not in switch statement" );
						break;
					}
				}
				
				//reportTA.text += " ";
				
				var numerator = -1;
				
				switch( numeratorSelector.selectedIndex ) {
					case 0: {
						numerator = characters.length;
						reportTA.text += numerator + " ";
						reportTA.text += i18n.tr( "characters" );
						break;
					}
					case 1: {
						reportTA.text += "NUMLOC" + " ";
						reportTA.text += i18n.tr( "locations" );
						break;
					}
					case 2: {
						reportTA.text += "NUMSCN" + " ";
						reportTA.text += i18n.tr( "scenes" );
						break;
					}
					case 3: {
						reportTA.text += "NUMPAGE" + " ";
						reportTA.text += i18n.tr( "pages" );
						break;
					}
					default: {
						console.error( "generateReport(): numeratorSelector.selectedIndex " + numeratorSelector.selectedIndex + " not in switch statement" );
						break;
					}
				}
				
				reportTA.text += " / ";
				
				var denominator = -1;
				
				switch( denominatorSelector.selectedIndex ) {
					case 0: {
						denominator = characters.length;
						reportTA.text += denominator + " ";
						reportTA.text += i18n.tr( "character" );
						break;
					}
					case 1: {
						reportTA.text += "NUMLOC" + " ";
						reportTA.text += i18n.tr( "location" );
						break;
					}
					case 2: {
						reportTA.text += "NUMSCN" + " ";
						reportTA.text += i18n.tr( "scene" );
						break;
					}
					case 3: {
						reportTA.text += "NUMPAGE" + " ";
						reportTA.text += i18n.tr( "page" );
						break;
					}
					default: {
						console.error( "generateReport(): denominatorSelector.selectedIndex " + denominatorSelector.selectedIndex + " not in switch statement" );
						break;
					}
				}
			}
			
			Picker {
				id: reportTypeSelector
				model: [ i18n.tr( "Number of" ) ]
				width: units.gu(15)
				//anchors.left: parent.left
				anchors.top: parent.top
				delegate: PickerDelegate {
					Label {
						text: modelData
						anchors.fill: parent
						verticalAlignment: Text.AlignVCenter
						horizontalAlignment: Text.AlignHCenter
					}
				}
				selectedIndex: 0
				
				onSelectedIndexChanged: parent.generateReport()
			}
			
			Picker {
				id: numeratorSelector
				model: shared
				width: units.gu(15)
				//anchors.left: reportTypeSelector.right
				anchors.top: parent.top
				delegate: PickerDelegate {
					Label {
						text: modelData
						anchors.fill: parent
						verticalAlignment: Text.AlignVCenter
						horizontalAlignment: Text.AlignHCenter
					}
				}
				selectedIndex: 0
				
				onSelectedIndexChanged: parent.generateReport()
			}
			
			Label {
				id: per
				text: " per "
				//anchors.left: numeratorSelector.right
				anchors.baseline: parent.verticalCenter
			}
			
			Picker {
				id: denominatorSelector
				model: shared
				width: units.gu(15)
				//anchors.left: per.right
				anchors.top: parent.top
				delegate: PickerDelegate {
					Label {
						text: modelData
						anchors.fill: parent
						verticalAlignment: Text.AlignVCenter
						horizontalAlignment: Text.AlignHCenter
					}
				}
				selectedIndex: 0
				
				onSelectedIndexChanged: parent.generateReport()
			}
		}
		
		Row {
			//anchors.top: selectors.bottom
			Button {
				text: "Export"
				id: exportButton
			}
		}
		
		TextArea {
			id: reportTA
		}
	}
}
