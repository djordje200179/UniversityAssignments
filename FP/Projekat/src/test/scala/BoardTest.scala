package com.djordjemilanovic.minesweeper

import Board.Cell.Closed
import minefield.Minefield
import minefield.Minefield.Cell.{Empty, Mine}
import utils.Location

import org.scalatest.funsuite.AnyFunSuite

import scala.util.Failure

class BoardTest extends AnyFunSuite {
  val startingMinefield = Minefield(Vector(
    Vector(Empty, Mine, Empty),
    Vector(Mine, Empty, Mine),
    Vector(Empty, Empty, Empty),
    Vector(Empty, Empty, Empty)
  ))

  val startingBoard = Board(startingMinefield)

  test("Initial indexing") {
    assert(startingBoard(Location(0, 0)) == Closed)
  }

  test("Initial marks count") {
    assert(startingBoard.leftMarks == 3)
  }

  test("Dimensions") {
    assert(startingBoard.dimensions == startingMinefield.dimensions)
  }

  test("Opening mine cell") {
    val result = startingBoard.openedCell(Location(0, 1))

    assert(result == Failure(Board.MineOpened(Location(0, 1))))
  }

  test("Opening empty cell with neighboring mines") {
    val result = startingBoard.openedCell(Location(0, 0)).get

    assert(result(Location(0, 0)) == Board.Cell.Opened(2))
  }

  test("Opening empty cell with no neighboring mines") {
    val result = startingBoard.openedCell(Location(3, 0)).get

    val expectedString =
      """■■■
        |■■■
        |121
        |   """.stripMargin

    assert(result.toString == expectedString)
  }

  test("Marking a closed cell") {
    val updatedBoard = startingBoard.markedCell(Location(0, 0))

    assert(updatedBoard(Location(0, 0)) == Board.Cell.Marked)
    assert(updatedBoard.leftMarks == 2)
  }

  test("Unmarking a marked cell") {
    val updatedBoard = startingBoard.markedCell(Location(0, 0)).markedCell(Location(0, 0))

    assert(updatedBoard(Location(0, 0)) == Board.Cell.Closed)
    assert(updatedBoard.leftMarks == 3)
  }

  test("String representation") {
    val markedBoard = startingBoard.markedCell(Location(0, 0)).markedCell(Location(1, 0))

    val expectedString =
      """⚑■■
        |⚑■■
        |■■■
        |■■■""".stripMargin

    assert(markedBoard.toString == expectedString)
  }

  test("Initial hint") {
    assert(startingBoard.hint == Location(0, 0))
  }
  
  test("Hint after opening a cell") {
    val updatedBoard = startingBoard.openedCell(Location(0, 0))
    assert(updatedBoard.get.hint == Location(0, 2))
  }
}
