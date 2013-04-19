import bb.cascades 1.0

Container {
    property variant dimensions: Dimensions {
    }
    property alias imageSource: view.imageSource
    verticalAlignment: VerticalAlignment.Center
    preferredWidth: dimensions.cmdWidth
    preferredHeight: dimensions.cmdHeight
    background: Color.Black
    rightMargin: dimensions.queueItemPadding

    ImageView {
        id: view
    }
}