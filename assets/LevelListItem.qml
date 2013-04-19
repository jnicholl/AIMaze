// Grid view with detail project template
import bb.cascades 1.0

// Item used by the list to present a small thumbnail image with text.
Container {
    id: listItem
    property int lastAvailableIndex
    layout: DockLayout {
    }
    Container {
        // this container is used to show selection background when item is selected
        id: mySelectionContainer
        opacity: 0.0
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
    }
    Container {
        layout: DockLayout {}
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
        preferredWidth: 240
        preferredHeight: 240
        // and text below
        ImageView {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            imageSource: "asset:///images/levelbutton.png"
            scalingMethod: ScalingMethod.AspectFit
        }
        Label {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            text: (listItem.ListItem.indexInSection > lastAvailableIndex)? "X" : ListItemData.text
            textStyle.base: SystemDefaults.TextStyles.BigText
            textStyle.textAlign: TextAlign.Center
        }
    }

    // Set visual appearance of activated and selected item.
    function setHighlight(active) {
        mySelectionContainer.opacity = active ? 1.0 : 0.0;
    }

    // Signal handler for list item activation.
    ListItem.onActivationChanged: {
        console.debug("ActivationChanged")
        setHighlight(ListItem.active);
    }
}
