import bb.cascades 1.0

Container {
    id:tutorial
    property variant dimensions: Dimensions {}    
    property int state: 0

    signal start
    signal showRun
    signal showCompile

    preferredWidth: dimensions.screenWidth
    preferredHeight: dimensions.screenHeight
    layout: AbsoluteLayout {
    }
    
    function showScreen()
    {
        if (state == 1) {
            tutorial.showCompile();
        }
        if (state == 2) {
            tutorial.showRun();
        }
        if (state == 3) {
            tutorial.showCompile();
        }
    }
    
    Button {
        text:"Next"
        onClicked:{
            state ++;
            showScreen();
        }
        visible:state<3
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
        visible:state>0
        layoutProperties: AbsoluteLayoutProperties {
            positionX: 50
            positionY: 600
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

	
    Label {
        preferredWidth: dimensions.playAreaWidth / 3
        text: "EMUU"
        multiline: true
        textStyle.textAlign: TextAlign.Center
        textStyle.color: Color.create("#BAEEFF")
        layoutProperties: AbsoluteLayoutProperties {
            positionX: 50
            positionY: 340
        }
        visible:state==0
    }
    
    Label {
        preferredWidth: dimensions.playAreaWidth / 3
        text: "Goal"
        multiline: true
        textStyle.textAlign: TextAlign.Center
        textStyle.color: Color.create("#BAEEFF")
        layoutProperties: AbsoluteLayoutProperties {
            positionX: 350
            positionY: 240
        }
        visible:state==0
    }

    Label {
        preferredWidth: dimensions.playAreaWidth / 3
        text: "Moves available"
        multiline: true
        textStyle.textAlign: TextAlign.Center
        textStyle.color: Color.create("#BAEEFF")
        layoutProperties: AbsoluteLayoutProperties {
            positionX: 440
            positionY: 100
        }
        visible:state==1
    }

    Container {
        preferredWidth: 185
        preferredHeight: 40
        layoutProperties: AbsoluteLayoutProperties {
            positionX: 210
            positionY: 0
        }
        visible:state==1
        background: Color.create("#BAEEFF")
        opacity:0.5
    }
    
    Label {
        preferredWidth: dimensions.playAreaWidth / 4
        text: "Add actions to queue"
        multiline: true
        textStyle.textAlign: TextAlign.Center
        textStyle.color: Color.create("#BAEEFF")
        layoutProperties: AbsoluteLayoutProperties {
            positionX: dimensions.playAreaWidth * 3 / 4
            positionY: dimensions.playAreaHeight / 4
        }
        visible:state==2
    }

    Label {
        preferredWidth: dimensions.playAreaWidth
        text: "Pending actions"
        multiline: true
        textStyle.textAlign: TextAlign.Center
        textStyle.color: Color.create("#BAEEFF")
        layoutProperties: AbsoluteLayoutProperties {
            positionX: 100
            positionY: 30
        }
        visible: state == 2
    }

    Label {
        preferredWidth: dimensions.playAreaWidth
        text: "Once you start the game, there is a limited amount of time to enter your moves as they are executed.
        
Press Start to begin"
        multiline: true
        textStyle.textAlign: TextAlign.Center
        textStyle.color: Color.create("#BAEEFF")
        layoutProperties: AbsoluteLayoutProperties {
            positionX: 0
            positionY: dimensions.playAreaHeight / 2
        }
        visible: state == 3
    }
}