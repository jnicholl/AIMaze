import bb.cascades 1.0

Container {
    property variant dimensions: Dimensions {
    }
    property alias imageSource: view.imageSource
    property bool showing: true
    verticalAlignment: VerticalAlignment.Center
    preferredWidth: dimensions.cmdWidth
    preferredHeight: dimensions.cmdHeight
    background: showing?normal.imagePaint:Color.Black
    rightMargin: dimensions.queueItemPadding

    ImageView {
        id: view
        visible: showing
    }

    attachedObjects: [
        ImagePaintDefinition {
            id: normal
            imageSource: "asset:///images/border.png"
        }
    ]
}