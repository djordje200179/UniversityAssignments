package com.djordjemilanovic.minesweeper
package dialogs

import scalafx.stage.FileChooser

object ActionFileDialog extends FileChooser {
  title = "Open action file"
  extensionFilters.add(
    FileChooser.ExtensionFilter("Minesweeper action files", "*.mact"),
  )
}

