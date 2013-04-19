import bb.cascades 1.0

Container {
    horizontalAlignment: HorizontalAlignment.Center
    preferredWidth: dimensions.cmdWidth
    preferredHeight: dimensions.cmdHeight
    background: normal.imagePaint
    bottomMargin: dimensions.itemPadding

    attachedObjects: [
        ImagePaintDefinition {
            id: normal
            imageSource: "asset:///images/border.png"
        }
    ]
}