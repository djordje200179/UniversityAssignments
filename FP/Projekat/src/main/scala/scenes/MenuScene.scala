package com.djordjemilanovic.minesweeper
package scenes

import components.NavigationButton
import dialogs.{BoardFileDialog, BuiltinMinefieldsFileDialog, CustomMinefieldFileDialog, MinefieldFileDialog}
import minefield.Minefield
import minefield.Minefield.Cell.Empty

import scalafx.geometry.Orientation.Horizontal
import scalafx.geometry.{Insets, Pos}
import scalafx.scene.Scene
import scalafx.scene.control.*
import scalafx.scene.layout.*
import scalafx.scene.paint.Color.{Black, LightGreen}

import scala.util.{Success, Try}

object MenuScene extends Scene {
  private val editMinefield = LevelCreatorScene.apply andThen App.navigate
  private val loadGame = GameScene.apply andThen App.navigate

  private def startNewGameFromMinefieldFile(minefieldChooser: MinefieldFileDialog): Try[Unit] =
    minefieldChooser.load()
      .fold(Success(())) { fileAccess =>
      for {
        file <- fileAccess
        minefield <- Serialization.readMinefield(file)
        board = Board(minefield)
      } yield loadGame(board)
    }

  private def editMinefieldFile(minefieldChooser: MinefieldFileDialog): Try[Unit] =
    minefieldChooser.load()
      .fold(Success(())) { fileAccess =>
      for {
        file <- fileAccess
        minefield <- Serialization.readMinefield(file)
      } yield editMinefield(minefield)
    }

  private def loadSavedGame(): Try[Unit] =
    Option(BoardFileDialog.showOpenDialog(App.stage))
      .map(Serialization.readBoard)
      .fold(Success(()))(_.map(loadGame))

  fill = LightGreen
  content = new VBox(5) {
    alignment = Pos.Center
    children = Seq(
      new VBox(3) {
        alignment = Pos.Center
        border = new Border(BorderStroke(Black, BorderStrokeStyle.Solid, CornerRadii(5), BorderWidths(1)))
        padding = Insets(5)
        children = Seq(
          new Label("Play game") {
            alignment = Pos.Center
            style = "-fx-font-weight: bold"
          },
          Separator(Horizontal),
          new HBox(5) {
            children = Seq(
              new Button("Built-in levels") {
                BuiltinMinefieldsFileDialog.result = null
                onAction = _ => startNewGameFromMinefieldFile(BuiltinMinefieldsFileDialog).get
              },
              new Button("Custom level") {
                onAction = _ => startNewGameFromMinefieldFile(CustomMinefieldFileDialog).get
              },
              new Button("Saved game") {
                onAction = _ => loadSavedGame().get
              },
            )
          }
        )
      },
      new VBox(3) {
        alignment = Pos.Center
        border = new Border(BorderStroke(Black, BorderStrokeStyle.Solid, CornerRadii(5), BorderWidths(1)))
        padding = Insets(5)
        children = Seq(
          new Label("Level creator") {
            alignment = Pos.Center
            style = "-fx-font-weight: bold"
          },
          Separator(Horizontal),
          new HBox(5) {
            children = Seq(
              new Button("New") {
                onAction = _ => editMinefield(Minefield(Vector(Vector(Empty))))
              },
              new Button("Built-in") {
                BuiltinMinefieldsFileDialog.result = null
                onAction = _ => editMinefieldFile(BuiltinMinefieldsFileDialog).get
              },
              new Button("Load") {
                onAction = _ => editMinefieldFile(CustomMinefieldFileDialog).get
              },
            )
          }
        )
      },
      NavigationButton("Best scores", scenes.ResultsScene),
    )
  }
}
