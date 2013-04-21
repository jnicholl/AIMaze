import bb.cascades 1.0

Container {
    id:tutorial
    property variant dimensions: Dimensions {}    
    property int state: 0

    signal hideTutorial

    preferredWidth: dimensions.screenWidth
    preferredHeight: dimensions.screenHeight
    layout: AbsoluteLayout {
    }

    Button {
        text: "Done"
        onClicked: {
            hideTutorial();
        }
        layoutProperties: AbsoluteLayoutProperties {
            positionX: 500
            positionY: 600
        }
    }

    Label {
        preferredWidth: dimensions.playAreaWidth / 2
        text: "Functions can call other functions,
BUT it costs a move"
        multiline: true
        textStyle.textAlign: TextAlign.Center
        textStyle.color: Color.create("#BAEEFF")
        layoutProperties: AbsoluteLayoutProperties {
            positionX: 300
            positionY: 300
        }
        visible: state == 0
    }

    Label {
        preferredWidth: 250
        text: "Change selected function"
        multiline: true
        textStyle.textAlign: TextAlign.Center
        textStyle.color: Color.create("#BAEEFF")
        layoutProperties: AbsoluteLayoutProperties {
            positionX: 0
            positionY: 600
        }
        visible:state==0
    }
}