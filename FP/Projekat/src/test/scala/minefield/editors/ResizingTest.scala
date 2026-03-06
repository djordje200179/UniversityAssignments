package com.djordjemilanovic.minesweeper
package minefield.editors

import minefield.Minefield.Cell.*
import minefield.editors.Resizing.*
import minefield.sampleMinefield
import utils.{Dimensions, Location}

import org.scalatest.funsuite.AnyFunSuite

class ResizingTest extends AnyFunSuite {
  test("Appending row") {
    val newMinefield = appendedRow(sampleMinefield)

    assert(newMinefield.dimensions == Dimensions(4, 3))
    assert(newMinefield(Location(0, 1)) == Mine)
  }

  test("Prepending row") {
    val newMinefield = prependedRow(sampleMinefield)

    assert(newMinefield.dimensions == Dimensions(4, 3))
    assert(newMinefield(Location(0, 1)) == Empty)
  }

  test("Appending column") {
    val newMinefield = appendedColumn(sampleMinefield)

    assert(newMinefield.dimensions == Dimensions(3, 4))
    assert(newMinefield(Location(1, 0)) == Mine)
  }

  test("Prepending column") {
    val newMinefield = prependedColumn(sampleMinefield)

    assert(newMinefield.dimensions == Dimensions(3, 4))
    assert(newMinefield(Location(1, 0)) == Empty)
  }

  test("Removing first row") {
    val newMinefield = removedFirstRow(sampleMinefield)

    assert(newMinefield.dimensions == Dimensions(2, 3))
    assert(newMinefield(Location(0, 1)) == Empty)
  }

  test("Removing last row") {
    val newMinefield = removedLastRow(sampleMinefield)

    assert(newMinefield.dimensions == Dimensions(2, 3))
    assert(newMinefield(Location(0, 1)) == Mine)
  }

  test("Removing first column") {
    val newMinefield = removedFirstColumn(sampleMinefield)

    assert(newMinefield.dimensions == Dimensions(3, 2))
    assert(newMinefield(Location(1, 0)) == Empty)
  }

  test("Removing last column") {
    val newMinefield = removedLastColumn(sampleMinefield)

    assert(newMinefield.dimensions == Dimensions(3, 2))
    assert(newMinefield(Location(1, 0)) == Mine)
  }
}
