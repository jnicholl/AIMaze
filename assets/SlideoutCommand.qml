import bb.cascades 1.0

Container {
    property variant dimensions: Dimensions {
    }
    property int index: 0
    property bool highlighted: false
    property alias imageSource: view.imageSource
    verticalAlignment: VerticalAlignment.Center
    preferredWidth: dimensions.cmdWidth
    preferredHeight: dimensions.cmdHeight
    background: highlighted?highlight.imagePaint:normal.imagePaint
    visible: imageSource != ""

    ImageView {
        id: view
    }
    attachedObjects: [
        ImagePaintDefinition {
            id: highlight
            imageSource: "asset:///images/highlight.png"
        },
        ImagePaintDefinition {
            id: normal
            imageSource: "asset:///images/border.png"
        }
    ]
}
