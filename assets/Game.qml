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
                positionY: compilePhaseContainer.visible ? dimensions.sidebarPadding : dimensions.queueHeight + dimensions.sidebarPadding
            }
            preferredWidth: dimensions.playAreaWidth
            preferredHeight: compilePhaseContainer.visible ? dimensions.compileMapHeight : dimensions.playAreaHeight
            layout: DockLayout {
            }
        }
        
        Label {
            id: movesLeftLabel
            objectName: "movesLeft"
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: compilePhaseContainer.visible ? 0 : dimensions.queueHeight - dimensions.itemPadding
            }
            preferredWidth: dimensions.playAreaWidth
            preferredHeight: dimensions.sidebarPadding
            textStyle.textAlign: TextAlign.Center
            text: "10 moves left"
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
                objectName: "progressBar"
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
                leftPadding: dimensions.sidebarPadding * 4/3
            }
        }
        
//        SideActionBar {
//            id: sidebar
//            layoutProperties: AbsoluteLayoutProperties {
//                positionX: dimensions.playAreaWidth
//                positionY: 0
//            }
//            preferredHeight: dimensions.screenHeight
//            preferredWidth: dimensions.sidebarWidth
//            topPadding: dimensions.itemPadding + 4
//            bottomPadding: dimensions.itemPadding
//            
//            functionCount: _app.functionCount
//            showViewFunctions: !compilePhaseContainer.visible
//            
//            onForwardPressed: {
//                _app.tapForward();
//            }
//            onLeftPressed: {
//                _app.tapLeft();
//            }
//            onRightPressed: {
//                _app.tapRight();
//            }
//            onFunctionPressed: {
//                switch (func) {
//                case 1:
//                    _app.tapF1();
//                    break;
//                case 2:
//                    _app.tapF2();
//                    break;
//                case 3:
//                    _app.tapF3();
//                    break;
//                }
//            }
//            onViewFunctionsPressed: {
//                _app.tapViewFunctions();
//            }
//        }

        Container {
            id: sidebar
            layoutProperties: AbsoluteLayoutProperties {
                positionX: dimensions.playAreaWidth
                positionY: 0
            }
            preferredHeight: dimensions.screenHeight
            preferredWidth: dimensions.sidebarWidth
            //background: Color.Black // FIXME: Image
            topPadding: dimensions.itemPadding+4
            bottomPadding: dimensions.itemPadding

			SidebarCommand {
                id: cmdForward
                ImageButton {
                    defaultImageSource: "asset:///images/forward.png"
                    pressedImageSource: "asset:///images/forwardp.png"
                    onClicked: {
                        _app.tapForward();
                    }
                }
            }

            SidebarCommand {
                id: cmdLeft
                ImageButton {
                    defaultImageSource: "asset:///images/left.png"
                    pressedImageSource: "asset:///images/leftp.png"
                    onClicked: {
                        _app.tapLeft();
                    }
                }
            }

            SidebarCommand {
                id: cmdRight
                ImageButton {
                    defaultImageSource: "asset:///images/right.png"
                    pressedImageSource: "asset:///images/rightp.png"
                    onClicked: {
                        _app.tapRight();
                    }
                }
            }

            SidebarCommand {
                id: cmdF1
                visible: _app.functionCount > 1
                ImageButton {
                    defaultImageSource: "asset:///images/f1.png"
                    pressedImageSource: "asset:///images/f1p.png"
                    onClicked: {
                        _app.tapF1();
                    }
                }
            }

            SidebarCommand {
                id: cmdF2
                visible: _app.functionCount > 2
                ImageButton {
                    defaultImageSource: "asset:///images/f2.png"
                    pressedImageSource: "asset:///images/f2p.png"
                    onClicked: {
                        _app.tapF2();
                    }
                }
            }

            SidebarCommand {
                id: cmdF3
                visible: _app.functionCount > 3
                ImageButton {
                    defaultImageSource: "asset:///images/f3.png"
                    pressedImageSource: "asset:///images/f3p.png"
                    onClicked: {
                        _app.tapF3();
                    }
                }
            }

            SidebarCommand {
                id: cmdViewFunctions
                visible: _app.functionCount > 1 && compilePhaseContainer.visible == false
                ImageButton {
                    defaultImageSource: "asset:///images/viewfunctions.png"
                    pressedImageSource: "asset:///images/viewfunctionsp.png"
                    onClicked: {
                        _app.tapViewFunctions();
                    }
                }
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
                    _app.showMenu();
                }
            }
        }

		CompilePhase {
		    id: compilePhaseContainer
		    objectName: "compilePhaseContainer"
		    preferredWidth: dimensions.playAreaWidth
		    preferredHeight: dimensions.screenHeight
            startButtonVisible: _app.tutorial == 0
		    onStart: {
                compilePhaseContainer.setVisible(false);
                _app.compilePhaseDone();
            }
		    onSelectNext: {
          		_app.selectNextFunction();
            }
		    onRemoveAction: {
          		_app.removeFunctionCommand(index);
            }
		}

        Tutorial1 {
            id: tutorial1Container
            objectName: "tutorial1Container"
            visible: (_app.tutorial == 1)
            onStart: {
                console.log("Started");
                _app.tutorial = 0;
                //compilePhaseContainer.start(); // FIXME: Tutorial needs changes
            }        
            onShowRun: {
                compilePhaseContainer.visible = false;
            }
            onShowCompile: {
                compilePhaseContainer.visible = true;
            }
        }

        Tutorial2 {
            id: tutorial2Container
            objectName: "tutorial2Container"
            visible: (_app.tutorial == 2)
            onShowRun: {
                compilePhaseContainer.visible = false;
            }
            onShowCompile: {
                compilePhaseContainer.visible = true;
            }
            onHideTutorial: {
                _app.tutorial = 0;
            }
        }

        Tutorial3 {
            id: tutorial3Container
            objectName: "tutorial3Container"
            visible: (_app.tutorial == 3)
            onHideTutorial: {
                _app.tutorial = 0;
            }
        }
        
        Container {
            id: creditsContainer
            objectName: "creditsContainer"
            visible: false
            preferredWidth: dimensions.screenWidth
            preferredHeight: dimensions.screenHeight
            background: Color.Black
            layout: DockLayout {
            }
            ImageView {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Top
                imageSource: "asset:///images/title.png"
            }
            Container {
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                background: Color.Black
                layout: StackLayout {
                }

                Label {
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    textStyle.textAlign: TextAlign.Center
                    multiline: true
                    text: "You have completed all levels!
Thank you for playing!"
                    bottomMargin: 20
                }
                Label {
                    text: "Game created by Jeremy Nicholl"
                }
            }
            Button {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Bottom
                text: "Menu"
                onClicked: {
                    creditsContainer.setVisible(false);
                    _app.back();
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
            ImageView {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Top
                imageSource: "asset:///images/title.png"
            }
            Container {
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                background: Color.Black // FIXME: Image
                layout: StackLayout {}
                
                Button {
                    objectName: "menuButton"
                    horizontalAlignment: HorizontalAlignment.Center
                    text: "Continue"
                    bottomMargin: 20
                    preferredWidth: 250
                    onClicked: {
                        menuContainer.setVisible(false);
                        _app.clickMenuButton();
                    }
                }
                Button {
                    horizontalAlignment: HorizontalAlignment.Center
                    text: "Menu"
                    preferredWidth: 250
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
                _app.showMenu();
            }
        }
    ]
}
