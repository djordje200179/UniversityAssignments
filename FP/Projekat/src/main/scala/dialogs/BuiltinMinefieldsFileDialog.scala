package com.djordjemilanovic.minesweeper
package dialogs

import minefield.Difficulty

import scalafx.geometry.Insets
import scalafx.scene.control.*
import scalafx.scene.layout.VBox

import java.io.File
import scala.util.{Random, Success, Try}

object BuiltinMinefieldsFileDialog extends Dialog[File]() with MinefieldFileDialog {
  private val availableLevels =
    Option(new File("levels").listFiles)
      .getOrElse(Array[File]())
      .filter(_.isDirectory)
      .sortBy(dir => Difficulty.valueOf(dir.getName.capitalize).ordinal)
      .map(dir => dir -> dir.listFiles().filter(f => f.isFile && f.getName.endsWith(".mlvl")))
      .toMap

  title = "Choose level"
  dialogPane.value.setPrefSize(400, 500)
  dialogPane.value.setContent(new ScrollPane() {
    prefHeight = 450
    fitToWidth = true
    content = new Accordion() {
      panes = availableLevels.map { case (dir, files) =>
        new TitledPane() {
          text = dir.getName.capitalize
          content = new VBox(5) {
            padding = Insets(5)
            children = files.map { file =>
              new Button(file.getName) {
                minWidth = 100
                onAction = _ => result = file
              }
            }.appended(new Button("* RANDOM *") {
              minWidth = 100
              onAction = _ => result = files(Random.nextInt(files.length))
            })
          }
        }
      }
    }
  })

  dialogPane.value.getButtonTypes.add(ButtonType.Cancel)

  resultConverter = dialogButton =>
    if dialogButton == ButtonType.Cancel then null
    else result.value

  override def load(): Option[Try[File]] = showAndWait().map(_.asInstanceOf[File]).map(Success.apply)
}