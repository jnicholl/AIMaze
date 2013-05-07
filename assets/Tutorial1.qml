import bb.cascades 1.0

Container {
    id:tutorial
    property variant dimensions: Dimensions {}    
    property int state: 0

    signal hideTutorial
    signal showRun
    signal showCompile

    preferredWidth: dimensions.screenWidth
    preferredHeight: dimensions.screenHeight
    layout: DockLayout {
    }

    function showScreen()
    {
    }

    Container {
        visible: state == 0
        preferredWidth: dimensions.screenWidth
        preferredHeight: dimensions.screenHeight
        background: Color.create(0, 0.3, 0.3, 0.7)
        layout: DockLayout {
        }
        Label {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            textStyle.textAlign: TextAlign.Center
            multiline: true
            text: "EMUU Training - Part 1

I see you've forgotten to bring your training manuals with you? It's simple. The robot doesn't know how to do anything. We plan the route, we run the route. Not sure why the folks at home were so excited about this advanced tech, falling from the stars, but they want to get this robot moving."
        }
    }

    Container {
        visible: state == 1
        preferredWidth: dimensions.screenWidth
        preferredHeight: dimensions.screenHeight
        background: Color.create(0, 0.3, 0.3, 0.7)
        layout: DockLayout {
        }
        Label {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            textStyle.textAlign: TextAlign.Center
            multiline: true
            text: "EMUU Training - Part 1

We'll start you on the simulator first. Just choose your moves, then hit start and make sure to hit the right buttons in time. There are a few glitches in the sim, but the robot should follow your orders."
        }
    }

    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Bottom
        bottomPadding: dimensions.itemPadding
        
        Button {
            text: "Previous"
            onClicked: {
                state --;
            }
            enabled: state > 0
            rightMargin: dimensions.queueItemPadding
        }

        Button {
            id: skip
            text: "Skip"
            onClicked: {
                tutorial.hideTutorial();
            }
            enabled: state < 1
            rightMargin: dimensions.queueItemPadding
        }

        Button {
            id: next
            text: state<1?"Next":"Done"
            onClicked: {
                state ++;
                if (state > 1)
                	tutorial.hideTutorial();
            }
            rightMargin: dimensions.queueItemPadding
        }
    }

    //    Button {
//        text: "Start"
//        onClicked: {            
//            tutorial.start();
//        }
//        visible: state == 3
//        layoutProperties: AbsoluteLayoutProperties {
//            positionX: 213
//            positionY: 195
//        }
//    }


	
//    Label {
//        preferredWidth: dimensions.playAreaWidth / 3
//        text: "EMUU"
//        multiline: true
//        textStyle.textAlign: TextAlign.Center
//        textStyle.color: Color.create("#BAEEFF")
//        layoutProperties: AbsoluteLayoutProperties {
//            positionX: 50
//            positionY: 340
//        }
//        visible:state==1
//    }
//    
//    Label {
//        preferredWidth: dimensions.playAreaWidth / 3
//        text: "Goal"
//        multiline: true
//        textStyle.textAlign: TextAlign.Center
//        textStyle.color: Color.create("#BAEEFF")
//        layoutProperties: AbsoluteLayoutProperties {
//            positionX: 350
//            positionY: 240
//        }
//        visible:state==1
//    }
//
//    Label {
//        preferredWidth: dimensions.playAreaWidth / 3
//        text: "Moves available"
//        multiline: true
//        textStyle.textAlign: TextAlign.Center
//        textStyle.color: Color.create("#BAEEFF")
//        layoutProperties: AbsoluteLayoutProperties {
//            positionX: 440
//            positionY: 100
//        }
//        visible:state==1
//    }
//
//    Container {
//        preferredWidth: 185
//        preferredHeight: 40
//        layoutProperties: AbsoluteLayoutProperties {
//            positionX: 210
//            positionY: 0
//        }
//        visible:state==1
//        background: Color.create("#BAEEFF")
//        opacity:0.5
//    }
//    
//    Label {
//        preferredWidth: dimensions.playAreaWidth / 4
//        text: "Add actions to queue"
//        multiline: true
//        textStyle.textAlign: TextAlign.Center
//        textStyle.color: Color.create("#BAEEFF")
//        layoutProperties: AbsoluteLayoutProperties {
//            positionX: dimensions.playAreaWidth * 3 / 4
//            positionY: dimensions.playAreaHeight / 4
//        }
//        visible:state==2
//    }
//
//    Label {
//        preferredWidth: dimensions.playAreaWidth
//        text: "Pending actions"
//        multiline: true
//        textStyle.textAlign: TextAlign.Center
//        textStyle.color: Color.create("#BAEEFF")
//        layoutProperties: AbsoluteLayoutProperties {
//            positionX: 100
//            positionY: 30
//        }
//        visible: state == 2
//    }
//
//    Label {
//        preferredWidth: dimensions.playAreaWidth
//        text: "Once you start the game, there is a limited amount of time to enter your moves as they are executed.
//        
//Press Start to begin"
//        multiline: true
//        textStyle.textAlign: TextAlign.Center
//        textStyle.color: Color.create("#BAEEFF")
//        layoutProperties: AbsoluteLayoutProperties {
//            positionX: 0
//            positionY: dimensions.playAreaHeight / 2
//        }
//        visible: state == 3
//    }
}