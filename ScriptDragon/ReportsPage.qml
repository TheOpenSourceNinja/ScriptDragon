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
				
				Component.onCompleted: {
					append( { "name": i18n.tr( "scenes" ) } )
					append( { "name": i18n.tr( "actions" ) } )
					append( { "name": i18n.tr( "characters" ) } )
					append( { "name": i18n.tr( "dialogs" ) } )
					append( { "name": i18n.tr( "parentheticals" ) } )
					append( { "name": i18n.tr( "transitions" ) } )
					append( { "name": i18n.tr( "shots" ) } )
					append( { "name": i18n.tr( "act breaks" ) } )
				}
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
				
				onSelectedIndexChanged: reportTA.text = ReportGenerator.generateReport( numeratorSelector.selectedIndex, denominatorSelector.selectedIndex );
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
				
				onSelectedIndexChanged: reportTA.text = ReportGenerator.generateReport( numeratorSelector.selectedIndex, denominatorSelector.selectedIndex );
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
			textFormat: TextEdit.RichText
		}
	}
}
