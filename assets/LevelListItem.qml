// Grid view with detail project template
import bb.cascades 1.0

// Item used by the list to present a small thumbnail image with text.
Container {
    id: listItem
    property int lastAvailableIndex
    property int score: ListItemData.score
    layout: DockLayout {
    }
    //verticalAlignment: VerticalAlignment.Fill
    //horizontalAlignment: HorizontalAlignment.Fill
    preferredWidth: 160
    preferredHeight: 160
    
    ImageButton {
        preferredWidth: 160
        preferredHeight: 160
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center
        defaultImageSource: "asset:///images/level" + ListItemData.text + ".png"
        pressedImageSource: "asset:///images/level" + ListItemData.text + "p.png"
        disabledImageSource: "asset:///images/level" + ListItemData.text + "d.png"
        enabled: (listItem.ListItem.indexInSection <= lastAvailableIndex)
        onClicked: {
            listItem.ListItem.view.triggered(listItem.ListItem.indexPath);
        }
    }
    Label {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Bottom
        textStyle.textAlign: TextAlign.Center
        textStyle.color: Color.create(0, 0.7, 0.7)
        text: score
        visible: score > 0
    }
}
    //    Container {
//        // this container is used to show selection background when item is selected
//        id: mySelectionContainer
//        opacity: 0.0
//        verticalAlignment: VerticalAlignment.Fill
//        horizontalAlignment: HorizontalAlignment.Fill
//    }
//    Container {
//        layout: DockLayout {}
//        verticalAlignment: VerticalAlignment.Fill
//        horizontalAlignment: HorizontalAlignment.Fill
//        preferredWidth: 160
//        preferredHeight: 160
//        background: Color.Red
//        // and text below
//        ImageButton {
//            horizontalAlignment: HorizontalAlignment.Center
//            verticalAlignment: VerticalAlignment.Center
//            defaultImageSource: "asset:///images/level"+ListItemData.text+".png"
//            pressedImageSource: "asset:///images/level"+ListItemData.text+"p.png"
//            disabledImageSource: "asset:///images/level"+ListItemData.text+"d.png"
//            enabled: (listItem.ListItem.indexInSection <= lastAvailableIndex)
//            onClicked: {
//            	listItem.ListItem.view.triggered(listItem.ListItem.indexPath);
//            }
//            //scalingMethod: ScalingMethod.AspectFit
//        }
        
//        Container {
//            horizontalAlignment: HorizontalAlignment.Center
//            verticalAlignment: VerticalAlignment.Center
//            preferredWidth: 160
//            preferredHeight: 160
//            opacity: 0.7
//            visible: (listItem.ListItem.indexInSection > lastAvailableIndex)
//            background: Color.Black
//        }
//        Label {
//            horizontalAlignment: HorizontalAlignment.Center
//            verticalAlignment: VerticalAlignment.Center
//            text: (listItem.ListItem.indexInSection > lastAvailableIndex)? "X" : ListItemData.text
//            textStyle.base: SystemDefaults.TextStyles.BigText
//            textStyle.textAlign: TextAlign.Center
//        }
//    }

    // Set visual appearance of activated and selected item.
//    function setHighlight(active) {
//        mySelectionContainer.opacity = active ? 1.0 : 0.0;
//    }
//
//    // Signal handler for list item activation.
//    ListItem.onActivationChanged: {
//        console.debug("ActivationChanged")
//        setHighlight(ListItem.active);
//    }
//}
