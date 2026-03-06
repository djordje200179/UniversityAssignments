package com.djordjemilanovic.minesweeper
package scenes

import Board.Cell.Opened
import Results.Result
import components.NavigationButton
import dialogs.{ActionFileDialog, BoardFileDialog}
import utils.{Location, Move}

import scalafx.Includes.jfxMouseEvent2sfx
import scalafx.application.Platform
import scalafx.beans.property.{IntegerProperty, ObjectProperty}
import scalafx.geometry.Pos
import scalafx.scene.Scene
import scalafx.scene.control.Alert.AlertType.Information
import scalafx.scene.control.{Alert, Button, Label}
import scalafx.scene.input.MouseButton.{Primary, Secondary}
import scalafx.scene.layout.{GridPane, HBox, VBox}
import scalafx.scene.paint.Color.LightBlue

import java.util.{Timer, TimerTask}
import scala.util.Success

class GameScene(private val originalBoard: Board) extends Scene {
  private val observableBoard = new ObjectProperty[Board]
  private val buttonsGrid = new GridPane {
    hgap = 2
    vgap = 2
  }

  private val scoreCounter = IntegerProperty(0)
  private val scoreLabel = new Label("You can do it!")
  private val timer = new Timer(true) {
    scheduleAtFixedRate(new TimerTask {
      def run(): Unit = scoreCounter.value = scoreCounter.value + 1
    }, 1000, 1000)
  }

  private def redrawScoreLabel(): Unit = Platform.runLater {
    scoreLabel.text = s"Score: ${scoreCounter.value}"
  }

  private def redrawGrid(board: Board): Unit = {
    buttonsGrid.children.clear()

    for (y <- 0 until board.dimensions.height; x <- 0 until board.dimensions.width) {
      val location = Location(y, x)
      val cell = board(location)
      val btn = new Button {
        text = cell.toString
        minWidth = 30
        minHeight = 30
        disable = cell.isInstanceOf[Opened]
        onMouseClicked = e => {
          e.button match {
            case Primary =>
              observableBoard.value = board.openedCell(location).getOrElse {
                timer.cancel()
                new Alert(Information) {
                  title = "Game Over"
                  headerText = "You hit a mine!"
                  contentText = s"Final Score: ${scoreCounter.value}"
                }.showAndWait()
                App.navigate(scenes.MenuScene)
                board
              }
              scoreCounter.value = scoreCounter.value + 2
            case Secondary => observableBoard.value = board.markedCell(location)
            case _ =>
          }
        }
      }
      buttonsGrid.add(btn, x, y)
    }
  }

  observableBoard.onChange { (_, _, newBoard) =>
    if (newBoard.isCleared) {
      timer.cancel()

      new Alert(Information) {
        title = "Congratulations!"
        headerText = "You won!"
        contentText = s"Final Score: ${scoreCounter.value}"
      }.showAndWait()

      val newResult = Result(newBoard.dimensions, scoreCounter.value)

      Serialization.writeResults(
        Serialization.readResults
          .getOrElse(Results.empty)
          .inserted(newResult)
      ).get

      scenes.ResultsScene.results.add(newResult)

      App.navigate(scenes.MenuScene)
    }
    redrawGrid(newBoard)
  }
  observableBoard.value = originalBoard

  scoreCounter.onChange { (_, _, _) => redrawScoreLabel() }

  fill = LightBlue
  content = new VBox(5) {
    alignment = Pos.Center
    children = Seq(
      scoreLabel,
      new HBox(5) {
        children = Seq(
          NavigationButton.Back(),
          new Button("Save") {
            onAction = _ => {
              Option(BoardFileDialog.showSaveDialog(App.stage))
                .foreach(Serialization.writeBoard(observableBoard.value))
            }
          },
          new Button("Hint") {
            onAction = _ => {
              val location = observableBoard.value.hint
              observableBoard.value = observableBoard.value.openedCell(location).get
              scoreCounter.value = scoreCounter.value + 10
            }
          },
          new Button("Load actions") {
            onAction = _ => {
              Option(ActionFileDialog.showOpenDialog(App.stage))
                .fold(Success(Seq[Move]())) { fileAccess =>
                  for (moves <- Serialization.readActions(fileAccess))
                    yield moves
                }.foreach(_.foreach { case Move(button, location) =>
                  button match {
                    case Move.Button.Left => observableBoard.value = observableBoard.value.openedCell(location).getOrElse {
                      timer.cancel()
                      new Alert(Information) {
                        title = "Game Over"
                        headerText = "You hit a mine!"
                        contentText = s"Final Score: ${scoreCounter.value}"
                      }.showAndWait()
                      App.navigate(scenes.MenuScene)
                      observableBoard.value
                    }
                    case Move.Button.Right => observableBoard.value = observableBoard.value.markedCell(location)
                  }
                })
            }
          }
        )
      },
      buttonsGrid
    )
  }
}
