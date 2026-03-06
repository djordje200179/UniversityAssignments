package com.djordjemilanovic.minesweeper
package minefield.editors

import minefield.Minefield.Cell.*
import minefield.editors.Transforming.*
import minefield.editors.Transforming.ReflectionAxis.*
import minefield.editors.Transforming.RotationDirection.*
import minefield.sampleMinefield
import utils.Location

import org.scalatest.funsuite.AnyFunSuite

class TransformingTest extends AnyFunSuite {
  test("Rotating clockwise") {
    val newMinefield = rotated(Clockwise)(sampleMinefield)

    assert(newMinefield.dimensions == sampleMinefield.dimensions.transposed)
    assert(newMinefield(Location(1, 0)) == Empty)
    assert(newMinefield(Location(2, 1)) == Mine)
  }

  test("Rotating counter-clockwise") {
    val newMinefield = rotated(CounterClockwise)(sampleMinefield)

    assert(newMinefield.dimensions == sampleMinefield.dimensions.transposed)
    assert(newMinefield(Location(1, 0)) == Mine)
    assert(newMinefield(Location(2, 1)) == Mine)
  }

  test("Reflecting horizontally") {
    val newMinefield = reflected(Horizontal)(sampleMinefield)

    assert(newMinefield.dimensions == sampleMinefield.dimensions)
    assert(newMinefield(Location(0, 1)) == Empty)
    assert(newMinefield(Location(1, 0)) == Mine)
  }

  test("Reflecting vertically") {
    assert(sampleMinefield == reflected(Vertical)(sampleMinefield))
  }

  test("Reflecting main-diagonally") {
    val newMinefield = reflected(MainDiagonal)(sampleMinefield)

    assert(newMinefield.dimensions == sampleMinefield.dimensions.transposed)
    assert(newMinefield(Location(2, 1)) == Mine)
    assert(newMinefield(Location(1, 2)) == Empty)
  }

  test("Reflecting anti-diagonally") {
    val newMinefield = reflected(AntiDiagonal)(sampleMinefield)

    assert(newMinefield.dimensions == sampleMinefield.dimensions.transposed)
    assert(newMinefield(Location(0, 1)) == Mine)
    assert(newMinefield(Location(1, 0)) == Empty)
  }
}
