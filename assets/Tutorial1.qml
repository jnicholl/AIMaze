import bb.cascades 1.0

Container {
    property variant dimensions: Dimensions {}
    
    preferredWidth: dimensions.screenWidth
    preferredHeight: dimensions.screenHeight
    touchPropagationMode: TouchPropagationMode.None
    layout: AbsoluteLayout {
    }
    Label {
        preferredWidth: dimensions.playAreaWidth
        text: "Press the start button"
        multiline: true
        textStyle.textAlign: TextAlign.Center
        layoutProperties: AbsoluteLayoutProperties {
            positionX: 0
            positionY: dimensions.playAreaHeight / 4
        }
    }
}