// Grid view with detail project template
import bb.cascades 1.0

NavigationPane {
    id: navigationPane
    peekEnabled: false
    backButtonsVisible: false
    
    firstPage: Page {
        id: pgMain
        content: Container {
            Label {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Top
                text: "Game Name"
                textStyle.textAlign: TextAlign.Center
            }
            ListView {
                id: listView
                // set object name to let listView to be discoverable from C++
                objectName: "listView"
                property int levelAvailable: _app.levelAvailable
                layout: GridListLayout {
                }
                dataModel: XmlDataModel {
                    source: "asset:///levels.xml"
                }
                listItemComponents: [
                    // define component which will represent list item GUI appearence
                    ListItemComponent {
                        type: "item"
                        LevelListItem {
                            lastAvailableIndex: ListItem.view.levelAvailable
                        }
                    }
                ]
                onTriggered: {
                    console.log("selected_index: " + indexPath)
                    if (indexPath <= levelAvailable)
                        _app.startLevel(indexPath);
                }
                horizontalAlignment: HorizontalAlignment.Fill
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1.0
                }
            }
        }
    }
    onCreationCompleted: {
        // enable layout to adapt to the device rotation
        // don't forget to enable screen rotation in bar-bescriptor.xml (Application->Orientation->Auto-orient)
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.DisplayLandscape;
    }
}