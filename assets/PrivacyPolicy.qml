import bb.cascades 1.0

Sheet {
    id: sheet
    peekEnabled: false
    Page {
        Container {
            id: root
            property variant dimensions: Dimensions {
            }
            layout: DockLayout {
            }
//            ScrollView {
//                verticalAlignment: VerticalAlignment.Top
//                horizontalAlignment: HorizontalAlignment.Center
//                preferredHeight: root.dimensions.screenHeight - 100
//                content: Container {
//                    ImageView {
//                        verticalAlignment: VerticalAlignment.Top
//                        horizontalAlignment: HorizontalAlignment.Center
//                        imageSource: "asset:///images/help.png"
//                    }
                    Label {
                        verticalAlignment: VerticalAlignment.Top
                        horizontalAlignment: HorizontalAlignment.Center
                        textStyle.textAlign: TextAlign.Center
                        text: "This application does not collect, use or disclose any user information."
                        multiline: true
                    }
//                }
//            }
            Button {
                id: closeButton
                verticalAlignment: VerticalAlignment.Bottom
                horizontalAlignment: HorizontalAlignment.Fill
                text: "Close"
                onClicked: {
                    sheet.close();
                }
            }
        }
    }
}
