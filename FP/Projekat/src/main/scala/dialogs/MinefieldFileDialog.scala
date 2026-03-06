package com.djordjemilanovic.minesweeper
package dialogs

import java.io.File
import scala.util.Try

trait MinefieldFileDialog {
  def load(): Option[Try[File]]
}