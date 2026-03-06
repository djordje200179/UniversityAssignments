package com.djordjemilanovic.minesweeper
package minefield

import minefield.Difficulty.Easy
import minefield.Minefield
import minefield.Minefield.Cell.Mine
import minefield.Minefield.InvalidCharacterException
import utils.{Dimensions, Location}

import org.scalatest.funsuite.AnyFunSuite

import scala.util.{Failure, Success}

class MinefieldTest extends AnyFunSuite {
  test("Invalid dimensions") {
    assertThrows[IllegalArgumentException](Minefield(Vector.empty))
  }

  test("Indexing") {
    assert(sampleMinefield(Location(1, 2)) == Mine)
  }

  test("Mines count") {
    assert(sampleMinefield.minesCount == 3)
  }

  test("String representation") {
    val expectedString =
      """-#-
        |#-#
        |---""".stripMargin
    assert(sampleMinefield.toString == expectedString)
  }

  test("Nearby mines count") {
    assert(sampleMinefield.nearbyMines(Location(1, 1)) == 3)
  }

  test("Dimensions") {
    assert(sampleMinefield.dimensions == Dimensions(3, 3))
  }

  test("Parsing") {
    val inputString: Iterable[Iterable[Char]] = Vector(
      "-#-",
      "#-#",
      "---"
    )

    assert(Minefield.parse(inputString) == Success(sampleMinefield))
  }

  test("Invalid parsing") {
    val inputString: Iterable[Iterable[Char]] = Vector(
      "-#-",
      "#x#",
      "---",
    )

    assert(Minefield.parse(inputString) == Failure(InvalidCharacterException('x')))
  }

  test("Difficulty measurement") {
    assert(sampleMinefield.difficulty == Easy)
  }
}
