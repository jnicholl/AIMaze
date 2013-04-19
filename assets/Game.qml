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
                positionY: compilePhaseContainer.visible ? 0 : dimensions.queueHeight
            }
            preferredWidth: dimensions.playAreaWidth
            preferredHeight: compilePhaseContainer.visible ? dimensions.compileMapHeight : dimensions.playAreaHeight
            layout: DockLayout {
            }
        }

        Container {
            id: functionSlideoutContainer
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 0
            }
            preferredHeight: dimensions.screenHeight
            preferredWidth: dimensions.playAreaWidth
            topPadding: dimensions.itemPadding
            bottomPadding: dimensions.itemPadding

            Container { // at forward button
                horizontalAlignment: HorizontalAlignment.Right
                preferredWidth: dimensions.cmdWidth
                preferredHeight: dimensions.cmdHeight
                bottomMargin: dimensions.itemPadding
                visible: cmdForward.visible
            }

            Container { // at left button
                horizontalAlignment: HorizontalAlignment.Right
                preferredWidth: dimensions.cmdWidth
                preferredHeight: dimensions.cmdHeight
                bottomMargin: dimensions.itemPadding
                visible: cmdLeft.visible
            }

            Container { // at right button
                horizontalAlignment: HorizontalAlignment.Right
                preferredWidth: dimensions.cmdWidth
                preferredHeight: dimensions.cmdHeight
                bottomMargin: dimensions.itemPadding
                visible: cmdRight.visible
            }

            Container { // at F1 button
                id: f1slideout
                horizontalAlignment: HorizontalAlignment.Right
                preferredWidth: dimensions.cmdWidth * 7
                preferredHeight: dimensions.cmdHeight
                bottomMargin: dimensions.itemPadding
                property bool childrenVisible: (_app.isInF1 || _app.showFunctions) && cmdF1.visible
                opacity: _app.isInF1?0.8:0.5
                layout: StackLayout {
                    orientation: LayoutOrientation.RightToLeft
                }
                SlideoutCommand {
                    objectName: "func1_act7"
                    property bool actionVisible: true
                    visible: f1slideout.childrenVisible && actionVisible
                }
                SlideoutCommand {
                    objectName: "func1_act6"
                    property bool actionVisible: true
                    visible: f1slideout.childrenVisible && actionVisible
                }
                SlideoutCommand {
                    objectName: "func1_act5"
                    property bool actionVisible: true
                    visible: f1slideout.childrenVisible && actionVisible
                }
                SlideoutCommand {
                    objectName: "func1_act4"
                    property bool actionVisible: true
                    visible: f1slideout.childrenVisible && actionVisible
                }
                SlideoutCommand {
                    objectName: "func1_act3"
                    property bool actionVisible: true
                    visible: f1slideout.childrenVisible && actionVisible
                }
                SlideoutCommand {
                    objectName: "func1_act2"
                    property bool actionVisible: true
                    visible: f1slideout.childrenVisible && actionVisible
                }
                SlideoutCommand {
                    objectName: "func1_act1"
                    property bool actionVisible: true
                    visible: f1slideout.childrenVisible && actionVisible
                }
            }

            Container { // at F2 button
                id: f2slideout
                horizontalAlignment: HorizontalAlignment.Right
                preferredWidth: dimensions.cmdWidth * 7
                preferredHeight: dimensions.cmdHeight
                bottomMargin: dimensions.itemPadding
                property bool childrenVisible: (_app.isInF2 || _app.showFunctions) && cmdF2.visible
                opacity: _app.isInF2 ? 0.8 : 0.5
                layout: StackLayout {
                    orientation: LayoutOrientation.RightToLeft
                }
                SlideoutCommand {
                    objectName: "func2_act7"
                    property bool actionVisible: true
                    visible: f2slideout.childrenVisible && actionVisible
                }
                SlideoutCommand {
                    objectName: "func2_act6"
                    property bool actionVisible: true
                    visible: f2slideout.childrenVisible && actionVisible
                }
                SlideoutCommand {
                    objectName: "func2_act5"
                    property bool actionVisible: true
                    visible: f2slideout.childrenVisible && actionVisible
                }
                SlideoutCommand {
                    objectName: "func2_act4"
                    property bool actionVisible: true
                    visible: f2slideout.childrenVisible && actionVisible
                }
                SlideoutCommand {
                    objectName: "func2_act3"
                    property bool actionVisible: true
                    visible: f2slideout.childrenVisible && actionVisible
                }
                SlideoutCommand {
                    objectName: "func2_act2"
                    property bool actionVisible: true
                    visible: f2slideout.childrenVisible && actionVisible
                }
                SlideoutCommand {
                    objectName: "func2_act1"
                    property bool actionVisible: true
                    visible: f2slideout.childrenVisible && actionVisible
                }
            }

            Container { // at F3 button
                id: f3slideout
                horizontalAlignment: HorizontalAlignment.Right
                preferredWidth: dimensions.cmdWidth * 7
                preferredHeight: dimensions.cmdHeight
                bottomMargin: dimensions.itemPadding
                property bool childrenVisible: (_app.isInF3 || _app.showFunctions) && cmdF3.visible
                opacity: _app.isInF3 ? 0.8 : 0.5
                layout: StackLayout {
                    orientation: LayoutOrientation.RightToLeft
                }
                SlideoutCommand {
                    objectName: "func3_act7"
                    property bool actionVisible: true
                    visible: f3slideout.childrenVisible && actionVisible
                }
                SlideoutCommand {
                    objectName: "func3_act6"
                    property bool actionVisible: true
                    visible: f3slideout.childrenVisible && actionVisible
                }
                SlideoutCommand {
                    objectName: "func3_act5"
                    property bool actionVisible: true
                    visible: f3slideout.childrenVisible && actionVisible
                }
                SlideoutCommand {
                    objectName: "func3_act4"
                    property bool actionVisible: true
                    visible: f3slideout.childrenVisible && actionVisible
                }
                SlideoutCommand {
                    objectName: "func3_act3"
                    property bool actionVisible: true
                    visible: f3slideout.childrenVisible && actionVisible
                }
                SlideoutCommand {
                    objectName: "func3_act2"
                    property bool actionVisible: true
                    visible: f3slideout.childrenVisible && actionVisible
                }
                SlideoutCommand {
                    objectName: "func3_act1"
                    property bool actionVisible: true
                    visible: f3slideout.childrenVisible && actionVisible
                }
            }
        }

        Container {
            id: queueUniverse
            objectName: "queueUniverse"
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 0
            }
            preferredHeight: dimensions.queueHeight
            preferredWidth: dimensions.screenWidth
            visible: ! compilePhaseContainer.visible
            layout: AbsoluteLayout {
            }
            ImageView {
                id: progressBarBackground
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0
                    positionY: 0
                }
                preferredHeight: dimensions.queueHeight
                preferredWidth: dimensions.screenWidth
                imageSource: "asset:///images/queue-glow.amd"
            }

            Container {
                id: progressBar
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0
                    positionY: 0
                }
                preferredHeight: dimensions.queueHeight
                preferredWidth: dimensions.screenWidth
                background: Color.Black
                opacity: 0.7
                animations: [
                    SequentialAnimation {
                        id: progressAnimation
                        objectName: "progressAnimation"
                        TranslateTransition {
                            id: translateLeft
                            fromX: 0
                            toX: - dimensions.screenWidth
                            duration: 1900 // FIXME: Connect duration to app timer
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
                background: Color.Black
                bottomMargin: dimensions.itemPadding

                ImageView {
                    imageSource: "asset:///images/forward.png"
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
                background: Color.Black
                bottomMargin: dimensions.itemPadding

                ImageView {
                    imageSource: "asset:///images/left.png"
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
                background: Color.Black
                bottomMargin: dimensions.itemPadding

                ImageView {
                    imageSource: "asset:///images/right.png"
                }

                gestureHandlers: [
                    TapHandler {
                        onTapped: {
                            _app.tapRight();
                        }
                    }
                ]
            }

            Container {
                id: cmdF1
                horizontalAlignment: HorizontalAlignment.Center
                preferredWidth: dimensions.cmdWidth
                preferredHeight: dimensions.cmdHeight
                background: Color.Black
                bottomMargin: dimensions.itemPadding
                visible: _app.functionCount > 0

                ImageView {
                    imageSource: "asset:///images/f1.png"
                }

                gestureHandlers: [
                    TapHandler {
                        onTapped: {
                            _app.tapF1();
                        }
                    }
                ]
            }

            Container {
                id: cmdF2
                horizontalAlignment: HorizontalAlignment.Center
                preferredWidth: dimensions.cmdWidth
                preferredHeight: dimensions.cmdHeight
                background: Color.Black
                bottomMargin: dimensions.itemPadding
                visible: _app.functionCount > 1

                ImageView {
                    imageSource: "asset:///images/f2.png"
                }

                gestureHandlers: [
                    TapHandler {
                        onTapped: {
                            _app.tapF2();
                        }
                    }
                ]
            }

            Container {
                id: cmdF3
                horizontalAlignment: HorizontalAlignment.Center
                preferredWidth: dimensions.cmdWidth
                preferredHeight: dimensions.cmdHeight
                background: Color.Black
                bottomMargin: dimensions.itemPadding
                visible: _app.functionCount > 2

                ImageView {
                    imageSource: "asset:///images/f3.png"
                }

                gestureHandlers: [
                    TapHandler {
                        onTapped: {
                            _app.tapF3();
                        }
                    }
                ]
            }

            Container {
                id: cmdViewFunctions
                horizontalAlignment: HorizontalAlignment.Center
                preferredWidth: dimensions.cmdWidth
                preferredHeight: dimensions.cmdHeight
                background: Color.Black
                bottomMargin: dimensions.itemPadding
                visible: _app.functionCount > 0 && compilePhaseContainer.visible == false

                ImageView {
                    imageSource: "asset:///images/viewfunctions.png"
                }

                gestureHandlers: [
                    TapHandler {
                        onTapped: {
                            _app.tapViewFunctions();
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
            layout: DockLayout {
            }

            Button {
                text: "Pause"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                visible: dimensions.screenWidth > 1000 && compilePhaseContainer.visible == false && menuContainer.visible == false
                onClicked: {
                    _app.pause();
                    menuContainer.setVisible(true);
                }
            }
        }

        Container {
            id: compilePhaseContainer
            objectName: "compilePhaseContainer"
            preferredWidth: dimensions.playAreaWidth
            preferredHeight: dimensions.screenHeight

            Container {
                preferredWidth: dimensions.compileMapWidth
                preferredHeight: dimensions.compileMapHeight
                layout: DockLayout {
                }

                Button {
                    text: "Start"
                    opacity: 0.7
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    onClicked: {
                        compilePhaseContainer.setVisible(false);
                        _app.compilePhaseDone();
                    }
                }
            }
            Container {
                objectName: "compileFunctionContainer"
                preferredWidth: dimensions.compileMapWidth
                preferredHeight: dimensions.compileFunctionHeight
                background: Color.Green
                layout: DockLayout {
                }

                Container {
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center

                    Container {
                        preferredWidth: dimensions.compileMapWidth
                        preferredHeight: dimensions.cmdHeight
                        bottomMargin: dimensions.itemPadding
                        layout: DockLayout {
                        }

                        Container {
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }

                            FunctionCommand {
                                objectName: "functionHeader"
                                background: Color.Gray
                                gestureHandlers: [
                                    TapHandler {
                                        onTapped: {
                                            _app.selectNextFunction();
                                        }
                                    }
                                ]
                            }
                            FunctionCommand {
                                objectName: "functionAction1"
                                gestureHandlers: [
                                    TapHandler {
                                        onTapped: {
                                            _app.removeFunctionCommand(0);
                                        }
                                    }
                                ]
                            }
                            FunctionCommand {
                                objectName: "functionAction2"
                                gestureHandlers: [
                                    TapHandler {
                                        onTapped: {
                                            _app.removeFunctionCommand(1);
                                        }
                                    }
                                ]
                            }
                            FunctionCommand {
                                objectName: "functionAction3"
                                gestureHandlers: [
                                    TapHandler {
                                        onTapped: {
                                            _app.removeFunctionCommand(2);
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
                                            _app.removeFunctionCommand(3);
                                        }
                                    }
                                ]
                            }
                            FunctionCommand {
                                objectName: "functionAction5"
                                gestureHandlers: [
                                    TapHandler {
                                        onTapped: {
                                            _app.removeFunctionCommand(4);
                                        }
                                    }
                                ]
                            }
                            FunctionCommand {
                                objectName: "functionAction6"
                                gestureHandlers: [
                                    TapHandler {
                                        onTapped: {
                                            _app.removeFunctionCommand(5);
                                        }
                                    }
                                ]
                            }
                            FunctionCommand {
                                objectName: "functionAction7"
                                gestureHandlers: [
                                    TapHandler {
                                        onTapped: {
                                            _app.removeFunctionCommand(6);
                                        }
                                    }
                                ]
                            }
                        }
                    }
                }
            }
        }

        Container {
            id: menuContainer
            objectName: "menuContainer"
            preferredWidth: dimensions.screenWidth
            preferredHeight: dimensions.screenHeight
            background: Color.Black
            layout: DockLayout {
            }
            visible: false

            Container {
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                background: Color.Red
                Label {
                    objectName: "menuTitle"
                    text: "Paused"
                    horizontalAlignment: HorizontalAlignment.Center
                    textStyle.textAlign: TextAlign.Center
                }
                Button {
                    objectName: "menuButton"
                    horizontalAlignment: HorizontalAlignment.Center
                    text: "Continue"
                    onClicked: {
                        menuContainer.setVisible(false);
                        _app.clickMenuButton();
                    }
                }
                Button {
                    horizontalAlignment: HorizontalAlignment.Center
                    text: "Menu"
                    onClicked: {
                        menuContainer.setVisible(false);
                        _app.back();
                    }
                }
            }
        }
    }

    shortcuts: [
        Shortcut {
            key: "space"
            onTriggered: {
                _app.pause();
                menuContainer.setVisible(true);
            }
        }
    ]
}
