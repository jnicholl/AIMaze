import bb.cascades 1.0

Container {
    property variant dimensions: Dimensions {
    }
    property int index: 0
    property alias imageSource: view.imageSource
    verticalAlignment: VerticalAlignment.Center
    preferredWidth: dimensions.cmdWidth
    preferredHeight: dimensions.cmdHeight
    background: Color.Black
    visible: imageSource != ""

    ImageView {
        id: view
    }
}
