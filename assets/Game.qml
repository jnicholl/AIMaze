import bb.cascades 1.0

Page {
    property variant dimensions: Dimensions {}
    Container {
        id: background
        background: Color.Green
        layout: StackLayout {
        }
        Container {
            id: topbar
            horizontalAlignment: HorizontalAlignment.Fill
            preferredHeight: dimensions.topBarHeight
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            background: Color.Cyan
            Container {
                id: minimapContainer
                background: Color.Black
                preferredHeight: dimensions.topBarHeight
                Container {
                    id: minimap
                    objectName: "minimap"
                    layout: AbsoluteLayout {}
                    horizontalAlignment: HorizontalAlignment.Center
                    preferredWidth: dimensions.topBarHeight
                    preferredHeight: dimensions.topBarHeight
                    background: Color.Red
                    gestureHandlers: [
                        TapHandler {
                            onTapped: {
                                programContainer.visible = ! programContainer.visible;
                                controlContainer.visible = ! controlContainer.visible;
                                var size = dimensions.topBarHeight;
                                if (minimap.preferredWidth == dimensions.topBarHeight) {
                                    if (_app.screenWidth > _app.screenHeight) size = _app.screenHeight - dimensions.topBarHeight;
                                    else size = _app.screenWidth
                                }
                                minimap.preferredWidth = size;
                                minimap.preferredHeight = size;
                                minimapContainer.preferredHeight = size;
                                topbar.preferredHeight = size;
                                controlContainer.preferredHeight = size;
                            }
                        }
                    ]
                }
            }
            Container {
                id: controlContainer
                background: Color.Gray
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 3.0
                }
                layout: DockLayout {}
                
                preferredHeight: dimensions.topBarHeight
                Container {
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center

                    Container {
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        
                        GameControl {
                            gestureHandlers: [
                                TapHandler {
                                    onTapped: {
                                        console.debug("Tapped control 1");
                                    }
                                }
                            ]
                        }

                        GameControl {
                            gestureHandlers: [
                                TapHandler {
                                    onTapped: {
                                        console.debug("Tapped control 2");
                                    }
                                }
                            ]
                        }

                        GameControl {
                            gestureHandlers: [
                                TapHandler {
                                    onTapped: {
                                        console.debug("Tapped control 3");
                                    }
                                }
                            ]
                        }
                    }

                    Container {
                        topMargin: 20
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }

                        GameControl {

                        }

                        GameControl {

                        }

                        GameControl {

                        }
                    }
                }
            }
        }
        Container {
            id: programContainer
            horizontalAlignment: HorizontalAlignment.Fill
            layoutProperties: StackLayoutProperties {
                spaceQuota: 3.0
            }
            background: Color.Blue
            Label {
                text: "Functions go here"
            }
        }
    }
    function resizeWorld() {
        programContainer.visible = ! programContainer.visible;
        controlContainer.visible = ! controlContainer.visible;
        var size = 180;
        if (minimap.preferredWidth == 180) {
            if (_app.screenWidth > _app.screenHeight) size = _app.screenHeight - 100;
            else size = _app.screenWidth
        }
        minimap.preferredWidth = size;
        minimap.preferredHeight = size;
        minimapContainer.preferredHeight = size;
        topbar.preferredHeight = size;
        controlContainer.preferredHeight = size;
    }
}
