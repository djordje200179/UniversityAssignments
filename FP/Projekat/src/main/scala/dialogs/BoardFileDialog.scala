package com.djordjemilanovic.minesweeper
package dialogs

import scalafx.stage.FileChooser

object BoardFileDialog extends FileChooser {
  title = "Open saved game"
  extensionFilters.add(
    FileChooser.ExtensionFilter("Minesweeper save files", "*.msav"),
  )
}

