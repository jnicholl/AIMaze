// Grid view with detail project template
import bb.cascades 1.0

NavigationPane {
    id: navigationPane
    peekEnabled: false
    backButtonsVisible: false

    Menu.definition: MenuDefinition {
        helpAction: HelpActionItem {
            onTriggered: {
                helpSheet.open();
            }
        }
        actions: [
            ActionItem {
                title: "Privacy Policy"
                onTriggered: {
                    privacyPolicy.open();
                }
            }
        ]
        //        settingsAction: SettingsActionItem {
        //            title: "Privacy Policy"
        //            onTriggered: {
        //                privacyPolicy.open();
        //            }
        //        }
    } // end of MenuDefinition

    attachedObjects: [
        HelpSheet {
            id: helpSheet
        },
        PrivacyPolicy {
            id: privacyPolicy
        }
    ]

    firstPage: Page {
        id: pgMain
        property variant dimensions: Dimensions {
        }
        content: Container {
            background: Color.Black
            ImageView {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Top
                imageSource: "asset:///images/title.png"
            }
            ListView {
                id: listView
                // set object name to let listView to be discoverable from C++
                objectName: "listView"
                property int levelAvailable: _app.levelAvailable
                layout: GridListLayout {
                    columnCount: pgMain.dimensions.screenWidth > 800 ? 5 : 3
                }
                dataModel: XmlDataModel {
                    source: "asset:///levels.xml"
                }
                listItemComponents: [
                    // define component which will represent list item GUI appearence
                    ListItemComponent {
                        //type: "item"
                        LevelListItem {
                            lastAvailableIndex: ListItem.view.levelAvailable
                        }
                    }
                ]
                onTriggered: {
                    console.log("selected_index: " + indexPath)
                    if (indexPath <= levelAvailable) _app.startLevel(indexPath);
                }
                horizontalAlignment: HorizontalAlignment.Fill
//                layoutProperties: StackLayoutProperties {
//                    spaceQuota: 1.0
//                }
            }
        }
    }

    onCreationCompleted: {
        // enable layout to adapt to the device rotation
        // don't forget to enable screen rotation in bar-bescriptor.xml (Application->Orientation->Auto-orient)
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.DisplayLandscape;
    }
}