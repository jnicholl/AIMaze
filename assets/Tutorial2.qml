import bb.cascades 1.0

Container {
    id:tutorial
    property variant dimensions: Dimensions {}    
    property int state: 0

    signal showRun
    signal showCompile
    signal hideTutorial

    preferredWidth: dimensions.screenWidth
    preferredHeight: dimensions.screenHeight
    layout: AbsoluteLayout {
    }
    
    function showScreen() {
        console.log("State = " + state);
        if (state == 0) {
            tutorial.showCompile();
        }
        if (state == 1) {
            tutorial.showRun();
        }
        if (state == 2) {
            tutorial.showCompile();
            tutorial.hideTutorial();
        }
    }

    Button {
        text: "Next"
        onClicked: {
            state ++;
            showScreen();
            if (state == 1) {
                text = "Done"
            }
        }
        visible: state < 2
        layoutProperties: AbsoluteLayoutProperties {
            positionX: 500
            positionY: 600
        }
    }

    Button {
        text: "Previous"
        onClicked: {
            state --;
            showScreen();
        }
        visible: state > 0
        layoutProperties: AbsoluteLayoutProperties {
            positionX: 50
            positionY: 600
        }
    }

    Label {
        preferredWidth: dimensions.playAreaWidth / 3
        text: "Add action to function"
        multiline: true
        textStyle.textAlign: TextAlign.Center
        textStyle.color: Color.create("#BAEEFF")
        layoutProperties: AbsoluteLayoutProperties {
            positionX: 400
            positionY: 100
        }
        visible: state == 0
    }

    Label {
        preferredWidth: dimensions.playAreaWidth / 2
        text: "Tap to remove "
        multiline: true
        textStyle.textAlign: TextAlign.Center
        textStyle.color: Color.create("#BAEEFF")
        layoutProperties: AbsoluteLayoutProperties {
            positionX: 200
            positionY: 520
        }
        visible:state==0
    }

    Label {
        preferredWidth: 500
        text: "All actions inside a function are executed within one move"
        multiline: true
        textStyle.textAlign: TextAlign.Center
        textStyle.color: Color.create("#BAEEFF")
        layoutProperties: AbsoluteLayoutProperties {
            positionX: 50
            positionY: 350
        }
        visible: state == 0
    }
    
    Label {
        preferredWidth: dimensions.playAreaWidth / 3
        text: "View actions within functions"
        multiline: true
        textStyle.textAlign: TextAlign.Center
        textStyle.color: Color.create("#BAEEFF")
        layoutProperties: AbsoluteLayoutProperties {
            positionX: 400
            positionY: 400
        }
        visible: state == 1
    }

    Label {
        preferredWidth: dimensions.playAreaWidth
        text: "Add function to queue"
        multiline: true
        textStyle.textAlign: TextAlign.Center
        textStyle.color: Color.create("#BAEEFF")
        layoutProperties: AbsoluteLayoutProperties {
            positionX: 120
            positionY: 325
        }
        visible: state == 1
    }
}