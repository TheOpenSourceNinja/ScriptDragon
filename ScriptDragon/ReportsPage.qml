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
				ListElement {
					name: "characters"
				}
				ListElement {
					name: "locations"
				}
				ListElement {
					name: "scenes"
				}
				ListElement {
					name: "pages"
				}
			}
			
			Picker {
				id: reportTypeSelector
				model: ["Number of"]
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
