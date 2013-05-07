import bb.cascades 1.0

Container {
    id: compilePhaseContainer
    property variant dimensions: Dimensions {}
    property alias startButtonVisible: startButton.visible
    property alias startButtonEnabled: startButton.enabled
    signal start
    signal selectNext
    signal removeAction(int index)
    
    preferredWidth: dimensions.playAreaWidth
    preferredHeight: dimensions.screenHeight

    Container {
        id: buttonContainer

        preferredWidth: dimensions.compileMapWidth
        preferredHeight: dimensions.compileMapHeight
        layout: DockLayout {}

        Button {
            id: startButton
            text: "Start"
            opacity: 0.7
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            onClicked: {
                compilePhaseContainer.start();
            }
        }
    }
    Container {
        objectName: "compileFunctionContainer"
        preferredWidth: dimensions.compileMapWidth
        preferredHeight: dimensions.compileFunctionHeight
        //background: Color.Black // FIXME: Image
        layout: DockLayout {
        }

        Container {
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center

            Container {
                preferredWidth: dimensions.compileMapWidth
                preferredHeight: dimensions.cmdHeight
                bottomMargin: dimensions.itemPadding
                layout: DockLayout {}

                Container {
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }

                    FunctionCommand {
                        objectName: "functionHeader"
                        background: highlight.imagePaint
                        gestureHandlers: [
                            TapHandler {
                                onTapped: {
                                    compilePhaseContainer.selectNext();
                                }
                            }
                        ]
                        attachedObjects: [
                            ImagePaintDefinition {
                                id: highlight
                                imageSource: "asset:///images/highlight.png"
                            }
                        ]
                    }
                    FunctionCommand {
                        objectName: "functionAction1"
                        gestureHandlers: [
                            TapHandler {
                                onTapped: {
                                    compilePhaseContainer.removeAction(0);
                                }
                            }
                        ]
                    }
                    FunctionCommand {
                        objectName: "functionAction2"
                        gestureHandlers: [
                            TapHandler {
                                onTapped: {
                                    compilePhaseContainer.removeAction(1);
                                }
                            }
                        ]
                    }
                    FunctionCommand {
                        objectName: "functionAction3"
                        gestureHandlers: [
                            TapHandler {
                                onTapped: {
                                    compilePhaseContainer.removeAction(2);
                                }
                            }
                        ]
                    }
                }
            }

            Container {
                preferredWidth: dimensions.compileMapWidth
                preferredHeight: dimensions.cmdHeight
                layout: DockLayout {
                }

                Container {
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }

                    FunctionCommand {
                        objectName: "functionAction4"
                        gestureHandlers: [
                            TapHandler {
                                onTapped: {
                                    compilePhaseContainer.removeAction(3);
                                }
                            }
                        ]
                    }
                    FunctionCommand {
                        objectName: "functionAction5"
                        gestureHandlers: [
                            TapHandler {
                                onTapped: {
                                    compilePhaseContainer.removeAction(4);
                                }
                            }
                        ]
                    }
                    FunctionCommand {
                        objectName: "functionAction6"
                        gestureHandlers: [
                            TapHandler {
                                onTapped: {
                                    compilePhaseContainer.removeAction(5);
                                }
                            }
                        ]
                    }
                    FunctionCommand {
                        objectName: "functionAction7"
                        gestureHandlers: [
                            TapHandler {
                                onTapped: {
                                    compilePhaseContainer.removeAction(6);
                                }
                            }
                        ]
                    }
                }
            }
        }
    }
}
