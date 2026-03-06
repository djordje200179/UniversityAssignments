package com.djordjemilanovic.minesweeper
package dialogs

import scalafx.stage.FileChooser

import java.io.File
import scala.util.{Success, Try}

object CustomMinefieldFileDialog extends FileChooser with MinefieldFileDialog {
  title = "Open minefield file"
  extensionFilters.add(
    FileChooser.ExtensionFilter("Minesweeper level files", "*.mlvl"),
  )

  override def load(): Option[Try[File]] = Option(showOpenDialog(App.stage)).map(Success.apply)
  def save(): Option[Try[File]] = Option(showSaveDialog(App.stage)).map(Success.apply)
}