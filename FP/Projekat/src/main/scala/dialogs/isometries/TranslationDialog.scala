package com.djordjemilanovic.minesweeper
package dialogs.isometries

import minefield.editors.isometries.Translation

import scalafx.beans.property.ObjectProperty
import scalafx.geometry.Insets
import scalafx.scene.control.{ButtonType, Label, Spinner}
import scalafx.scene.layout.{HBox, VBox}

class TranslationDialog extends IsometryCreatorDialog {
  title = "Create translation isometry"

  private val observableDx = new ObjectProperty[Int]
  private val observableDy = new ObjectProperty[Int]

  resultConverter = {
    case ButtonType.OK => Some(Translation(observableDy.value, observableDx.value))
    case _ => None
  }

  dialogPane.value.getButtonTypes.addAll(ButtonType.OK, ButtonType.Cancel)

  dialogPane.value.setContent(new VBox(10) {
    padding = Insets(10)
    children = Seq(
      new HBox(10) {
        children = Seq(
          new Label("Delta X:"),
          new Spinner[Int](-100, 100, 0) {
            observableDx <== value
          }
        )
      },
      new HBox(10) {
        children = Seq(
          new Label("Delta Y:"),
          new Spinner[Int](-100, 100, 0) {
            observableDy <== value
          }
        )
      }
    )
  })
}