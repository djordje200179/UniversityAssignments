package com.djordjemilanovic.minesweeper
package dialogs.isometries

import minefield.Minefield
import minefield.editors.isometries.CentralSymmetry
import minefield.editors.isometries.CentralSymmetry.Direction

import scalafx.geometry.Insets
import scalafx.scene.control.{Button, ButtonType}
import scalafx.scene.layout.GridPane

class CentralSymmetryDialog(minefield: Minefield) extends IsometryCreatorDialog {
  private def createButton(label: String, value: Direction) = new Button(label) {
    onAction = _ => result = Some(CentralSymmetry(minefield.dimensions, value))
  }

  title = "Create central symmetry isometry"
  headerText = "You should choose a direction for the central symmetry."
  dialogPane.value.getButtonTypes.add(ButtonType.Cancel)

  dialogPane.value.setContent(new GridPane {
    hgap = 10
    vgap = 10
    padding = Insets(20)
    add(createButton("↖", Direction.UpLeft), 0, 0)
    add(createButton("↗", Direction.UpRight), 1, 0)
    add(createButton("↙", Direction.DownLeft), 0, 1)
    add(createButton("↘", Direction.DownRight), 1, 1)
  })

  resultConverter = dialogButton =>
    if dialogButton == ButtonType.Cancel then None
    else result.value
}