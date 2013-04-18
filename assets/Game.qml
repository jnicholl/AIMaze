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
            background: Color.Black
            layout: DockLayout {
            }
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
                        toX: - dimensions.screenWidth
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

            QueueCommand {
            }
            QueueCommand {
            }
            QueueCommand {
            }
            QueueCommand {
            }
            QueueCommand {
            }

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
            topPadding: dimensions.itemPadding
            bottomPadding: dimensions.itemPadding

            Container {
                id: cmdForward
                horizontalAlignment: HorizontalAlignment.Center
                preferredWidth: dimensions.cmdWidth
                preferredHeight: dimensions.cmdHeight
                background: Color.Gray
                bottomMargin: dimensions.itemPadding

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
                bottomMargin: dimensions.itemPadding

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
                bottomMargin: dimensions.itemPadding

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

        Container {
            id: menuButtonContainer
            preferredWidth: dimensions.backButtonHeight * 3
            preferredHeight: dimensions.queueHeight
            layoutProperties: AbsoluteLayoutProperties {
                positionX: dimensions.playAreaWidth - menuButtonContainer.preferredWidth - dimensions.sidebarPadding
                positionY: 0
            }
            layout: DockLayout{}

            Button {
                text: "Pause"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                visible: dimensions.screenWidth > 1000
                onClicked: {
                    _app.back(); // FIXME: Menu
                }
            }
        }
    }
}
