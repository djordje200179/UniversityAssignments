package com.djordjemilanovic.minesweeper
package dialogs.isometries

import minefield.editors.Isometry

import scalafx.scene.control.Dialog

trait IsometryCreatorDialog extends Dialog[Option[Isometry]] {
  def create: Option[Isometry] = showAndWait().flatMap(_.asInstanceOf[Option[Isometry]])
}