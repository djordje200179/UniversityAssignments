package com.djordjemilanovic.minesweeper
package scenes

import Results.Result
import components.NavigationButton

import scalafx.beans.property.StringProperty
import scalafx.collections.ObservableBuffer
import scalafx.geometry.Pos
import scalafx.scene.Scene
import scalafx.scene.control.{TableColumn, TableView}
import scalafx.scene.layout.{HBox, VBox}
import scalafx.scene.paint.Color.LightBlue

object ResultsScene extends Scene {
  val results: ObservableBuffer[Result] = ObservableBuffer[Result](Serialization.readResults.getOrElse(Results.empty).toSeq*)

  fill = LightBlue
  content = new VBox(5) {
    alignment = Pos.Center
    children = Seq(
      new HBox(5) {
        children = Seq(
          NavigationButton.Back(),
        )
      },
      new TableView[Result](results) {
        columns ++= Seq(
          new TableColumn[Result, String] {
            text = "Dimensions"
            cellValueFactory = r => StringProperty(r.value.dimensions.toString)
          },
          new TableColumn[Result, String]() {
            text = "Score"
            cellValueFactory = r => StringProperty(r.value.score.toString)
          }
        )
      }
    )
  }
}