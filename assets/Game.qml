import bb.cascades 1.0

Page {
    property variant dimensions: Dimensions {
    }

    content: Container {
        id: background
        background: Color.Black
        layout: AbsoluteLayout {
        }
        Container {
            id: mapArea
            objectName: "mapArea"
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: dimensions.queueHeight
            }
            preferredWidth: dimensions.playAreaWidth
            preferredHeight: dimensions.playAreaHeight
            background: Color.Green
            layout: DockLayout {}
        }

        Container {
            id: progressBar
            layoutProperties: AbsoluteLayoutProperties {
                positionX: dimensions.screenWidth
                positionY: 0
            }
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            preferredHeight: dimensions.queueHeight
            preferredWidth: dimensions.screenWidth
            background: Color.Red
            leftPadding: dimensions.sidebarPadding
            animations: [
                SequentialAnimation {
                    id: progressAnimation
                    objectName: "progressAnimation"
	                TranslateTransition {
	                    id: translateLeft
	                    fromX: 0
	                    toX: -dimensions.screenWidth
	                    duration: 1900 // FIXME: Connect to app
	                }
	            }
            ]
        }

        Container {
            id: queueContainer
            objectName: "queueContainer"
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 0
            }
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            preferredHeight: dimensions.queueHeight
            preferredWidth: dimensions.playAreaWidth
            leftPadding: dimensions.sidebarPadding
        }
        
        Container {
            id: sidebar
            layoutProperties: AbsoluteLayoutProperties {
                positionX: dimensions.playAreaWidth
                positionY: 0
            }
            preferredHeight: dimensions.screenHeight
            preferredWidth: dimensions.sidebarWidth
            background: Color.Blue
            topPadding: dimensions.sidebarPadding
            bottomPadding: dimensions.sidebarPadding

            Container {
                id: cmdForward
                horizontalAlignment: HorizontalAlignment.Center
                preferredWidth: dimensions.cmdWidth
                preferredHeight: dimensions.cmdHeight
                background: Color.Gray
                bottomMargin: dimensions.sidebarPadding

                Label {
                    text: "F"
                }

                gestureHandlers: [
                    TapHandler {
                        onTapped: {
                            _app.tapForward();
                        }
                    }
                ]
            }

            Container {
                id: cmdLeft
                horizontalAlignment: HorizontalAlignment.Center
                preferredWidth: dimensions.cmdWidth
                preferredHeight: dimensions.cmdHeight
                background: Color.Gray
                bottomMargin: dimensions.sidebarPadding

                Label {
                    text: "L"
                }

                gestureHandlers: [
                    TapHandler {
                        onTapped: {
                            _app.tapLeft();
                        }
                    }
                ]
            }

            Container {
                id: cmdRight
                horizontalAlignment: HorizontalAlignment.Center
                preferredWidth: dimensions.cmdWidth
                preferredHeight: dimensions.cmdHeight
                background: Color.Gray
                bottomMargin: dimensions.sidebarPadding

                Label {
                    text: "R"
                }

                gestureHandlers: [
                    TapHandler {
                        onTapped: {
                            _app.tapRight();
                        }
                    }
                ]
            }
        }
        Button {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: dimensions.screenHeight - dimensions.backButtonHeight - dimensions.sidebarPadding
            }
            preferredWidth: dimensions.backButtonHeight*3
            opacity: 0.5
            text: "Back"
            onClicked: {
                _app.back();
            }
        }
    }   
}
