package com.djordjemilanovic.minesweeper
package dialogs.isometries

import minefield.Minefield
import minefield.editors.Transforming.ReflectionAxis
import minefield.editors.Transforming.ReflectionAxis.*
import minefield.editors.isometries.Reflection
import utils.Location

import scalafx.geometry.Insets
import scalafx.scene.control.{Button, ButtonType, Dialog}
import scalafx.scene.layout.{GridPane, VBox}

class ReflectionDialog(minefield: Minefield) extends IsometryCreatorDialog {
  title = "Create reflection isometry"

  private var index = Option.empty[Int]
  private var axis = Option.empty[ReflectionAxis]

  private val buttonsGrid = new GridPane {
    hgap = 2
    vgap = 2
  }
  redraw()

  dialogPane.value.setContent(new VBox(10) {
    padding = Insets(10)
    children = Seq(
      buttonsGrid,
    )
  })

  resultConverter = {
    case ButtonType.OK => Some(Reflection(minefield.dimensions, axis.get, index))
    case _ => None
  }

  dialogPane.value.getButtonTypes.addAll(ButtonType.OK, ButtonType.Cancel)

  private def redraw(): Unit = {
    buttonsGrid.children.clear()

    buttonsGrid.add(new Button {
      text = "↘"
      minWidth = 30
      minHeight = 30
      style = if (axis.contains(MainDiagonal)) "-fx-background-color: yellow;" else ""
      onAction = _ => {
        index = None
        axis = Some(MainDiagonal)
        redraw()
      }
    }, 0, 0)

    for (x <- 0 until minefield.dimensions.width) {
      val btn = new Button {
        text = "↓"
        minWidth = 30
        minHeight = 30
        style = if (index.contains(x) && axis.contains(Vertical)) "-fx-background-color: yellow;" else ""
        onAction = _ => {
          index = Some(x)
          axis = Some(Vertical)
          redraw()
        }
      }

      buttonsGrid.add(btn, x + 1, 0)
    }

    buttonsGrid.add(new Button {
      text = "↙"
      minWidth = 30
      minHeight = 30
      style = if (axis.contains(AntiDiagonal)) "-fx-background-color: yellow;" else ""
      onAction = _ => {
        index = None
        axis = Some(AntiDiagonal)
        redraw()
      }
    }, minefield.dimensions.width + 2, 0)

    for (y <- 0 until minefield.dimensions.height; x <- 0 until minefield.dimensions.width) {
      if (x == 0) {
        val btn = new Button {
          text = "→"
          minWidth = 30
          minHeight = 30
          style = if (index.contains(y) && axis.contains(Horizontal)) "-fx-background-color: yellow;" else ""
          onAction = _ => {
            index = Some(y)
            axis = Some(Horizontal)
            redraw()
          }
        }
        buttonsGrid.add(btn, 0, y + 1)
      }

      val location = Location(y, x)
      val cell = minefield(location)
      val btn = new Button {
        text = cell.toString
        minWidth = 30
        minHeight = 30
        disable = true
      }
      buttonsGrid.add(btn, x + 1, y + 1)
    }
  }
}
