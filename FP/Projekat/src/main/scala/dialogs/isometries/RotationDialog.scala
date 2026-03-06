package com.djordjemilanovic.minesweeper
package dialogs.isometries

import minefield.Minefield
import minefield.editors.Transforming.RotationDirection
import minefield.editors.isometries.Rotation
import utils.Location

import scalafx.beans.property.ObjectProperty
import scalafx.geometry.Insets
import scalafx.scene.control.*
import scalafx.scene.layout.{GridPane, VBox}

class RotationDialog(minefield: Minefield) extends IsometryCreatorDialog {
  title = "Create rotation isometry"
  
  private val observableDirection = new ObjectProperty[RotationDirection]
  private var chosenAnchorLocation = Option.empty[Location]
  
  private val buttonsGrid = new GridPane {
    hgap = 2
    vgap = 2
  }
  redraw()

  dialogPane.value.setContent(new VBox(10) {
    padding = Insets(10)
    children = Seq(
      buttonsGrid,
      new ComboBox[RotationDirection](RotationDirection.values) {
        promptText = "Select rotation direction"
        observableDirection <== selectionModel().selectedItemProperty()
      }
    )
  })

  resultConverter = {
    case ButtonType.OK => Some(Rotation(minefield.dimensions, observableDirection.value, chosenAnchorLocation.get))
    case _ => None
  }

  dialogPane.value.getButtonTypes.addAll(ButtonType.OK, ButtonType.Cancel)

  private def redraw(): Unit = {
    buttonsGrid.children.clear()

    for (y <- 0 until minefield.dimensions.height; x <- 0 until minefield.dimensions.width) {
      val location = Location(y, x)
      val cell = minefield(location)
      val btn = new Button {
        text = cell.toString
        minWidth = 30
        minHeight = 30
        style = if (chosenAnchorLocation.contains(location)) "-fx-background-color: yellow;" else ""
        onMouseClicked = e => {
          chosenAnchorLocation = Some(location)
          redraw()
        }
      }
      buttonsGrid.add(btn, x, y)
    }
  }
}
